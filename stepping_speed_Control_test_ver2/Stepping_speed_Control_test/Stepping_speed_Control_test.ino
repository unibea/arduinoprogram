#include <TimerOne.h>
#include <FlexiTimer2.h>
#include <VarSpeedServo.h>

VarSpeedServo servo;

//繰り返しモードの設定用
short int  flag = 0;//動作パターン(FLCG)
bool flag_check = 0;//動作パターンのカウント操作
bool FLAG_check = 0;
short int flag_SWITCH = 0;//繰り返しモード
short int FLAG = 0;

/***1ステップ制御***/
//ステップ数
int step_count1=0;
int step_count2=0;
int step_count3=0;
int step_count4=0;
//1ステップ用のカウント
int Onestep_count1;
int Onestep_count2;
//タイマー割込みの周期[μs]
int T=500;
//クロック出力
boolean output_R1=LOW;
boolean output_L1=LOW;
boolean output_R2=LOW;
boolean output_L2=LOW;
//切り替えカウント指定の変数
int A=0;
int B=0;
int C=0;
int D=0;
//切り替えカウント変数
int Ai=0;
int Bi=0;
int Ci=0;
int Di=0;
//波長:λ
int r=0;

//スイッチ用のint
int S1=0;
int S2=0;
int S3=0;
int S4=0;

//吸引器用のpin
int K1=0;

//赤外線センサー用のpin
const int analogInPin = A0;
int L1=0;
int ad;

//ステップ計算用
double Mph_dt=0;
double MPh_setpR1=0;
double MPh_setpL1=0;
double MPh_d=0;
double MPh_log=0;//////201906121729
double Phi=0;
double Phi_log=0;//////201906121729
short int P=0;

//位置のチェック&モーター速度
int Position_check=0;
int step_check=0;
int dir=0;
int axis=0;
int speed=0;

void setup(){
  //シリアルモニターを開始
  Serial.begin(9600);
  
  //吸引器に使用するピン
  servo.attach(1);
  pinMode(13,OUTPUT);
  
  //吸引器がある側のモーターピン
     pinMode(3,OUTPUT); //ピンをデジタル出力に設定
     pinMode(4,OUTPUT); //ピンをデジタル出力に設定
     pinMode(5,OUTPUT); //ピンをデジタル出力に設定
     pinMode(6,OUTPUT); //ピンをデジタル出力に設定
     pinMode(7,OUTPUT); //ピンをデジタル出力に設定
     
   //吸引器がない側のモーターピン
     pinMode(8,OUTPUT); //ピンをデジタル出力に設定
     pinMode(9,OUTPUT); //ピンをデジタル出力に設定
     pinMode(10,OUTPUT); //ピンをデジタル出力に設定
     pinMode(11,OUTPUT); //ピンをデジタル出力に設定
     pinMode(12,OUTPUT); //ピンをデジタル出力に設定

  
   //タイマー割込み設定
     Timer1.initialize(T);
     Timer1.attachInterrupt(timerPulse);
}

void loop(){
  //赤外線センサーの値えを見る
  ad=analogRead(analogInPin);
  Serial.print(ad);
  Serial.print(" ,");
  delay(100);
  
  //スイッチ
  S1=digitalRead(14);
  S2=digitalRead(15);
  S3=digitalRead(17);
  S4=digitalRead(18);

  //吸引器に使用するpin
  K1=digitalRead(13);

  //赤外線センサー
  L1=analogRead(A0);

  if(flag_check==0){
    switch(flag){
      case 0:
      servo.write(32,30,true);
      motor_runA(axis,dir,speed);
      delay(1000);
      step_stop();
      if(S1==LOW&&S2==LOW){
        step_stop1();
        dir=1;
        motor_runA(axis,dir,speed);
        step_stop();
        flag=1;
      }
      break;
      case 1:
      axis=0,dir=0;
      motor_runB(axis,dir,speed);
      if(S3==LOW&&S4==LOW){
        step_stop2();
        dir=1;
        motor_runB(axis,dir,speed);
        
        flag=2;
      }
      break;
      case 2:
      dir=0;
      motor_runA(axis,dir,speed);
      if(S1==LOW&&S2==LOW){
        step_stop1();
        dir=1;
        motor_runA(axis,dir,speed);
        step_stop1();
        flag=3;
      }
      
      break;
      case 3:
      dir=0;
      motor_runB(axis,dir,speed);
      if(S3==LOW&&S4==LOW){
        step_stop2();
        flag=4;
      }
      break;
      case 4:
      flag_check=1;
      break;
    }
  }
  if(flag_check==1){
    switch(FLAG){
      case 0:
      flag=0;
      FLAG=1;
      break;
      case 1:
      dir=1;
      motor_runB(axis,dir,speed);
      step_stop();
      if(L1>=400){
        step_stop();
        servo.write(120,30,true);
        servo.write(32,30,true);
      }
      break;
    }
  }
}
