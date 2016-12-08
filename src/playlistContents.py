#python script to generate four txt files:
#  1. user1 artist:track
#  2. user1 artist:artistID
#  3. user2 artist:track
#  4. user2 artist:artistID
#Takes 2 arguments, the Spotify user IDs for two Spotify users

import sys
import spotipy
import spotipy.util as util

def save_tracks(tracks, userfile):
    for i, item in enumerate(tracks['items']):
        track = item['track']
        userfile.write(track['artists'][0]['name'].encode('utf8')) #track artist
	userfile.write(":") # artist/track separator
	userfile.write(track['name'].encode('utf8')) # track name
	userfile.write("\n") # newline

def save_artist(tracks, userfile):
    for i, item in enumerate(tracks['items']):
        track = item['track']
        artists = track['artists']
        artist_name = artists[0]['name']
        artist_id = artists[0]['id']
        userfile.write(artist_name.encode('utf8')) # track artist
	userfile.write(":") # artist/id separator
	userfile.write(artist_id.encode('utf8')) # id
	userfile.write("\n") # newline


if __name__ == '__main__':
    if len(sys.argv) > 2:
        username = sys.argv[1]
        username2 = sys.argv[2]
    else:
        print "Please supply two usernames"
        print "usage: python playlistContents.py [username1] [username2]"
        sys.exit()


    print "USER 1"
    token = util.prompt_for_user_token(username)
    if token:
        sp = spotipy.Spotify(auth=token)
        userinfo = sp.user(username)
        print userinfo['display_name']
        playlists = sp.user_playlists(username)
    	user1_artist_song = open("data/user1_artist_song.txt", 'w') # open file for write only
        user1_artist_id = open("data/user1_artist_id.txt", 'w') # open file for write only
	user1_artist_song.write(userinfo['display_name'].encode('utf8'))
	user1_artist_song.write("\n")
        for playlist in playlists['items']: # fill data files
            if playlist['owner']['id'] == username:
                results = sp.user_playlist(username, playlist['id'], fields="tracks,next")
                tracks = results['tracks']
                save_tracks(tracks, user1_artist_song)
		save_artist(tracks, user1_artist_id)
                while tracks['next']:
                    tracks = sp.next(tracks)
                    save_tracks(tracks, user1_artist_song)
                    save_artist(tracks, user1_artist_id)
    	user1_artist_song.close() # close file
        user1_artist_id.close()
    else:
        print "Can't get token for", username


    print "USER 2"
    token2 = util.prompt_for_user_token(username2)
    if token2:
        sp2 = spotipy.Spotify(auth=token2)
        userinfo = sp2.user(username2)
        print userinfo['display_name']
    	playlists2 = sp2.user_playlists(username2)
    	user2_artist_song = open("data/user2_artist_song.txt", 'w') # open file for write only
        user2_artist_id = open("data/user2_artist_id.txt", 'w') # open file for write only
	user2_artist_song.write(userinfo['display_name'].encode('utf8'))
	user2_artist_song.write("\n")
    	for playlist2 in playlists2['items']: #fill data files
       	    if playlist2['owner']['id'] == username2:
                results2 = sp2.user_playlist(username2, playlist2['id'], fields="tracks,next")
        	tracks2 = results2['tracks']
                save_tracks(tracks2, user2_artist_song)
		save_artist(tracks2, user2_artist_id)
                while tracks2['next']:
                    tracks2 = sp2.next(tracks2)
               	    save_tracks(tracks2, user2_artist_song)
	      	    save_artist(tracks2, user2_artist_id)
    	user2_artist_song.close() #close file
        user2_artist_id.close()

    else:
        print "Can't get token for", username2
