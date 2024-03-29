/*
  ESP8266 4051 Multiplexer by Brian Lough
  An example showing how to use a 4051 multiplexer with an esp8266
  to connect up to 8 analog sensors.
  Wiring:
  Wemos ->  4051
  ---------------
  D5   ->   S0 (A)
  D6    ->   S1 (B)
  D7    ->   S2 (C)
  A0    ->   Common
  3.3v  ->   VCC
  G     ->   GND
  G     ->   Inhibit
  G     ->   VEE
  4051 Option pins are then wired to whatever Analog sensors required
  One thing to note: say for example if you only require 2 analog sensors,
  You can just wire up S0(A) and connect S1 & S2 to GND and you will be
  able to switch between option 1 and option 2 pins.
  Same goes for up to 4 pins (just use S0 & S1)
*/
#include <ESP8266WiFi.h>
#define MUX_A D5
#define MUX_B D6
#define MUX_C D7
#define Switch D4
#define ANALOG_INPUT A0

#include <FirebaseArduino.h>
#include <Firebase.h>
#include "Wire.h"
// Set these to run example.
#define FIREBASE_HOST "flex2-54ea3.firebaseio.com"
#define FIREBASE_AUTH "CnpK4YvqCaDUtO5z1jVBOpCoZBfZTw4Z7lqL9I4m"
#define WIFI_SSID "rameet"
#define WIFI_PASSWORD "0142PPpp"
int data = 0;
int data1 = 0;
int data2 = 0;
int data3 = 0;
int data4 = 0;
String ch;
int a;
const int MPU_addr=0x68; // I2C address of the MPU-6050
 
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;

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
  Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); // PWR_MGMT_1 register
Wire.write(0); // set to zero (wakes up the MPU-6050)
Wire.endTransmission(true);
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(Switch,INPUT);
}

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

void loop() {
  int value;
   int switchValue = digitalRead(Switch);
 
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
  Wire.beginTransmission(MPU_addr);
Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true); // request a total of 14 registers
AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 

Serial.println("AcX = "); Serial.println(AcX);
Serial.println(" | AcY = "); Serial.println(AcY);
Serial.println(" | AcZ = "); Serial.println(AcZ);
delay(500);

//      if (  AcZ<10000&&data < 140 && data1 < 120 && data2 < 120 && data3 <120 && data4 > 120)
//  { ch = "ME";
//    String name = Firebase.pushString("logs", ch);
//    delay(1500);
//  } 
     if ( AcY>9000&&AcZ<-2000&&data > 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 < 120)
  { ch = "I WANT TO PEE";
    String name = Firebase.pushString("logs", ch);
    delay(1500);
  }
     if (  AcX<-10000&&data > 140 && data1 > 130 && data2 > 130 && data3 > 130 && data4 > 130)
  { ch = "THANK YOU";
    String name = Firebase.pushString("logs", ch);
    delay(1500);
  } 
     else if ( AcY>-8000&&data > 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 > 120  )
  { ch = "EMERGENCY CALL";
    String name = Firebase.pushString("logs", ch);
    delay(500);
}

  
      if (  AcY<-8000&&data > 130 && data1 > 130 && data2 > 130 && data3 > 130 && data4 > 130)
  { ch = ".";
    String name = Firebase.pushString("logs", ch);
    delay(1500);}
    
      if (  AcY>9000&&data > 130 && data1 > 130 && data2 > 130 && data3 > 130 && data4 > 130)
  { ch = "HELLO";
    String name = Firebase.pushString("logs", ch);
    delay(1000);   
  }
     if (  AcY>9000&&data > 130 && data1 > 130 && data2 > 130 && data3 > 130 && data4 < 130)
  { ch = "WE ARE";
    String name = Firebase.pushString("logs", ch);
    delay(500);
  }
   else if ( AcZ<10000&&AcY>-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 < 120)
  { ch = "MESSAGE";
    String name = Firebase.pushString("logs", ch);
    delay(1500);
  }
      if (  AcY>9000&&data > 130 && data1 < 130 && data2 < 130 && data3 > 130 && data4 > 130)
  { ch = "C";
    String name = Firebase.pushString("logs", ch);
    delay(1500);
  }
