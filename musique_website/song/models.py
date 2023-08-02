from django.db import models

class Song(models.Model):
    title = models.CharField(max_length=200)
    artists = models.CharField(max_length=200)
    album = models.CharField(max_length=200, blank=True, null=True)
    release_date = models.IntegerField(default=0)
    instruments = models.CharField(max_length=1000)
    difficulty_level = models.FloatField()
    tags = models.CharField(max_length=1000, blank=True, null=True)
    description_file = models.FileField(upload_to='description_files', null=True)
    visibility = models.IntegerField(default=1) # 0 = private, 1 = public

    def __str__(self):
        return self.title

class SongFile(models.Model):
    file = models.FileField(upload_to='music_files')
    song = models.ForeignKey(Song, on_delete=models.CASCADE, related_name='files')

    score_name = models.CharField(max_length=200, blank=True)

    def __str__(self):
        return self.file.url

class SongGroup(models.Model):
    title = models.CharField(max_length=150)
    songs = models.ManyToManyField(Song)

    def __str__(self):
        return self.title