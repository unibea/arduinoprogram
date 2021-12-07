#include <TimerThree.h>
#include <TimerOne.h>

//一番後ろのｻｰﾎﾞの値:1150閉める1700開ける
//一番前のｻｰﾎﾞ値:1380で前向く値:2300でしまう
//
//ﾓｰﾀｰﾄﾞﾗｲﾊﾞｰ用端子 
#define CLOCK_L 6
#define CLOCK_R 3
#define CWCCW_R 7
#define CWCCW_L 2
#define MOTOR_ONOFFL 5
#define MOTOR_ONOFFR 4

#define LEFT   0//左側のｾﾝｻｰを0番ﾋﾟﾝとする
#define MLEFT  1//真ん中左側のｾﾝｻｰを1番ﾋﾟﾝとする
#define MRIGHT 2//真ん中右側のｾﾝｻｰを2番ﾋﾟﾝとする
#define RIGHT  3//右側のｾﾝｻｰを3番ぴんとする

int phase=0;//動作の段階

//ﾄﾚｰｽｾﾝｻｰ用変数
int S0=0;
int S1=0;
int S2=0;
int S3=0;

float a=0;

int count=0;//ライン数の変数
int point=0;//ライン連続読み防止変数
  
//ﾎﾞｰﾙ認識用距離ｾﾝｻｰ変数
int T0=0;
//ｽﾃｯﾋﾟﾝｸﾞﾓｰﾀｰ関連変数
double il=0 , ir=0; //ﾊﾟﾙｽ幅調整用
double ls=0 , rs=0; //ﾊﾟﾙｽ幅調整用
boolean outputl=LOW; //ﾊﾟﾙｽ用変数
boolean outputr=LOW; //ﾊﾟﾙｽ用変数


int Pulse1=0;
int Pulse2=0;

int StepL=0;
int StepR=0;

 int rd=0;



//ﾊﾟﾙｽ周波数=ｽﾃｯﾌﾟ速度(1秒間に何ｽﾃｯﾌﾟ)
//500usそのままだった場合1秒間に1000ｽﾃｯﾌﾟ
double ppsL=1060; //Lのﾊﾟﾙｽ周波数(ｽﾃｯﾌﾟ速度step/s)////////////////////////////////////////////////////////////////////////////////////////////////パルス周波数変更
//1060>ppsL>450
int ppsR=0; //Rのﾊﾟﾙｽ周波数(ｽﾃｯﾌﾟ速度step/s)

double AT=0.0;//割り込みスピード（μs単位）

///////////////////////////////////////////////カラーセンサー
#define RANGE 9   // 8番ピンをRange端子に設定
#define GATE  11   // 9番ピンをGate端子に設定
#define CK   10    //10番ピンをCK端子に設定
#define DOUT 8    //11番ピンをDout端子に設定


const int signal = 12;//buzzer

int sen=5;

////////////////////////////////////////////////////////////////////////
void timerPulse1() {
  //if(il>=ls){
    digitalWrite(CLOCK_R,outputr);
    outputr = !outputr;
    //Serial.print("outputr=");
    //Serial.println(outputr);
    //Serial.print("outputr=");
    //Serial.println(outputr);
    //il=0;
   // }
//il++;
}

void timerPulse2() {
  digitalWrite(CLOCK_L,outputl);
  //digitalWrite(CLOCK_R,outputr);
outputl = !outputl;
//Serial.print("outputl=");
//Serial.println(outputl);
//outputr = !outputr;
}


//シフト㊞カラーセンサー関数に使用
void se(double noteNum, double len);


int shiftIn();
int color_judgement();
int  colorcheck();

int i=-100;//ｻｰﾎﾞの出力時間調整変数

void trace ();
void servo3(int p);
void servo1(int s);
void servoloopL();
void servoloopR();


//タイヤの速度計測関数
float SpeedL(int ls){
  float ST=0.0;//1ステップの時間
  float SL=0.0;
  ST=((ls+2)*0.0005)*2.0;
  SL=ST*400;//1回転のスピード
  return SL/58; //タイヤの直径仮で60mm
  }
  
float SpeedR(int rs){
  float ST=0.0;//1ステップの時間
  float SR=0.0;
  ST=((rs+2)*0.0005)*2.0;
  SR=ST*400;//1回転のスピード
  return SR/58; //タイヤの直径仮で60mm
  }

