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

void initLTE()
{
  Serial2.print("AT+CFUN=1\r");
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+CSTT=\"igprs.claro.com.ar\"\r");
  vTaskDelay(2000);
  Serial.println(recep());
  Serial2.print("AT+CIICR\r");
  vTaskDelay(2000);
  Serial.println(recep());
  Serial2.print("AT+CIFSR\r");
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+CDNSCFG = \"8.8.8.8\",\"8.8.4.4\"\r");
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");  /* Connection type GPRS */
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"\r");  /* APN of the provider */
  vTaskDelay(1000);
  Serial.println(recep());
  
  return;
}

void sms(String tel, String msg)
{
  String codigo = "AT+CMGS=\"";
  codigo.concat(tel);
  codigo.concat(String("\""));

  Serial2.print("AT+CMGF=1\r");
  vTaskDelay(100);
  Serial.println(recep());
  Serial2.println(codigo);
  vTaskDelay(100);
  Serial.println(recep());
  Serial2.println(msg);
  vTaskDelay(100);
  Serial.println(recep());
  Serial2.println((char)26);
  vTaskDelay(5000);
  Serial.println(recep());

  return;
}

void getRequest(String api)
{
  String aux = "AT+HTTPPARA=\"URL\",\"";
  aux.concat(url);
  aux.concat(api);
  aux.concat("\"\r");

  Serial2.print("AT+SAPBR=1,1\r"); /* Open GPRS context */
  vTaskDelay(1000);
  Serial.println(recep());
//  Serial2.print("AT+CGATT?\r");
//  vTaskDelay(1000);
//  Serial.println(recep());
  Serial2.print("AT+HTTPINIT\r"); /* Initialize HTTP service */
  vTaskDelay(1000);
  Serial.println(recep());
//  Serial2.print("AT+HTTPSSL=1\r");
//  vTaskDelay(5000);
//  Serial.println(recep());
  Serial2.print("AT+HTTPPARA=\"CID\",1\r");  /* Set parameters for HTTP session */
//  vTaskDelay(5000);
//  Serial2.print("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r");  /* Set parameters for HTTP session */
  vTaskDelay(1000);
  Serial.println(recep());
//  Serial2.print(String(String("AT+HTTPPARA=\"URL\",\"").concat(url)).concat("/api/prueba/\"\r"));  /* Set parameters for HTTP session */
  Serial2.print(aux);
  vTaskDelay(2000);
  Serial.println(recep());
  Serial2.print("AT+HTTPACTION=0\r");  /* Start GET session */
  vTaskDelay(5000);
  Serial.println(recep());
  Serial2.print("AT+HTTPREAD=0,30000\r");  /* Read data from HTTP server */
  vTaskDelay(4000);
  Serial.println(getData(recep()));
  Serial2.print("AT+HTTPTERM\r");  /* Terminae HTTP service */
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=0,1\r."); /* Close GPRS context */
  vTaskDelay(1000);
  Serial.println(recep());

  return;
}

void postRequest(String api, String obj)
{
  String aux = "AT+HTTPPARA=\"URL\",\"";
  aux.concat(url);
  aux.concat(api);
  aux.concat("\"\r");

  Serial2.print("AT+SAPBR=1,1\r"); /* Open GPRS context */
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+HTTPINIT\r"); /* Initialize HTTP service */
  vTaskDelay(1000);
  Serial.println(recep());
//  Serial2.print("AT+HTTPSSL=1\r");
//  vTaskDelay(5000);
//  Serial.println(recep());
  Serial2.print("AT+HTTPPARA=\"CID\",1\r");  /* Set parameters for HTTP session */
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print(aux); // URL
  vTaskDelay(2000);
  Serial.println(recep());
  Serial2.print("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r");  /* Set parameters for HTTP session */
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.println("AT+HTTPDATA=99,10000");
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.println(obj);
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+HTTPACTION=1\r");  /* Start GET session */
  vTaskDelay(5000);
  Serial.println(recep());
  Serial2.print("AT+HTTPREAD=0,30000\r");  /* Read data from HTTP server */
  vTaskDelay(4000);
  Serial.println(getData(recep()));
  Serial2.print("AT+HTTPTERM\r");  /* Terminae HTTP service */
  vTaskDelay(1000);
  Serial.println(recep());
  Serial2.print("AT+SAPBR=0,1\r."); /* Close GPRS context */
  vTaskDelay(1000);
  Serial.println(recep());

  return;
}
