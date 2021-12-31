from django.db import models
from django.contrib.auth.models import *
from sacrApi.edificios.models import *

# Create your models here.


'''
Tabla donde, pasado un determinado tiempo desde que la puerta se encuentra abierta,
se registra una entrada para informar al monitoreo de ello.
'''
class PuertaAbierta(models.Model):
    idRegistro = models.AutoField(primary_key=True)
    fechaHora = models.DateTimeField(null=False)
    edificio = models.ForeignKey(Edificio, null=False, db_column='idEdificio', related_name='puertaAbierta', on_delete=models.CASCADE)
    equipo = models.ForeignKey(EquipoSACR, null=False, db_column='usuario', related_name='puertaAbiertaEquipo', on_delete=models.CASCADE)
    resuelta = models.BooleanField(default=False) # Verifica si la alarma fue resuelta
    observaciones = models.CharField(max_length=300) # Campo para observaciones una vez resuelta la alarma
    responsable = models.ForeignKey(User, db_column='username', related_name='puertaAbiertaResp', on_delete=models.DO_NOTHING)
    residente = models.ForeignKey(Residente, null=False, db_column='idResidente', related_name='puertaAbiertaRedt', on_delete=models.CASCADE)