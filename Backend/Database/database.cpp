#pragma once

#include <iostream>
#include <fstream>
#include "database.hpp"
#include "../User/user.hpp"
#include "../Functions/functions.cpp"

// Opening the database schema
Database::Database():  
    users("Data/users.bin", std::ios::in | std::ios::out | std::ios::binary), 
    passwords("Data/passwords.bin", std::ios::in | std::ios::out | std::ios::binary),
    groups("Data/groups.bin", std::ios::in | std::ios::out | std::ios::binary),
    organisations("Data/organisations.bin", std::ios::in | std::ios::out | std::ios::binary),
    password_sharings_to_group("Data/password_sharings_to_group.bin", std::ios::in | std::ios::out | std::ios::binary),
    password_sharings_to_organisation("Data/password_sharings_to_organisation.bin", std::ios::in | std::ios::out | std::ios::binary){
        User::setCount(*this);
    }
   


Database::~Database(){
    users.close();
    passwords.close();
    groups.close();
    organisations.close();
    password_sharings_to_group.close();
    password_sharings_to_organisation.close();

    std::cout << "The database has been closed.\n";
}
template <typename T> void Database::createEntry(const T& obj, std::fstream& db){
    if(db){
        db.write((char*)&obj, sizeof(T));
        std::cout << "The entry was created successfully\n";
    } 
    else std::cout << "Something with the database went wrong.\n";
};


bool Database::validateUserEntry(const HTTP::UserCredentials& credentials, User* user){
    if(users){
        users.seekg(0, users.beg);

        while (users.read((char*)user, sizeof(User))){
            if(user->is_deleted) continue;
            std::cout << user->email << '\n';
            if(compare_strings(user->email, credentials.email)){
                if(compare_strings(user->master_password, credentials.password)){
                    std::cout << "The user has been validated\n" << user << '\n';
                    
                    // Reset the stream's error state flags
                    users.clear();
                    return true;
                }
                
            }
        }
        // Reset the stream's error state flags
        users.clear();
        std::cout << "The specified user " << credentials.email << " has NOT been validated.\n";
        return false; 
    }
    else std::cout << "Something with the database went wrong.\n";
    return false; 
}

template <typename T>void Database::markAsDeleted(ID id, std::fstream& db){
    T* obj;
    if(db){
        db.seekg(0, db.beg);
        db.seekp(0, db.beg);
        while(db.read((char*)obj, sizeof(T))){
            if(obj->is_deleted) continue;
            if(obj->id == id){
                obj->is_deleted = true;
                db.seekp(-sizeof(T), std::ios::cur);
                db.write((char*)obj, sizeof(T));
                std::cout << "The user was marked as deleted successfully\n";
                break;
            }
        }
        // Reset the stream's error state flags
        db.clear();
    
    }
    else std::cout << "Something with the database went wrong.\n";
    
}

void Database::setCookieToken(User* user, char token[HEX_LEN]){
    User obj;
    if(users){
        users.seekg(0, users.beg);
        users.seekp(0, users.beg);
        while(users.read((char*)&obj, sizeof(User))){
            if(obj.is_deleted) continue;
            if(obj.id == user->id){
                assign(token, user->cookie_token);
                users.seekp(-sizeof(User), std::ios::cur);
                users.write((char*)user, sizeof(User));
                std::cout << "The user token was updated successfully\n";
                break;
            }
        }
        // Reset the stream's error state flags
        users.clear();
    
    }
    else std::cout << "Something with the database went wrong.\n";
    
}

template<typename T> void Database::outputAllEntries(std::fstream& db){
    T* obj;
    if(db){
        db.seekg(0, db.beg);
        while(db.read((char*)obj, sizeof(T))){
            std::cout << obj << '\n';
        }

        // Reset the stream's error state flags
        db.clear();
    }
    else std::cout << "Something with the database went wrong.\n";
}

template<typename T>ID Database::countObjects(std::fstream& db){
    if(db){
        // get the length of a file:
        db.seekg(0, db.end);
        ID length = db.tellg();

        // Reset the stream's error state flags
        db.clear();
        return length/sizeof(T);
    }
    else {
        std::cout << "Something with the database went wrong.\n";
        return -1;
    }
    
    
    
}


bool Database::findByToken(char token[HEX_LEN], const User* user){
    
    if(users){
        users.seekg(0, users.beg);
        while(users.read((char*)user, sizeof(User))){
            
            if(user->is_deleted) continue;
            
            if(compare_strings(user->cookie_token, token)){
                std::cout << "The user has been found by their token\n" << user << '\n';
                // Reset the stream's error state flags
                users.clear();
                return true;
            }
        }
        // Reset the stream's error state flags
        users.clear();
        std::cout << "The found user " << user->username << " has NOT been found\n";
        return false; 
    }
    else std::cout << "Something with the database went wrong.\n";
    return false; 
}