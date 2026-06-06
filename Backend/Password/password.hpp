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
        ID folder_id;
        
    public:
        Password();
        Password(Password&);
        Password(const User* user, const char[NAME_LENGTH_LIMIT], const char[PASSWORD_LENGTH_LIMIT], bool is_blocked, ID folder_id);
        friend std::ostream& operator<<(std::ostream& os, const Password* password);
        ~Password();
       
        
};