#pragma once

#include "user.hpp"
#include <iostream>
#include "../Functions/functions.cpp"
#include "../Password/password.hpp"
#include "../Database/database.hpp"

// Static ID initialisation
ID User::count = 0;

User::User(): id{count++}, username{"unknown"}, master_password{"unknown"}, email{"unknown"}{}

User::User(User& user): id{user.id}{
    assign(user.username, username);
    assign(user.master_password, master_password);
    assign(user.email, email);
    plan = user.plan;

}

User::User(char username[USERNAME_LENGTH_LIMIT], char master_password[PASSWORD_LENGTH_LIMIT], char email[EMAIL_LENGTH_LIMIT]): id{count++}{
    assign(username, this->username);
    assign(master_password, this->master_password);
    assign(email, this->email);
    std::cout << "The user " << this->username << " was created \n";
}

User::~User(){
    std::cout << "The user " << username << " was deleted \n";
}


std::ostream& operator<<(std::ostream& os, const User* user) {
    os << user->id << ", " << user->username << ", " << user->master_password << ", " << user->email << ", " << user->is_deleted << '\n';
    return os; // Return the stream for chaining
}

bool User::isReal(){
    std::cout << username;
    return (username == "unknown")? false: true; 
}
bool User::confirmMasterPassword(char password[PASSWORD_LENGTH_LIMIT]){
    return compare_strings(master_password, password);
}

void User::create_user_password(const char name[NAME_LENGTH_LIMIT], const char value[PASSWORD_LENGTH_LIMIT], Database* db, bool is_blocked){
    db->createEntry<Password>(Password(this, name, value, is_blocked, 0), db->passwords);
}
void User::rename_password(ID password_id){
    
}