void setup() {/////////////////////////////////////////////////////////////////////setup()/////////////////
  //ｼﾘｱﾙ通信
  Serial.begin(9600);
  
  pinMode(CLOCK_L,OUTPUT);
  pinMode(CLOCK_R,OUTPUT);
  pinMode(CWCCW_L,OUTPUT);
  pinMode(CWCCW_R,OUTPUT);
  pinMode(MOTOR_ONOFFL,OUTPUT);
  pinMode(MOTOR_ONOFFR,OUTPUT);
  pinMode(13,OUTPUT);//ﾓｰﾀｰﾄﾞﾗｲﾊﾞｰ用電源5V

  pinMode(30,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(34,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(40,OUTPUT);//ｻｰﾎﾞﾓｰﾀｰﾋﾟﾝ  

  pinMode(12,OUTPUT);//ブザーピン
  
  digitalWrite(CWCCW_L,HIGH); //回転方向//
  digitalWrite(CWCCW_R,LOW);//回転方向//直進
  
  digitalWrite(MOTOR_ONOFFL,HIGH);//ﾓｰﾀｰへのﾊﾟﾙｽ出力 LOWで出力 HIGHで停止
  digitalWrite(MOTOR_ONOFFR,HIGH);//ﾓｰﾀｰへのﾊﾟﾙｽ出力
  
  digitalWrite(13,HIGH);//ﾓｰﾀｰﾄﾞﾗｲﾊﾞｰ用電源5V

  AT=((1/(2*ppsL))*1000000);
 
  se(987,0.2);
  se(1318,0.2);
  
  //Serial.print("buzer");
 // delay(500);///////////////////////////

  digitalWrite(MOTOR_ONOFFL,LOW);//ﾓｰﾀｰへのﾊﾟﾙｽ出力 LOWで出力 HIGHで停止
  digitalWrite(MOTOR_ONOFFR,LOW);//ﾓｰﾀｰへのﾊﾟﾙｽ出力
   
     //ﾀｲﾏｰ割り込み設定
 Timer1.initialize(500);//ﾏｲｸﾛ秒単位で設定(500000us=0.5sで1秒に1ﾊﾟﾙｽ)
  Timer1.attachInterrupt(timerPulse1);

 Timer3.initialize(500);
 Timer3.attachInterrupt(timerPulse2);
 
}//void setup END...
double spchL(double warikomiL){
    return (1/(2*warikomiL))*1000000;
  }

double spchR(double warikomiR){
    return (1/(2*warikomiR))*1000000;
  }

//割り込みスピード用変数
int HzSPL=spchL(1000),HzSPR=spchR(1000);

const float Kp=0.7;
const int targetL=530;
const int targetR=530;
const int bias=0;

int diffL=0,diffR=0;
int outL=0,outR=0;
void loop() {
      Timer1.initialize(HzSPL);
      Timer3.initialize(HzSPR);
      
  /*
ls=3;
rs=3;
Serial.print("計算結果");
Serial.print(AT);
Serial.print("パルス周波数＝");
Serial.println(ppsL);

  Serial.print(S0);
  Serial.print("\t");
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");
  Serial.print(S3);
  Serial.println();
*/ 

  S0=analogRead(LEFT);//0番ﾋﾟﾝ
  S1=analogRead(MLEFT);//1番ﾋﾟﾝ
  S2=analogRead(MRIGHT);//2番ﾋﾟﾝ
  S3=analogRead(RIGHT);//3番ﾋﾟﾝ


   
 diffL=targetL-S1;
 diffR=targetR-S2;

  outL=Kp*diffL+bias;
  outR=Kp*diffR+bias;

   HzSPL=spchL(450+outL);
   HzSPR=spchR(450+outR);

  /*Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");*/
  Serial.print(diffL);//青
  Serial.print("\t");
  Serial.print(diffR);//赤
  Serial.print("\t");
  Serial.print(600+outL);//緑
  Serial.print("\t");
  Serial.print(600+outR);//オレンジ
  Serial.println();

   

   
  

  if(S1>850&&S2>850){
   HzSPL=spchL(450);
   HzSPR=spchR(450);
    } //真ん中左のｾﾝｻｰが反応 左へ向ける
  /*if(S1<500&&S2>600){
   HzSPL=spchL(700);
   HzSPR=spchR(500);
    }//真ん中右のｾﾝｻｰが反応 右へ向ける
  */
 }//void 
