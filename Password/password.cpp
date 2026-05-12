#pragma once 
#include "../config.hpp"
#include "password.hpp"
#include "../Functions/functions.cpp"

ID Password::count = 0;

Password::Password(ID owner_id, char name[NAME_LENGTH_LIMIT], char value[PASSWORD_LENGTH_LIMIT], bool is_blocked): id{count++}, owner_id{owner_id}{
    assign(name, this->name);
    assign(value, this->value);
    this->is_blocked = is_blocked;
}
