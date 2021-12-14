const String url = "http://sacr.com.ar/api";
void initLTE()
{
  Serial2.print("AT+CFUN=1\r");
  delay(1000);
  Serial.println(recep());
  Serial2.print("AT+CSTT=\"igprs.claro.com.ar\"\r");
  delay(2000);
  Serial.println(recep());
  Serial2.print("AT+CIICR\r");
  delay(2000);
  Serial.println(recep());
  Serial2.print("AT+CIFSR\r");
  delay(1000);
  Serial.println(recep());
  Serial2.print("AT+CDNSCFG = \"8.8.8.8\",\"8.8.4.4\"\r");
  delay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");  /* Connection type GPRS */
  delay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"\r");  /* APN of the provider */
  delay(1000);
  Serial.println(recep());
  
  return;
}

String recep()
{
  String reciv;
  while (Serial2.available()) {
    reciv.concat(char(Serial2.read()));
  }
  return reciv;
}

String getData(String raw)
{
  String aux, dat;

  aux = raw.substring(raw.indexOf("+HTTPREAD:")+1);
  aux = aux.substring(aux.indexOf("\n")+1);
  dat = aux.substring(0, aux.indexOf("\n")-1);

  return dat;
}

void postRequest(String api, String obj)
{
  String aux = "AT+HTTPPARA=\"URL\",\"";
  aux.concat(url);
  aux.concat(api);
  aux.concat("\"\r");

  Serial2.print("AT+SAPBR=1,1\r"); /* Open GPRS context */
  delay(1000);
  Serial.println(recep());
  Serial2.print("AT+HTTPINIT\r"); /* Initialize HTTP service */
  delay(1000);
  Serial.println(recep());
//  Serial2.print("AT+HTTPSSL=1\r");
//  delay(5000);
//  Serial.println(recep());
  Serial2.print("AT+HTTPPARA=\"CID\",1\r");  /* Set parameters for HTTP session */
  delay(1000);
  Serial.println(recep());
  Serial2.print(aux); // URL
  delay(2000);
  Serial.println(recep());
  Serial2.print("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r");  /* Set parameters for HTTP session */
  delay(1000);
  Serial.println(recep());
  Serial2.println("AT+HTTPDATA=18,10000");
  delay(6000);
  Serial.println(recep());
  Serial2.println(obj);
  delay(6000);
  Serial.println(recep());
  Serial2.print("AT+HTTPACTION=1\r");  /* Start GET session */
  delay(5000);
  Serial.println(recep());
  Serial2.print("AT+HTTPREAD=0,30000\r");  /* Read data from HTTP server */
  delay(4000);
  Serial.println(getData(recep()));
  Serial2.print("AT+HTTPTERM\r");  /* Terminae HTTP service */
  delay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=0,1\r."); /* Close GPRS context */
  delay(1000);
  Serial.println(recep());

  return;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial2.begin(19200);
  initLTE();
  postRequest("/post/", "{\"datos\":\"Holass\"}");
}

void loop() {
  // put your main code here, to run repeatedly:

}
