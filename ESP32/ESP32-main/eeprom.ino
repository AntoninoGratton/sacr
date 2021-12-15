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
    Serial.println("ERROR: Can't find eeprom (stopped)...");
    return;
  }
}

void EEPROMguardar(byte *id)
{
  ultimaPosicion = ee.readByte(0);

  for(int i=0; i<4; i++)
  {
    ee.writeByte(ultimaPosicion+1, *(id+i));
    ultimaPosicion++;
  }
  Serial.println(ultimaPosicion);
  ee.updateByte(0, ultimaPosicion);
}

void ejemplo()
{
  byte id[4] = {0};
  ultimaPosicion = ee.readByte(0);

  for(int i=0; i<4; i++)
  {
    id[i] = ee.readByte(ultimaPosicion-3+i);
    Serial.print(id[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void ponerACero()
{
//  ee.writeByte(0, 0);
  uint8_t pos = -1;
  pos = ee.readByte(0);
  Serial.println(pos);
}
