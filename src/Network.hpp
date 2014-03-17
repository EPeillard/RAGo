/** \file Network.hpp
  *
  * \date 16/03/2014
  *
  * \author Nicolas David & Sylvain Palominos
  *
  * \version 0
  *
  * Declaration of the class Network
  **/

/** \class  rago::Network Network.hpp Network
  *
  **/

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>

namespace rago{

class Network{

public:

    Network(std::string, int);
    void connexion();
    bool sendRequest(int, std::string);
    std::string getInfo();

private:

    std::string ipHostAddr;
    int sockfd, portNo;
    struct sockaddr_in serv_addr;
    struct hostent *server;

};

}

#endif // NETWORK_HPP
