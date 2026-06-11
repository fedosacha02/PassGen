#pragma once

#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h> // Definitions for internet operations: inet_pton(), inet_ntop()
#include <unistd.h>     // For close()
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h> // get a file size
#include "../config.hpp"
#include <string.h>
#include "../Functions/functions.cpp" // compare_strings
#include "server.hpp"
#include "../Database/database.cpp"

/* Include the appropriate header file for SOCK_STREAM */
#ifdef _WIN32 /* Windows */
#include <stdarg.h>
#include <winsock2.h>
#else /* Linux/Unix */
#include <err.h>
#include <sys/socket.h> // Core socket functions: socket(), bind(), connect(), accept()
#include <sys/select.h>
#endif

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

static const char cache_id[] = "OpenSSL Demo Server";

#ifdef _WIN32
static const char *progname;

static void vwarnx(const char *fmt, va_list ap)
{
    if (progname != NULL)
        fprintf(stderr, "%s: ", progname);
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
}

static void errx(int status, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vwarnx(fmt, ap);
    va_end(ap);
    exit(status);
}

static void warnx(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vwarnx(fmt, ap);
    va_end(ap);
}
#endif

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

void send_file(SSL *ssl, const char* file_path, unsigned short status_code, const char* content_type) {
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
    char http_header[512];
    sprintf(http_header, "HTTP/1.1 %d\r\nServer: Custom/1.0\r\nContent-Type: %s; charset=UTF-8\r\nContent-Length: %d\r\nCache-Control: no-store\r\nContent-Security-Policy: upgrade-insecure-requests;\r\nConnection: close\r\n\r\n", (int)status_code, content_type, size);
    std::cout << http_header << '\n';
    if(SSL_write(ssl, http_header, strlen(http_header)) <= 0){
        perror("send: http header");
    }

    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if(SSL_write(ssl, buffer, bytes_read) <= 0) perror("send: file content");
    }
    std::cout << "The file located at " << file_path << " was successfully sended.\n"; 
    
    fclose(file);
    

}

HTTP::UserCredentials get_form_data(char* request){
    struct HTTP::UserCredentials credentials;
    char *email_ptr, *password_ptr;

    // Scan the string buffer for the 'email=' and "password=" strings
    if(email_ptr = strstr(request, "email=")){
        if(password_ptr = strstr(request, "password=")){
            email_ptr += 6;
            for(size_t i = 0; email_ptr < password_ptr-1; email_ptr++, i++)
            {
                credentials.email[i] = *email_ptr;
            }
            char* end = strchr(request, '\0');
            std::cout << end-request << '\n';
            password_ptr += 9;
            for (size_t i = 0; password_ptr < end; password_ptr++, i++)
            {
                credentials.password[i] = *password_ptr;
            }
        }
    }
    
    return credentials;

}

int get_http_info(char* output, const char* buffer, int displacement = 0){
    char a = buffer[displacement];
    size_t i, j;
    for(i = displacement, j = 0; a != ' '; a = buffer[++i]){
        output[j++] = a; 
    }
    return j;
}


