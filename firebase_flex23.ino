
#include <ESP8266WiFi.h>
#define MUX_A D5
#define MUX_B D6
#define MUX_C D7

#define ANALOG_INPUT A0

#include <FirebaseArduino.h>
#include <Firebase.h>
#include "Wire.h"
// Set these to run example.
#define FIREBASE_HOST "flex2-54ea3.firebaseio.com"
#define FIREBASE_AUTH "CnpK4YvqCaDUtO5z1jVBOpCoZBfZTw4Z7lqL9I4m"
#define WIFI_SSID "Manik1"
#define WIFI_PASSWORD "manik1936"
int data = 0;
int data1 = 0;
int data2 = 0;
int data3 = 0;
int data4 = 0;
String ch;
int a;

void setup() {
  //Deifne output pins for Mux
  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Wire.begin();
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
}

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

void loop() {
  int value;
  changeMux(LOW, LOW, LOW);
  data = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux

  Serial.println(data);



  changeMux(LOW, LOW, HIGH);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux


  changeMux(LOW, HIGH, LOW);
  data1 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux

  Serial.println(data1);



  changeMux(LOW, HIGH, HIGH);
  data2 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 3 pin of Mux
  Serial.println(data2);

  changeMux(HIGH, LOW, LOW);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 4 pin of Mux

  changeMux(HIGH, LOW, HIGH);
  data3 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 5 pin of Mux
  Serial.println(data3);



  changeMux(HIGH, HIGH, LOW);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 6 pin of Mux

  changeMux(HIGH, HIGH, HIGH);
  data4 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 7 pin of Mux
  Serial.println(data4);

  
  if ( data < 150 && data1 < 60 && data2 < 80 && data3 < 150 && data4 > 150)
  { ch = "A";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data > 150 && data1 > 60 && data2 > 80 && data3 > 150 && data4 < 150)
  { ch = "E";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }

  else if ( data > 150 && data1 > 60 && data2 < 80 && data3 > 150 && data4 > 150)
  { ch = "C";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }

  else if ( data < 150 && data1 < 60 && data2 < 80 && data3 > 150 && data4 < 150)
  { ch = "D";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data < 150 && data1 < 60 && data2 < 80 && data3 < 150 && data4 < 150)
  { ch = "E";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data > 150 && data1 > 60 && data2 > 80 && data3 <150 && data4 > 150)
  { ch = ".";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }

  else if ( data > 150 && data1 > 60 && data2 < 80 && data3 < 150 && data4 < 150)
  { ch = "";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data > 150 && data1 > 60 && data2 > 80 && data3 < 150 && data4 < 150)
  { ch = "F";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data < 150 && data1 > 60 && data2 > 80 && data3 < 150 && data4 < 150)
  { ch = "G";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }

  else if ( data > 150 && data1 < 60 && data2 < 80 && data3 < 150 && data4 < 150)
  { ch = "I";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data < 150 && data1 < 60 && data2 > 80 && data3 > 150 && data4 < 150)
  { ch = "K";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( data < 150 && data1 < 60 && data2 < 80 && data3 > 150 && data4 > 150)
  { ch = "L";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if ( data > 120 && data1 < 30 && data2 > 80 && data3 > 150 && data > 120)
  { ch = "H";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }

  else if ( data < 120 && data1 > 60 && data2 > 80 && data3 > 150 && data4 > 120)
  { ch = "O";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  
 


  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
  }
}
