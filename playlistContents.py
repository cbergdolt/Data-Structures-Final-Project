import sys
import spotipy
import spotipy.util as util

def show_tracks(tracks):
    for i, item in enumerate(tracks['items']):
        track = item['track']
        print "   %d %32.32s %s" % (i, track['artists'][0]['name'],
            track['name'])


if __name__ == '__main__':
    if len(sys.argv) > 1:
        username = sys.argv[1]
        username2 = sys.argv[2]
    else:
        print "Whoops, need your username!"
        print "usage: python user_playlists.py [username]"
        sys.exit()


    token = util.prompt_for_user_token(username)

    if token:
        sp = spotipy.Spotify(auth=token)
        playlists = sp.user_playlists(username)
        for playlist in playlists['items']:
            if playlist['owner']['id'] == username:
                print
                print playlist['name']
                print '  total tracks', playlist['tracks']['total']
                results = sp.user_playlist(username, playlist['id'],
                    fields="tracks,next")
                tracks = results['tracks']
                show_tracks(tracks)
                while tracks['next']:
                    tracks = sp.next(tracks)
                    show_tracks(tracks)
    else:
        print "Can't get token for", username



    playlists2 = sp.user_playlists(username2)
    for playlist2 in playlists2['items']:
        if playlist2['owner']['id'] == username2:
            print
            print playlist2['name']
            print '  total tracks', playlist2['tracks']['total']
            results2 = sp.user_playlist(username2, playlist2['id'],
                fields="tracks,next")
            tracks2 = results2['tracks']
            show_tracks(tracks2)
            while tracks2['next']:
                tracks2 = sp.next(tracks2)
                show_tracks(tracks2)



