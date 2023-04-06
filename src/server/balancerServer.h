#ifndef BALANCERSERVER_H
#define BALANCERSERVER_H

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "balancerTimer.h"
#include "configfile.h"

namespace server
{
  struct Connection
  {
    int m_socket;
    struct sockaddr_in m_servaddr, m_cliaddr;
    socklen_t m_servlen, m_clilen;
    unsigned long int m_port;
    int m_counter;
    Connection() : m_counter(0) {}
    Connection(int port) : m_port(port), m_counter(0) {}
  };

  class ServerBalancer
  {
  protected:
    ConfParameters cparams;
    std::unique_ptr<Connection> receiver;
    std::vector<std::unique_ptr<Connection>> nodes;
    static void *handle_client(void *arg);

  public:
    ServerBalancer(const ConfParameters &cparams);
    ~ServerBalancer();
    void startServer(int receiver_port);
    bool sendData(Connection *conn, std::shared_ptr<std::vector<char>> data, const long int &length);
    bool receiveData(Connection *conn, std::shared_ptr<std::vector<char>> data, long int &length);
    void prepareAnyConnection(Connection *conn);
    void prepareNodeConnection(Connection *conn);
    void prepareReceiverConnection(Connection *conn);
    void loadBalancing();

  protected:
    char *gethostip();
  };

} // namespace server

#endif // BALANCERSERVER_H
