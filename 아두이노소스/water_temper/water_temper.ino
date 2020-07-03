#include <SoftwareSerial.h>
#include <Wire.h>
#include "SHT1x.h"   //CNDL 온습도센서라이브러리
#include "OneWire.h"
#include "DallasTemperature.h"
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define dataPin 10
#define clockPin 11 

SoftwareSerial BTSerial(2, 3); // (RX,TX)
String str_temp;
String str_temp_CNDI;
SHT1x sht1x(dataPin, clockPin); 
void setup() {
  BTSerial.begin(9600);
  

  sensors.begin();
}

void loop() {
 
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    float tempC = sht1x.readTemperatureC();
     str_temp = String(temp);
     str_temp_CNDI=String(tempC);
    BTSerial.print(str_temp_CNDI);
    BTSerial.print(',');
    BTSerial.print(str_temp);
       BTSerial.print('\n');
       delay(1000);
}
