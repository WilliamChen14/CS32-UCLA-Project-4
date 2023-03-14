#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include "treemm.h"
#include <iostream>
#include <chrono>

#include <string>
#include <vector>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    udb = user_database;
    mdb = movie_database;
    totalInitialized = 0;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count)
{
    vector<MovieAndRank> recommendations;
    if(udb.get_user_from_email(user_email) == nullptr){
        return recommendations;
    }
    User* grace = udb.get_user_from_email(user_email);
    watchHistory = grace->get_watch_history();

    for(int i = 0; i < watchHistory.size(); i++){
        string watchedMovie = watchHistory[i];

        string watchedMovieID = "";

        for(int i = 2; i < watchedMovie.size(); i++){
            watchedMovieID += watchedMovie[i];
        }

        int watchedMovieIDInt = stoi(watchedMovieID);


        movieCompatibility[watchedMovieIDInt] = -1;
    }


    for(int i = 0; i < watchHistory.size(); i++){
        string watchedMovie = watchHistory[i];


        addDirectorCompat(watchedMovie);
        addGenreCompat(watchedMovie);
        addActorCompat(watchedMovie);

        
    }
    vector<int> topMovieCount;
    for(int i = 0; i < movie_count; i++){
        topMovieCount.push_back(0);
    }


    cout << "initialized " << totalInitialized << " values" << endl;
    for(int i = 0; i < totalInitialized; i++){
        
        if(movieCompatibility[initializedMovies[i]] > movieCompatibility[topMovieCount[movie_count]]){
            for(int j = movie_count - 1; j >=0; j--){
                if(movieCompatibility[initializedMovies[i]] > movieCompatibility[topMovieCount[j]]){
                    topMovieCount[j+1] =  topMovieCount[j];
                    topMovieCount[j] = initializedMovies[i];
                }
                else if(movieCompatibility[initializedMovies[i]] == movieCompatibility[topMovieCount[j]]){

                    if(mdb.get_movie_from_id(unConCat(initializedMovies[i]))->get_rating() > mdb.get_movie_from_id(unConCat(topMovieCount[j]))->get_rating()){
                        topMovieCount[j+1] =  topMovieCount[j];
                        topMovieCount[j] = initializedMovies[i];
                    }
                    else if(mdb.get_movie_from_id(unConCat(initializedMovies[i]))->get_rating() < mdb.get_movie_from_id(unConCat(topMovieCount[j]))->get_rating()){
                        continue;
                    }
                    else{
                        if(mdb.get_movie_from_id(unConCat(initializedMovies[i]))->get_title() < mdb.get_movie_from_id(unConCat(topMovieCount[j]))->get_title()){
                            topMovieCount[j+1] =  topMovieCount[j];
                            topMovieCount[j] = initializedMovies[i];
                        }
                    }
                }
                else if(movieCompatibility[initializedMovies[i]] < movieCompatibility[topMovieCount[j]]){
                    break;
                }
            }
            
        }
    }
    for(int i = 0; i < movie_count; i++){
        if(topMovieCount[i] == 0){
            return recommendations;
        }
        else{
            string m_id;
            m_id = unConCat(topMovieCount[i]);
            MovieAndRank rec = MovieAndRank(m_id, movieCompatibility[topMovieCount[i]]);
            recommendations.push_back(rec);
        }
    }


    return recommendations;  // Replace this line with correct code.
}

bool Recommender::hasWatched(int id){
    if(movieCompatibility[id] < 0){
        return true;
    }
    return false;
}

int Recommender::conCat(string id){
    if(id.size() < 2){
        return -1;
    }
    string temp = "";
    for(int i = 2; i < id.size(); i++){
        temp += id[i];
    }
    return stoi(temp);
}

string Recommender::unConCat(int i){
    string m_id;
    if(i == 0){
        return "ID00000";
    }
    else if(i < 10){
        m_id = "ID0000" + to_string(i);
    }
    else if(i < 100){
        m_id = "ID000" + to_string(i);
    }
    else if(i < 1000){
        m_id = "ID00" + to_string(i);
    }
    else if(i < 10000){
        m_id = "ID0" + to_string(i);
    }
    else{
        m_id = "ID" + to_string(i);
    }

    return m_id;
}

bool Recommender::alreadyInitialized(int id){
    if(movieCompatibility[id] > 0){
        return true;
    }
    return false;
}

void Recommender::addDirectorCompat(string id){
    vector<string> directors = mdb.get_movie_from_id(id)->get_directors();
    for(int i = 0; i < directors.size(); i++){
        vector<Movie*> sharedDirectorMovie = mdb.get_movies_with_director(directors[i]);
        for(int j = 0; j < sharedDirectorMovie.size(); j++){
            int movieID = conCat(sharedDirectorMovie[j]->get_id());
            string movID = "ID" + to_string(movieID);
            if(!hasWatched(movieID)){
                if(!alreadyInitialized(movieID)){
                    initializedMovies[totalInitialized] = movieID;
                    totalInitialized++;
                }
                movieCompatibility[movieID] += 20;
            }
        }

    }
}
void Recommender::addActorCompat(string id){
    vector<string> actors = mdb.get_movie_from_id(id)->get_actors();
    for(int i = 0; i < actors.size(); i++){
        vector<Movie*> sharedActorMovie = mdb.get_movies_with_actor(actors[i]);
        for(int j = 0; j < sharedActorMovie.size(); j++){
            int movieID = conCat(sharedActorMovie[j]->get_id());
            string movID = "ID" + to_string(movieID);
            if(!hasWatched(movieID)){
                if(!alreadyInitialized(movieID)){
                    initializedMovies[totalInitialized] = movieID;
                    totalInitialized++;
                }
                movieCompatibility[movieID] += 30;
            }
        }

    }
}

void Recommender::addGenreCompat(string id){

    vector<string> genres = mdb.get_movie_from_id(id)->get_genres();

    for(int i = 0; i < genres.size(); i++){
        vector<Movie*> sharedGenreMovie = mdb.get_movies_with_genre(genres[i]);
        
        for(int j = 0; j < sharedGenreMovie.size(); j++){
            
            int movieID = conCat(sharedGenreMovie[j]->get_id());
            string movID = "ID" + to_string(movieID);
            if(!hasWatched(movieID)){
                if(!alreadyInitialized(movieID)){
                    initializedMovies[totalInitialized] = movieID;
                    totalInitialized++;
                }
                movieCompatibility[movieID] += 1;
            }
        }
        

    }

}