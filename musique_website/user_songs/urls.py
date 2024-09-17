from django.urls import path
from . import views

urlpatterns = [
    path("getlist/", views.get_list, name="get_list"),
    path("getfile/", views.get_file, name="get_file"),
]