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

#ifndef NETWORK_HPP
#define NETWORK_HPP

//C includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

//C++ includes
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstdio>

#include <sys/time.h>
#include <sys/socket.h>

namespace rago{

/** \class  rago::Network Network.hpp Network
  *
  * \brief Class containing the network functions of the project.
  *
  * This class allow the communication throw server sockets with QGo client.
  **/
class Network{

public:

    /** \fn Network(std::string, int)
      * Main class constructor
      * \arg string of the ip adress of the server
      * \arg number of the port of the socket server
      **/
    Network(std::string, int);

    /** \fn connexion()
      * Estabmlish the connexion with the server
      **/
    void connexion();

    /** \fn bool sendRequest(int, std::string)
      * Send informations to the server
      * \arg id of the request
      * \arg text of the request
      * \return true if the request was send
      **/
    bool sendRequest(int, std::string);

    /** \fn std::string getInfo()
      * read the network for messages
      * \return the text of the message
      **/
    std::string getInfo();

private:

    /** ip adresss of the host **/
    std::string ipHostAddr;
    /** socket file descriptor **/
    int sockfd;
    /** Number of the host port **/
    int portNo;
    struct sockaddr_in serv_addr;
    struct hostent *server;

};

}

#endif // NETWORK_HPP
