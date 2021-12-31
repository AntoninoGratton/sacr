from django.shortcuts import render

from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.models import User
from sacrApi.internos import *
import json

# Create your views here.
