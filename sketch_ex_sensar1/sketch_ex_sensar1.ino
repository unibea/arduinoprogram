int sd1,sd2,sd3,sd4,sd5;    //センサー読み取り用

void setup() {


  //PCとの通信を開始
  Serial.begin(115200);
  Serial.print("sensar");
  Serial.println();
}

void loop() {

  sd1 = analogRead(7);
 // sd2 = analogRead(39);
  //sd3 = analogRead(34);
  //sd4 = analogRead(35);
  //sd5 = analogRead(32);

  Serial.print("sensar :");
  Serial.print(sd1);
  Serial.println();
  
  delay(100);           //100ms待つ
}
