from django.urls import path
from . import views

urlpatterns = [
    path("register/", views.CreateNewUserView.as_view(), name="register"),
    path("login/", views.login_view, name="login"),
    path("logout/", views.logout_view, name="logout"),
    path("get/", views.get_user_view, name="get"),
]