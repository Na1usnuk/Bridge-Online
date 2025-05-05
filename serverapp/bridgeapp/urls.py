from django.urls import path
from bridgeapp import views
from .views import GameView, LobbyView, LeaderBoard

urlpatterns = [
    path(r"", LobbyView, name='lobbys'),
    path("<str:lobbyName>/", GameView, name='game')
]