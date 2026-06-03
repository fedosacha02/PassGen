#include "folder.hpp"
#include "../Functions/functions.cpp"

// Static ID initialisation
ID User::count = 0;

template <typename T>Folder::Folder(name[NAME_LENGTH_LIMIT], T* owner): id{count++}, owner_id{owner->id}{
    assign(name, this->name);
}

std::ostream& operator<<(std::ostream& os, const Folder* folder) {
    os << folder->id << ", " << folder->name << ", " << folder->owner_id;
    return os; // Return the stream for chaining
}

Folder::~Folder(){
    std::cout << "The folder " << this << " was deleted.\n";
}