int start(Database& database){
    int res = EXIT_FAILURE;
    long opts;
    const char *hostport;
    SSL_CTX *ctx = NULL;
    BIO *acceptor_bio;

    #ifdef _WIN32
        progname = argv[0];
    #endif
    /*
     * An SSL_CTX holds shared configuration information for multiple
     * subsequent per-client SSL connections.
     */
    ctx = SSL_CTX_new(TLS_server_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        errx(res, "Failed to create server SSL_CTX");
    }
    /*
    * TLS versions older than TLS 1.2 are deprecated by IETF and SHOULD
    * be avoided if possible.
    */
    if (!SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION)) {
        SSL_CTX_free(ctx);
        ERR_print_errors_fp(stderr);
        errx(res, "Failed to set the minimum TLS protocol version");
    }
    
    /*
    * Tolerate clients hanging up without a TLS "shutdown".  Appropriate in all
    * application protocols which perform their own message "framing", and
    * don't rely on TLS to defend against "truncation" attacks.
    */
    opts = SSL_OP_IGNORE_UNEXPECTED_EOF;
    /*
    * Block potential CPU-exhaustion attacks by clients that request frequent
    * renegotiation.  This is of course only effective if there are existing
    * limits on initial full TLS handshake or connection rates.
    */
    opts |= SSL_OP_NO_RENEGOTIATION;
    
    /*
    * Most servers elect to use their own cipher, group or signature preference
    * rather than that of the client.
    */
    opts |= SSL_OP_SERVER_PREFERENCE;

    /* Apply the selection options */
    SSL_CTX_set_options(ctx, opts);

    /*
    * Load the server's certificate *chain* file (PEM format), which includes
    * not only the leaf (end-entity) server certificate, but also any
    * intermediate issuer-CA certificates.  The leaf certificate must be the
    * first certificate in the file.
    *
    * In advanced use-cases this can be called multiple times, once per public
    * key algorithm for which the server has a corresponding certificate.
    * However, the corresponding private key (see below) must be loaded first,
    * *before* moving on to the next chain file.
    */
    if (SSL_CTX_use_certificate_chain_file(ctx, "chain.pem") <= 0) {
        SSL_CTX_free(ctx);
        ERR_print_errors_fp(stderr);
        errx(res, "Failed to load the server certificate chain file");
    }

    /*
    * Load the corresponding private key, this also checks that the private
    * key matches the just loaded end-entity certificate.  It does not check
    * whether the certificate chain is valid, the certificates could be
    * expired, or may otherwise fail to form a chain that a client can validate.
    */
    if (SSL_CTX_use_PrivateKey_file(ctx, "pkey.pem", SSL_FILETYPE_PEM) <= 0) {
        SSL_CTX_free(ctx);
        ERR_print_errors_fp(stderr);
        errx(res, "Error loading the server private key file, "
                "possible key/cert mismatch???");
    }

    /*
    * Servers that want to enable session resumption must specify a cache id
    * byte array, that identifies the server application, and reduces the
    * chance of inappropriate cache sharing.
    */
    SSL_CTX_set_session_id_context(ctx, (const unsigned char *)cache_id, sizeof(cache_id));
    SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_SERVER);

    /*
    * How many client TLS sessions to cache.  The default is
    * SSL_SESSION_CACHE_MAX_SIZE_DEFAULT (20k in recent OpenSSL versions),
    * which may be too small or too large.
    */
    SSL_CTX_sess_set_cache_size(ctx, 1024);

    /*
    * Sessions older than this are considered a cache miss even if still in
    * the cache.  The default is two hours.  Busy servers whose clients make
    * many connections in a short burst may want a shorter timeout, on lightly
    * loaded servers with sporadic connections from any given client, a longer
    * time may be appropriate.
    */
    SSL_CTX_set_timeout(ctx, 3600);
    
    /*
    * Clients rarely employ certificate-based authentication, and so we don't
    * require "mutual" TLS authentication (indeed there's no way to know
    * whether or how the client authenticated the server, so the term "mutual"
    * is potentially misleading).
    *
    * Since we're not soliciting or processing client certificates, we don't
    * need to configure a trusted-certificate store, so no call to
    * SSL_CTX_set_default_verify_paths() is needed.  The server's own
    * certificate chain is assumed valid.
    */
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    /*
    * Create a listener socket wrapped in a BIO.
    * The first call to BIO_do_accept() initialises the socket
    */
    acceptor_bio = BIO_new_accept("8080");
    if (acceptor_bio == NULL) {
        SSL_CTX_free(ctx);
        ERR_print_errors_fp(stderr);
        errx(res, "Error creating acceptor bio");
    }

    BIO_set_bind_mode(acceptor_bio, BIO_BIND_REUSEADDR);
    if (BIO_do_accept(acceptor_bio) <= 0) {
        SSL_CTX_free(ctx);
        ERR_print_errors_fp(stderr);
        errx(res, "Error setting up acceptor socket");
    }
    
    while(1) {  // main accept() loop
        BIO *client_bio;
        SSL *ssl;
        unsigned char buf[8192];
        size_t nread;
        size_t nwritten;
        size_t total = 0;

        /* Pristine error stack for each new connection */
        ERR_clear_error();

        /* Wait for the next client to connect */
        if (BIO_do_accept(acceptor_bio) <= 0) {
            /* Client went away before we accepted the connection */
            continue;
        }   
        /* Pop the client connection from the BIO chain */
        client_bio = BIO_pop(acceptor_bio);

        fprintf(stderr, "New client connection accepted\n");

        /* Associate a new SSL handle with the new connection */
        if ((ssl = SSL_new(ctx)) == NULL) {
            ERR_print_errors_fp(stderr);
            warnx("Error creating SSL handle for new connection");
            BIO_free(client_bio);
            continue;
        }
        SSL_set_bio(ssl, client_bio, client_bio);

        /* Attempt an SSL handshake with the client */
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            warnx("Error performing SSL handshake with client");
            SSL_free(ssl);
            continue;
        }

        /*while (SSL_read_ex(ssl, buf, sizeof(buf), &nread) > 0) {
            if (SSL_write_ex(ssl, buf, nread, &nwritten) > 0 &&
                nwritten == nread) {
                total += nwritten;
                continue;
            }
            warnx("Error echoing client input");
            break;
        }*/
        /*// listen on sock_fd, new connection on client_bio
        int sockfd, client_bio;
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
        printf("server: waiting for connections...\n");*/

        
        /*sin_size = sizeof their_addr;
        client_bio = accept(sockfd, (struct sockaddr *)&their_addr,
            &sin_size);
        
        if(client_bio == -1) {
            perror("accept");
            continue;
        }*/
        
        /*inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);*/
        
        
        //BIO_free(acceptor_bio); // child doesn't need the listener
        if(!fork()){
            char buffer[1024] = {0};

            SSL_read(ssl, buffer, sizeof(buffer));
            std::cout << buffer << '\n';
            char method[7] = {};
            int method_length = get_http_info(method, buffer);
            char path[50] = {};
            int path_length = get_http_info(path, buffer, method_length+1);
            
            std::cout << "The acquired method is " << method << ' ' << method_length << '\n';
            std::cout << "The required path is " << path << ' ' << path_length << '\n';
            
            
            if(compare_strings(method, (const char*)"GET")){
                //SSL_write(ssl, "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: h2c\r\n\r\n",70);
                if(compare_strings(path, (const char*)"/")){
                    std::cout << "Sending login.html...\n"; 
                    send_file(ssl, "../Layout/markup/login.html", 200, "text/html");
                }
                else if(compare_strings(path, (const char*)"/styles/styles.css")){
                    std::cout << "Sending styles.css...\n"; 
                    send_file(ssl, "../Layout/styles/styles.css", 200, "text/css");
                }
                else if(compare_strings(path, (const char*)"/media/logotype.svg")){
                    std::cout << "Sending logotype.svg...\n"; 
                    send_file(ssl, "../Layout/media/logotype.svg", 200, "image/svg+xml");
                }
                else if(compare_strings(path, (const char*)"/styles/not_found.css")){
                    std::cout << "Sending not_found.css...\n"; 
                    send_file(ssl, "../Layout/styles/not_found.css", 200, "text/css");
                }
                else if(compare_strings(path, (const char*)"/styles/passwords.css")){
                    std::cout << "Sending passwords.css...\n"; 
                    send_file(ssl, "../Layout/styles/passwords.css", 200, "text/css");
                }
                else if(compare_strings(path, (const char*)"/media/favicon.ico")){
                    std::cout << "Sending favicon.ico...\n"; 
                    send_file(ssl, "../Layout/media/favicon.ico", 200, "image/x-icon");
                }
                else if(compare_strings(path, (const char*)"/scripts/script.js")){
                    std::cout << "Sending /scripts/script.js...\n"; 
                    send_file(ssl, "../Layout/scripts/script.js", 200, "text/javascript");
                }
               
                else{
                    std::cout << "Sending not_found.html...\n"; ;
                    send_file(ssl, "../Layout/markup/not_found.html", 404, "text/html");
                }
                
            }
            else if(compare_strings(method, (const char*)"POST")){
                HTTP::UserCredentials credentials = get_form_data(buffer);
                std::cout << credentials.email << '\n' << credentials.password << '\n';
                if(database.searchUserEntry(credentials)){
                    send_file(ssl, "../Layout/markup/passwords.html", 200, "text/html");
                }
            }
            SSL_shutdown(ssl);
            SSL_free(ssl);
            exit(0);
        } else SSL_free(ssl);  // frees parent's reference without shutdown
        
    }
    /*
    * Unreachable placeholder cleanup code, the above loop runs forever.
    */
    SSL_CTX_free(ctx);
    return EXIT_SUCCESS;
}