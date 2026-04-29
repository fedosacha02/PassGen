#include "../User/user.h"
#include "../Group/group.h"
#include "../Organisation/organisation.h"
#include "../Password/password.h"
#include "../Password/password_sharing.h"

class Database{
    private:
        User users[USERS_LIMIT];
        Group groups[GROUPS_LIMIT];
        Organisation organisations[ORGANISATIONS_LIMIT];
        Password passwords[PASSWORDS_LIMIT];
        PasswordSharingToGroup password_sharings_to_group[PASSWORD_SHARINGS_TO_GROUP_LIMIT];
        PasswordSharingToOrganisation password_sharings_to_organisation[PASSWORD_SHARINGS_TO_ORGANISATION_LIMIT];
        
 
};