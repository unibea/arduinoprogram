#include <TimerOne.h>

//モータードライバー用端子 
#define CLOCK_L 6
#define CLOCK_R 3
#define CWCCW_L 7
#define CWCCW_R 2
#define MOTOR_ONOFFL 5
#define MOTOR_ONOFFR 4

#define LEFT   0//左側のセンサーを0番ピンとする
#define MLEFT  1//真ん中左側のセンサーを1番ピンとする
#define MRIGHT 2//真ん中右側のセンサーを2番ピンとする
#define RIGHT  3//右側のセンサーを3番ぴんとする

//トレースセンサー用変数
int S0=0;
int S1=0;
int S2=0;
int S3=0;

//ステッピングモーター関連変数
double il=0 , ir=0; //パルス幅調整用
double ls=0 , rs=0; //パルス幅調整用
boolean outputl=LOW; //パルス用変数
boolean outputr=LOW; //パルス用変数


int Pulse1=0;
int Pulse2=0;

int StepL=0;
int StepR=0;

//パルス周波数＝ステップ速度（1秒間に何ステップ）
//500usそのままだった場合1秒間に1000ステップ
int ppsL=0; //Lのパルス周波数（ステップ速度step/s）
int ppsR=0; //Rのパルス周波数（ステップ速度step/s）
void timerPulse() {
  if(il>ls){//ここを何秒で実行する　という風にへんこうする？割り込みは500usなのでそれを基準に設定
    digitalWrite(CLOCK_L,outputl);
    outputl = !outputl;
    il=0;
    Pulse1++;
  }
  
  if(ir>rs){
    digitalWrite(CLOCK_R,outputr);
    outputr = !outputr;
    ir=0;
    Pulse2++;
  }
 il++;
 ir++;
  if(Pulse1==2){
        StepL++;
        Pulse1=0;
      }//ステップ数の算出と距離の測定
  if(Pulse2==2){
        StepR++;
        Pulse2=0;
      }
       
}

void setup() {
  
  //シリアル通信
  Serial.begin(9600);
  
  pinMode(CLOCK_L,OUTPUT);
  pinMode(CLOCK_R,OUTPUT);
  pinMode(CWCCW_L,OUTPUT);
  pinMode(CWCCW_R,OUTPUT);
  pinMode(MOTOR_ONOFFL,OUTPUT);
  pinMode(MOTOR_ONOFFR,OUTPUT);
  pinMode(13,OUTPUT);//モータードライバー用電源５V
  digitalWrite(CWCCW_L,LOW);
  digitalWrite(CWCCW_R,HIGH);//回転方向
  digitalWrite(MOTOR_ONOFFL,LOW);
  digitalWrite(MOTOR_ONOFFR,LOW);
  
  digitalWrite(13,HIGH);

   //タイマー割り込み設定
  Timer1.initialize(500);//500マイクロ秒単位で設定（250000で1秒に1パルス）
  Timer1.attachInterrupt(timerPulse);
  
}//void setup END...

void loop() {
 //1ステップで0.6535mm進む
 Serial.print(StepL);
 Serial.print("\t");
 Serial.print(outputl);
 Serial.print("\t");
 Serial.print(StepR);
 Serial.print("\t");
 Serial.print(outputr);
 Serial.print("\t");
 Serial.println();
 
ls=1.0;
rs=1.0;
    

}//void 
