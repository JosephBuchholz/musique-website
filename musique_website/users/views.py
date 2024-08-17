from django.shortcuts import render
from django.contrib.auth.models import User
from rest_framework import generics
from rest_framework.permissions import AllowAny
from .serializers import UserSerializer
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.http import HttpResponse, JsonResponse
import json

class CreateNewUserView(generics.CreateAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    permission_classes = [AllowAny]

def login_view(request):
    if request.method == "POST":
        data = request.body
        data = data.decode("utf-8")
        data = json.loads(data)

        username = data["username"]
        password = data["password"]
        real_username = username

        user_match = User.objects.filter(username=username)
        if not user_match: # try to find user by email
            user_match = User.objects.filter(email=username)
            if user_match:
                real_username = user_match[0].username

        if not user_match: # not found
            return JsonResponse({"username":["User not found."]}, status=403)

        # authenticate
        user = authenticate(request, username=real_username, password=password)
        
        # login/set session id
        if user is not None:
            login(request, user)
        else:
            return JsonResponse({"error":["Username/email or password do not match."]}, status=403)
    
    return HttpResponse(status=204)

def logout_view(request):
    logout(request)
    return HttpResponse(status=204)

def get_user_view(request):
    if request.user.is_authenticated:
        return JsonResponse({"username":request.user.username, "email":request.user.email})
    else:
        return HttpResponse(status=204)