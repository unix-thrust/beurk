#!/usr/bin/env python2

import sys
import time
import socket
import select
import termios
import tty
import signal
import fcntl
import os
import struct


class Client:
    def _netfail(self, string):
        self.close()
        sys.exit("host [%s], port [%d], bind port [%d] : %s" \
                        % (self.rhost, self.rport, self.bport, string))

    def __init__(self, rhost="127.0.0.1", rport=3000, bport=64832):
        self.rhost = rhost
        self.rport = rport
        self.bport = bport
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.s.bind(('0.0.0.0', bport))
            self.s.connect((rhost, rport))
        except Exception as e:
            self._netfail(str(e))

    @staticmethod
    def usage(name):
        print "%s <address> <port> <bind_port> [password] [batch_file | -c command]" % name

    def tty_raw_mode(self):
        self.old_attrs = termios.tcgetattr(sys.stdin)
        tty.setcbreak(sys.stdin)

    def _recv_timeout(self, timeout=0.1):
        # **NOTE** The socket need to be non blocking
        total_data = [];
        data = '';
        begin = time.time()
        while 1:
            # if you got some data, then break after timeout
            if total_data and time.time()-begin > timeout:
                break
            # if you got no data at all, wait a little longer, twice the timeout
            elif time.time()-begin > timeout*2:
                break
            try:
                data = self.s.recv(8192)
                if data:
                    total_data.append(data)
                    begin = time.time()
                else:
                    time.sleep(0.1)
            except:
                pass
        return ''.join(total_data)

    def _print_response(self):
        data = self._recv_timeout()
        sys.stdout.write(data)
        sys.stdout.flush()

    def password(self, passwd):
        # Need to be run before run_command or loop_batch
        self.s.setblocking(0)
        self.s.send(passwd + "\n")
        self._recv_timeout()

    def run_command(self, cmd):
        self.s.send(cmd)
        self._print_response()

    def connection(self, passwd=""):
        if passwd == "":
            sys.stdout.write("password:")
            sys.stdout.flush()
            passwd = sys.stdin.readline()
        else:
            passwd += "\n"
        self.s.send(passwd)
        try:
            data = self.s.recv(1024)
        except (socket.error, socket.herror) as e:
            (errno, error) = e
            self._netfail(error)
        if not data:
            sys.stdout.write("\n")
            self.close()
            exit(1)
        sys.stdout.write(data)
        sys.stdout.flush()

    def loop_batch(self, batch, fd):
        for line in fd:
            self.run_command(line)
        sys.stdout.write("\n")

    def loop(self):
        while 1:
            inputready, outputready,exceptrdy = select.select([0, self.s], [],[])
            for i in inputready:
                if i == 0:
                    data = sys.stdin.read(1)
                    self.s.send(data)
                elif i == self.s:
                    data = self.s.recv(1024)
                    if not data:
                        return 0
                    sys.stdout.write(data)
                    sys.stdout.flush()

    def close(self):
        try:
            self.s.shutdown(socket.SHUT_RDWR)
            self.s.close()
        except:
            pass
        if hasattr(self, 'old_attrs'):
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.old_attrs)


if len(sys.argv) < 4:
    Client.usage(sys.argv[0])
    exit(1)
elif len(sys.argv) == 6:
    try:
        fd = open(sys.argv[5], "r")
    except IOError:
        print "cannot open", sys.argv[5]
        Client.usage(sys.argv[0])
        exit(1)

client = Client(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))
if len(sys.argv) == 4:
    client.tty_raw_mode()
    client.connection()
    client.loop()
if len(sys.argv) == 5:
    client.tty_raw_mode()
    client.connection(sys.argv[4])
    client.loop()
elif len(sys.argv) == 6:
    client.password(sys.argv[4])
    client.loop_batch(sys.argv[5], fd)
elif len(sys.argv) == 7:
    client.password(sys.argv[4])
    client.run_command(sys.argv[6] + "\n")
client.close()
