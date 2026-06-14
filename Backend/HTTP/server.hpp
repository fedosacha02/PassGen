#pragma once
#include "../config.hpp"
namespace HTTP{

struct UserCredentials{
    char username[USERNAME_LENGTH_LIMIT];
    char email[EMAIL_LENGTH_LIMIT];
    char password[PASSWORD_LENGTH_LIMIT];
    char repeat_password[PASSWORD_LENGTH_LIMIT];
};


}
