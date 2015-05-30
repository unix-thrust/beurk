BEURK's TODO List
=================

### Due for BEURK 1.0
- [ ] Unit test with principal antirootkit
- [x] Invisible backdoor
- [x] Work on common linux server distributions
- [x] PAM backdoor for local privesc and remote entry
- [ ] Anti-debugging
- [ ] Avoids unhide, lsof, ps, ldd detection
- [x] Hides files and directories
- [x] Hides remote connections
- [x] Hides processes
- [x] Hides logins
- [ ] PCAP hooks avoids local sniffing

### Core (evil hooking library)
- [x] Implement **DEBUG_MODE** (as defined in API Documentation)
- [ ] Add an `uninstallation feature` on **DEBUG_MODE**


### Client (PTY backdoor connection client)
- [x] Create a socat POC pty backdoor
- [ ] Create a basic PTY client in python 2.7


### Builder (Makefile and config file builder)
- [x] Create a lite, basic Makefile
- [x] Describe config-file variables for building
- [x] Check for dependencies in Makefile
