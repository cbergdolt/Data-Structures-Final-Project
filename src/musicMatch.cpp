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
    user1.store_data("user1.txt");
    user2.store_data("user2.txt");


    d.sameSongs = user1.compare_songs(user2.getSongs());

    //d.sameArtists = user1.compare_artists(user2.songs);
    // Analyze data 
    // compare user.songs.size() with d.sameSongs.data()
    // ^ ditto for artists
    // print top overlapping artists

    return 0;
}
