/* match.cpp
 * Contains implementation of User class
 * Has all methods to be performed on user data:
 *      - Store data
 *      - Compare data
 */

#include "match.h"
using namespace std;

User::User() {
    set<string> s;
    map<string, int> m;
    songs = s;
    artists = m;
}

// Store user's songs and artists into data structures using output files from python script
void User::store_data() {


}

// Return a set of songs that are in both users' playlists
set<string> User::compare_songs() {

} 


set<string> User::compare_artists() {

}


