# Chat appplication

A client-server user-level chat application using sockets Programming in C.The server and client processes can run on same or different machines. Multiple client processes can connect to the same server. Messages are end-to-end using a simple algorithm for generating public key,private key pair.

## Tags
C programming, Socket programming,Multithreading in C using pthread Library
## Demo

To compile server.c and client.c
<br/>

```
gcc server.c -o server
gcc client.c -o client
```

To start server, PORT number is to be given as argument(ex. 9999)

```
./server PORT_NUMBER
````

To execute the client process, the IP address and PORT number of server is to be passed as command line argument.
```
./client IP_ADDR_OF_SERVER PORT_NUMBER
```

Choose a username of your choice and start chatting
