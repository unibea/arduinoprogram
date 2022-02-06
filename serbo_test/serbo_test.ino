#define pulse 1700 
void setup(){
  //パルス出力ピンの設定
  //（デジタル出力なのでどのピンでも可）
  pinMode(30,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(34,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(40,OUTPUT);

  Serial.begin(9600);
}
int i=-50;
void servo(int a){
  digitalWrite(40,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(a); 
  digitalWrite(40,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(20000-a);
  }
void loop(){
  /*//可変抵抗器の読み込み値を２倍にする（振幅値：約2000）
  digitalWrite(30,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(4000); 
  digitalWrite(30,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(4000);
  */
  //可変抵抗器の読み込み値を２倍にする（振幅値：約2000）
  /*
  digitalWrite(30,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(500); 
  digitalWrite(30,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(500);
  */
  while(i<0){
   servo(820);//820: 閉まる1300:開く
   i=i+1;
  }
    
  
}
