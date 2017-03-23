#!/usr/bin/python

import sys
import socket
import select
import termios
import tty
import signal
import fcntl
import os
import struct


class Client:
    def __init__(self, rhost="127.0.0.1", rport=3000, bport=64832):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((rhost, bport))
        self.s.connect((rhost, rport))

    @staticmethod
    def usage(name):
        print "%s <address> <port> <bind_port> [password] [batch_file]" % name

    def tty_raw_mode(self):
        self.old_attrs = termios.tcgetattr(sys.stdin)
        tty.setcbreak(sys.stdin)

    def password(self, passwd):
        self.s.send(passwd + "\n")
        data = self.s.recv(1024)
        data = self.s.recv(1024)

    def connection(self, passwd=""):
        if passwd == "":
            sys.stdout.write("password:")
            sys.stdout.flush()
            passwd = sys.stdin.readline()
        else:
            passwd += "\n"
        self.s.send(passwd)
        data = self.s.recv(1024)
        if not data:
            sys.stdout.write("\n")
            self.close()
            exit(1)
        sys.stdout.write(data)
        sys.stdout.flush()

    def loop_batch(self, batch, fd):
        self.s.send("\n")
        for line in fd:
            self.s.send(line)
        data = self.s.recv(1024)
        if data[0:2] == "\r\n":
            data = data[2:]
        sys.stdout.write(data)
        while data:
            data = self.s.recv(1024)
            sys.stdout.write(data)

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
        self.s.close()
        if hasattr(client, 'old_attrs'):
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.old_attrs)


if len(sys.argv) < 4:
    Client.usage(sys.argv[0])
    exit(1)
elif len(sys.argv) >= 6:
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
client.close()

