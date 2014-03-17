#include "Network.hpp"

using namespace rago;

Network::Network(std::string ipHostAddr, int portNo)
{
    int n;
    this->portNo = portNo;
    this->ipHostAddr = ipHostAddr;
}

void Network::connexion()
{
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname(ipHostAddr.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)(server->h_addr), (char *)(&serv_addr.sin_addr.s_addr), server->h_length);
    serv_addr.sin_port = htons(portNo);

    /* Now connect to the server */
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
    std::ostringstream oss1, oss2;
    oss1<<code;
    std::string send = oss1.str()+"#"+request;
    n = write(sockfd,request.c_str(),strlen(request.c_str()));
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
    n = read(sockfd,buffer,255);
    if (n < 0)
    {
         perror("ERROR reading from socket");
         return "";
    }
    return buffer;
}
