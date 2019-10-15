
// Tiny RTC Module http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-tiny-rtc-d1307-tutorial/ 
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

// SENSORS
// Temperature Sensor Setup (DS18bB20)
#include <OneWire.h> 
#include <DallasTemperature.h> 
#define ONE_WIRE_BUS 2 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);


// VARIABLES
// Relays 
const int IntakeFan = 6;
const int QuantumBoardRelay =  7;
const int Pump =  8;


void setup() {
  // Booting
  RTC.begin();
  sensors.begin();
  Wire.begin();
  Serial.begin(9600);
  
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  
  // Outputs
  pinMode(QuantumBoardRelay, OUTPUT);  
  pinMode(Pump, OUTPUT); 
  pinMode(IntakeFan, OUTPUT);  
}


void loop() {
  
   Temperature();
   LightSchedule();
   TimeStamp(); 
   delay(60000); 
   
}


// FUNCTIONS
// Contents:
//    Light Control
//    Time Control
//    Temperature Control



// Light Control
void LightSchedule() {

  DateTime now = RTC.now(); 
  
  int DayLength = 18; // Hours
  int NightLength = 6; // Hours

  if (now.hour() < 6) {
     digitalWrite(QuantumBoardRelay, 0);
     Serial.println("Light Status: NIGHT TIME.");
  } 
  else if (now.hour() >= 6) {
     digitalWrite(QuantumBoardRelay, 1);
     Serial.println("Light Status: DAY TIME."); 
  } 
  else {
     Serial.println("Quantum Board code confused");
  }
}

// Time Control
void TimeStamp() {
   DateTime now = RTC.now(); 
   
   Serial.print("Time: ");
   Serial.print(now.hour(), DEC);
   Serial.print(":");
   Serial.print(now.minute(), DEC);
   Serial.print(":");
   Serial.println(now.second(), DEC);
   Serial.println(" ");
}

// Temperature Control
void Temperature() {


  int FanOff = 16; // Set Lower Limit ˚C, turns fan off to help maintain heat

  if (sensors.getTempCByIndex(0) < FanOff) {
     digitalWrite(IntakeFan, 1); // Intake fan wired to relay 'Normally Open', 1 turns it OFF.
     sensors.requestTemperatures(); 
     Serial.print("Temperature: ");
     Serial.print(sensors.getTempCByIndex(0));
     Serial.println("˚C");
  }
  else if (sensors.getTempCByIndex(0) >= FanOff) {
     digitalWrite(IntakeFan, 0); // Intake fan wired to relay 'Normally Open', 0 turns it ON.
     sensors.requestTemperatures(); 
     Serial.print("Temperature: ");
     Serial.print(sensors.getTempCByIndex(0));
     Serial.println("˚C");
  }
  else {
    Serial.println("Intake Fan code confused");
  }
}



  
