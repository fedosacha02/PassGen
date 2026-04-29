#include "user.h"
#include <iostream>

// Static ID initialisation
ID User::count = 0;


void assign(char* in, char* out){
    char c = in[0];
    for (size_t i = 0; c != '\0'; i++)
    {
        c = in[i];
        out[i] = c;
    }
}
User::User(char username[USERNAME_LENGTH_LIMIT], char master_password[PASSWORD_LENGTH_LIMIT], char email[EMAIL_LENGTH_LIMIT]): id{count++}{
    assign(username, this->username);
    assign(master_password, this->master_password);
    assign(email, this->email);
    std::cout << "The user " << this->username << " was created \n";
}

User::~User(){
    std::cout << "The user " << username << master_password << email << " was deleted \n";
}