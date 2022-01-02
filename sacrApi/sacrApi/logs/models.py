from django.db import models
from django.contrib.auth.models import *
from sacrApi.edificios.models import *

# Create your models here.

# # Tipos de registro posibles
# class TipoRegistro(models.Model):
#     idTipoReg = models.AutoField(primary_key=True)
#     nombre = models.CharField(max_length=45, null=False, blank=False)
#     descripcion = models.CharField(max_length=50, null=False, blank=False)


# Tabla que contiene los registros de acceso de cada edificio
class Acceso(models.Model):
    idReg = models.AutoField(primary_key=True)
    fechaHora = models.DateTimeField(null=False, blank=False)
    residente = models.ForeignKey(Residente, db_column='idResidente', related_name='registros', on_delete=models.DO_NOTHING)
    edificio = models.ForeignKey(Edificio, db_column='idEdificio', related_name='registrosEdificio', on_delete=models.CASCADE)

class Bateria(models.Model):
    idReg = models.AutoField(primary_key=True)
    fechaHora = models.DateTimeField(null=False, blank=False)
    edificio = models.ForeignKey(Edificio, db_column='idEdificio', related_name='bateria', on_delete=models.CASCADE)
    voltaje = models.FloatField(null=False, blank=False)
    estado = models.CharField(max_length=25, null=False, blank=False)