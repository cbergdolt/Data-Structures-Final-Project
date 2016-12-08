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


//#pragma once

// Include -------------------------------------
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

// Include client ID, secret, redirect as constants to use when we call the python script?

// Contains stats on users
struct Data {
    set<pair<string, string>> sameSongs;      // overlap of users' songs
    set<string> sameArtists;    // overlap of users' artists
};


// Classes -------------------------------------------
class cmpfunc{
 public:
  bool operator()(pair<string, int> a, pair<string, int> b){
    if (a.second < b.second) return true;
    else return false;
  }
};
class User {
  public:
    User();                                     // Constructor, should we include a destructor?
    set<pair <string, string> > getSongs();
    priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> getArtists();
    map<string, string> getArtistIds();
    void store_data(string filename);           // Stores user's songs and artists
    void storeArtistData(string filename);           // Stores user's songs and artists
    set<pair<string, string>> compare_songs(set<pair<string, string>> s);   // returns set of overlapping songs b/w 2 users
    map<string, int> compare_artists(map<string, int> a); // returns map of overlapping artists and "rank" of artist
  private:
    //set<string> songs;          // This contains user's songs
    set<pair <string, string> > songs;
    //map<string, int> artists;
    priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> artists;
    map<string, string> artist_ids;
    //string picture;           // Possibly save url to user's picture for output purposes?
};


// Implementation -------------------------------------
User::User() {
    set<pair<string, string> > s;
    priority_queue<pair<string, int>, vector<pair<string, int>>, cmpfunc> p;
    map<string, string> a;    /// TODO need this, what does this do???
    songs = s;
    artists = p;
    artist_ids = a;   /// TODO need this, what does this do???
}

set<pair<string, string> > User::getSongs(){
  return songs;
}

priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> User::getArtists(){
  return artists;
}

map<string, string> User::getArtistIds() {
  return artist_ids;
}


 // Store user's songs and artists into data structures using output files from python script
void User::store_data(string filename) {
  ifstream data;
  data.open(filename.c_str());
  if (data.good()) {
    map<string, int> tmpArt;
    string artist;
    string song;
    // Read in user data and make map of artist and song count
    while (getline(data, artist, ':')) {
      getline(data, song);
      songs.insert(make_pair(song, artist));
      if (tmpArt.count(artist) != 0) {     // artist already in map
        tmpArt[artist]++;
      } else {
        tmpArt.insert(pair<string, int>(artist, 1));
      }
    }
    // Insert map elements into priority queue
    for(map<string, int>::iterator it = tmpArt.begin(); it != tmpArt.end(); it++){
      artists.push(*it); //might need *
    }
    data.close();
  }
}


// Store artist data in a map. Save map to allow for access to artist ids.
void User::storeArtistData(string filename) {
  ifstream artist_data;
  artist_data.open(filename.c_str());
  if (artist_data.good()) {
    string artist;
    string id;
    // Read in user data and make map of artist and song count
    while (getline(artist_data, artist, ':')) {
      getline(artist_data, id);
      artist_ids[artist] = id;
    }
    artist_data.close();
  }
}


// Return a set of pairs (song, artist) that are in both users' playlists
set<pair<string, string>> User::compare_songs(set<pair<string, string>> s){
  set<pair<string, string>> commonSongs;
  cout << "comparing songs" << endl;
  set_intersection(songs.begin(), songs.end(), s.begin(), s.end(), inserter(commonSongs, commonSongs.begin()));
  for(auto it= commonSongs.begin(); it != commonSongs.end(); it++){
    cout << it->first << " "<< it->second << endl;
  }
  return commonSongs;
}

/*
set<string> User::compare_songs(set<string> s) {
  set<string> commonSongs;
  set_intersection(songs.begin(), songs.end(), s.begin(), s.end(), inserter(commonSongs, commonSongs.begin()));
  for(auto it= commonSongs.begin(); it != commonSongs.end(); it++){
    cout << *it << endl;
  }
  return commonSongs;
}
*/

//map<string, int> User::compare_artists(map<string, int> a) {

//}
