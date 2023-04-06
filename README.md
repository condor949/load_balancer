# load_balancer
Test program for balancing traffic on several nodes

Build
```
$ make
```
After building the project, three binary files will appear:
* **balancer** - the balancer server receives data from the sender and sends it to the nodes in the receiver
* **receiver** - the program is a stub, opens three connections (more is possible, this is changed in the code) and receives packets from the balancer
* **sender** - the program is a generator of useless traffic, sends small data packets to the balancer, in large quantities

Run
```
$ ./balancer
```
information about network connection parameters is taken from **balancer.conf**
you can run it from the build folder (src)
to check the performance, a balancer and a sender are enough
```
$ ./sender
```
the balancer will output debug information even without a receiver, the packets will just drop
```
debug information contains connection ports
the number of packets sent for each
number of dropped packages
...
8670 8671 8672 
671 671 671 
Packet dropped: 15569800
...
```
