#pragma once 
#include "../config.hpp"
#include "password.hpp"
#include "../Functions/functions.cpp"

ID Password::count = 0;

Password::Password(): id(count++), owner_id(0){
    assign((char*)"password1", this->name);
    assign((char*)"value1", this->value);
    is_blocked = false;
}

// The implementation the a copy constructor

Password::Password(Password& password): id{password.id}, owner_id{password.owner_id}{
    assign(password.name, name);
    assign(password.value, value);
    is_blocked = password.is_blocked;
}

// A constructor
Password::Password(const User* user, const char* name, const char* value, bool is_blocked): id(count++), owner_id(user->id){
    assign(name, this->name);
    assign(value, this->value);
    this->is_blocked = is_blocked;
    std::cout << this << '\n';
}

std::ostream& operator<<(std::ostream& os, const Password* password) {
    os << password->id << ", " << password->name << ", " << password->value << ", " << password->is_blocked;
    return os; // Return the stream for chaining
}

Password::~Password(){
    std::cout << "The password " << this << " was deleted.\n";
}
