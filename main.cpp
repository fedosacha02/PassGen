#include <iostream>
#include <fstream>
#include "User/user.cpp"
#include "Database/database.cpp"
#include "Password/password.cpp"
#include "HTTP/server.cpp"

int main(){
    Server server(8080);
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;
    int s_fd;
    s_fd = server.server_socket_fd;
    client_addr_size = sizeof(struct sockaddr_in);

    /*char username1[USERNAME_LENGTH_LIMIT] = "fedosacha02";
    char master_password1[PASSWORD_LENGTH_LIMIT] = "psd2024";
    char email1[EMAIL_LENGTH_LIMIT] = "fedosacha02@gmail.com";

    User user1(username1, master_password1, email1);

    char username2[USERNAME_LENGTH_LIMIT] = "j.smith";
    char master_password2[PASSWORD_LENGTH_LIMIT] = "K#8mP9!vR2@qLp9";
    char email2[EMAIL_LENGTH_LIMIT] = "alex.smith.personal@gmail.com";

    User user2(username2, master_password2, email2);*/

    Database database;

    /*database.createEntry<User>(user1, database.users);
    database.createEntry<User>(user2, database.users);*/

    char username[USERNAME_LENGTH_LIMIT];
    /*database.markAsDeleted<User>(0, database.users);
    database.outputAllEntries<User>(database.users);*/
    std::cout << "Login\n-------------------------------------------------------\nPlease, enter the username: ";
    std::cin >> username;
    User user;
    
    while(!database.searchUserEntry(username, user)){
        std::cout << "The entered username is incorrect. Please, try to enter again: ";
        std::cin >> username;
    }
    char master_password[PASSWORD_LENGTH_LIMIT];
    std::cout << "Please, enter the master password: ";
    std::cin >> master_password;
    while(!user.confirmMasterPassword(master_password)){
        std::cout << "The entered password is incorrect. Please, try to enter again: ";
        std::cin >> master_password;
    }
    std::cout << "You have logged in successfully!\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << "CREATING A PASSWORD\n";
    std::cout << "-------------------------------------------------------\n";
    char password_name[NAME_LENGTH_LIMIT];
    char value[PASSWORD_LENGTH_LIMIT];
    bool is_blocked;
    
    std::cout << "Please, enter the name of a new password: ";
    std::cin >> password_name;
    std::cout << "Please, enter the value of the new password: ";
    std::cin >> value;
    std::cout << "Please, specify if the password is blocked: ";
    std::cin >> is_blocked;
    user.create_user_password(password_name, value, &database, is_blocked);
    //database.outputAllEntries<Password>(database.passwords);


    return 0;
}