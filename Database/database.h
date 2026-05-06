#pragma once

#include <fstream>
#include "../User/user.h"
#include "../Group/group.h"
#include "../Organisation/organisation.h"
#include "../Password/password.h"
#include "../Password/password_sharing.h"

// Flat File Database 
struct Database{
    std::fstream users;
    std::fstream passwords;
    std::fstream groups;
    std::fstream organisations;
    std::fstream password_sharings_to_group;
    std::fstream password_sharings_to_organisation;

    // Opening the database schema
    Database();
    // Closing the database schema
    ~Database();
    template <typename T> void createEntry(T, std::fstream&);
    bool searchUserEntry(char username[USERNAME_LENGTH_LIMIT], User& user);
    template <typename T>void markAsDeleted(ID id, std::fstream& db);
    template<typename T> void outputAllEntries(std::fstream&);
};