Welcome to Music Match! This program accesses two users' public playlists on 
Spotify. From the songs and artists contained within these playlists, you will 
get a list of songs that appear on playlists by both users, both users' 
most-appearing artist among all their playlists, and the shortest path between 
these two artists.

SET-UP INSTRUCTIONS
-----------------------------------------------------------
This program requires the use of Spotify credentials to access the Spotify API 
data. We obtained these by registering our application-- in return, we got a 
client_id, client_secret, and redirect_uri. These credentials will be emailed 
separately to Professor Bui. These credentials should then be saved in the 
.cshrc file. This can be done with the following commands:

setenv SPOTIPY_CLIENT_ID yourclientid

setenv SPOTIPY_CLIENT_SECRET yourclientsecret

setenv SPOTIPY_REDIRECT_URL https://sites.google.com/nd.edu/music-match/

We downloaded Spotipy, a Python wrapper for the Spotify API to aid in accessing 
Spotify data, to the student machines. We did this with the commands:

$ pip install spotipy --user

$ ls .local/lib/python2.7/site-packages/

     spotipy spotipy-2.3.8-py2.7.egg-info

$ setenv $PYTHONPATH ~/.local/lib/python2.7/site-packages

$ python

>>> import spotipy

RUNNING THE CODE:
Each time a new user profile is accessed with this program, their Spotify data 
is not cached, and their account must be verified. To run the program with two 
new Spotify users, first obtain their URIs (available on their Spotify profile).
then type the command:
     make user1=firstuserURI user2=seconduserURI

You should then be given a URL. Copy this, paste it into your browser, and you 
will be redirected to our Google Music Match site. Copy this URL you were 
redirected to, then paste it into the terminal. You will have to repeat this 
process for the second user. (Please note that, for reasons beyond our control,
the google site we set up sometimes produces a 404 error. If this happens,
simply copy the URL you were directed to and paste it in the terminal as usual.)
The rest of the program should compile.

In order to run musicMatch, you must specify how many related artists should 
be searched when finding the shortest path between two artists. For instance, 
if you want to look at 5 related artists for each artists, type: ./musicMatch 5

