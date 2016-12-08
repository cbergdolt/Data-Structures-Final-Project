/* musicMatch.cpp
 * Main function
 * Runs python script that gathers info
 * Calls match.h to process the info
*/

#include "match.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

    // Run python script to generate output files of data


    // Store data & compare
    Data d;
    User user1;
    User user2;
    user1.store_data("data/user1.txt");
    user2.store_data("data/user2.txt");
    cout << "stored data" << endl;

    // d.sameSongs = user1.compare_songs(user2.getSongs());

    priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> ranked_artists_user1;
    pair <string, int> top_artist_user1;
    ranked_artists_user1 = user1.getArtists();    // priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> User::getArtists()
    cout << "got artists data" << endl;
    if (ranked_artists_user1.empty()) {
      cout << "empty" << endl;
    }
    top_artist_user1 = ranked_artists_user1.top();

    cout << top_artist_user1.first << " " << top_artist_user1.second << endl;

    //d.sameArtists = user1.compare_artists(user2.songs);
    // Analyze data
    // compare user.songs.size() with d.sameSongs.data()
    // ^ ditto for artists
    // print top overlapping artists

    return 0;
}
