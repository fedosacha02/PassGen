#include <iostream>
#include <fstream>
#include "User/user.cpp"
#include "Database/database.cpp"
#include "Password/password.cpp"
#include "HTTP/server.cpp"


int main(){
    Database database;
 
    
    /*char username1[USERNAME_LENGTH_LIMIT] = "fedosacha02";
    char master_password1[PASSWORD_LENGTH_LIMIT] = "nigde1234%23";
    char email1[EMAIL_LENGTH_LIMIT] = "fedosacha02%40gmail.com";

    User user1(username1, master_password1, email1);
    database.createEntry<User>(user1, database.users);*/
    start(database);
    /*char username2[USERNAME_LENGTH_LIMIT] = "j.smith";
    char master_password2[PASSWORD_LENGTH_LIMIT] = "K#8mP9!vR2@qLp9";
    char email2[EMAIL_LENGTH_LIMIT] = "alex.smith.personal@gmail.com";

    User user2(username2, master_password2, email2);*/
    return 0;
}