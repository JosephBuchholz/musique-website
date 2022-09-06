from django.urls import path
from . import views

urlpatterns = [
    path('get/', views.song_provider, name='song-provider'),
]