//   else if ( AcY<-8000&& data > 140 && data1 > 120 && data2 < 120 && data3 < 120 && data4 < 120)
//  { ch = "TEAM PIE";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
// }

//  else if (  AcY<-8000&&data < 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 > 120)
//  { ch = "#";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
  else if ( AcZ>10000&& AcZ>-8000&&AcY>-8000&&data < 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 > 120)
  { ch = "A";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( AcY<9000&&AcY>-8000&& data > 140 && data1 > 120 && data2 > 120 && data3 > 120 && data4 < 120)
  { ch = "B";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
    else if ( AcY<9000&&AcY>-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 > 120 && data4 < 120)
  { ch = "D";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if (AcY<9000&& AcY>-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 < 120)
  { ch = "E";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( AcZ>10000&&AcY>-8000&& data > 140 && data1 > 120 && data2 > 120 && data3 < 120 && data4 < 120)
  { ch = "F";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if ( AcY<-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 > 120 && data4 > 120)
  { ch = "G";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
//   else if ( AcY>-8000&& data < 140 && data1 < 120 && data2 > 120 && data3 > 120 && data4 < 120)
//  { ch = "H";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
   else if ( AcZ>-2000&&AcY>-8000&& data > 140 && data1 > 120 && data2 < 120 && data3 < 120 && data4 < 120)
  { ch = "I";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if ( AcY<-8000&& data > 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 < 120)
  { ch = "J";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
//    else if ( AcY<9000 &&AcY>-8000&& data < 140 && data1 < 120 && data2 > 120 && data3 > 120 && data4 > 120)
//  { ch = "K";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
  else if ( AcZ>-8000&&AcY>-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 > 120 && data4 > 120)
  { ch = "L";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if ( AcY<-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 < 120)
  { ch = "M";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
     else if ( AcY<-8000&& data > 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 > 120)
  { ch = "O";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
//    else if ( AcZ>-8000&&AcY<-8000&& data < 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 > 120)
//  { ch = "N";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
   else if (AcY<9000&& AcY>-8000&&data > 140 && data1 > 120 && data2 > 120 && data3 < 120 && data4 > 120 )
  { ch = "R";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
  else if ( AcY<-8000&& data < 140 && data1 < 120 && data2 > 120 && data3 > 120 && data4 < 120)
  { ch = "P";
   String name = Firebase.pushString("logs", ch);
   delay(2000);
  }
   else if ( AcY<-8000&& data > 140 && data1 > 120 && data2 >120 && data3 < 120 && data4 < 120)
  { ch = "Q";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if (  AcY>-8000&& data > 140 && data1 < 100 && data2 > 100 && data3 > 100 && data4 > 100)
  { ch = "TEAM PIE";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
   else if ( AcY<9000&&AcY>-8000&& data > 140 && data1 < 120 && data2 < 120 && data3 > 120 && data4 > 120)
  { ch = "T";
    String name = Firebase.pushString("logs", ch);
    delay(2000);
  }
//    else if ( AcY<-8000&& data > 150 && data1 < 150 && data2 > 150 && data3 > 150 && data4 >150)
//  { ch = "U";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
//   else if ( AcY<-8000&& data < 150 && data1 > 150 && data2 > 150 && data3 > 150 && data4 < 150)
//  { ch = "V";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
//  else if ( AcY>-8000&& data < 150 && data1 > 150 && data2 > 150 && data3 > 150 && data4 < 150)
//  { ch = "W";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
//    else if ( AcY<-8000&& data > 150 && data1 < 150 && data2 < 150 && data3 < 150 && data4 > 150)
//  { ch = "X";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
//   else if (  AcZ>-2000&&AcY>-8000&& data > 140 && data1 < 120 && data2 < 120 && data3 < 120 && data4 > 120)
//  { ch = "C";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//// }
//   else if ( AcY>-8000&& data > 150 && data1 > 150 && data2 < 150 && data3 < 150 && data4 < 150)
//  { ch = "Z";
//    String name = Firebase.pushString("logs", ch);
//    delay(2000);
//  }
  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
  }
  }
