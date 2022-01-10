from django.urls import path
from sacrApi.logs.views import *

urlpatterns = [
    path('pruebita/', pruebita),
    path('logs/', log)
]