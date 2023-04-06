#include <exception>
#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <queue>

#include "balancerServer.h"
#include "configfile.h"

int main(int argc, char **argv)
{

  ConfigFile cfg("../balancer.conf");

  // std::cout << "Print config file:" << std::endl;
  // cfg.print();

  ConfParameters cparams(cfg.getvalue<int>("bvalue"),
                         cfg.getvalue<int>("input"),
                         cfg.getvector<int>("nodes"));

  server::ServerBalancer serv(cparams);
  serv.loadBalancing();
}
