#pragma once 

#include "../config.hpp"
#include "../Password/password.hpp"
#include "../Database/database.hpp"

class User{
    private:
        static ID count;
        const ID id;
        char username[USERNAME_LENGTH_LIMIT];
        char master_password[PASSWORD_LENGTH_LIMIT];
        char email[EMAIL_LENGTH_LIMIT];
        char plan; // F: Free, B; Basic, P: Premium
        bool is_deleted = false;
        
    
    public:
        // Constructors including a copy one and a desctructor
        User();
        User(User& user);
        User(char[USERNAME_LENGTH_LIMIT], char[PASSWORD_LENGTH_LIMIT], char[EMAIL_LENGTH_LIMIT]);

        ~User();
        // As an ordinal user
        void create_user_password(char name[NAME_LENGTH_LIMIT], bool is_blocked);
        void rename_password(ID password_id);
        void folder_user_password(ID password_id, ID folder_id);
        void create_group(char name[NAME_LENGTH_LIMIT]);
        void change_master_password(char old_password[PASSWORD_LENGTH_LIMIT], char new_password[PASSWORD_LENGTH_LIMIT]);
        void ask_join_group(ID group_id);
        void delete_user_account(char master_password[PASSWORD_LENGTH_LIMIT]);
        // As a group member
        void request_for_sharing_password_to_group(ID password_id, ID group_id);
        void unshare_password_from_group(ID password_id, ID group_id);
        void leave_group(ID group_id);
        // As a group administrator
        void invite_user_to_group(ID user_id);
        void accept_user_to_group(ID user_id);
        void delete_group_member(ID user_id);
        void accept_sharing_password_to_group_request(ID password_id);
        void reject_sharing_password_to_group_request(ID password_id);
        void make_group_password_unshared(ID password_id);
        void folder_group_password(ID password_id, ID folder_id);
        ID create_organisation(char name[NAME_LENGTH_LIMIT]);
        void ask_join_organisation(ID group_id);
        void leave_organisation(ID group_id);
        void rename_group(ID group_id, char new_name[NAME_LENGTH_LIMIT]);
        void delete_group(ID target_group);

        // As a superadministrator
        void invite_group_to_organisation(ID group_id);
        void accept_group_to_organisation(ID group_id);
        void delete_group_member_from_organisation(ID group_id);

        void folder_organisation_password(ID password_id, ID folder_id);

        void accept_sharing_password_to_organisation_request(ID password_id);
        void reject_sharing_password_to_organisation_request(ID password_id);
        void make_organisation_password_unshared(ID password_id);

        void rename_organisation(ID target_organisation);
        void delete_organisation(ID target_organisation);

        // The interaction with database
        friend Database;

        // The interaction with passwords
        friend Password;
        
        // Overloading of << operator function
        friend std::ostream& operator<<(std::ostream& os, const User& user);
        bool isReal();
        bool confirmMasterPassword(char password[PASSWORD_LENGTH_LIMIT]);
};



