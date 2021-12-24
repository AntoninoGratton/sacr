def json_response(s):
    import json
    return json.dumps(s, default=str)

################################################################
# Chequea si los elementos del arreglo son validos             #
# Se usa para comprobar que esten todos los elementos del post #
################################################################
def checkpost(params):
#   print params
    pasa = True
    for elemento in params:
        if elemento is None or elemento == "":
            pasa = False
    return pasa

class respuesta():
    def __init__(self):
        self.success = None
