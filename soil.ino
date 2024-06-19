#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <Servo.h>


#define API_KEY "AIzaSyANq6WLzOBjSDODbCjJN5NwIgtR62ljFZA"
#define FIREBASE_PROJECT_ID "smart-city-2cd2c"
FirebaseData Sfbdo;
FirebaseData Afbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long dataMillis = 0;
int count = 0;
bool actData;
bool taskcomplete = false;
void init_wifi(String WIFI_SSID, String WIFI_PASSWORD);
void SendSensorData(String sensorName, int sensorData);
bool GetActuatorData(String actuatorName);
void createEsp(String esp_name);
#define USER_EMAIL "smartcity@gmail.com"
#define USER_PASSWORD "12345678"
#define Token "3jXVxxfE9AbL3cnV7Hm0X2dz1Hp2"
#define soil_moisture_pin A0
#define SOIL_WET 500   // Define max value we consider soil 'wet'
#define SOIL_DRY 750   // Define min value we consider soil 'dry'    
#define PUMPPIN 5     //D1
bool auto_pumb;
bool manual_pumb;

void setup() {
 
  
  Serial.begin(115200);
  init_wifi("iScrowleey", "ISC_6969.");
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

#if defined(ESP8266)
  Sfbdo.setBSSLBufferSize(512 /* Rx buffer size in bytes from 512 - 16384 */, 512 /* Tx buffer size in bytes from 512 - 16384 */);
#endif
  Sfbdo.setResponseSize(512);
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(PUMPPIN, OUTPUT);
    pinMode(soil_moisture_pin, INPUT);
}
void loop() {


 auto_pumb = GetActuatorData("auto_pumb");
 manual_pumb = GetActuatorData("manual_pumb");
 Serial.println(auto_pumb);
   if (auto_pumb) {
     if(analogRead(soil_moisture_pin)>SOIL_DRY){
     digitalWrite(PUMPPIN,HIGH);
    }
    else{
     digitalWrite(PUMPPIN,LOW);
    }
  }
  else if(auto_pumb==false&&manual_pumb==true){
  digitalWrite(PUMPPIN,HIGH);
  }
  else{
  digitalWrite(PUMPPIN,LOW);
  }

}

void init_wifi(String WIFI_SSID, String WIFI_PASSWORD) {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void createslote(bool sensorData, String slotName) {
  if (Firebase.ready() && (millis() - dataMillis > 1000 || dataMillis == 0)) {
    // dataMillis = millis();
    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
    FirebaseJson content;
    // aa is the collection id, bb is the document id.
    String documentPath = "authority";
    documentPath += "/";
    documentPath += "control";
    documentPath += "/";
    String filed = "fields/";
    filed += slotName;
    filed += "/booleanValue";
    if (!taskcomplete) {
      taskcomplete = true;
      content.clear();

      content.set(filed, String(sensorData).c_str());

      if (Firebase.Firestore.createDocument(&Sfbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw())) {
      }
      // Serial.printf("ok\n%s\n\n", Sfbdo.payload().c_str());
      else
        Serial.println(Sfbdo.errorReason());
    }


    count++;
    content.clear();
    content.set(filed, String(sensorData).c_str());
    //Serial.print("Update a document... ");

    /** if updateMask contains the field name that exists in the remote document and 
         * this field name does
         not exist in the document (content), that field will be deleted from remote document
         */

    if (Firebase.Firestore.patchDocument(&Sfbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), String(slotName) /* updateMask */)) {
    }
    //Serial.printf("ok\n%s\n\n", Sfbdo.payload().c_str());
    else
      Serial.println(Sfbdo.errorReason());
  }
}


bool GetActuatorData(String actuatorName) {
  String documentPath = "authority";
  documentPath += "/";
  documentPath += "control";
  documentPath += "/";
  String filed = "fields/";
  filed += actuatorName;
  filed += "/booleanValue";
  if (Firebase.ready())  //&& (millis() - dataMillis > 10000 || dataMillis == 0)
  {


    // dataMillis = millis();
    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
    FirebaseJson content;
    if (!taskcomplete) {
      taskcomplete = true;
      content.clear();
      content.set(filed, bool());
      if (Firebase.Firestore.createDocument(&Afbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw())) {
      }
      // Serial.printf("ok\n%s\n\n", Afbdo.payload().c_str());
      else
        Serial.println(Afbdo.errorReason());
    }

    count++;

    content.set(filed, bool());
    //Serial.print("Update a document... ");

    /** if updateMask contains the field name that exists in the remote document and
         * this field name does not exist in the document (content), that field will be deleted from remote document
         */
    if (Firebase.Firestore.patchDocument(&Afbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), "data" /* updateMask */)) {
    }
    // Serial.printf("ok\n%s\n\n", Afbdo.payload().c_str());
    else
      Serial.println(Afbdo.errorReason());
  }
  FirebaseJson payload;
  payload.setJsonData(Afbdo.payload().c_str());
  FirebaseJsonData jsonData;
  payload.get(jsonData, filed, bool());
  return jsonData.boolValue;
}