from django.http import HttpResponse
from sacrApi.internos import *
import json

def prueba(request):
    if request.method == 'GET':
        return HttpResponse(json_response("Holass desde el back"), status=200, content_type="application/json")

def pruebaPost(request):
    if request.method == 'POST':
        post_data = json.loads(request.body.decode("UTF-8"))
        print(post_data)
        return HttpResponse(json_response("Correcto"), status=200, content_type="application/json")