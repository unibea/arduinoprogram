
// Arduino UNOでローテーショナルサーボGWS S35 STDを制御

//#include<Servo.h>
/*
Servo servo1;// サーボのインスタンス
Servo servo2;*/
void setup() {
  // サーボの信号線を3番ピンに接続
 /* servo1.attach (1);
  servo2.attach (0);
  */
}

void loop() {
  /*
  // サーボ出力 1.5ms(停止)を中心に、1.0ms~2.0msのパルス幅で動く。パルス幅によって速度が変わる
  servo1.writeMicroseconds(1500);//1500で停止？
  delay(500);
  servo2.writeMicroseconds(1500);
  delay(500);*/
  analogWrite(A0,0);
  analogWrite(A1,255);
}
  
 
 
