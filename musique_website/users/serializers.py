from django.contrib.auth.models import User
from rest_framework import serializers

def required(value):
    print("hello: ")
    print(value)
    if value is None:
        raise serializers.ValidationError("This field is required.")

def email_is_unique(value):
    if len(User.objects.filter(email=value)) != 0:
        raise serializers.ValidationError("This email has already been used.")

class UserSerializer(serializers.ModelSerializer):

    email = serializers.EmailField(validators=[required, email_is_unique])

    class Meta:
        model = User
        fields = ["id", "username", "email", "password"]
        extra_kwargs = {"password": {"write_only": True}}

    def create(self, validated_data):
        user = User.objects.create_user(**validated_data)
        return user