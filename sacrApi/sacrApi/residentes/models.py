from django.db import models

# Create your models here.

class TipoDoc(models.Model):
    idDoc = models.AutoField(primary_key=True)
    tipo = models.CharField(max_length=35, null=False, blank=False)
    descripcion = models.CharField(max_length=60, null=False, blank=False)


# Residentes del edificio
class Residente(models.Model):
    idResidente = models.AutoField(primary_key=True)
    nombre = models.CharField(max_length=75, null=False, blank=False)
    apellido = models.CharField(max_length=75, null=False, blank=False)
    tipoDoc = models.ForeignKey(TipoDoc, db_column='idDoc', related_name='residente', on_delete=models.DO_NOTHING)
    doc = models.CharField(max_length=45, null=False, blank=False)
    # Puede haber residentes extranjeros temporalmente, por eso charfield. Algunos paises tienen dni con letras o el pasaporte
    telefono = models.CharField(max_length=65, null=False, blank=False)
    telefono2 = models.CharField(max_length=65, null=True, blank=True)
    email = models.CharField(max_length=65, null=False, blank=False)
    departamento = models.CharField(max_length=6, null=False, blank=False) # Departamento del edificio
    propietario = models.BooleanField(default=False) # Es propietario del departamento?

    class Meta():
        db_table = 'residentes_lista'

class Tag(models.Model):
    idTag = models.AutoField(primary_key=True)
    nroTag = models.BigIntegerField(null=False, blank=False)
    codSeguridad = models.BigIntegerField(null=False, blank=False)
    residente = models.ForeignKey(Residente, null=False, blank=False, db_column='idResidente', related_name='tags', on_delete=models.CASCADE)