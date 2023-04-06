#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

void receiver(int port)
{
  sockaddr_in socket_in;
  int m_socket;
  int recv_len = 0;
  std::vector<char> buffer(1024, '0');

  if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    return;
  }

  bzero(&socket_in, sizeof(socket_in));
  socket_in.sin_family = AF_INET;
  socket_in.sin_port = htons(port);
  socket_in.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(m_socket, (struct sockaddr *)&socket_in, sizeof(socket_in)) == -1)
  {
    close(m_socket);
    return;
  }

  while (1)
  {
    if ((recv_len = recv(m_socket, buffer.data(), buffer.size(), 0)) == -1)
    {
      close(m_socket);
      return;
    }
  }

  close(m_socket);
}

int main()
{
  std::vector<std::thread> threads;
  for (int i = 0; i < 3; i++)
  {
    int port = 8670 + i;
    threads.emplace_back(receiver, port);
  }

  for (auto it = threads.begin(); it != threads.end(); ++it)
  {
    it->join();
  }

  return 0;
}
