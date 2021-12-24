from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.models import User
from sacrApi.internos import *
import json

@csrf_exempt
def login_view(request):
    if request.method == 'POST':
        post_data = json.loads(request.body.decode("UTF-8"))

        if not checkpost([post_data.get('usuario'), post_data.get('password')]):
            return HttpResponse(json_response('Datos incompletos'), status=406, content_type='application/json')

        usuario = post_data.get('usuario')
        password = post_data.get('password')

        r = respuesta()

        user = authenticate(
            request=request,
            username=usuario,
            password=password,
        )

        if user is not None:
            if user.is_active:
                login(request, user)
                r.success = 1
        else:
            r.success = 0

    else:
        r.method = request.method
    return HttpResponse(json_response(r.__dict__), status=200, content_type='application/json')

@login_required
@csrf_exempt
def logout_view(request):
    if request.method == 'GET':
        if request.user.is_authenticated:
            logout(request)
            return HttpResponse(json_response('Deslogueado'), status=200, content_type='application/json')
        else:
            return HttpResponse(json_response('No esta autenticado'), status=400, content_type='application/json')
    else:
        return HttpResponse(json_response('Metodo invalido'), status=405, content_type='application/json')


@csrf_exempt
def esta_autenticado(request):
    if request.method == 'GET':
        response = {'autenticado': request.user.is_authenticated}
        print(request.user.is_authenticated)
        return HttpResponse(json_response(response), status=200, content_type='application/json')
    else:
        return HttpResponse(json_response('Metodo invalido'), status=405, content_type='application/json')

@login_required
@csrf_exempt
def prueba(request):
    if request.method == 'GET':
        print(request.user)
        # user = User.objects.get(username=request.user.username)
        # print(user)
        return HttpResponse(json_response("Holass desde el back"), status=200, content_type="application/json")


@csrf_exempt
def pruebaPost(request):
    if request.method == 'POST':
        post_data = json.loads(request.body.decode("UTF-8"))
        print(post_data)
        return HttpResponse(json_response("Correcto"), status=200, content_type="application/json")