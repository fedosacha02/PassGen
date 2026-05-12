#pragma once
#include "../config.hpp"

class Password{
    private:
        static ID count;
        const ID id;
        const ID owner_id;

        char name[NAME_LENGTH_LIMIT];
        char value[PASSWORD_LENGTH_LIMIT];
        bool is_blocked;
        
    public:
        Password(ID owner_id, char name[NAME_LENGTH_LIMIT], char value[PASSWORD_LENGTH_LIMIT], bool is_blocked);
        ~Password();
        
};