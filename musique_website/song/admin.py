from django.contrib import admin
from .models import Song, SongFile, SongGroup

admin.site.register(Song)
admin.site.register(SongFile)
admin.site.register(SongGroup)
