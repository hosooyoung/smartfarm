#include <SoftwareSerial.h>
#include <Wire.h>
#include "SHT1x.h"   //CNDL 온습도센서라이브러리
#include "OneWire.h"
#include "DallasTemperature.h"
#include "BH1750.h"
#define dataPin 10
#define clockPin 11  // SDA와 CLK 핀 둘다 아날로그 혹은 PWM핀 지금은 10,11 디지털핀 (PWM)
#define WIND_N    0 //정 북향
#define WIND_NNE  22.5
#define WIND_NE    45
#define WIND_ENE  67.5
#define WIND_E    90  //정 동향
#define WIND_ESE  112.5
#define WIND_SE    135
#define WIND_SSE  157.5
#define WIND_S    180
#define WIND_SSW  202.5
#define WIND_SW   225
#define WIND_WSW  247.5
#define WIND_W    270
#define WIND_WNW  292.5
#define WIND_NW   315
#define WIND_NNW  337.5
SoftwareSerial BTSerial(2, 3);
/*볼트, 토양*/
float vout = 0.0;
float vin = 0.0;
float r1 = 30000.0;
float r2 = 7500.0;
int SPIN = A1;
int value = 0;
int soil_sensor = A2;
/////////////////////////

String str_lux;
String str_vin; 
String str_soil;
String str_temp;
BH1750 lightMeter;
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int windVanePin = 0;  // 아날로그 0번핀에 연결
int winVaneValue = 0;        //
float windSpeed = 0;
float rainGauge = 0;
float windDirection = 0;
char windName[4];  // N (North)  S South,   NS(north south)   NNS(north north south)
unsigned long windSpeedTimer;
int windSpeedState = true;
int windSpeedPin = 4;     //풍속핀 DIGITAL 4번핀
int windSpeedCounter = 0; //카운터? -> 스위치 카운트수가 풍속
unsigned long rainGaugeTimer;
int rainGaugeState = true;
int rainGaugePin = 5;     // 디지털 5번핀
int rainGaugeCounter = 0;
SHT1x sht1x(dataPin, clockPin);  //온습도 센서
String str_humidity;
String str_tempC;
String str_windSpeed;
String str_rainGauge;
String str_waterTemp; 
void setup()
{
  BTSerial.begin(9600);
  //Serial.begin(9600);
  //wind speed sensor
  pinMode(windSpeedPin, INPUT);
  digitalWrite( windSpeedPin, HIGH);
  pinMode(rainGaugePin, INPUT);
  digitalWrite( rainGaugePin, HIGH);
  pinMode(SPIN, INPUT);
  lightMeter.begin();
  sensors.begin();
}
/*
  char 배열의 문자열정보를 Serial통해 문자로 전송 ,  PC에서 참고용
  char 배열을 사용한것은 차후 인터넷통신(트위터전송)을 위한 것입니다.
*/
/*
  f2h와 f2p는  float형의 정수부와 소수부를 위한 것입니다.
  sprintf에서 float형 사용에 문제가 있어서 땜빵으로 사용된 함수입니다.
*/
/*
  int f2h( float num){
  return int(num);
  }
  int f2p( float num){
  return (num - int(num)) * 100;
  }*/

