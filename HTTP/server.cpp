#pragma once

#include <iostream>
#include <sys/socket.h> // Core socket functions: socket(), bind(), connect(), accept()
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h> // Definitions for internet operations: inet_pton(), inet_ntop()
#include <unistd.h>     // For close()
#include <cstring>
#include <sys/wait.h>
#include <signal.h>

#include "../Functions/functions.cpp" // compare_strings

#define PORT "8080"  // the port users will be connecting to
#define BACKLOG 1   // how many pending connections queue will hold
#define BUFFER_SIZE 1024


void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void send_html(int client_socket, const char* file_path) {
    FILE* html_file = fopen(file_path, "r");
    if (html_file == nullptr) {
        perror("Error opening HTML file.");
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    char* http_header = (char*)"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    if(send(client_socket, http_header, strlen(http_header), 0) == -1){
        perror("send: http header");
    }

    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, html_file)) > 0) {
        if(send(client_socket, buffer, bytes_read, 0) == -1) perror("send: html content");
    }
    std::cout << "The HTML page located at " << file_path << " was successfully sended.\n";  
    fclose(html_file);
}

int get_http_info(char* output, const char* buffer, int displacement = 0){
    char a = buffer[displacement];
    size_t i, j;
    for(i = displacement, j = 0; a != ' '; a = buffer[++i]){
        output[j++] = a; 
    }
    return j;
}

void start(){
    // listen on sock_fd, new connection on new_fd
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address info
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    hints.ai_protocol = 0;       
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }
     // loop through all the results and bind to the first we can
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    freeaddrinfo(servinfo); // all done with this structure

    if (p == nullptr)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
            &sin_size);
        
        if(new_fd == -1) {
            perror("accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
            char buffer[1024] = {0};
            read(new_fd, buffer, sizeof(buffer) - 1);
            std::cout << buffer << '\n';
            char method[7] = {};
            int method_length = get_http_info(method, buffer);
            char path[50] = {};
            int path_length = get_http_info(path, buffer, method_length+1);
            
            std::cout << "The acquired method is " << method << ' ' << method_length << '\n';
            std::cout << "The required path is " << path << ' ' << path_length << '\n';
            
            
            if(compare_strings(method, (const char*)"GET")){
                
                if(compare_strings(path, (const char*)"/")){
                    std::cout << "GET response:\n"; 
                    send_html(new_fd, "../Layout/markup/login.html");
                }
            }
       
            //close(sockfd); // child doesn't need the listener
           
            //close(new_fd);
        }
        
        
        //close(new_fd);  // parent doesn't need this
    }
}