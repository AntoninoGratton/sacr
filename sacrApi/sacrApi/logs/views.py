from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from sacrApi.internos import *
import json

# Create your views here.

@csrf_exempt
def pruebita(request):
    if request.method == 'GET':
        return HttpResponse(json_response("Andaaaa"), status=200, content_type="application/json")