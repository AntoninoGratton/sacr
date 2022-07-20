//LOGSApiService

String postAcceso(uint32_t tag)
{
//  String apiUrl = "http://sacr.com.ar/api/logs/";
  String obj = "{\"tag\":";
  obj.concat(String(tag));
  obj.concat(String("}"));
  return postRequest("/logs/logs/", obj);
}
