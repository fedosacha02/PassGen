#pragma once 
#include "../config.hpp"

class Group
{
    private:
        static ID count;
        const ID id;
        const ID admin_id; // the reference to User
        char name[NAME_LENGTH_LIMIT];
        ID users[GROUP_MEMBERS_LIMIT];
        
    public:
        Group(ID id, char name[NAME_LENGTH_LIMIT], ID admin_id);
        ~Group();
        friend std::ostream& operator<<(std::ostream& os, const Group* group);
};
