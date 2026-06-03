#include "../config.hpp"

class Folder{
    private:
        static ID count;
        const ID id;
        const ID owner_id; // It can be a user, a group, or an organisation
        enum target{
            user,
            group, 
            organisation
        };
        target type = user;

        char name[NAME_LENGTH_LIMIT];
        ID passwords[USER_PASSWORDS_LIMIT];
    public:
        template <typename T>Folder(name[NAME_LENGTH_LIMIT], T* owner);

        // Overloading of << operator function
        friend std::ostream& operator<<(std::ostream& os, const Folder* folder);

        ~Folder();

         
};
class UserFolder;
class GroupFolder;
class OrganisationFolder;