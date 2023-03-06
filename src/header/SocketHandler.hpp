#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
using std::string;

class SocketHandler{
public:
    unsigned short port;
    string addr;
    int sockfd;
    sockaddr_in sock_addr;
    char buf[1024];

    static bool checkIPaddress(const string&){return true;}
    SocketHandler(unsigned short &_port, const string& _addr):port(_port), addr(_addr){
        int one = 1;
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(port);
        sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
    char* getBuffer(){
        return buf;
    }
    string getBufferString(){
        return string(buf);
    }

};

class ServerSocketHandler : public SocketHandler{
    sockaddr_in server_addr;
    sockaddr_in from_addr;
    int clientfd;
    
public:
    ServerSocketHandler(unsigned short& _port, const string& _addr):SocketHandler(_port,_addr){  
        int err = bind(sockfd, (sockaddr*)&sock_addr, sizeof(sock_addr));
        if(err < 0){
            perror("Cannot Bind Address\n");
        }
        err = listen(sockfd,1);
        if(err < 0){
            perror("Cannot listen to port\n");
        }
    }
    int tryAccept(){
        socklen_t len = sizeof(from_addr);
        clientfd = accept(sockfd, (sockaddr*)&from_addr, &len);
        if(clientfd < 0){
            perror("Unable to accept!\n");
        }
        return clientfd;
    }
};

class ClientSocketHandler : public SocketHandler{
public:
    ClientSocketHandler(unsigned short& _port, const string& _addr):SocketHandler(_port,_addr){
        int ret = connect(sockfd, (sockaddr*)&sock_addr, sizeof(sock_addr));
        if (ret < 0){
            perror("Cannot connect to Server!\n");
            exit(0);
        } 
    }
    
    void trySend(string ss){
        if (ss.size() > 1000){
            perror("String is too long!");
            return;
        }
        write(sockfd, ss.c_str(), ss.size());
    }
    void tryReceive(string &err_msg){
        memset(buf,0,sizeof(buf));
        int count = recv(sockfd, buf, 1024, 0);
        if(count == 0){
            err_msg = "Empty";
        }
    }
};

