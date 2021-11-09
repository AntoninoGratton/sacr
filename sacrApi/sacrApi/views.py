from django.http import HttpResponse

def prueba(request):
    if request.method == 'GET':
        return HttpResponse(json_response("Holass desde el back"), status=200, content_type="application/json")