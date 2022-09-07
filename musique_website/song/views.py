from django.shortcuts import render
from django.http import HttpResponse
from .models import Song, SongGroup

def song_provider(request):

    for object in Song.objects.all():
        print(object.title)
        print(object.artists)
        for file in object.file:
            print(file.file.url)

    print(request.GET)
    key = request.GET.get('key')
    print(key)
    if key == 'ABC123':
        method = request.GET.get('method')
        if method == 'all':
            print("all!!")
            return HttpResponse("hello this is some text")
        if method == 'get':
            print("get!!")
        if method == 'search':
            print("searching!!")

        return HttpResponse('<h1>Hello</h1>')
    return HttpResponse('<h1>HI</h1>')
