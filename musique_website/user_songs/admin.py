from django.contrib import admin
from user_songs.models import UserSong
from user_songs.models import UserSongFile

admin.site.register(UserSong)
admin.site.register(UserSongFile)