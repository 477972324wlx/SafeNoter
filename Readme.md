## SafeNoter
-----
### 1.How to use it
#### Install 
- sudo ./install.sh (require root privellege)

#### Start Server
- sudo ./Launch_Server.sh

#### Kill Server
- sudo ./Kill.sh

---
### Code Structure
cpp files:

- src/Client.cpp : The Client program for users. User use this program to read/write/remove notes.

- src/Server.cpp : The Server program. This program is supposed to run as a different user in a access-limited area. So that normal user cannot grant access to note files

- src/header/FileHandler.hpp: Some File operations: read, write, append, remove. Also some interactions with bash

- src/header/SocketHandler.hpp: Socket operations: Server(Accept/Bind/Listen), Server(Connect). And some I/O stuff

- src/header/ThreadWorker.hpp : Thread functions. Each Connection use a different thread to deal with.
- src/header/util.hpp: uitilities.
---
