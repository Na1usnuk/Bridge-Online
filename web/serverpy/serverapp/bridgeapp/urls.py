from django.urls import path
from bridgeapp import views

urlpatterns = [
    path(r'', views.HomePageView.as_view()),    
]