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
    template <typename T> bool createEntry(const T&, std::fstream&);
    bool validateUserEntry(const HTTP::UserCredentials& credentials, User* user);
    template <typename T>void markAsDeleted(ID, std::fstream&);
    template<typename T> void outputAllEntries(std::fstream&);
    template<typename T>ID countObjects(std::fstream&);

    void setCookieToken(User* user, char token[HEX_LEN]);
    bool findByToken(char token[HEX_LEN], const User* result);
    bool checkUserForUniqueness(char username[USERNAME_LENGTH_LIMIT]);
};