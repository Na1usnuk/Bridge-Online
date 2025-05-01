from re import match
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.models import User
from django.contrib.auth import login
from django import forms
from django.http.response import HttpResponseRedirect
from django.shortcuts import render, redirect
from django.views.generic import TemplateView
import datetime





class HomePageView(TemplateView):
    def get(self, request, **kwargs):
        return render(request, 'index.html', context=None)

class RegistrationForm(UserCreationForm):

    class Meta:
        model = User
        fields = ['username', 'password1', 'password2']


def RegistrationView(req):
    if req.method == 'POST':
        form = RegistrationForm(req.POST)
        if form.is_valid():
            user = form.save()
            login(req, user)
            return HttpResponseRedirect('/accounts/profile')
    else:
        form = RegistrationForm()

    return render(req, 'registration/registration.html', {'form':form})




def ProfileView(req):
    if req.user.is_authenticated:
        date_joined = str(req.user.date_joined)[:10]
        context = {'username': req.user.username, 'date_joined': date_joined, 'wins_num': '--', 'games_num': User.objects.get(username=req.user.username).player.points}
        return render(req, 'registration/profile.html', context)
    else:
        return redirect('login')

            

