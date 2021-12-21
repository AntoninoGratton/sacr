#include <SPI.h>
#include "Wire.h"
#include "I2C_eeprom.h"
#include <MFRC522.h>

// SPI Pins
#define SS_PIN 5
#define RST_PIN 15

// Parametros RTOS
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

// Puerto serie para el modulo LTE
#define RXD2 16
#define TXD2 17

// I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 // Pin 2 en la esp32
#endif

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
byte nuidPICC[2][4] = {{206, 06, 36, 121}, {154, 14, 160, 12}};

const String url = "http://sacr.com.ar/api";

// define two tasks for Blink & AnalogRead
void LectorTag( void *pvParameters );
//void Logs( void *pvParameters );
void LTE( void *pvParameters );

TaskHandle_t LogsHandle;
TaskHandle_t LTEHandle;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
//  initLTE();
  EEPROMInit();
//  EEPROMGuardar(nuidPICC[0]);
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Modulo LTE inicializado");
//  preferences.begin("logs", false);

  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    LectorTag
    ,  "LectorTag"   // A name just for humans
    ,  3072  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    Logs
    ,  "Logs"   // A name just for humans
    ,  3072  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &LogsHandle
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    LTE
    ,  "LTE"   // A name just for humans
    ,  3072  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &LTEHandle
    ,  ARDUINO_RUNNING_CORE);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void LectorTag(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init();
  uint8_t flag = 0;

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  for (;;) // A Task shall never return or exit.
  {

//    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//    vTaskDelay(500);  // one tick delay (15ms) in between reads for stability
//    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    flag = 0;
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {

      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      // Check is the PICC of Classic MIFARE type
      if (piccType == MFRC522::PICC_TYPE_MIFARE_MINI ||
        piccType == MFRC522::PICC_TYPE_MIFARE_1K ||
        piccType == MFRC522::PICC_TYPE_MIFARE_4K) {
          flag = 1;
          for(int i=0; i<2; i++) {
            if (rfid.uid.uidByte[0] == nuidPICC[i][0] &&
              rfid.uid.uidByte[1] == nuidPICC[i][1] &&
              rfid.uid.uidByte[2] == nuidPICC[i][2] &&
              rfid.uid.uidByte[3] == nuidPICC[i][3] ) {
    //          Serial.println(F("A new card has been detected."));
                digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
                vTaskDelay(2000);  // one tick delay (15ms) in between reads for stability
                vTaskDelay(2000);  // one tick delay (15ms) in between reads for stability
                digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //            vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
                flag = 0;
              // Store NUID into nuidPICC array
    //          for (byte i = 0; i < 4; i++) {
    //            nuidPICC[i] = rfid.uid.uidByte[i];
    //          }
    //
    //          Serial.println(F("The NUID tag is:"));
    //          Serial.print(F("In hex: "));
    //          printHex(rfid.uid.uidByte, rfid.uid.size);
    //          Serial.println();
    //          Serial.print(F("In dec: "));
    //          printDec(rfid.uid.uidByte, rfid.uid.size);
    //          Serial.println();
            }
          }
        if(flag == 1)
        {
          Serial.println(F("No papu"));
        }

        // Halt PICC
        rfid.PICC_HaltA();

        // Stop encryption on PCD
        rfid.PCD_StopCrypto1();
      }

      else {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
      }

    }
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

//void Logs(void *pvParameters)
//{
//  (void) pvParameters;
////  uint16_t counter = preferences.getUInt("counter", 0);
////  counter++;
//
//  for(;;)
//  {
//    digitalWrite(LED_BUILTIN, HIGH);
//    vTaskDelay(500);
//    Serial.println(counter);
//    preferences.putUInt("counter", counter);
//    preferences.end();
//    digitalWrite(LED_BUILTIN, LOW);
//    vTaskDelete(LogsHandle);
//  }
//}

void LTE(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

//  initLTE();
////  getRequest("/prueba/");
//  postRequest("/post/", "{\"datos\":\"Holass\"}");
////  sms("+5493482583998", "Buuueeeenass");

  for (;;) // A Task shall never return or exit.
  {
    vTaskDelay(10);
  }
}

void Logs(void *pvParameters)
{
  (void) pvParameters;
  vTaskDelay(4000);
  ejemplo();
  Serial.print("ASUNTO: ");
//  Serial.println(byteAInt(nuidPICC[0]));
//  byte aux[4] = {0};
  byte *aux;
  aux = intAByte(3456509049);
  for(int i=0; i<4; i++)
  {
    Serial.print(*(aux+i));
    Serial.print("\t");
  }
  Serial.println();
 // EEPROMGuardar(nuidPICC[0], nuidPICC[1]);
  ponerACero();

  for(;;)
  {
//    digitalWrite(LED_BUILTIN, HIGH);
//    vTaskDelay(500);
//    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(10);
  }
}
