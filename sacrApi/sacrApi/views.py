from django.http import HttpResponse
from django.contrib.auth import authenticate, login
from django.views.decorators.csrf import csrf_exempt
from sacrApi.internos import *
import json

@csrf_exempt
def log_in(request):
    if request.method == 'POST':
        post_data = json.loads(request.body.decode("UTF-8"))

        if not checkpost([post_data.get('usuario'), post_data.get('password')]):
            return HttpResponse(json_response('Datos incompletos'), status=406, content_type='application/json')

        user = post_data.get('usuario')
        password = post_data.get('password')

        response = {}

        user = authenticate(
            request=request,
            username=user,
            password=password,
        )

        if user is not None:
            login(request, user)
            response = {'success': 1}
        else:
            response = {'success': 0}
        
        return HttpResponse(json_response(response), status=200, content_type='application/json')
    else:
        return HttpResponse(json_response('Metodo invalido'), status=405, content_type='application/json')


@csrf_exempt
def prueba(request):
    if request.method == 'GET':
        return HttpResponse(json_response("Holass desde el back"), status=200, content_type="application/json")


@csrf_exempt
def pruebaPost(request):
    if request.method == 'POST':
        post_data = json.loads(request.body.decode("UTF-8"))
        print(post_data)
        return HttpResponse(json_response("Correcto"), status=200, content_type="application/json")