float getWindDirection(void) {
 
  int readValue = analogRead(windVanePin);
  // 0~1023 사이의 입력값을 0~5V 기준 값으로 비례변경함.
  winVaneValue = map(readValue, 0, 1023, 0, 500);
  if ( winVaneValue < 35) {
    //0~0.35V를 동동서 로 인식함
    //   112.5  0.32v (31 32)    0~35
    windDirection = WIND_ESE;
    strcpy(windName , "ESE");
  } else if ( winVaneValue < 43 ) {
    //   67.5  0.41v(40 41)    ~42
    windDirection = WIND_ENE;
    strcpy(windName , "ENE");
  } else if ( winVaneValue < 50 ) {
    //   90  0.45v(44 45)      ~50
    windDirection = WIND_E;
    strcpy(windName , "E");
  } else if ( winVaneValue < 70 ) {
    //   157.5  0.62v(60 62)   ~70
    windDirection = WIND_SSE;
    strcpy(windName , "SSE");
  } else if ( winVaneValue < 100 ) {
    //   135  0.90v(89 90)      ~100
    windDirection = WIND_SE;
    strcpy(windName , "SE");
  } else if ( winVaneValue < 130 ) {
    //   202.5  1.19v(119 120)  ~130
    windDirection = WIND_SSW;
    strcpy(windName , "SSW");
  } else if ( winVaneValue < 170 ) {
    //   180  1.40v(140 141)   ~170
    windDirection = WIND_S;
    strcpy(windName , "S");
  } else if ( winVaneValue < 210 ) {
    //   22.5  1.98v(198 199)  ~210
    windDirection = WIND_NNE;
    strcpy(windName , "NNE");
  } else if ( winVaneValue < 250 ) {
    //   45  2.25v(226 227)    ~250
    windDirection = WIND_NE;
    strcpy(windName , "NE");
  } else if ( winVaneValue < 300 ) {
    //  247.5  2.93v(293 294)  ~300
    windDirection = WIND_WSW;
    strcpy(windName , "WSW");
  } else if ( winVaneValue < 320 ) {
    //  225  3.08v(308 310)   ~320
    windDirection = WIND_SW;
    strcpy(windName , "SW");
  } else if ( winVaneValue < 360 ) {
    //  337.5  3.43 (343 345)  ~360
    windDirection = WIND_NNW;
    strcpy(windName , "NNW");
  } else if ( winVaneValue < 395 ) {
    //   0  3.84v(384~385)    ~395
    windDirection = WIND_N;
    strcpy(windName , "N");
  } else if ( winVaneValue < 415 ) {
    //  292.5  4.04v(405 406)  ~415
    windDirection = WIND_WNW;
    strcpy(windName , "WNW");
  } else if ( winVaneValue < 450 ) {
    //  315  4.34(433 434)  ~450
    windDirection = WIND_NW;
    strcpy(windName , "NW");
  } else if ( winVaneValue < 490 ) {
    //  270  4.62v(461 463)    ~490
    windDirection = WIND_W;
    strcpy(windName , "W");
  } else {
    //error  알수없는 값범위
  }
}
void loop()
{
 // if (BTSerial.available() > 0 ) {
    // 10초마다 측정결과를 PC에 전달
    windSpeedTimer = millis() + 10000; // 10초를 주기로 반복됩니다.
    windSpeedCounter = 0;
    while ( millis() < windSpeedTimer ) {
      delay(20); // 20ms 주기로 센서의 스위칭을 감지합니다.
      //wind speed  풍속계 센서 감지부
      if ( (windSpeedState == true ) && !digitalRead( windSpeedPin) ) {
        windSpeedCounter++;  //스위치 상태가 high에서 low로 떨어지는 순간을 감지합니다.
        windSpeedState = false;
      } else if ( (windSpeedState == false) && digitalRead(windSpeedPin) ) {
        windSpeedState = true;
      }
      //rain gauge
      if ( (rainGaugeState == true ) && !digitalRead( rainGaugePin) ) {
        rainGaugeCounter++;   //스위치 상태가  high에서 low로 떨어지는 순간을 감지합니다.
        rainGaugeState = false;
      } else if ( (rainGaugeState == false) && digitalRead(rainGaugePin) ) {
        rainGaugeState = true;
      }
    }
    rainGaugeTimer++;
      if ( rainGaugeTimer > 360) { 
      rainGauge = rainGaugeCounter * 0.2794;
      rainGaugeCounter = 0;
      rainGaugeTimer = 0;
    }
    windSpeed = windSpeedCounter * 0.24 ;   // 1초당 1회 스위칭시 2.4km/h 속력이며 10초 기간이므로 0.24가 됨
    getWindDirection();  // 풍향은 발표시점에 1회만 측정
    
    sensors.requestTemperatures();
    float water_temp = sensors.getTempCByIndex(0);
    float tempC = sht1x.readTemperatureC();
    float humidity = sht1x.readHumidity();
    str_humidity = String(humidity);
    str_tempC = String(tempC);
    str_windSpeed = String(windSpeed);
    str_rainGauge = String(rainGauge);
    str_waterTemp=String(water_temp);
//SCL-SCL(analog pin 5)
//
//SDA-SDA(analog pin 4)
    
    uint16_t lux = lightMeter.readLightLevel();
    value = analogRead(SPIN);
    vout = (value * 5.0) / 1024;
    vin = vout / (r2 / (r1 + r2));
    vin = (vin * 1.025);
      str_lux = String(lux);
    str_vin = String(vin);
   str_soil = String((analogRead(soil_sensor) * 100.0) / 1024);
    BTSerial.print(str_tempC);
    BTSerial.print(',');
    BTSerial.print(str_humidity);
    BTSerial.print(',');
    BTSerial.print(str_windSpeed);
    BTSerial.print(',');
    BTSerial.print(windName);
    BTSerial.print(',');
    BTSerial.print(str_rainGauge);
    BTSerial.print(',');
    BTSerial.print(str_lux);
    BTSerial.print(',');
    BTSerial.print(str_soil);
    BTSerial.print(',');
    BTSerial.print(str_vin);
    BTSerial.print(',');
    BTSerial.print(str_waterTemp);
    BTSerial.print('\n');
 // }
}
