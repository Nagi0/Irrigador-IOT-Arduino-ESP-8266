#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <Wire.h>

#define FIREBASE_HOST "testeconnect-7d6e3-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "nu6MfXGGrT9S0vB1cbxYQgWcKLEC0fgMhrvcVwfh" //Your Firebase Database Secret goes here

#define WIFI_SSID "GABI LED"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "24011997"                                      //Password of your wifi network 

WiFiUDP ntpUDP;

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
const int relay1 = 4;
int off;

int hora_inicio, min_inicio, hora_fim, min_fim;

int horas, minutos, segundos;
NTPClient timeClient(ntpUDP, "b.st1.ntp.br", 3600, 60000);

void setup() {

  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor

  Serial.println("Serial communication started\n\n");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }


  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  Wire.begin(5, 4);
  delay(1000);
}

void loop() {

  timeClient.update();

  horas = timeClient.getHours() - 4;
  minutos = timeClient.getMinutes();
  segundos = timeClient.getSeconds();

  if (horas < 0)
  {
    horas = 24 + horas;
  }

  if (off == 1)
  {
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("{\"gpio\":3,\"state\":0}");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
  }
  else
  {
    if ((horas == hora_inicio) && (minutos == min_inicio))
    {
      //digitalWrite(relay1, HIGH);
      //Serial.println(digitalRead(relay1));
      Wire.beginTransmission(8); /* begin with device address 8 */
      Wire.write("{\"gpio\":3,\"state\":1}");  /* sends hello string */
      Wire.endTransmission();    /* stop transmitting */
    }

    if (horas == hora_fim && minutos == min_fim)
    {
      //digitalWrite(relay1, LOW);
      //Serial.println(digitalRead(relay1));
      Wire.beginTransmission(8); /* begin with device address 8 */
      Wire.write("{\"gpio\":3,\"state\":0}");  /* sends hello string */
      Wire.endTransmission();    /* stop transmitting */
      }
  }

  // Firebase Error Handling And Reading Data From Specified Path ************************************************

  if (Firebase.getInt(firebaseData, "/hora_inicio")) {                           // On successful Read operation, function returns 1

      if (firebaseData.dataType() == "int") {                            // print read data if it is integer

        hora_inicio = firebaseData.intData();
        //Serial.println(val);
        //Serial.println("\n Change value at firebase console to see changes here.");

      }

    } else {
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.getInt(firebaseData, "/hora_fim")) {                           // On successful Read operation, function returns 1

      if (firebaseData.dataType() == "int") {                            // print read data if it is integer

        hora_fim = firebaseData.intData();
        //Serial.println(val);
        //Serial.println("\n Change value at firebase console to see changes here.");
      }

    } else {
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.getInt(firebaseData, "/min_inicio")) {                           // On successful Read operation, function returns 1

      if (firebaseData.dataType() == "int") {                            // print read data if it is integer

        min_inicio = firebaseData.intData();
      }

    } else {
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.getInt(firebaseData, "/min_fim")) {                           // On successful Read operation, function returns 1

      if (firebaseData.dataType() == "int") {                            // print read data if it is integer

        min_fim = firebaseData.intData();
      }

    } else {
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.getInt(firebaseData, "/irrigar_off")) {                           // On successful Read operation, function returns 1

      if (firebaseData.dataType() == "int") {                            // print read data if it is integer

        off = firebaseData.intData();
      }

    } else {
      Serial.println(firebaseData.errorReason());
    }


    Serial.print(hora_inicio);
    Serial.print(":");
    Serial.print(min_inicio);
    Serial.print("-------");
    Serial.print(hora_fim);
    Serial.print(":");
    Serial.print(min_fim);
    Serial.println("");

    Serial.print(horas);
    Serial.print(":");
    Serial.print(minutos);
    Serial.print(":");
    Serial.print(segundos);
    Serial.println("");
  }
