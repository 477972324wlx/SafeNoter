## SafeNoter
-----
https://github.com/477972324wlx/SafeNoter

### 1.How to use it
#### Install (Build binaries & Start Server) 
- sudo ./install.sh (require root privellege)

#### Start Server
- sudo ./Launch_Server.sh (require root privellege)

#### Kill Server
- sudo ./Kill.sh (require root privellege)

### Build Binaries only
- ./make.sh 

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

### WorkFlow
1. Server Start , run as user "safe_noter"
2. User launch ./Client [Operation] [filename]
3. Establish Connection between Client and Server
4. Server execute commands based on Client input.

### Operations:
- Write [filename]: 
    - Server generated some random string. Append it to the filename provided by the user and obtain a new unique filename [newfile]
    - Create "newfile.note" on the local directory. 
    - Generate a random password, send it to user. 
    - Create a file called "newfile.pwd", store the hashed(repeat 100 times) value of password in it.

- Read [filename]: 
    - Server send the content of all the files whose filename is like "\*filename\*.note" to user

- Delete [filename]:
    - Server check whether exists filename like "\*filename\*.note" exists.
    - If there are 0 or more than 1 results, reject.
    - Else require the user to input password
    - Compare the hash value of the password with .pwd file
    - If success delete the .pwd and .note file.

---

## Secure components
1. Server is deployed on a isolated space, only root can access that area. Users have no permission to read/write/execute the content inside this system's direcotry.
2. The Server is run as a non-root user, which prevents attackers to do "extremely" dangerous things.
3. Use some Linux built-in commands to handle files.
4. Check filenames before execution to prevent from shell injections.
5. Use random password. Store password in hashed form.
6. Limit the buffer size of I/O



    