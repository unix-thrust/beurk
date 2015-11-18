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
import argparse


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
            self.s.bind((rhost, bport))
            self.s.connect((rhost, rport))
        except socket.gaierror:
            self._netfail("Socket error")
        except OverflowError:
            self._netfail("Bind error")
        except:
            self._netfail("Connection refused")


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



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-a','--address',metavar='<address>', dest='address',help='Ipaddress  connection target')
    parser.add_argument('-p','--port',dest='port',metavar='<port>',type=int,help='Port connection')
    parser.add_argument('-b', '--bind-port', dest='bind',metavar='<bind_port>',type=int,help='Bind port connection')
    parser.add_argument('-pw', '--password', dest='password',metavar='<password>',help='password')
    parser.add_argument('-c','--command' ,dest='command',metavar='command bash',help='send command on target')
    parser.add_argument('-f', '--file',dest='file',metavar='<file>',help='batch_file command send')
    options = parser.parse_args()
    if options.address and options.port and options.bind:
        client = Client(options.address, options.port, options.bind)
        if options.password and options.file:
            try:
                fd = open(options.file, "r")
            except IOError:
                print "cannot open", options.file
                exit(1)
            client.password(options.password)
            client.loop_batch(options.file, fd)
        elif options.password and options.command:
            client.password(options.password)
            client.run_command(options.command + "\n")
        else:
            if options.password:
                client.connection(options.password)
            else:client.connection()
            client.tty_raw_mode()
            client.loop()
        client.close()
    parser.print_help()
