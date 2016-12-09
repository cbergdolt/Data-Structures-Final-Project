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

// Print stats function
void print_stats(User u1, User u2, set<pair<string, string>> common) {
    float u1percent = 100 * ((float)common.size()/(float)u1.getSongs().size());
    float u2percent = 100 * ((float)common.size()/(float)u2.getSongs().size());
    cout << "\n" << u1.getName().first << " listens to " << u2percent << "% of "<< u2.getName().first<< "'s songs" << endl;
    cout << u2.getName().first << " listens to " << u1percent << "% of "<< u1.getName().first <<"'s songs\n"<< endl;
    pair <string, int> top_artist_user1;
    pair <string, int> top_artist_user2;
    top_artist_user1 = u1.getArtists().top();
    cout << "\n\nCOMPARING ARTISTS..." << endl;
    cout << "Here are your top artists:"<< endl;
    cout << "---------------------------------------------------------------------------" << endl;
    cout << u1.getName().first << "'s Top Artist: " << top_artist_user1.first << " " << endl;
    top_artist_user2 = u2.getArtists().top();
    cout << u2.getName().first << "'s Top Artist: " << top_artist_user2.first << " \n" << endl;
}

// Function to find neighbors (related artists) by executing python script. Creates and returns a set<string, string> of artists and artist ids.
set< pair<string, string> > findNeighbors(frontierElement node, char * neighborQuantity) {
  set< pair <string, string> > neighbors;
  string user1TopArtistId;
  user1TopArtistId = node.name.second;
  // Call relatedArtists.py to identify related artists of user1's top artist
  string command = "python src/relatedArtists.py " + user1TopArtistId + " " + neighborQuantity;
  FILE * f = popen(command.c_str(), "r"); // Use c_str() to convert to string to const char *
  if (f == 0) {
      fprintf(stderr, "Could not execute\n");
      return neighbors;   // TODO check this. empty for sure? better way?
  }
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

// Artist analysis Function
void analyze_artists(User user1, User user2, char * neighborQuantity) {
  // Use priority queue (artists) to identify top artist of user1 and user2
  pair <string, int> top_artist_user1;
  pair <string, int> top_artist_user2;
  top_artist_user1 = user1.getArtists().top();
  top_artist_user2 = user2.getArtists().top();

  // Use artist-artist id map (artist-ids) to match top artist to its id
  string user1TopArtistName;
  string user1TopArtistId;
  string user2TopArtistName;
  string user2TopArtistId;
  map<string, string> artistIds;
  artistIds = user1.getArtistIds();
  for(auto it = artistIds.begin(); it != artistIds.end(); it++) {
    if (it->first == top_artist_user1.first) {
      user1TopArtistName = it->first;
      user1TopArtistId = it->second;
    }
  }
  artistIds = user2.getArtistIds();
  for(auto it = artistIds.begin(); it != artistIds.end(); it++) {
    if (it->first == top_artist_user2.first) {
      user2TopArtistName = it->first;
      user2TopArtistId = it->second;
    }
  }

  cout << "We are going to look for a relationship between your top artists." << endl;

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

  // Add neighbors to frontier
  frontierElement curr = frontier.top();
  neighbors = findNeighbors(curr, neighborQuantity);
  int currLevelDeep = 0;

  while (!frontier.empty() && !destFound && levelsDeep<16) {
    // cout << "LEVELS DEEP: " << levelsDeep << endl;
    frontierElement curr = frontier.top();
    frontier.pop();
    // cout << endl << "Popped { " << curr.cost << ", (" << curr.name.first << "," << curr.name.second <<"), (" << curr.prev.first << "," << curr.prev.second << ") }" << endl;
    if (! marked.count(curr.name)) {
      // Check if it is going into a new level
      if (curr.cost > currLevelDeep) {
        cout << "\n----- Going into degree #" << levelsDeep << " of separation... -----" << endl;
        currLevelDeep = curr.cost;
      }
      // Add current to marked
      marked.insert(pair<pair<string, string>, pair<string,string>>(curr.name, curr.prev));
      // cout << "Inserting into MARKED: (" << curr.name.first << "," << curr.name.second << ") : (" << curr.prev.first << "," << curr.prev.second << ") " << endl;
      // Add neighbors to frontier
      cout << "Searching for artists related to " << curr.name.first << "." << endl;
      neighbors = findNeighbors(curr, neighborQuantity);
      for (set<pair<string, string>>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {   // TODO auto?
        frontier.push({1+curr.cost, *it, curr.name});
        // cout << "Inserting into FRONTIER: { " << 1+curr.cost<< ", (" << (*it).first << ", " << (*it).second << "), (" << curr.name.first << ", " << curr.name.second << ") }" << endl;
        // Update levelsDeep
        levelsDeep=1+curr.cost;
        if (end == *it) {c
          destFound = true;
          marked.insert(pair<pair<string, string>, pair<string,string>>(*it, curr.name));
          // cout << "Inserting into MARKED: (" << (*it).first << "," << (*it).second << ") : (" << curr.name.first << "," << curr.name.second << ") " << endl;
          break;  // Will only get the first match (even if there is another match on the same level)
        }
      } // Done iterating through neighbors
    }   // Finished !marked()
    // cout << "---------------------------------------------------------------------------" << endl;
  }
  if (destFound) {
    cout << "\nWe found a path between your top artists!" << endl << "These artists you both might like!" << "\nHere it is: " << endl;
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
      cout << "\t" << path.top().first << endl;
      path.pop();
    }
  }
  else {
    cout << "We're sorry, we could not find a sufficiently close relationship between " << user1TopArtistName << " and " << user2TopArtistName << "! " << endl;
  }
}

int main(int argc, char *argv[]) {
    char * neighborQuantity;
    if (argc == 2) {
      neighborQuantity = argv[1];
    }
    else {
      cout << "usage: ./musicMatch [neighborQuantity]" << endl;
      cout << "neighborQuantity refers to the number of related artists to consider per visited artist" << endl;
      return 0;
    }

    // Welcome user
    cout << "WELCOME TO MUSIC MATCH" << endl;

    system("scroll -lps=300 data/asciiart.txt");

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

    // Print stats of common songs
    print_stats(user1, user2, d.sameSongs);

    // Analyze artist compatability
    analyze_artists(user1, user2, neighborQuantity);

    return 0;
}
