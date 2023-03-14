#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <iostream>

using namespace std;

MovieDatabase::MovieDatabase()
{
    isLoaded = false;
}

bool MovieDatabase::load(const string& filename)
{
    if(isLoaded){
        return false;
    }
    ifstream file;
    file.open(filename);

    string line;

    while (getline(file, line)) {
        string id;
        string title;
        string year;
        vector<string> directors;
        vector<string> actors;
        vector<string> genres;
        float rating;

        for (int i = 0; i < line.size(); i++) {
            id += tolower(line[i]);
        }

        getline(file, line);
        title = line;

        getline(file, line); 
        year = line;

        getline(file, line);
        string director = ""; 
        for (int i = 0; i < line.size(); i++) {

            if (line[i] != ',') {
                director += tolower(line[i]);
            }

            else {
                directors.push_back(director);
                director = "";
            }
        }                
        directors.push_back(director);

        getline(file, line);
        string actor = "";
        for (int i = 0; i < line.size(); i++) {

            if (line[i] != ',') {
                actor += tolower(line[i]);
            }

            else {
                actors.push_back(actor);
                actor = "";
            }
        }
        actors.push_back(actor);

        getline(file, line);
        string genre = "";
        for (int i = 0; i < line.size(); i++) {

            if (line[i] != ',') {
                genre += tolower(line[i]);
            }

            else {
                genres.push_back(genre);
                genre = "";
            }
        }
        genres.push_back(genre);

        getline(file, line);
        rating = stof(line);

        fullList.push_back(Movie(id, title, year, directors, actors, genres, rating));


        getline(file, line);
    }

    for(int i = 0; i < fullList.size(); i++){
        Movie* t_movie = &fullList[i];

        string t_id = (fullList[i]).get_id();

        keyID.insert(t_id, t_movie);

        for(int j = 0; j < fullList[i].get_directors().size(); j++){
            keyDirector.insert(fullList[i].get_directors()[j], t_movie);
        }

        for(int j = 0; j < fullList[i].get_actors().size(); j++){
            string t_actor = fullList[i].get_actors()[j];
            keyActor.insert(t_actor, t_movie);
        }

        for(int j = 0; j < fullList[i].get_genres().size(); j++){
            string t_genre = fullList[i].get_genres()[j];
            keyGenre.insert(t_genre, t_movie);
        }
    }

    

    isLoaded = true;
    return true;  // Replace this line with correct code.
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string t_id = "";
    for(int i = 0; i < id.size(); i++){
        t_id += tolower(id[i]);
    }

    if(!keyID.find(t_id).is_valid()){
        return nullptr;
    }

    Movie* temp = keyID.find(t_id).get_value();
    return temp;  // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> temp;
    string t_director = "";
    for(int i = 0; i < director.size(); i++){
        t_director += tolower(director[i]);
    }
    if(!keyDirector.find(t_director).is_valid()){
        return temp;
    }
    TreeMultimap<string, Movie*>::Iterator it = keyDirector.find(t_director);
    while(it.is_valid()){
        temp.push_back(it.get_value());
        it.advance();
    }
    return temp;  // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> temp;
    string t_actors = "";
    for(int i = 0; i < actor.size(); i++){
        t_actors += tolower(actor[i]);
    }

    if(!keyActor.find(t_actors).is_valid()){
        return temp;
    }
    TreeMultimap<string, Movie*>::Iterator it = keyActor.find(t_actors);
    while(it.is_valid()){
        temp.push_back(it.get_value());
        it.advance();
    }
    return temp; 
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> temp;
    string t_genre = "";
    for(int i = 0; i < genre.size(); i++){
        t_genre += tolower(genre[i]);
    }
    if(!keyGenre.find(t_genre).is_valid()){
        return temp;
    }
    TreeMultimap<string, Movie*>::Iterator it = keyGenre.find(t_genre);
    while(it.is_valid()){
        temp.push_back(it.get_value());
        it.advance();
    }
    return temp; 
}
