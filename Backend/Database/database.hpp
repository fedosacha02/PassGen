#pragma once

#include <fstream>
#include "../User/user.hpp"
#include "../Group/group.hpp"
#include "../Organisation/organisation.hpp"
#include "../Password/password.hpp"
#include "../Password/password_sharing.hpp"
#include "../HTTP/server.hpp"

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
    template <typename T> void createEntry(const T&, std::fstream&);
    bool searchUserEntry(const HTTP::UserCredentials& credentials);
    template <typename T>void markAsDeleted(ID, std::fstream&);
    template<typename T> void outputAllEntries(std::fstream&);
};