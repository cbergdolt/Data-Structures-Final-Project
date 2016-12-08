#python script to generate two txt files of playlists
# (one for each user)
import sys
reload(sys)                         # Avoid UnicodeEncodeError: 'ascii' codec can't encode character u'\xd3' in position 0: ordinal not in range(128)
sys.setdefaultencoding('utf-8')     # Same as above
import spotipy
import spotipy.util as util

if __name__ == '__main__':
    if len(sys.argv) > 1:
        top_artist = sys.argv[1]
    else:
        print "Please supply an artist ID"
        print "usage: python relatedArtists.py [artistID]"
        sys.exit()

    my_output_file = open("data/related_artists.txt", 'w') # open file for write only

    # API Call to retrieve related artist info
    sp = spotipy.Spotify()
    results = sp.artist_related_artists(top_artist)
    related_artists = results['artists']

    # Save related artist info
    for rel_artist_counter in range(0,5):
        try:
            print related_artists[rel_artist_counter]['name']
            print related_artists[rel_artist_counter]['id']
            my_output_file.write(related_artists[rel_artist_counter]['name'].encode('utf8'))
            my_output_file.write(":")
            my_output_file.write(related_artists[rel_artist_counter]['id'].encode('utf8'))
            my_output_file.write("\n")
        except IndexError:
            pass
