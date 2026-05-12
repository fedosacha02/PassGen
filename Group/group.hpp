#pragma once 
#include "../config.hpp"

class Group
{
    private:
        ID id;
        char name[NAME_LENGTH_LIMIT];
        ID admin_id; // the reference to User
};
