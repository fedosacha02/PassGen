#pragma once
#include "../config.hpp"

class User;

class Password{
    private:
        static ID count;
        const ID id;
        const ID owner_id;

        char name[NAME_LENGTH_LIMIT];
        char value[PASSWORD_LENGTH_LIMIT];
        bool is_blocked;
        
    public:
        Password();
        Password(Password&);
        Password(User* user, const char* name, const char* value, bool is_blocked = false);
        friend std::ostream& operator<<(std::ostream& os, const Password& password);
        ~Password();
       
        
};