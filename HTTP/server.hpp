#pragma once

class Server{
    public:
        int IP_address; // 192.168.1.1
        unsigned short port_number; // 8080
        // file descriptor of server socket.
        int server_socket_fd;

        Server(unsigned short);
        ~Server();

};