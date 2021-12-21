I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC256);

uint8_t ultimaPosicion = 0;

struct Usuario
{
  long int id;
  long int codigo;
} user;

uint32_t byteAInt(byte *buffer)
{
  return uint32_t((unsigned char)(*buffer) << 24 |
            (unsigned char)(*(buffer+1)) << 16 |
            (unsigned char)(*(buffer+2)) << 8 |
            (unsigned char)(*(buffer+3)));
}

byte *intAByte(uint32_t buffer)
{
  static byte aux[4] = {0};

  for (int i = 0; i < 4; ++i)
  {
    aux[3-i] = (buffer >> 8 * i) & 0xFF;
  }
  return aux;
}

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
  ultimaPosicion = ee.readByte(0); // En el primer Byte esta almacenada la ultima posicion

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
