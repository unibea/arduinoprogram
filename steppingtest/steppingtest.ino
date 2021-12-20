#include <TimerOne.h>

//モータードライバー用端子 
#define CLOCK_L 8
//#define CLOCK_R 3
//#define CWCCW_R 7
#define CWCCW_L 7
#define MOTOR_ONOFFL 5
#define MOTOR_ONOFFR 4
boolean outputl=LOW; //パルス用変数
boolean outputr=LOW; //パルス用変数


int Pulse1=0;
int Pulse2=0;

int StepL=0;
int StepR=0;

int point=0;
int count=0;

//パルス周波数＝ステップ速度（1秒間に何ステップ）
//500usそのままだった場合1秒間に1000ステップ
int ppsL=0; //Lのパルス周波数（ステップ速度step/s）
int ppsR=0; //Rのパルス周波数（ステップ速度step/s）
void timerPulse() {
  
 
outputl=!outputl;
digitalWrite(CLOCK_L,outputl);
Serial.println(outputl);
}

void setup() {
  
  //シリアル通信
  Serial.begin(9600);
  
  pinMode(CLOCK_L,OUTPUT);
 // pinMode(CLOCK_R,OUTPUT);
  pinMode(CWCCW_L,OUTPUT);
  //pinMode(CWCCW_R,OUTPUT);
  pinMode(MOTOR_ONOFFL,OUTPUT);
  pinMode(MOTOR_ONOFFR,OUTPUT);
  pinMode(13,OUTPUT);//モータードライバー用電源５V
  digitalWrite(CWCCW_L,LOW);
  //digitalWrite(CWCCW_R,LOW);//回転方向/////////////////////////////////////////////////////////////
  digitalWrite(MOTOR_ONOFFL,LOW);
  digitalWrite(MOTOR_ONOFFR,LOW);//電源出力
  
  digitalWrite(13,HIGH);
  
  
   //タイマー割り込み設定
  Timer1.initialize(500);//500マイクロ秒単位で設定（250000で1秒に1パルス）
  Timer1.attachInterrupt(timerPulse);
  
}//void setup END...
    
void loop() {


}
//void End...
