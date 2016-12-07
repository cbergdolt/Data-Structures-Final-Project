#python script to generate two txt files of playlists
# (one for each user)
import sys
import spotipy
import spotipy.util as util

def save_tracks(tracks, userfile):
    for i, item in enumerate(tracks['items']):
        track = item['track']
        userfile.write(track['artists'][0]['name'].encode('utf8')) # track artist
	userfile.write(":") # artist/track separator
	userfile.write(track['name'].encode('utf8')) # track name
	userfile.write("\n") # newline


if __name__ == '__main__':
    if len(sys.argv) > 2:
        username = sys.argv[1]
        username2 = sys.argv[2]
    else:
        print "Please supply two usernames"
        print "usage: python playlistContents.py [username1] [username2]"
        sys.exit()


    token = util.prompt_for_user_token(username)

    if token:
        sp = spotipy.Spotify(auth=token)
        playlists = sp.user_playlists(username)
	user1 = open("data/user1.txt", 'w') # open file for write only
	userinfo = sp.user(username)
	user1.write(userinfo['display_name'])
	user1.write("\n")
        for playlist in playlists['items']:
            if playlist['owner']['id'] == username:
                results = sp.user_playlist(username, playlist['id'],
                    fields="tracks,next")
                tracks = results['tracks']
                save_tracks(tracks, user1)
                while tracks['next']:
                    tracks = sp.next(tracks)
                    save_tracks(tracks, user1)
	user1.close() # close file
    else:
        print "Can't get token for", username


    token2 = util.prompt_for_user_token(username2)

    if token2:
        sp2 = spotipy.Spotify(auth=token2)
	playlists2 = sp2.user_playlists(username2)
	user2 = open("data/user2.txt", 'w') # open file for write only
	userinfo2 = sp2.user(username2)
        user2.write(userinfo2['display_name'])
        user2.write("\n")
	for playlist2 in playlists2['items']:
            if playlist2['owner']['id'] == username2:
        	results2 = sp2.user_playlist(username2, playlist2['id'], fields="tracks,next")
	        tracks2 = results2['tracks']
           	save_tracks(tracks2, user2)
            	while tracks2['next']:
            	    tracks2 = sp2.next(tracks2)
            	    save_tracks(tracks2, user2)
    	user2.close() #close file
    else:
	print "Can't get token for", username2

