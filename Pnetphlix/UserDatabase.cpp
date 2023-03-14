#include "UserDatabase.h"
#include "User.h"
#include "treemm.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
using namespace std;

UserDatabase::UserDatabase()
{
    // Replace this line with correct code.
    isLoaded = false;


}

bool UserDatabase::load(const string& filename)
{
    if(isLoaded){
        return false;
    }
    fstream file;
    file.open(filename);
    string line;


    while(getline(file, line)){
        string name = "";
        string email;
        int totalMovies;
        string movie;
        vector<string> vecMovie;

        name = line;

        getline(file, line);
        email = line;

        getline(file,line);
        totalMovies = stoi(line);

        movie = "";
        for(int i = 0; i < totalMovies; i++){
            getline(file,movie);
            vecMovie.push_back(movie);
            movie = "";
        }
        User temp(name,email,vecMovie);
        
        fullList.push_back(temp);
        
        getline(file, line);
    }

    for(int i = 0; i < fullList.size(); i++){
        User* t_user = &fullList[i];
        string t_email = fullList[i].get_email();

        keyEmail.insert(t_email, t_user);
    }

    isLoaded = true;
    return true;  // Replace this line with correct code.
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    if(!isLoaded){
        return nullptr;
    }


    TreeMultimap<string, User*>::Iterator it = keyEmail.find(email);
    if(it.is_valid()){
        return it.get_value();
    }
    return nullptr;  // Replace this line with correct code.
}
