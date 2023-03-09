#include <bits/stdc++.h>
#include <sys/socket.h>
#include "FileHandler.hpp"
#include "util.hpp"
#define DEBUG true

#define MAX_BUFFER 1024
const char * ok = "OK\n";

struct ThreadArgs{
    int clientfd;
    char filename[MAX_BUFFER];
    char buf[MAX_BUFFER];

    int pid;
    std::mutex mu;
};
typedef std::function<void(ThreadArgs*)> ThreadFunction;

// Thread Operations
void ThreadMain(ThreadArgs *args);
void ThreadRead(ThreadArgs *args);
void ThreadWrite(ThreadArgs *args);
void ThreadRemove(ThreadArgs *args);



void ThreadRead(ThreadArgs *args){
    DEBUG && cout << "Start Reading!\n";

    string execute_command = "cat *"+ string(args->filename)+"*.note";

    FILE * stream = FileHandler::GetPipeFromCommand(execute_command);
    if (stream) {
        while (!feof(stream))
            if (fgets(args->buf,MAX_BUFFER, stream) != NULL) {
                write(args->clientfd, args->buf, strlen(args->buf));
            }
        pclose(stream);
    } else {
        write(args->clientfd, "Error", 6);
    }
}

void ThreadWrite(ThreadArgs *args){
    
    string user_filename(args->filename);
    string local_filename, suffix, password, execute_command;
    FileHandlerReply reply;

    cout << "Start Writing\n";

    if(util::checkFilename(user_filename) == false){
        return;
    }


    suffix = util::makeRandStr(6, true);
    local_filename = user_filename + "_" + suffix;

    password = util::makeRandStr(6, true);
    string hashed_pwd = util::getHashvValue(password);

    reply = FileHandler::createFile(local_filename + ".note");
    if(reply.error_code){
        return ;
    }

    reply = FileHandler::createFile(local_filename + ".pwd");
        if(reply.error_code){
        return ;
    }

    reply = FileHandler::appendFile(local_filename + ".pwd", hashed_pwd);
    cout << reply.response << endl;

    sprintf(args->buf, "Filename: %s, Password: %s\n", local_filename.c_str(), password.c_str());

    write(args->clientfd, args->buf, strlen(args->buf));
    

    ofstream fout(local_filename + ".note");

    bool first = true;
    while(1){
        memset(args->buf, 0 ,sizeof(args->buf));
        int cnt = read(args->clientfd, args->buf, MAX_BUFFER);
        if(!cnt) break;
    
        // Remove the empty line
        fout << args->buf ;
        if(!first)
            cout << endl;
        first = false;
    }
    fout.close();
}

void ThreadRemove(ThreadArgs *args){
    string filename = "*"+string(args->filename) +".note";
    string pwdfile = "*"+string(args->filename) +".pwd";

    FileHandlerReply reply = FileHandler::testFile(filename);
    if(reply.error_code){
        write(args->clientfd, reply.response.c_str(), reply.response.size());
        return;
    }
    reply = FileHandler::readFile("*"+string(args->filename)+".pwd");

    string hashed_pwd = reply.response;
    const char * msg = "Please input the password!";
    write(args->clientfd, msg, strlen(msg));

    read(args->clientfd, args->buf, MAX_BUFFER);
    string user_pwd = string(args->buf);

    if(util::getHashvValue(user_pwd) == hashed_pwd){
        reply = FileHandler::removeFile(filename);
        if(reply.error_code){
            write(args->clientfd, reply.response.c_str(), reply.response.size());
            return;
        }
        reply = FileHandler::removeFile(pwdfile);
        if(reply.error_code){
            write(args->clientfd, reply.response.c_str(), reply.response.size());
            return;
        }
        write(args->clientfd, ok, strlen(ok));
    } else {
        const char * WrongPassword = "Wrong Password!\n";
        write(args->clientfd, WrongPassword, strlen(WrongPassword));
    }
}


bool LoadFileName(ThreadArgs* args){
    int len = strlen(args->buf);
    int id = 0;
    int endpos = 0;
    bool delim = false;
    for (int i = 0 ; i < len ; ++i){
        if(delim){
            args->filename[id++] = args->buf[i];
        }
        if(args->buf[i] == ';'){
            delim = true;
            endpos = i;
        }
    }
    args->buf[endpos] = 0;
    return strlen(args->filename) > 0;
}

ThreadFunction getOperation(ThreadArgs * args){
    memset(args->buf,0,sizeof(args->buf));
    int cnt = read(args->clientfd, args->buf, MAX_BUFFER);


    // Arguments too long, drop
    if(cnt > 1000){
        return NULL;
    }

    if(!LoadFileName(args)){
        return NULL;
    }

    if(strcmp(args->buf, "read") == 0){
        //cout << "It's read" << endl;
        return ThreadRead;
    } else if(strcmp(args->buf, "write") == 0){
        return ThreadWrite;
    } else if(strcmp(args->buf, "remove") == 0){
        return ThreadRemove;
    }

    return NULL;
}



void ThreadMain(ThreadArgs* args){
   // printf("In Thread %d \n", args->pid);
    auto Function = getOperation(args);
    if(!Function){
        cout << "UnKnown Operation OR filename" << endl;
        return;
    }
    printf("Thread Working On %s of filename %s\n",args->buf, args->filename);
    Function(args);
    shutdown(args->clientfd, SHUT_RDWR);
}
