from django.db import models
from django.contrib.auth.models import User



class Player(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    points = models.fields.IntegerField('Очки', default=0)

    



