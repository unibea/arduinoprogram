#include<Servo.h>

//モータードライバー用端子 
#define CLOCK_L  12
#define CLOCK_R  9
#define CWCCW_R 8
#define CWCCW_L 11 
#define MOTOR_ONOFF 10

Servo servo1;// サーボのインスタンス
Servo servo2;

//Servo myServoF;
//Servo myServoB;
#include <FlexiTimer2.h>
#include <TimerOne.h>
int S0=0;
int S1=0;
int S2=0;
int S3=0;
int S4=0;

int s=0;

int hajime=0;//起動トルク確保のため分岐用変数
int n=0;  
double il=0 , ir=0;  //パルス幅調整用
double ls=0 , rs=0;  //パルス幅調整用
boolean outputl=LOW;  //パルス用変数
boolean outputr=LOW;  //パルス用変数
boolean outcvl=HIGH;  //回転方向用変数
boolean outcvr=LOW;  //回転方向用変数


double SL=0;//速度
double SR=0;//速度

double NPL=0;//今のパルス周波数
double NPR=0;

//パルス出力用割り込み関数 us単位で呼び出される。
/*void timerPulse(){

  if(il > ls){
    digitalWrite(9,outputl);  //9番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    outputl = ! outputl;      //出力信号反転
    il=0.1;//元の速度の変更0.1→1.0早くなる
  }

  if(ir > rs){
    digitalWrite(12,outputr); //12番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    outputr = ! outputr;       //出力信号反転
    ir=0.1;//元の速度の変更0.1→1.0早くなる
  }
  
  il++;
  ir++;
  n++;
    
}
*/
void timerPulse();
////*//////////////////////////////////////////////////////////////////////////////////////////*
//スイッチ文を使って割り込みスピードを制御できる？
//ステップ距離（仮）0.8954
//ステップ速度（パルス周波数）割り込みスピード約250000usで１pps
//割り込みスピードからパルス周波数を割り出す計算式が必要
//100(1ステップのパルス幅):1pps=今のパルス幅:今のパルス周波数
//今のパルス周波数＝今のパルス幅/100
/*
double NPL=0;
NPL=ls/100;
int SL=0;//速度
//速度＝パルス周波数＊ステップ距離
SL=NPL*0.8954;

double NPR=0;
NPR=rs/100;
int SR=0;//速度
//速度＝パルス周波数＊ステップ距離
SR=NPR*0.8954;
*/
////////////////////////////////////////////////////////////////////////////////////////////////
   


void setup() {
  Serial.begin(9600);
    /*pinMode(0,OUTPUT); //ピンをデジタル出力に設定
     pinMode(1,OUTPUT); //ピンをデジタル出力に設定
     pinMode(2,OUTPUT); //ピンをデジタル出力に設定
     pinMode(3,OUTPUT); //ピンをデジタル出力に設定
     pinMode(4,OUTPUT); //ピンをデジタル出力に設定
     pinMode(5,OUTPUT); //ピンをデジタル出力に設定
     pinMode(6,OUTPUT); //ピンをデジタル出力に設定
     pinMode(7,OUTPUT); //ピンをデジタル出力に設定
     */pinMode(8,OUTPUT); //ピンをデジタル出力に設定
       pinMode(9,OUTPUT); //ピンをデジタル出力に設定
      
     pinMode(10,OUTPUT); //ピンをデジタル出力に設定
     pinMode(11,OUTPUT); //ピンをデジタル出力に設定
     pinMode(12,OUTPUT); //ピンをデジタル出力に設定
     //pinMode(A3,OUTPUT); //アナログピンをデジタル出力に設定
     //pinMode(A4,OUTPUT); //アナログピンをデジタル出力に設定
     pinMode(A5,OUTPUT); //アナログピンをデジタル出力に設定

     pinMode(13,OUTPUT); //ピンをデジタル出力に設定         
     // pinMode(A0,OUTPUT); //アナログピンをデジタル出力に設定
     // pinMode(A1,OUTPUT); //アナログピンをデジタル出力に設定
     
     digitalWrite(10, LOW); //10番ピン(LED)に出力する (HIGH>ON LOW>OFF)
     digitalWrite(11, LOW); //11番ピン(LED)に出力する (HIGH>ON LOW>OFF)
     digitalWrite(12, LOW); //12番ピン(LED)に出力する (HIGH>ON LOW>OFF)
     digitalWrite(13, HIGH); //13番ピン(LED)に出力する (HIGH>ON LOW>OFF)
     digitalWrite(A3, LOW); //A3番ピン(LED)に出力する (HIGH>ON LOW>OFF)
     digitalWrite(A4, LOW); //A4番ピン(LED)に出力する (HIGH>ON LOW>OFF)
     digitalWrite(A5, LOW); //A5番ピン(LED)に出力する (HIGH>ON LOW>OFF)

    //タイマー割り込み設定
     Timer1.initialize(500); //500us マイクロ秒単位で設定//250000で1秒に1パルス
     Timer1.attachInterrupt(timerPulse);

     ls=35;
     rs=35;//1000で1秒間に1パルス

      // サーボの信号線を3番ピンに接続
  servo1.attach (1);
  servo2.attach (0);
  
//  myServoF.attach(0);
 // myServoB.attach(1);
  
}

