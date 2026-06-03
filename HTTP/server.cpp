#pragma once

#include <iostream>
#include <sys/socket.h> // Core socket functions
#include <netinet/in.h> // Internet address structures
#include <arpa/inet.h> // Definitions for internet operations
#include <unistd.h>     // For close()
#include "server.hpp"


Server::Server(unsigned short port_number){
    // Create server socket.
    server_socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if(server_socket_fd < 0){
        std::cerr << "Failed to create server socket.\n";
        exit(1);
    }
    this->port_number = port_number;
    
    // Set socket options
    // 1 = IPv6 only, 0 = allow IPv4-mapped addresses if the platform supports it 

    int v6only = 1;
    setsockopt(server_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only));

    // Configure the address structure
    struct sockaddr_in6 server_addr; //structure for server address it helps to config and bind server socket.
  
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port_number);
    server_addr.sin6_flowinfo = 0; // Flow info (usually 0)
    server_addr.sin6_scope_id = 0; // Scope ID (0 when not using a scoped address)
    server_addr.sin6_addr = in6addr_any; // INADDR_ANY allows the server to listen on all available network interfaces.
    
    //bind socket to port.
    if(bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        std::cerr << "Failed to bind server socket." << std::endl;
        exit(1);
    }

    //listens on socket.
    if(listen(server_socket_fd, 5) < 0){
        std::cerr << "Failed to listen on server socket." << std::endl;
        exit(1);
    }
    
    std::cout << "The server started on port : " << port_number << std::endl;

    // Accept connections
    struct sockaddr_in6 client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client = accept(server_socket_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client >= 0) {
        char client_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &client_addr.sin6_addr, client_ip, sizeof(client_ip));
        printf("Client connected: [%s]:%d\n",
               client_ip, ntohs(client_addr.sin6_port));

        write(client, "Hello IPv6 client!\n", 19);
        close(client);
    }

}

Server::~Server(){
    //close the server socket.
    close(server_socket_fd);
    std::cout << "The server has just closed.\n";
}