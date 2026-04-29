#include <iostream>
#include "User/user.h"
#include "User/user.cpp"

int main(){
    char username[USERNAME_LENGTH_LIMIT] = "fedosacha02";
    char master_password[PASSWORD_LENGTH_LIMIT] = "psd2024";
    char email[EMAIL_LENGTH_LIMIT] = "fedosacha02@gmail.com";

    User user1(username, master_password, email);

    return 0;
}