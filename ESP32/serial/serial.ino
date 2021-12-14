#define RXD2 16
#define TXD2 17

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));

  Serial2.print("AT\r");
  delay(200);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  Serial2.print("AT+CMGF=1\r");
  delay(100);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  Serial2.println("AT+CMGS=\"+5493482583998\"");
  delay(100);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  Serial2.println("Hola desde ESP32");
  delay(100);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  Serial2.println((char)26);
  delay(100);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  Serial2.println();
  delay(5000);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
}

void loop() { //Choose Serial1 or Serial2 as required
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
}
