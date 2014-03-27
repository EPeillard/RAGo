#include "Network.hpp"

using namespace rago;

Network::Network(std::string ipHostAddr, int portNo)
{
    this->portNo = portNo;
    this->ipHostAddr = ipHostAddr;
}

void Network::connexion()
{
    ///Create a socket point
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    ///Get the server
    server = gethostbyname(ipHostAddr.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    ///
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)(server->h_addr), (char *)(&serv_addr.sin_addr.s_addr), server->h_length);
    serv_addr.sin_port = htons(portNo);

    /// Now connect to the server
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
         perror("ERROR connecting");
         exit(1);
    }
}

bool Network::sendRequest(int code, std::string request)
{
    char buffer[256];
    int n;
    ///Make the request
    std::ostringstream oss1, oss2;
    oss1<<code;
    std::string send = oss1.str()+"#"+request;
    ///Send it
    n = write(sockfd,send.c_str(),strlen(send.c_str()));
    if (n < 0)
    {
         perror("ERROR writing to socket");
         return false;
    }
    return true;
}


std::string Network::getInfo()
{
    char buffer[256];
    int n;
    bzero(buffer,256);
    ///Read the network
    n = read(sockfd,buffer,255);
    if (n < 0)
    {
         perror("ERROR reading from socket");
         return "";
    }
    return buffer;
}
