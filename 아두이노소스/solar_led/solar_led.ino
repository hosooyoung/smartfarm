#include <SoftwareSerial.h>
SoftwareSerial softwareSerial(2, 3);
void setup() {
  softwareSerial.begin(9600);
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  
}
//char command='0';
void loop() {
//  Serial.println(command);


  if (softwareSerial.available() > 0 ) {
   char command = BTSerial.read();
    switch (command) {
      case '1':
        BTSerial.println("ON모드 활성화.\n");
        digitalWrite(8, LOW);
        digitalWrite(9, HIGH);

        break;
      case '0':

        BTSerial.println("OFF모드활성화.\n");
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);

        break;
      case '2':
        digitalWrite(8, HIGH);
        digitalWrite(9, LOW);

        BTSerial.println("AUTO 모드 활성화\n");

        break;
    }

  }
  
}
