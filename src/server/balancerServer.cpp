#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

#include "balancerServer.h"

using namespace server;

void print_stats(const std::vector<std::unique_ptr<server::Connection>> &nodes, int drop)
{
  for (auto &a : nodes)
  {
    std::cout << a.get()->m_port << " ";
  }
  std::cout << std::endl;
  for (auto &a : nodes)
  {
    std::cout << a.get()->m_counter << " ";
  }
  std::cout << std::endl
            << "Packet dropped: " << drop << std::endl
            << std::endl;
}

ServerBalancer::ServerBalancer(const ConfParameters &cparams_t) : cparams(cparams_t) {}
ServerBalancer::~ServerBalancer()
{
  for (auto &a : nodes)
  {
    close(a->m_socket);
  }
  close(receiver->m_socket);
};

void ServerBalancer::prepareAnyConnection(Connection *conn)
{
  if ((conn->m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    std::cerr << "Can't create socket by port " << conn->m_port << std::endl;
    conn = nullptr;
    return;
  }

  // bzero(&m_servaddr, sizeof(m_servaddr));
  // m_servaddr.sin_addr.s_addr = INADDR_ANY; // htonl(atoi(ip));
  // m_servaddr.sin_family = AF_INET;
  // m_servaddr.sin_port = htons(port);
  bzero(&conn->m_cliaddr, sizeof(conn->m_cliaddr));
  conn->m_cliaddr.sin_addr.s_addr = INADDR_ANY;
  conn->m_cliaddr.sin_family = AF_INET;
  conn->m_cliaddr.sin_port = htons(conn->m_port);
}
void ServerBalancer::prepareNodeConnection(Connection *conn)
{
  // bzero(&conn->m_cliaddr, sizeof(conn->m_cliaddr));
  // conn->m_cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // conn->m_cliaddr.sin_family = AF_INET;
  // conn->m_cliaddr.sin_port = htons(port);
  prepareAnyConnection(conn);
  if (conn)
    conn->m_clilen = sizeof(conn->m_cliaddr);
  return;
}

void ServerBalancer::prepareReceiverConnection(Connection *conn)
{
  prepareAnyConnection(conn);
  if (conn)
  {
    bzero(&conn->m_servaddr, sizeof(conn->m_servaddr));
    conn->m_servaddr.sin_addr.s_addr = INADDR_ANY;
    conn->m_servaddr.sin_family = AF_INET;
    conn->m_servaddr.sin_port = htons(conn->m_port);

    if (bind(conn->m_socket, (struct sockaddr *)&conn->m_servaddr, sizeof(conn->m_servaddr)) == -1)
    {
      close(conn->m_socket);
      std::cerr << "Can't bind socket to port " << conn->m_port << std::endl;
    }
  }
}

void ServerBalancer::loadBalancing()
{
  BalancerTimer timer;
  Connection *tmp = new Connection(cparams.input_port);
  prepareReceiverConnection(tmp);
  if (tmp)
    receiver = std::make_unique<Connection>(*tmp);
  long int received_length;

  for (auto &a : cparams.nodes)
  {
    Connection *tmp = new Connection(a);
    prepareNodeConnection(tmp);
    if (tmp)
      nodes.emplace_back(std::make_unique<Connection>(*tmp));
    else
    {
      std::cerr << "Unable to connect to port " << a << std::endl;
    }
  }

  auto node_it = nodes.begin();
  int error_cntr = 0;
  while (1)
  {

    std::shared_ptr<std::vector<char>> received_data = std::make_shared<std::vector<char>>(1024, '0');

    if (!receiveData(receiver.get(), received_data, received_length))
    {
      return;
    }

    if (timer.allowSendingData(cparams.max_load))
    {
      if (!sendData(node_it->get(), received_data, received_length))
      {
        error_cntr++;
      }
      node_it->get()->m_counter++;

      timer.addCurrentSenderTime();

      node_it++;
      if (node_it == nodes.end())
      {
        node_it = nodes.begin();
        print_stats(nodes, error_cntr);
      }
    }
    else
    {
      error_cntr++;
    }
  }
};

bool ServerBalancer::sendData(Connection *conn, std::shared_ptr<std::vector<char>> data, const long int &length)
{
  if (sendto(conn->m_socket, data->data(), length, 0, (struct sockaddr *)&conn->m_cliaddr, conn->m_clilen) == -1)
  {
    std::cerr << "Can't send data to client port " << conn->m_port << std::endl;
    return false;
  }
  return true;
};

bool ServerBalancer::receiveData(Connection *conn, std::shared_ptr<std::vector<char>> data, long int &length)
{
  length = recv(conn->m_socket, data->data(), data->size(), 0);
  if (length < 0)
  {
    std::cerr << "Can't receive data from receiver port " << conn->m_port << std::endl;
    return false;
  }
  return true;
};

char *ServerBalancer::gethostip()
{
  char hostbuffer[256];
  char *IPbuffer;
  struct hostent *host_entry;
  int hostname;

  if ((hostname = gethostname(hostbuffer, sizeof(hostbuffer))) < 0)
  {
    perror("gethostname");
    exit(1);
  }

  if ((host_entry = gethostbyname(hostbuffer)) == NULL)
  {
    perror("gethostbyname");
    exit(1);
  }

  IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
  if (NULL == IPbuffer)
  {
    perror("inet_ntoa");
    exit(1);
  }
  return IPbuffer;
}
