#pragma once
#include "stdint.h"

/*
    Constraints (for the free plan):
    User:
        1. can create 10 passwords maximum;
        2. can create 3 groups maximum;
        3. can have 3 folders maximum (1 minimum);
    Group:
        1. 5 members maximum;
        2. 3 shared passwords maximum;
        3. 3 folders maximum (1 minimum);
    Group admin:
        1. can create 1 organisation maximum.
    Organisation:
        1. has 4 group-members maximum (1 minimum);
        2. has 3 folders maximum (1 minimum);
        3. has 12 shared password maximum (3 x 4 = 12)
*/



// The constraints for the Free Plan

// User
#define USER_PASSWORDS_LIMIT 10
#define USER_CREATION_GROUP_LIMIT 3
#define USER_FOLDERS_LIMIT 3
// Group
#define GROUP_MEMBERS_LIMIT 5
#define GROUP_SHARED_PASSWORDS_LIMIT 3
#define GROUP_FOLDERS_LIMIT 3
// Group admin
#define GROUP_ADMIN_ORGANISATION_LIMIT 1
//Organisation
#define ORGANISATION_MEMBERS_LIMIT 4
#define ORGANISATION_FOLDERS_LIMIT 3
#define ORGANISATION_SHARED_PASSWORDS_LIMIT (ORGANISATION_MEMBERS_LIMIT*GROUP_SHARED_PASSWORDS_LIMIT)
// Length limits
#define USERNAME_LENGTH_LIMIT 30
#define EMAIL_LENGTH_LIMIT 30
#define NAME_LENGTH_LIMIT 30
#define PASSWORD_LENGTH_LIMIT 20


//Database limits
#define USERS_LIMIT 100
#define GROUPS_LIMIT 30
#define ORGANISATIONS_LIMIT 10
#define PASSWORDS_LIMIT (USERS_LIMIT*USER_PASSWORDS_LIMIT)
#define PASSWORD_SHARINGS_TO_GROUP_LIMIT (GROUPS_LIMIT*GROUP_SHARED_PASSWORDS_LIMIT)
#define PASSWORD_SHARINGS_TO_ORGANISATION_LIMIT (ORGANISATIONS_LIMIT*ORGANISATION_SHARED_PASSWORDS_LIMIT)
#define ALL_USER_FOLDERS_LIMIT (USERS_LIMIT*USER_FOLDERS_LIMIT)


// The type of IDs
using ID = uint32_t;