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
#include <sys/stat.h> // get a file size

#include "../Functions/functions.cpp" // compare_strings

#define PORT "8080"  // the port users will be connecting to
#define BACKLOG 1   // how many pending connections queue will hold
#define BUFFER_SIZE 1024

/*HTTP::HTTPResponse(char status_code[4], char content_length[10], char content_type[10], char* body){
    assign(status_code, this->status_code);
    assign(content_lenght, this->content_length);
    assign(content_type, this->content_type);
    assign(file_path, this->file_path);
};*/

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

void send_file(const int client_socket, const char* file_path, unsigned short status_code, const char* content_type) {
    FILE* file = fopen(file_path, "rb");
    if (file == nullptr) {
        perror("Error opening the file.");
        return;
    }
    // Get a file size by means of reading metadata
    struct stat st;
    stat(file_path, &st);
    int size = st.st_size;
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    char http_header[150];
    sprintf(http_header, "HTTP/1.1 %d\r\nServer: Custom/1.0\r\nContent-Type: %s; charset=UTF-8\r\nContent-Length: %d\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n", (int)status_code, content_type, size);
    std::cout << http_header << '\n';
    if(send(client_socket, http_header, strlen(http_header), 0) == -1){
        perror("send: http header");
    }

    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if(send(client_socket, buffer, bytes_read, 0) == -1) perror("send: file content");
    }
    std::cout << "The file located at " << file_path << " was successfully sended.\n"; 
    
    fclose(file);
    

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
        return;
    }
     // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
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
            close(sockfd); // child doesn't need the listener

            char buffer[1024] = {0};
            read(new_fd, buffer, sizeof(buffer));
            std::cout << buffer << '\n';
            char method[7] = {};
            int method_length = get_http_info(method, buffer);
            char path[50] = {};
            int path_length = get_http_info(path, buffer, method_length+1);
            
            std::cout << "The acquired method is " << method << ' ' << method_length << '\n';
            std::cout << "The required path is " << path << ' ' << path_length << '\n';
            
           
            if(compare_strings(method, (const char*)"GET")){
                //send(new_fd, "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: h2c\r\n\r\n",70, 0);
                if(compare_strings(path, (const char*)"/")){
                    std::cout << "Sending login.html...\n"; 
                    send_file(new_fd, "../Layout/markup/login.html", 200, "text/html");
                }
                else if(compare_strings(path, (const char*)"/styles/styles.css")){
                    std::cout << "Sending styles.css...\n"; 
                    send_file(new_fd, "../Layout/styles/styles.css", 200, "text/css");
                }
                else if(compare_strings(path, (const char*)"/media/logotype.svg")){
                    std::cout << "Sending logotype.svg...\n"; 
                    send_file(new_fd, "../Layout/media/logotype.svg", 200, "image/svg+xml");
                }
                else if(compare_strings(path, (const char*)"/styles/not_found.css")){
                    std::cout << "Sending not_found.css...\n"; 
                    send_file(new_fd, "../Layout/styles/not_found.css", 200, "text/css");
                }
                else{
                    std::cout << "Sending not_found.html...\n"; ;
                    send_file(new_fd, "../Layout/markup/not_found.html", 404, "text/html");
                }
                
            }
    
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }
}