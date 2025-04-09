from django.shortcuts import render
from django.shortcuts import render, redirect
from django.http.response import HttpResponseRedirect, HttpResponse, JsonResponse
from django.views.generic import TemplateView
from .BridgeGame import BridgeSession, Card, Player



globalSession = BridgeSession()
isGameStarted = False

def GameView(req):
    if req.method == 'POST':
        return GameViewPost(req)
    if not req.user.is_authenticated:
        return redirect(req, '/accounts/login')
    if not IsUserPlaying(req.user.username):
        print('User not playing')
        if isGameStarted:
            return HttpResponse(req, '<p style="font-size: 100px">Game started.</p>')
        else:
            globalSession.AddPlayer(req.user.username)
            return render(req, 'game.html', MakeGameContext(req))


    
    return render(req, 'game.html', MakeGameContext(req))


def GameViewPost(req):
    print(req.body)
    return JsonResponse(req, {'res': 'gay'})


def MakeGameContext(req):
    plist = globalSession.ListOfPlayers()
    context = {}
    i = 1
    for p in plist:
        context['player' + str(i)] = p.nick()
        context['points' + str(i)] = p.points()
        i = i + 1
    context['top'] = 0
    context['right'] = 0
    context['left'] = 0
    return context


def IsUserPlaying(nick):
    for p in globalSession.ListOfPlayers():
        if p.nick() == nick:
            return True
    return False
                



   