void loop() {

hajime++;

  
  S0=analogRead(0);//一番右
  S1=analogRead(1);
  S2=analogRead(2);
  S3=analogRead(3);//一番左
  //S4=analogRead(4);//ゴールライン専用
  
  Serial.print(rs);
  Serial.print("\t");
  Serial.print(ls);
  Serial.print("\t");
  Serial.print(S0);
  Serial.print("\t");
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");
  Serial.print(S3);
  Serial.print("\t");
  Serial.print(hajime);
  Serial.println("\t");


  
digitalWrite(CWCCW_R, outcvl); //回転方向　8番ピン(LED)に出力する (HIGH>ON LOW>OFF)
digitalWrite(CWCCW_L, outcvr); //回転方向　11番ピン(LED)に出力する (HIGH>ON LOW>OFF)
digitalWrite(10,HIGH); //モータON 10番ピン(LED)にHIGHを出力する (HIGH>ON LOW>OFF)


/*
SR=0.32;
SL=0.32;

//NPL=ls/100;//100で1パルス1秒
ls=NPL*100;
//速度＝パルス周波数＊ステップ距離
//SL=NPL*0.8954;
NPL=SL/0.8954;
//NPR=rs/100;
rs=100*NPR;
//速度＝パルス周波数＊ステップ距離
//SR=NPR*0.8954;
NPR=SR/0.8954;
*/


ls=ls-0.8;
rs=rs-0.8;

if(ls<3.0){
  ls=3.0;//この方法で起動トルクを確保した結果速度３が限界だった（直進）
  }
  if(rs<3.0){
  rs=3.0;
  }
/*
if(S1>60&&S2>60){
ls=3.0;
rs=3.0; //switch文
 }   //白

if(S1<40&&S2<40){
  ls=3.0;//右
  rs=3.0;//左
  }  //黒
  */
if(S1>900&&S2<300){
  ls=3.0;//右
  rs=20.0;//左
  //中心二つ
}
  if(S2>900&&S1<300){
ls=20.0;//右
rs=3.0;//左
    }}
    //中心二つ
 /*
 if(S2>970&&S3<300){
ls=5.0;//右
rs=3.0;//左
  }
  if(S0<300&&S1>970){
ls=3.0;//右
rs=5.0;//左
  }
 /* if(S2>50&&S3>50){
ls=1.5;//右
rs=7.8;//左
   
    }
  if(S1>50&&S0>50){
 ls=7.8;//右
 rs=1.5;//左
    }

if(S3>60&&S0<40&&S1<40&&S2<40){
  ls=1.5;
  rs=7.5;
  }
  if(S0>60&&S3<40&&S1<40&&S2<40){
  ls=7.5;//右
  rs=1.5;
 */
 /*
if(hajime>500){
  
  // サーボ出力 1.5ms(停止)を中心に、1.0ms~2.0msのパルス幅で動く。パルス幅によって速度が変わる
  servo1.writeMicroseconds(2200);
  //delay(500);
  servo2.writeMicroseconds(800);
  //delay(500);
}

//if(650>hajime>600){
//  myServoB.write(0);
  //delay(1000);
  //myServoB.write(180);
//}
  // サーボの角度を180度にする
}

 
