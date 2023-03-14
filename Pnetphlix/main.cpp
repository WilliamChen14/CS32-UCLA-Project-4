#include "User.h"
#include "UserDatabase.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include <iostream>
#include <string>
#include "treemm.h"
#include "Recommender.h"

#include <chrono>
#include <iostream>
using namespace std;

using namespace std;

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

/*
int main() {
 TreeMultimap<std::string, int> tmm;
 tmm.insert("carey", 5);
 tmm.insert("carey", 6);
 tmm.insert("carey", 7);
 tmm.insert("david", 420);
 tmm.insert("david", 40);

 TreeMultimap<std::string,int>::Iterator it = tmm.find("carey");
 // prints 5, 6, and 7 in some order
 while (it.is_valid()) {
 std::cout << it.get_value() << std::endl;
 it.advance();
 }

 it = tmm.find("david");
 while (it.is_valid()) {
 std::cout << it.get_value() << std::endl;
 it.advance();
 }

 it = tmm.find("laura");
 if (!it.is_valid())
 std::cout << "laura is not in the multimap!\n";
}
*/


int main()
{
	
	UserDatabase udb;
	if (!udb.load(USER_DATAFILE))  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}

	MovieDatabase mdb;
	if (!mdb.load(MOVIE_DATAFILE))  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << MOVIE_DATAFILE << "!" << endl;
		return 1;
	}


	/*
	for (;;)
	{
		cout << "Enter user email address (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		User* u = udb.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
			cout << "Found " << udb.get_user_from_email(email) << endl;


	}
	
	for (;;)
	{
		cout << "Enter user email address (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		Movie* u = mdb.get_movie_from_id(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
			cout << "Found " << mdb.get_movie_from_id(email) << endl;

		cout << u->get_title() << endl;
	}
	*/

	
	Recommender rec = Recommender(udb,mdb);
	for (;;)
	{
		cout << "Enter user email address for recomendation (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		

		cout << "Enter amount for recomendation (or quit): ";
		string recNum;
		getline(cin, recNum);
		if (recNum == "quit")
			return 0;

		int recNu = stoi(recNum);

		auto start = chrono::steady_clock::now();

    	vector<MovieAndRank> recomendations = rec.recommend_movies(email, recNu);

		auto stop = chrono::steady_clock::now();

		cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms" << endl;

		cout << recomendations.size() << " recomendation size" << endl;
		
		for(int i = 0; i < recomendations.size(); i++){
			string m_id = recomendations[i].movie_id;
			
			Movie* m_movie = mdb.get_movie_from_id(m_id);
			if(m_movie == nullptr){
				cout << m_id << endl;
				cout << "error" << endl;
			}
			else{
				cout << m_movie->get_title() << " compatability: " << recomendations[i].compatibility_score <<  endl;
				cout << m_movie->get_rating() << " rating" << endl;

			}
		}
		
		
		User* u = udb.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else{
			cout << "Found " << u << endl;
			cout << u->get_full_name() << endl;
		}

	}
	
}
