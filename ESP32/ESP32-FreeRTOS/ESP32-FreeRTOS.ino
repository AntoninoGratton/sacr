#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
byte nuidPICC[4] = {206, 06, 36, 121};

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskGPRS( void *pvParameters );
void TaskSerialPrint( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial2.begin(19200);
  
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskBlink
    ,  "TaskBlink"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskGPRS
    ,  "TaskGRPS"
    ,  1024  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskSerialPrint
    ,  "TaskSerialPrint"
    ,  3072  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
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

//void configGPRS() {
//  Serial2.print("AT+CFUN=1\r");
//  vTaskDelay(4000);
//  Serial2.print("AT+CSTT=\"igprs.claro.com.ar\"\r");
//  vTaskDelay(4000);
//  Serial2.print("AT+CIICR\r");
//  vTaskDelay(4000);
//  Serial2.print("AT+CIFSR\r");
//  vTaskDelay(4000);
//  Serial2.print("AT+CDNSCFG = \"8.8.8.8\",\"8.8.4.4\"\r");
//  vTaskDelay(4000);
//  Serial2.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");  /* Connection type GPRS */
//  vTaskDelay(5000);
//  Serial2.print("AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"\r");  /* APN of the provider */
//  vTaskDelay(5000);
//  Serial2.print("AT+SAPBR=1,1\r"); /* Open GPRS context */
//  vTaskDelay(5000);
//  Serial2.print("AT+CGATT?\r");
//  vTaskDelay(5000);
//  Serial2.print("AT+HTTPINIT\r"); /* Initialize HTTP service */
//  vTaskDelay(5000);
//  Serial2.print("AT+HTTPSSL=1\r");
//  vTaskDelay(5000);
//  Serial2.print("AT+HTTPPARA=\"CID\",1\r");  /* Set parameters for HTTP session */
////  vTaskDelay(5000);
////  Serial2.print("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r");  /* Set parameters for HTTP session */
//  vTaskDelay(5000);
//  Serial2.print("AT+HTTPPARA=\"URL\",\"casa.org.ar/\"\r");  /* Set parameters for HTTP session */
//  vTaskDelay(5000);
//  Serial2.print("AT+HTTPACTION=0\r");  /* Start GET session */
//  vTaskDelay(10000);
//  Serial2.print("AT+HTTPREAD=0,30000\r");  /* Read data from HTTP server */
//  vTaskDelay(8000);
//  Serial2.print("AT+HTTPTERM\r");  /* Terminae HTTP service */
//  vTaskDelay(5000);
//  Serial2.print("AT+SAPBR=0,1\r."); /* Close GPRS context */
//  vTaskDelay(5000);
//
//  return;
//}
//
//void enviarSMS(char* mensaje) {
//  Serial2.print("AT+CMGF=1\r"); 
//  vTaskDelay(100);
//  Serial2.println("AT+CMGS=\"+5493482583998\""); 
//  vTaskDelay(100);
//  Serial2.println(mensaje); 
//  vTaskDelay(100);
//  Serial2.println((char)26); 
//  vTaskDelay(100);
//  Serial2.println();
//
//  return;
//}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
    
  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  for (;;) // A Task shall never return or exit.
  {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {

//      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//      Serial.println(rfid.PICC_GetTypeName(piccType));
    
      // Check is the PICC of Classic MIFARE type
      if (piccType == MFRC522::PICC_TYPE_MIFARE_MINI ||  
        piccType == MFRC522::PICC_TYPE_MIFARE_1K ||
        piccType == MFRC522::PICC_TYPE_MIFARE_4K) {
        if (rfid.uid.uidByte[0] == nuidPICC[0] && 
          rfid.uid.uidByte[1] == nuidPICC[1] && 
          rfid.uid.uidByte[2] == nuidPICC[2] && 
          rfid.uid.uidByte[3] == nuidPICC[3] ) {
//          Serial.println(F("A new card has been detected."));
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
            vTaskDelay(1500);  // one tick delay (15ms) in between reads for stability
            digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//            vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
      
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
        else
        {
//          Serial.println(nuidPICC[0]);
//          Serial.println(nuidPICC[1]);
//          Serial.println(nuidPICC[2]);
//          Serial.println(nuidPICC[3]);
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
    
      
  
//      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//      vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
//      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//      vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    }
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskGPRS(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
  vTaskDelay(20000);
  char* texto = "Holaaaa";
//  enviarSMS(texto);
  configGPRS();
  for (;;)
  {
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskSerialPrint(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
  for (;;)
  {
    while (Serial2.available() != 0) {
      Serial.write(Serial2.read());
      vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
    }
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}
