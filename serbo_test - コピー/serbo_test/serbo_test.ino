//一番後ろのサーボの値：1150閉める1700開ける

int i=-50;//サーボの出力時間調整変数
int S0=0;//センサー値格納用変数

void servo3(int p){
  digitalWrite(38,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(p); 
  digitalWrite(38,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(20000-p);
  }//固定サーボ一番前
void servo1(int s){
  digitalWrite(30,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(s); 
  digitalWrite(30,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(20000-s);
  }//固定サーボ一番後ろ 
void servoloopL(){
  digitalWrite(32,HIGH);
  delayMicroseconds(1580);
  digitalWrite(32,LOW);
  delayMicroseconds(1580);
  }//無限サーボ前向いて左側  
void servoloopR(){
  digitalWrite(34,HIGH);
  delayMicroseconds(1580);
  digitalWrite(34,LOW);
  delayMicroseconds(1580);
  }//無限サーボ前向いて左側
void setup(){
  //ﾊﾟﾙｽ出力ﾋﾟﾝの設定
  //(ﾃﾞｼﾞﾀﾙ出力なのでどのﾋﾟﾝでも可)
  pinMode(30,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(34,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(40,OUTPUT);
  pinMode(12,OUTPUT);
 while(i<0){
   servo1(1150);
   i=i+1;
  }
  i=-50;//while文の値リセット
  while(i<0){
   servo3(1380);
   i=i+1;
  }
  Serial.begin(9600);
  //tone(7,1000, 1000);//setup
  tone(7,987,130);
  delay(300);
  tone(7,1318,180);
  delay(500);
  
}
void sensor(){
  S0=analogRead(7);
  }  
void loop(){
 sensor();
 Serial.println(S0);
 if(S0>600){
  
  }
  
}
