#pragma once

#include <iostream>
#include "client.hpp"
#include "server.hpp"

Client::Client(){
    // Create client socket.
    socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if(socket_fd < 0){
        std::cerr << "Failed to create a client socket.\n";
        exit(1);
    }
}
Client::~Client(){}

void Client::closeSocket(){
    // close a client socket
    close(socket_fd);
    std::cout << "The client has just closed.\n";
}
int Client::connectToServer(Server server){
    int connection_status = connect(socket_fd, (struct sockaddr*)&(server.addr), sizeof(server.addr));
    return connection_status;
}