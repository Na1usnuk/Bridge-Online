from django.urls import path
from bridgeapp import views
from .views import GameView, LobbyView

urlpatterns = [
    path(r"", LobbyView, name='lobbys'),
    path("<str:lobbyName>/", GameView, name='game')
]