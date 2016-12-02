#!/usr/bin/python
import json
import urllib
import urllib2
import string
import re

# ACCESS_TOKEN = "86c24923538142578c5a85be2fc627f1"
# Client ID: 86c24923538142578c5a85be2fc627f1
# Client Secret: 31febb7d90454421958dc97991982e49

def get_request(url, params=None):
    result = urllib2.urlopen(url)
    response_data = result.read()
    return json.loads(response_data)

def pull_jsondata(ex_request_call):
    data = get_request(ex_request_call)
    return data

if __name__ == '__main__':
    # ex_request_call = "https://api.spotify.com/v1/albums/4aawyAB9vmqN3uQ7FjRGTy"+ACCESS_TOKEN     # Typically you need your access token for a request (the numbers we have on the google doc; also at the top of this document)
    ex_request_call = "https://api.spotify.com/v1/albums/4aawyAB9vmqN3uQ7FjRGTy"                    # Example from online. This is a request for albums that meet the given ID
    results = pull_jsondata(ex_request_call)
    print results
