#pragma once
#include "../config.hpp"
namespace HTTP{

struct UserCredentials{
    char email[EMAIL_LENGTH_LIMIT];
    char password[PASSWORD_LENGTH_LIMIT];
};


}
