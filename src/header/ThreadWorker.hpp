#include <bits/stdc++.h>

#include "FileHandler.hpp"
#define MAX_BUFFER 1024
struct ThreadArgs{
    int clientfd;
    char filename[256];
    char buf[MAX_BUFFER];

    int pid;
    std::mutex mu;
};

void ThreadRead(ThreadArgs *args){
    cout << "Start Reading!" << endl;

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

}
void ThreadRemove(ThreadArgs *args){
    
}

typedef std::function<void(ThreadArgs*)> ThreadFunction;
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
    read(args->clientfd, args->buf, MAX_BUFFER);
   // cout << args->buf << endl;
    if(!LoadFileName(args)){
        return NULL;
    }

    if(strcmp(args->buf, "read") == 0){
        cout << "It's read" << endl;
        return ThreadRead;
    } else if(strcmp(args->buf, "write") == 0){
        return ThreadWrite;
    } else if(strcmp(args->buf, "remove") == 0){
        return ThreadRemove;
    }

    return NULL;
}



void ThreadMain(ThreadArgs* args){
    printf("In Thread %d \n", args->pid);
    auto Function = getOperation(args);
    if(!Function){
        cout << "UnKnown Operation" << endl;
        return;
    }
    printf("Thread Working On %s of filename %s\n",args->buf, args->filename);
    Function(args);
    shutdown(args->clientfd, SHUT_RDWR);
}
