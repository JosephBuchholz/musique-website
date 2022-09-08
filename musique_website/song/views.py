from django.shortcuts import render
from django.http import HttpResponse
from .models import Song, SongGroup
import json

def song_provider(request):
    print(request.GET)
    key = request.GET.get('key')
    if key == 'ABC123':
        method = request.GET.get('method')
        if method == 'all':
            returnDict = {
                "song_count": Song.objects.count(),
                "songs": []
            }

            for object in Song.objects.all():

                print(object.title)
                print(object.artists)
                files = []
                for file in object.files.all():
                    print(file.file.url)
                    files.append(file.file.url)
                
                songDict = {
                    "id": object.pk,
                    "title": object.title,
                    "artists": object.artists.split(','),
                    "instruments": object.instruments.split(','),
                    "files": files
                }
                
                returnDict["songs"].append(songDict)

            returnJson = json.dumps(returnDict)

            return HttpResponse(returnJson)
        if method == 'get':
            song_id_str = request.GET.get('id')
            if song_id_str:
                if song_id_str.isdigit():
                    song_id = int(song_id_str)
                    print(song_id)
                
        if method == 'search':
            print("searching!!")

        return HttpResponse('<h1>Hello</h1>')
    return HttpResponse('<h1>HI</h1>')
