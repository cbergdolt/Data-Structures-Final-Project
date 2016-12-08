/* musicMatch.cpp
 * Main function
 * Runs python script that gathers info
 * Calls match.h to process the info
*/

#include <cstring>
#include <stack>
#include "match.h"
#include <iostream>

using namespace std;

// Data structures for djiktra's algorithm
struct frontierElement {
  int cost;
  pair<string,string> name;
  pair<string,string> prev;
};

class compareFrontier {
public:
    bool operator()(frontierElement& element1, frontierElement& element2) {
      return element1.cost > element2.cost;
    }
};

// Function to find neighbors (related artists) by executing python script. Creates and returns a set<string, string> of artists and artist ids.
set< pair<string, string> > findNeighbors(frontierElement node) {
  set< pair <string, string> > neighbors;
  string user1TopArtistId;
  user1TopArtistId = node.name.second;
  // Call relatedArtists.py to identify related artists of user1's top artist
  string command = "python src/relatedArtists.py " + user1TopArtistId;
  FILE * f = popen(command.c_str(), "r"); // Use c_str() to convert to string to const char *
  if (f == 0) {
      fprintf(stderr, "Could not execute\n");
      return neighbors;   // TODO check this. empty for sure? better way?
  }
  // string artistName;
  // string artistId;
  // while (getline(f, artistName, ':')) {
  //   getline(f, artistId);
  //   neighbors.insert(make_pair(artistName, artistId));
  // }
  const int BUFSIZE = 200;
  char artistName[ BUFSIZE ];
  char artistId[ BUFSIZE ];
  while(fgets(artistName, BUFSIZE, f)) {
      artistName[strcspn(artistName, "\n")] = 0;
      fgets(artistId, BUFSIZE, f);
      artistId[strcspn(artistId, "\n")] = 0;
      neighbors.insert(make_pair(artistName, artistId));
  }
  pclose(f);
  return neighbors;
}

int main(int argc, char *argv[]) {
    // Run python script to generate output files of data


    // Instantiate User objects
    Data d;
    User user1;
    User user2;

    // Store data (artist and songs, artist and artist ids)
    user1.store_data("data/user1_artist_song.txt");
    user2.store_data("data/user2_artist_song.txt");
    user1.storeArtistData("data/user1_artist_id.txt");
    user2.storeArtistData("data/user2_artist_id.txt");

    // Find common songs between the 2 users
    d.sameSongs = user1.compare_songs(user2.getSongs());

    // Use priority queue (artists) to identify top artist of user1 and user2
    pair <string, int> top_artist_user1;
    pair <string, int> top_artist_user2;
    top_artist_user1 = user1.getArtists().top();
    cout << "Top Artist (user1): " << top_artist_user1.first << " " << endl;
    top_artist_user2 = user2.getArtists().top();
    cout << "Top Artist (user2): " << top_artist_user2.first << " " << endl;

    // Use artist-artist id map (artist-ids) to match top artist to its id
    string user1TopArtistName;
    string user1TopArtistId;
    string user2TopArtistName;
    string user2TopArtistId;
    map<string, string> artistIds;
    artistIds = user1.getArtistIds();
    for(auto it = artistIds.begin(); it != artistIds.end(); it++) {
      if (it->first == top_artist_user1.first) {
        cout << "Id of artist (user1) " << it->first << " is " << it->second << endl;
        user1TopArtistName = it->first;
        user1TopArtistId = it->second;
      }
    }
    artistIds = user2.getArtistIds();
    for(auto it = artistIds.begin(); it != artistIds.end(); it++) {
      if (it->first == top_artist_user2.first) {
        cout << "Id of artist (user2) " << it->first << " is " << it->second << endl;
        user2TopArtistName = it->first;
        user2TopArtistId = it->second;
      }
    }

    // Set up data structures for djiktra's algorithm to find distance between two artists and intermediate artists
    priority_queue<frontierElement, vector<frontierElement>, compareFrontier> frontier; // cost, name, previous
    map<pair<string, string>, pair<string, string>> marked; // Matches target and source (each having an artist name and id)
    set<pair<string, string> > neighbors; // Each neighbor has its artist name and id
    bool destFound = false;
    int levelsDeep = 0;

    // Perform djiktra's algorithm
    pair<string, string> start = make_pair(user1TopArtistName, user1TopArtistId);
    pair<string, string> end = make_pair(user2TopArtistName, user2TopArtistId);
    frontier.push({0, start, start});
    cout << "End is: (" << end.first << ", " << end.second << ") " << endl;

    // Add neighbors to frontier
    frontierElement curr = frontier.top();  // TODO pop?
    neighbors = findNeighbors(curr);

    while (!frontier.empty() && !destFound && levelsDeep<=16) { //TODO modify levels down
      cout << "LEVELS DEEP: " << levelsDeep << endl;
      frontierElement curr = frontier.top();
      frontier.pop();
      cout << endl << "Popped { " << curr.cost << ", (" << curr.name.first << "," << curr.name.second <<"), (" << curr.prev.first << "," << curr.prev.second << ") }" << endl;
      if (! marked.count(curr.name)) {
        // Add current to marked
        marked.insert(pair<pair<string, string>, pair<string,string>>(curr.name, curr.prev));
        cout << "Inserting into MARKED: (" << curr.name.first << "," << curr.name.second << ") : (" << curr.prev.first << "," << curr.prev.second << ") " << endl;
        // Add neighbors to frontier
        neighbors = findNeighbors(curr);
        for (set<pair<string, string>>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {   // TODO auto?
          frontier.push({1+curr.cost, *it, curr.name});
          cout << "Inserting into FRONTIER: { " << 1+curr.cost<< ", (" << (*it).first << ", " << (*it).second << "), (" << curr.name.first << ", " << curr.name.second << ") }" << endl;
	  levelsDeep=1+curr.cost;
          if (end == *it) {
            cout << "FOUND MATCH" << endl;
            destFound = true;
            marked.insert(pair<pair<string, string>, pair<string,string>>(*it, curr.name));
            cout << "Inserting into MARKED: (" << (*it).first << "," << (*it).second << ") : (" << curr.name.first << "," << curr.name.second << ") " << endl;
            break;  // TODO yes?
          }
        } // Done iterating through neighbors
      }   // Finished !marked()
      cout << "---------------------------------------------------------------------------" << endl;
    }
    if (destFound) {
      cout << "PATH FOUND:" << endl;
      //Reconstruct path
      int distance = 0;
      stack <pair<string, string> > path;
      while (end != start) {
        path.push(end);
        end = marked[end];
        distance++;
      }
      path.push(start);
      while (!path.empty()) {
        cout << path.top().first << " " << path.top().second << endl;
        path.pop();
      }
    }
    else {
      cout << "We're sorry, we could not find a sufficiently close relationship between your two top artists." << endl;
    }



    //d.sameArtists = user1.compare_artists(user2.songs);
    // Analyze data
    // compare user.songs.size() with d.sameSongs.data()
    // ^ ditto for artists
    // print top overlapping artists

    return 0;
}
