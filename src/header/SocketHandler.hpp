#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
using std::string;

class SocketHandler{
protected:
    unsigned short port;
    string addr;
    int sockfd;
    sockaddr_in sock_addr;
public:
    char buf[1000];
    static bool checkIPaddress(const string&){return true;}
    SocketHandler(unsigned short &_port, const string& _addr):port(_port), addr(_addr){
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(port);
        sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    }

};

class ServerSocketHandler : SocketHandler{
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
    void tryAccept(){
        socklen_t len = sizeof(from_addr);
        clientfd = accept(sockfd, (sockaddr*)&from_addr, &len);
        if(clientfd < 0){
            perror("Unable to accept!\n");
        }
    }
    bool tryReceive(){
        memset(buf,0,sizeof(buf));
        int count = recv(clientfd, buf, sizeof(buf),0);
        printf("%s",buf);
        return count > 0;
        //send(clientfd, buf, sizeof(buf),0);
    }
    void trySend(string ss){
        if (ss.size() > 1000){
            perror("String is too long!");
            return;
        }
        write(clientfd, ss.c_str(), ss.size());
    }

};

class ClientSocketHandler : SocketHandler{
public:
    ClientSocketHandler(unsigned short& _port, const string& _addr):SocketHandler(_port,_addr){
        int ret = connect(sockfd, (sockaddr*)&sock_addr, sizeof(sock_addr));
        if (ret < 0){
            perror("Cannot connect to Server!\n");
        } 
    }
    
    void trySend(string ss){
        if (ss.size() > 1000){
            perror("String is too long!");
            return;
        }
        write(sockfd, ss.c_str(), ss.size());
    }
    bool tryReceive(){
        int count = recv(sockfd, buf, sizeof(buf), 0);
        return count >= 0;
    }
};

