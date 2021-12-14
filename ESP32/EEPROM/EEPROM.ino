#include <SPI.h>

#define SS 21

byte resultado = 124;

void setup() {
  // put your setup code here, to run once:
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  Serial.begin(115200);
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  byte habilitar = 0b00000110;
  byte escribir = 0b00000010;
  int address = 0;
  byte dato = 0b11111100;
  byte deshabilitar = 0b00000100;
  digitalWrite(SS, LOW);
  SPI.transfer(habilitar);
  digitalWrite(SS, HIGH);
  delay(10);
  digitalWrite(SS, LOW);
  SPI.transfer(escribir);
  delayMicroseconds(20);
  SPI.transfer((char)(address>>8));   //send MSByte address first
  resultado = SPI.transfer((char)(address));
  delay(10);
//  delayMicroseconds(20);
//  delayMicroseconds(20);
  SPI.transfer(dato);
//  delayMicroseconds(20);
  digitalWrite(SS, HIGH);
  delay(5000);
  byte leer = 0b00000101;
//  int address = 0;
//  direccion2 = 0b00000000;
  digitalWrite(SS, LOW);
  SPI.transfer(leer);
  SPI.transfer((char)(address>>8));   //send MSByte address first
  resultado = SPI.transfer((char)(address));      //send LSByte address
//  resultado = SPI.transfer(0xFF);
//  delayMicroseconds(20);
//  SPI.transfer(direccion1);
//  resultado = SPI.transfer(direccion2);
  Serial.println(resultado);
  digitalWrite(SS, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
}
