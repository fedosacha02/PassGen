#pragma once
#include "group.hpp"
#include "../Functions/functions.cpp"

ID Group::count = 0;

Group::Group(ID id, char name[NAME_LENGTH_LIMIT], ID admin_id): id{count++}, admin_id{admin_id}{
    assign(name, this->name);
}

std::ostream& operator<<(std::ostream& os, const Group* group) {
    os << group->id << ", " << group->name << ", " << group->admin_id << ", " << *group->users;
    return os; // Return the stream for chaining
}

Group::~Group(){
    std::cout << "The group " << this << " was deleted.";
}