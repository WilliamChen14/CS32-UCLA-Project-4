#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

using namespace std;

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
  bool isLoaded;
  vector<Movie> fullList;
  TreeMultimap<string, Movie*> keyID;
  TreeMultimap<string, Movie*> keyDirector;
  TreeMultimap<string, Movie*> keyActor;
  TreeMultimap<string, Movie*> keyGenre;
};

#endif // MOVIEDATABASE_INCLUDED
