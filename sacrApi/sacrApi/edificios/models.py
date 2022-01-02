from django.db import models
from django.contrib.auth.models import *

# Create your models here.

# Tabla donde se registra cada edificio donde se encuentra instalado un equipo SACR
class Edificio(models.Model):
    idEdificio = models.AutoField(primary_key=True)
    nombre = models.CharField(max_length=45, null=False, blank=False)
    calle = models.CharField(max_length=45, null=False, blank=False)
    numero = models.IntegerField(null=True)
    barrio = models.CharField(max_length=45, null=False, blank=False)
    ciudad = models.CharField(max_length=60, null=False, blank=False)
    provincia = models.CharField(max_length=45, null=False, blank=False)


# Tabla donde se registran los equipos SACR que se encuentran instalados en los edificios
class EquipoSACR(models.Model):
    idEquipo = models.AutoField(primary_key=True)
    nroSerie = models.CharField(max_length=60, null=False, blank=False)
    usuario = models.ForeignKey(User, null=False, db_column='username', related_name='equipoSACR_user', on_delete=models.CASCADE)
    ip = models.CharField(max_length=45, null=True, blank=True)
    mac = models.CharField(max_length=45, null=True, blank=True)
    edificio = models.ForeignKey(Edificio, null=True, db_column='idEdificio', related_name='equipoSACR', on_delete=models.SET_NULL)