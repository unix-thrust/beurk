#!/usr/bin/python

import sys
import socket
import select
import termios
import tty


class Client:
    def __init__(self, rhost="127.0.0.1", rport=3000, bport=64832):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((rhost, bport))
        self.s.connect((rhost, rport))
        #self.s.send("aaaa\n")
        self.old_attrs = termios.tcgetattr(sys.stdin)
        tty.setcbreak(sys.stdin)

    def loop(self):
        sys.stdout.write("password:")
        sys.stdout.flush()
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
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.old_attrs)


client = Client(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))

client.loop()
client.close()


exit(1)
