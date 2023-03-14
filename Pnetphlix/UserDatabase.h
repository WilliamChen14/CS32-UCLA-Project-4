#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include "treemm.h"
#include "Movie.h"

using namespace std;

class User;

class UserDatabase
{
  public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
  bool isLoaded;
  vector<User> fullList;
  TreeMultimap<string, User*> keyEmail;
};

#endif // USERDATABASE_INCLUDED
