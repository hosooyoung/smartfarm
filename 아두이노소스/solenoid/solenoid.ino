void setup() {
  pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
digitalWrite(8,HIGH);
digitalWrite(7,HIGH);
Serial.begin(9600);
}                              // 4번 pin에 버튼을 연결, 7번핀에 솔벨브를 연결한

void loop() {
  char command = Serial.read();

  switch (command) {
    case '0':
     Serial.println("솔벨브 개방.\n");
-      digitalWrite(7, HIGH);

      break;
    case '1':

     Serial.println("솔벨브 폐쇄.\n");
      digitalWrite(7, LOW);
      break;
    case '2':

     Serial.println("솔벨브 폐쇄.\n");
      digitalWrite(8, HIGH);
      break;
    case '3':

      Serial.println("솔벨브 폐쇄.\n");
      digitalWrite(8, LOW);
      break;
  }
}
