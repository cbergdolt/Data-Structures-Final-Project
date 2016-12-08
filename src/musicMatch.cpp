/* musicMatch.cpp
 * Main function
 * Runs python script that gathers info
 * Calls match.h to process the info
*/

#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
// #include <Python.h>
#include "match.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

    // Run python script to generate output files of data


    // Instantiate User objects
    Data d;
    User user1;
    User user2;

    // Store data (artist and songs, artist and artist ids)
    user1.store_data("data/user1.txt");
    user2.store_data("data/user2.txt");
    user1.storeArtistData("data/user2_artist_id.txt");

    // d.sameSongs = user1.compare_songs(user2.getSongs());

    // Use priority queue (artists) to identify top artist of user1
    priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> ranked_artists_user1;
    pair <string, int> top_artist_user1;
    ranked_artists_user1 = user1.getArtists();
    top_artist_user1 = ranked_artists_user1.top();
    cout << "Top Artist: " << top_artist_user1.first << " " << top_artist_user1.second << endl;
    // Use artist-artist id map (artist-ids) to match top artist to its id
    string user1TopArtistName;
    string user1TopArtistId;
    map<string, string> artistIds;
    artistIds = user1.getArtistIds();
    for(auto it = artistIds.begin(); it != artistIds.end(); it++) {
      if (it->first == top_artist_user1.first) {
        cout << "Id of artist " << it->first << " is " << it->second << endl;
        user1TopArtistName = it->first;
        user1TopArtistId = it->second;
      }
    }

    // Call relatedArtists.py to identify related artists of user1's top artist
    // string command = "python src/relatedArtists.py " + user1TopArtistId;
    // const char * cmd = command.c_str();
    // string output = system("ls");
    // cout << "output is: " << output << endl;

    // Py_SetProgramName(argv[0]);  /* optional but recommended */
    // Py_Initialize();
    // string command = "src/relatedArtists.py " + user1TopArtistId;
    // cout << command << endl;
    // PyRun_SimpleString(command);
    // Py_Finalize();


    //d.sameArtists = user1.compare_artists(user2.songs);
    // Analyze data
    // compare user.songs.size() with d.sameSongs.data()
    // ^ ditto for artists
    // print top overlapping artists

    return 0;
}
