#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string>

struct Sender
{
  int m_socket;
  sockaddr_in m_sock_sender;
  int slen;

  Sender(const unsigned long int &port)
  {
    if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
      std::cerr << "Getting socket error" << std::endl;
      return;
    }

    bzero(&m_sock_sender, sizeof(m_sock_sender));
    m_sock_sender.sin_addr.s_addr = INADDR_ANY;
    m_sock_sender.sin_family = AF_INET;
    m_sock_sender.sin_port = htons(port);
    slen = sizeof(m_sock_sender);
  };

  ~Sender()
  {
    close(m_socket);
  };

  void sendData(const std::string &data)
  {
    if (sendto(m_socket, data.c_str(), data.length(), 0, (struct sockaddr *)&m_sock_sender, slen) == -1)
    {
      std::cerr << "Can't send data" << std::endl;
      return;
    }
  }
};

int main()
{
  Sender sender(8585);
  for (int counter = 0;; counter++)
    sender.sendData(std::to_string(counter));
  return 0;
}
