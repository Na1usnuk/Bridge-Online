from django.urls import path
from bridgeapp import views
from .views import GameView

urlpatterns = [
    path(r"", GameView),
]