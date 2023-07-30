from django.shortcuts import render
from django.http import HttpResponse
from .models import Song, SongGroup
import json

KEY = 'ABC123'

METHOD_ALL = 'all'
METHOD_GET_FILE = 'get_file'
METHOD_SEARCH = 'search'
METHOD_GET_DESCRIPTION = 'get_desc'

def song_provider(request):
    print(request.GET)
    key = request.GET.get('key')
    if key == KEY:
        method = request.GET.get('method')

        # -------- ALL --------
        if method == METHOD_ALL:
            returnJson = create_json_from_query_set(Song.objects.all())
            return HttpResponse(returnJson)
        
        # -------- GET FILE --------
        if method == METHOD_GET_FILE:
            song_id_str = request.GET.get('id')
            file_index_str = request.GET.get('file_index')
            if song_id_str and file_index_str:
                if song_id_str.isdigit() and file_index_str.isdigit():
                    song_id = int(song_id_str)
                    file_index = int(file_index_str)
                    print("Song ID and Index: ", song_id, ", ", file_index)

                    file = Song.objects.get(id=song_id).files.all()[file_index].file
                    with file.open() as f:
                        print("gopt")
                        return HttpResponse(f.read())
        
        # -------- GET DESCRIPTION --------
        if method == METHOD_GET_DESCRIPTION:
            song_id_str = request.GET.get('id')
            if song_id_str:
                if song_id_str.isdigit():
                    return get_description_method(song_id=int(song_id_str));
    
        # -------- SEARCH --------
        if method == METHOD_SEARCH:
            # not required
            query_str = request.GET.get('query')
            difficulty_str = request.GET.get('difficulty')
            tags_str = request.GET.get('tags')
            instruments_str = request.GET.get('instruments')

            difficulty = -1.0 # an invalid value
            if difficulty_str:
                if difficulty_str.isdigit():
                    difficulty = float(difficulty_str)

            tags = []
            if tags_str:
                tags = tags_str.split(',')
            
            instruments = []
            if instruments_str:
                instruments = instruments_str.split(',')
            
            if not query_str:
                query_str = ""

            return search_method(query_str, difficulty, tags, instruments)
    return HttpResponse()

def get_description_method(song_id):
    print("Song ID and Index for GET_DESCRIPTION: ", song_id)
    file = Song.objects.get(id=song_id).description_file
    if file:
        with file.open() as f:
            return HttpResponse(f.read())
    else:
        return HttpResponse("none")

def search_method(query_str, difficulty, tags, instruments):
    print("Searching!!: ", query_str, difficulty, tags, instruments)

    query_set = Song.objects.all()

    if query_set != "":
        query_set = query_set.filter(title__icontains=query_str) or query_set.filter(artists__icontains=query_str) or query_set.filter(instruments__icontains=query_str) or query_set.filter(tags__icontains=query_str)
    
    if difficulty >= 0.0:
        query_set = query_set.filter(difficulty_level=difficulty)

    for tag in tags:
        query_set = query_set.filter(tags__icontains=tag)
    
    for instrument in instruments:
        query_set = query_set.filter(instruments__icontains=instrument)

    returnJson = create_json_from_query_set(query_set)

    return HttpResponse(returnJson)

def create_json_from_query_set(query_set):
    returnDict = {
        "song_count": query_set.count(),
        "songs": []
    }

    for object in query_set:
        files = []
        for file in object.files.all():
            print(file.file.url, file.score_name)
            files.append(file.score_name)
        
        songDict = {
            "id": object.pk,
            "title": object.title,
            "artists": object.artists.split(','),
            "album": object.album,
            "release_date": object.release_date,
            "instruments": object.instruments.split(','),
            "difficulty_level": object.difficulty_level,
            "tags": object.tags,
            "files": files
        }
        
        returnDict["songs"].append(songDict)

    return json.dumps(returnDict)