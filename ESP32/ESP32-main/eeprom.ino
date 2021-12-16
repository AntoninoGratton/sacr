I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC256);

uint8_t ultimaPosicion = 0;

struct Usuario
{
  long int id;
  long int codigo;
} user;

void EEPROMInit()
{
  ee.begin();
  if (! ee.isConnected())
  {
    Serial.println("ERROR: No se encuentra la EEPROM");
    return;
  }
}

void EEPROMGuardar(byte *id, byte *codigo)
{
  ultimaPosicion = ee.readByte(0);

  for(int i=0; i<4; i++)
  {
    ee.writeByte(ultimaPosicion+1, *(id+i));
    ultimaPosicion++;
  }

  for(int i=0; i<4; i++)
  {
    ee.writeByte(ultimaPosicion+1, *(codigo+i));
    ultimaPosicion++;
  }

  // Serial.println(ultimaPosicion);
  ee.updateByte(0, ultimaPosicion);
}

void ejemplo()
{
  byte id[2][4] = {0};
  ultimaPosicion = ee.readByte(0);

  for(int i=0; i<4; i++)
  {
    id[0][i] = ee.readByte(ultimaPosicion-7+i);
    Serial.print(id[0][i]);
    Serial.print("\t");
  }
  Serial.println();

  for(int i=0; i<4; i++)
  {
    id[1][i] = ee.readByte(ultimaPosicion-3+i);
    Serial.print(id[1][i]);
    Serial.print("\t");
  }
  Serial.println();
}

void ponerACero()
{
 // ee.writeByte(0, 0);
  uint8_t pos = -1;
  pos = ee.readByte(0);
  Serial.println(pos);
}
