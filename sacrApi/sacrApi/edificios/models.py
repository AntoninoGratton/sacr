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

class Residente(models.Model):
    TIPO_DOC_CHOICES = [
        ('DNI', 'DNI'),
        ('Pasaporte', 'Pasaporte'),
        ('LC', 'Libreta Cívica'),
        ('LE', 'Libreta de Enrolamiento')
    ]
    idResidente = models.AutoField(primary_key=True)
    nombre = models.CharField(max_length=75, null=False, blank=False)
    apellido = models.CharField(max_length=75, null=False, blank=False)
    tipoDoc = models.CharField(max_length=10, choices=TIPO_DOC_CHOICES, null=False, blank=True)
    doc = models.CharField(max_length=45, null=False, blank=False)
    # Puede haber residentes extranjeros temporalmente, por eso charfield. Algunos paises tienen dni con letras o el pasaporte
    telefono = models.CharField(max_length=65, null=False, blank=False)
    telefono2 = models.CharField(max_length=65, null=True, blank=True)
    email = models.CharField(max_length=65, null=False, blank=False)
    departamento = models.CharField(max_length=6, null=False, blank=False) # Departamento del edificio
    propietario = models.BooleanField(default=False) # Es propietario del departamento?