/* match.h
 * This is the header file for User class
 * Contains the declaration of the User functions
*/

// Notes -------------------------------------
/* Maybe in addition to showing the compatibility b/w both users
 * we can also include a short "profile" of each user that
 * contains the user's top artists/picture. If we do "top artists"
 * we could include a priority queue (which would be another data
 * structure, which might look good). Just a thought! We can always
 * add it later too if we finish early.
 */


#pragma once

// Include -------------------------------------
#include <iostream>
#include <set>
#include <map>
#include <string>
using namespace std;

// Include client ID, secret, redirect as constants to use when we call the python script?

// Contains stats on users
struct Data {
    set<string> sameSongs;      // overlap of users' songs
    set<string> sameArtists;    // overlap of users' artists
};


// Classes -------------------------------------
class User {
    public:
        User();                                     // Constructor, should we include a destructor?
        void store_data();                          // Stores user's songs and artists
        set<string> compare_songs(set<string> s);   // returns set of overlapping songs b/w 2 users
        map<string, int> compare_artists(map<string, int> a); // returns map of overlapping artists and "rank" of artist
    private:
        set<string> songs;          // This contains user's songs
        map<string, int> artists;   // Contains artist and frequency of that artist, 
                                    //    maybe make it a prioity queue to find top artists??
        //string picture;           // Possibly save url to user's picture for output purposes?
};



