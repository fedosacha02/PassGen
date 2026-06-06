#pragma once
#include <netinet/in.h> // Internet address structures: struct sockaddr_in6, in6addr_any>

class Server;

class Client{
    private:
        // file descriptor of a client socket.
        int socket_fd;
        struct sockaddr_in6 addr; // structure for client address
        socklen_t addr_size;
    public:
        Client();
        ~Client();
        void closeSocket();
        int connectToServer(Server server);
        friend class Server;
};