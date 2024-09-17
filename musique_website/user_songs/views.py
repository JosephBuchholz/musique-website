from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
from .models import UserSong

def get_list(request):
    if not request.user.is_authenticated:
        return HttpResponse(status=403)

    songs = UserSong.objects.filter(user=request.user)

    songsDict = {
        "song_count": songs.count(),
        "songs": []
    }

    for song in songs:
        songDict = {
            "id": song.id,
            "title": song.title,
            "created_at": song.created_at,
            "updated_at": song.updated_at
        }

        songsDict["songs"].append(songDict)

    return JsonResponse(songsDict)

def get_file(request):
    if not request.GET:
        return HttpResponse(status=400)

    if not request.user.is_authenticated:
        return HttpResponse(status=403)

    songs = UserSong.objects.filter(user=request.user)

    song_id_str = request.GET.get('id')
    file_index_str = request.GET.get('file_index')
    if song_id_str and file_index_str:
        if song_id_str.isdigit() and file_index_str.isdigit():
            song_id = int(song_id_str)
            file_index = int(file_index_str)
            print("Song ID and Index: ", song_id, ", ", file_index)

            try:
                song = songs.get(id=song_id)
                
                file = song.files.all()[file_index].file
                with file.open() as f:
                    return HttpResponse(f.read())
            except:
                HttpResponse(status=400)

    return HttpResponse(status=400)