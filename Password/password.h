#pragma once
#include "../config.h"

class Password{
    private:
        ID id;
        ID owner_id;

        char name[NAME_LENGTH_LIMIT];
        char value[PASSWORD_LENGTH_LIMIT];
        bool is_blocked;
        
    public:
};