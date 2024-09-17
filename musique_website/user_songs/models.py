from django.db import models
from django.contrib.auth.models import User

class UserSong(models.Model):
    title = models.CharField(max_length=200)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    visibility = models.IntegerField(default=0) # 0 = private, 1 = public
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='songs')

    def __str__(self):
        return self.title

class UserSongFile(models.Model):
    file = models.FileField(upload_to='user_files')
    song = models.ForeignKey(UserSong, on_delete=models.CASCADE, related_name='files')

    score_name = models.CharField(max_length=200, blank=True)
    file_type = models.CharField(max_length=50, default="harmonyxml")

    def __str__(self):
        return self.file.url

