from re import L
from django.shortcuts import render
from django.shortcuts import render, redirect
from django.http.response import HttpResponseRedirect, HttpResponse, JsonResponse
from django.views.generic import TemplateView
from .BridgeGame import BridgeSession, Card

from django.conf import settings

sessions = {}


def LobbyView(req):
    data = {}
    for name, ses in sessions.items():
        data[name] = {
        'pcount': ses.PlayersCount(),
        'isStarted': ses.IsGameStarted()
        }
    return render(req, 'lobby.html', {'lobbies': data})


def GameView(req, lobbyName):
    if not req.user.is_authenticated:
        return redirect('/accounts/login')
    if req.method == 'POST':
        return GameViewPost(req, lobbyName)
    if not lobbyName in sessions:
        sessions[lobbyName] = BridgeSession()
        sessions[lobbyName].AddPlayer(req.user.username)
    elif not sessions[lobbyName].IsPlayerPlaying(req.user.username) and sessions[lobbyName].PlayersCount() < 4 and not sessions[lobbyName].IsGameStarted():
        sessions[lobbyName].AddPlayer(req.user.username)
    elif sessions[lobbyName].IsGameStarted() and not sessions[lobbyName].IsPlayerPlaying(req.user.username):   
        return render(req, 'join_denied.html')
    

    context = PLayersGameContext(req, lobbyName)
    if sessions[lobbyName].IsGameStarted():
        context = context | GameContext(req, lobbyName)
    
    return render(req, 'game.html', context)


def GameViewPost(req, lobbyName):
    if not lobbyName in sessions:
        return JsonResponse({'error': 'Lobby closed'})

    lobby = sessions[lobbyName]
    action = req.headers['Action']
    context = {}

    if action == 'StartGame':
        lobby.StartGame()
        context = PLayersGameContext(req, lobbyName) | GameContext(req, lobbyName)

    elif action == 'PreGameCheck':
        context = PLayersGameContext(req, lobbyName)

    elif action == 'GameCheck':
        context = PLayersGameContext(req, lobbyName) | GameContext(req, lobbyName)

    #User turn area 

    elif lobby.CurrentTurn().nick() != req.user.username:
        context = {'error': 'Not Your turn!'}

    elif action == 'PlayerTurn':
        lobby.Turn(Card(req.headers['Card']))
        context = GameContext(req, lobbyName)

    elif action == 'PlayerEndTurn':
        if not lobby.EndTurn():
            context = {'error': 'Invalid turn!'}
        else:
            print("GAME END")
            if lobby.IsGameEnded():
                lobby.CalculateAllPoints()
                lobby.EndGame()
                lobby.StartGame()
                context = {'gameEnded': 'true'}
            context = context | GameContext(req, lobbyName)

    elif action == 'PlayerPickCard':
        if not lobby.PlayerPickCard():
            context = {'error': 'You can`t pick card!'}
        else:
            context = GameContext(req, lobbyName)

    return JsonResponse(context)


def GameContext(req, lobbyName):
    context = {}
    cards = []

    for c in sessions[lobbyName].GetPlayerCards(req.user.username):
        cards.append(c.repr())
    context['yourCards'] = cards
    context['topCard'] = sessions[lobbyName].OnTopCard().repr()
    context['isYourTurn'] = sessions[lobbyName].CurrentTurn().nick() == req.user.username

    return context



def PLayersGameContext(req, lobbyName):
    context = {}
    players = []

    for p in sessions[lobbyName].ListOfPlayers():
        players.append((p.nick(), p.points()))
    context['players'] = players
    usernames = [p[0] for p in players]
    n = usernames.index(req.user.username)
    n = (n + 1) % len(players)
    if len(players) >= 2:
        context['top'] = sessions[lobbyName].PlayerCardsCount(players[n][0])
    n = (n + 1) % len(players)
    if len(players) >= 3:
        context['left'] = sessions[lobbyName].PlayerCardsCount(players[n][0])
    n = (n + 1) % len(players)
    if len(players) == 4:
        context['right'] = sessions[lobbyName].PlayerCardsCount(players[n][0])
    n = (n + 1) % len(players)
    if sessions[lobbyName].IsGameStarted():
        context['isGameStarted'] = 'true'
    else:
        context['isGameStarted'] = 'false'
    return context



def IsUserPlaying(nick, lobbyName):
    for p in sessions[lobbyName].ListOfPlayers():
        if p.nick() == nick:
            return True
    return False
                



def LeaderBoard(req):
    return render(req, 'leaderboard.html')


