from django.urls import path
from .views import index

urlpatterns = [
    path('', index),
    path('home/', index),
    path('editor/<int:id>/', index),
    path('register/', index),
    path('login/', index),
    path('logout/', index),
    path('songlist/', index),
]