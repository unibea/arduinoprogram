#include <TimerOne.h>

//モータードライバー用端子 
#define CLOCK_L  12
#define CLOCK_R  9
#define CWCCW_R 8
#define CWCCW_L 11 
#define MOTOR_ONOFF 10


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
boolean outcvl=HIGH;//回転方向用変数
boolean outcvr=LOW;//回転方向用変数

//パルス数カウント変数
boolean Pulsecount=HIGH;//パルスカウント変数　HIGH=ON  LOW=OFF
int Pulsecountl=0;
int Pulsecountr=0;

void timerPulse() {

  if(il>ls){
    digitalWrite(9,outputl);
    outputl = !outputl;
    il=0;
    if(Pulsecount==HIGH){
        Pulsecountl++;
      }
  }
  
  if(ir>rs){
    digitalWrite(12,outputr);
    outputr = !outputr;
    ir=0;
    if(Pulsecount==HIGH){
        Pulsecountr++;
      }
  }
    
  il++;
  ir++;  
}

void seil (int s){
    S0=analogRead(LEFT);//0番ピン
    S1=analogRead(MLEFT);//1番ピン
    S2=analogRead(MRIGHT);//2番ピン
    S3=analogRead(RIGHT);//3番ピン

    Serial.print(S0);
    Serial.print("\t");
    Serial.print(S1);
    Serial.print("\t");
    Serial.print(S2);
    Serial.print("\t");
    Serial.print(S3);
    Serial.println("\t");
   
    digitalWrite(8, outcvl);//outcvlに入力されている方向を8番ピンに出力する(実際に実行する)
    digitalWrite(11,outcvr);//outcvrに入力されている方向を11番ピンに出力する(実際に実行する)
    digitalWrite(10,HIGH);
    delayMicroseconds(1);
    ls=s;
    rs=s;
    if(S1>700 && S2<300){
        ls=s;
        rs=s+1;
       }
    if(S1<300&&S2>700){
        ls=s+1;
        rs=s;
      }
    if(S0<300&&S1>500){
        ls=s+47;
        rs=s;
      }
  if(S2>500&&S3<300){
      ls=s;
      rs=s+47;
      }
  }//void seli END...

void turn90R(){
      digitalWrite(10,LOW);
      Timer1.detachInterrupt();
      delay(1);
      ls=5.0;
      rs=5.0;
      delay(1);
      outcvr = HIGH;//後転
      outcvl = HIGH;//前転
      digitalWrite(11,outcvr);
      digitalWrite(8 ,outcvl);
      delay(1);
      Timer1.attachInterrupt(timerPulse);
      digitalWrite(10,HIGH);
      delay(5000);
      digitalWrite(10,LOW);
      Timer1.detachInterrupt();
      outcvr = LOW;//後転
      outcvl = HIGH;//前転
      digitalWrite(11,outcvr);
      digitalWrite(8 ,outcvl);
      delay(5000);
      
      
    }//90度右旋回

void setup() {
  //シリアル通信
  Serial.begin(9600);
  
  pinMode(CLOCK_L,OUTPUT);
  pinMode(CLOCK_R,OUTPUT);
  pinMode(CWCCW_L,OUTPUT);
  pinMode(CWCCW_R,OUTPUT);
  pinMode(MOTOR_ONOFF,OUTPUT);
  pinMode(13,OUTPUT);//モータードライバー用電源５V

  digitalWrite(MOTOR_ONOFF,LOW);
  digitalWrite(CWCCW_R,LOW);
  digitalWrite(CLOCK_L,LOW);
  digitalWrite(13,HIGH);

   //タイマー割り込み設定
  Timer1.initialize(500);//500マイクロ秒単位で設定（250000で1秒に1パルス）
  Timer1.attachInterrupt(timerPulse);
  
}//void setup END...

void loop() {
    digitalWrite(13,HIGH);
    Serial.print(Pulsecountl);
    Serial.print("\t");
    Serial.println(Pulsecountr);
    if(Pulsecountl==5000){
      digitalWrite(13,LOW);
      delay(5000);
      }
    if(Pulsecountr==5000){
      digitalWrite(13,LOW);
      delay(5000);
      }  
    
}//void 
