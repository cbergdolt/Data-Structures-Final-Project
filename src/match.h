/* match.h
 * This is the header file for User class
 * Contains the declaration and implementation of the User functions
*/

#pragma once

// Include -------------------------------------
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;


// Contains stats on users---------------------------
struct Data {
    set<pair<string, string> > sameSongs;      // overlap of users' songs
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
        User(); // Constructor, should we include a destructor?
        set<pair <string, string> > getSongs();
        priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> getArtists();
        map<string, string> getArtistIds();
        pair<string, string> getName();
        void store_data(string filename);  // Stores user's songs and artists
        void storeArtistData(string filename); // Stores user's songs and artists
	     // returns set of overlapping songs b/w 2 users
        set<pair<string, string> > compare_songs(set<pair<string, string> > s);
	     // returns map of overlapping artists and "rank" of artist
        map<string, int> compare_artists(map<string, int> a);
    private:
        pair<string, string> name; //user name; potentially needed for outputting purposes
        set<pair <string, string> > songs;
        priority_queue< pair <string, int>, vector<pair <string, int> >, cmpfunc> artists;
        map<string, string> artist_ids;
};


// Implementation -------------------------------------
User::User() {
    set<pair<string, string> > s;
    priority_queue<pair<string, int>, vector<pair<string, int> >, cmpfunc> p;
    map<string, string> a;
    songs = s;
    artists = p;
    artist_ids = a;
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

pair<string, string> User::getName() {
    return name;
}

 // Store user's songs and artists into data structures using output files from python script
void User::store_data(string filename) {
    ifstream data;
    data.open(filename.c_str());
    if (data.good()) {
        map<string, int> tmpArt;
        string artist;
        string song;
        getline(data, name.first, ' ');
        getline(data, name.second);
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
set<pair<string, string> > User::compare_songs(set<pair<string, string> > s){
    set<pair<string, string> > commonSongs;
    cout << "\n\nCOMPARING SONGS..." << endl;
    set_intersection(songs.begin(), songs.end(), s.begin(), s.end(), inserter(commonSongs, commonSongs.begin()));
    if (commonSongs.size() == 0) {
        cout << "\nSorry, there are no common songs between these two users." << endl;
    } else {
        cout << "There's a match! Here's a list of the common songs between these two users:" << endl;
        cout << "---------------------------------------------------------------------------" << endl;
        for(auto it= commonSongs.begin(); it != commonSongs.end(); it++){
            cout << it->first << " - "<< it->second << endl;
        }
        cout << "---------------------------------------------------------------------------" << endl;
    }
    return commonSongs;
}

// vim: set sts=4 sw=4 ts=8 expandtab ft=cpp:
