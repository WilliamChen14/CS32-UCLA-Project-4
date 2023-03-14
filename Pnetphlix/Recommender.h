#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "UserDatabase.h"
#include "MovieDatabase.h"

class UserDatabase;
class MovieDatabase;

using namespace std;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count);

  private:
  int movieCompatibility[40000];
  int initializedMovies[40000];
  int totalInitialized;

  vector<string> watchHistory;
  bool hasWatched(int id);

  UserDatabase udb;
  MovieDatabase mdb;

  void addDirectorCompat(string id);
  void addActorCompat(string id);
  void addGenreCompat(string id);

  bool alreadyInitialized(int id);

  int conCat(string id);
  string unConCat(int i);


  void orderRating(vector<MovieAndRank> mad);
  void orderAlpha(vector<MovieAndRank> mad);
  
};

#endif // RECOMMENDER_INCLUDED
