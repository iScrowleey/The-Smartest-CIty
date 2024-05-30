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

#define IR_Sensor1 D0
#define IR_Sensor2 D1
#define IR_Sensor3 D2
#define IR_Sensor4 D3
#define IR_Sensor5 D4

#define IR_SensorSurvo D5

Servo servo_3;
int IR1;
int IR2;
int IR3;
int IR4;
int IR5;

int IRSurvo;

bool control; 


void setup() {

  pinMode(IR_Sensor1, INPUT); /*IR Pin D14 defined as Input*/
  pinMode(IR_Sensor2, INPUT); /*IR Pin D14 defined as Input*/
  pinMode(IR_Sensor3, INPUT); /*IR Pin D14 defined as Input*/
  pinMode(IR_Sensor4, INPUT); /*IR Pin D14 defined as Input*/
  pinMode(IR_Sensor5, INPUT); /*IR Pin D14 defined as Input*/

  pinMode(IR_SensorSurvo, INPUT);
  servo_3.attach(D7, 500, 2500);
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
    createslote(0,"control");

}
void loop() {
control= GetActuatorData("control");
 if(control){
 
  IR1 = digitalRead(IR_Sensor1);
  createslote(IR1,"slot_1");
  Serial.print("IR1");
  Serial.println(IR1);
////////////////////////////////////////////////////
  IR2 = digitalRead(IR_Sensor2);
  createslote(IR2,"slot_2");
  Serial.print("IR2");
  Serial.println(IR2);
///////////////////////////////////////////////////
  IR3 = digitalRead(IR_Sensor3);
  createslote(IR3,"slot_3");
  Serial.print("IR3");
  Serial.println(IR3);
///////////////////////////////////////////////////
  IR4 = digitalRead(IR_Sensor4);
  createslote(IR4,"slot_4");
  Serial.print("IR4");
  Serial.println(IR4);
//////////////////////////////////////////////////
  IR5 = digitalRead(IR_Sensor5);
  createslote(IR5,"slot_5");
  Serial.print("IR5");
  Serial.println(IR5);

    if(!IR1&&!IR2&&!IR3&&!IR4&&!IR5){
      Serial.print("gvcbngvgbcgb");
   }else{
    IRSurvo = digitalRead(IR_SensorSurvo);
   if (IRSurvo == 0) {
    servo_3.write(90);
    delay(2000);  // Wait for 1000 millisecond(s)
      servo_3.write(0);

  } else {
    servo_3.write(0);
    delay(1000);  // Wait for 1000 millisecond(s)
  } 


}




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
    String documentPath = "smart_parking";
    documentPath += "/";
    documentPath += "parking1";
    documentPath += "/";
    documentPath += "slots";
    documentPath += "/";
    documentPath += "sensors";
    // documentPath += "/";
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
  String documentPath = "smart_parking";
  documentPath += "/";
  documentPath += "parking1";
  documentPath += "/";
  documentPath += "slots";
  documentPath += "/";
  documentPath += "sensors";
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