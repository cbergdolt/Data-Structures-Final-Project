import sys
import spotipy
import spotipy.util as util

# export SPOTIPY_CLIENT_ID='your-spotify-client-id'
# export SPOTIPY_CLIENT_SECRET='your-spotify-client-secret'
# export SPOTIPY_REDIRECT_URI='your-app-redirect-url'

scope = 'user-library-read'

if len(sys.argv) > 1:
    username = sys.argv[1]
else:
    print "Usage: %s username" % (sys.argv[0],)
    sys.exit()

token = util.prompt_for_user_token(username, scope) #, client_id='3f17326a1dae4429a85ea44571ea30ad', client_secret='76f2e03f124c485ea7b1e1e720f0061f', redirect_uri='https://www.google.com/')
#print token
#token = 'AQCdOoeyrBC0e94_z48hwAWtQcUBrahAqzT71_baSEOE2siTsBMWquC-3b5D6DJ-wgtzQlBvi2IyZBVMnUhEBd9_nkXLvwrC8dmfW4blVEod5p3FaIWi04rxmXFt7GSXZBfkR-juoCpZSz7KmUsovBUoyMyR6_RlpNt0mLVuhB4cpK-CXi4_6BT-kgLYvpx9AKR3NooYmvQ0aOqT6r8Rsg'

if token:
    sp = spotipy.Spotify(auth=token)
    results = sp.current_user_saved_tracks()
    for item in results['items']:
        track = item['track']
        print track['name'] + ' - ' + track['artists'][0]['name']
else:
    print "Can't get token for", username
