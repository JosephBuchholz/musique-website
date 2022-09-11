from django.db import models

class Song(models.Model):
    title = models.CharField(max_length=200)
    artists = models.CharField(max_length=200)
    instruments = models.CharField(max_length=1000)
    difficulty_level = models.CharField(max_length=100)

    def __str__(self):
        return self.title

class SongFile(models.Model):
    file = models.FileField(upload_to='music_files')
    song = models.ForeignKey(Song, on_delete=models.CASCADE, related_name='files')

    def __str__(self):
        return self.file.url

class SongGroup(models.Model):
    title = models.CharField(max_length=150)
    songs = models.ManyToManyField(Song)

    def __str__(self):
        return self.title