#include <TimerOne.h>

//モータードライバー用端子 
#define CLOCK_L 6
#define CLOCK_R 3
#define CWCCW_R 7
#define CWCCW_L 2
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

int point=0;
int count=0;

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

void trace (){ 
  
  //1ｽﾃｯﾌﾟで0.6535mm進む
  Serial.print(S0);
  Serial.print("\t");
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");
  Serial.print(S3);
  Serial.print("\t");
  Serial.print(StepL);
  Serial.print("\t");
  Serial.println();
 
 ls=2.0;
 rs=2.0;

  S0=analogRead(LEFT);//0番ﾋﾟﾝ
  S1=analogRead(MLEFT);//1番ﾋﾟﾝ
  S2=analogRead(MRIGHT);//2番ﾋﾟﾝ
  S3=analogRead(RIGHT);//3番ﾋﾟﾝ

  if(S1>600&&S2<500){
      ls=2.0;
      rs=5.0;
    }//真ん中左のｾﾝｻｰが反応 左へ向ける
  if(S1<500&&S2>600){
      ls=5.0;
      rs=2.0;
    }//真ん中右のｾﾝｻｰが反応 右へ向ける
  
  if(S0<500&&S1>600){
      ls=25.0;
      rs=2.0;
    }//真ん中左のｾﾝｻｰが反応 左へ向ける
  if(S2>600&&S3<500){
      ls=2.0;
      rs=25.0;
    }//真ん中右のｾﾝｻｰが反応 右へ向ける
    
    if(S0<300&&S1<300&&S2>600&&S3>600){
      ls=50;
      rs=1.0;
    }//真ん中左のｾﾝｻｰが反応 左へ向ける
  if(S0>600&&S1>600&&S2<300&&S3<300){
      ls=1.0;
      rs=50;
    }//真ん中右のｾﾝｻｰが反応 右へ向ける
    //ラインカウンタ
      if(point==0){
          if(S0<300&&S3<300){
            count++;
            point=1;
            }
       }
       if(S0>700&&S3>700){
        point=0;
        }

 }

//旋回90度
void senkaiR(int spl,int Step){
   Timer1.detachInterrupt();
   Serial.print('1');
   delay(500);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(500);
   ls=spl;
   rs=spl;   
   Timer1.attachInterrupt(timerPulse);
   while(1){
    Serial.print('\t');
    Serial.println(StepL);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
     if(StepL>Step){
      break;
      }
    }
     Timer1.detachInterrupt(); 
     digitalWrite(CWCCW_L,HIGH);
     Serial.print('3');
     delay(1000); 
  }
  
int senkaiL(int SPR,int Step){
   Timer1.detachInterrupt();
   Serial.print('1');
   delay(500);
   digitalWrite(CWCCW_R,HIGH);//回転方向変更
   StepL=0;
   StepR=0;//ステップ数リセット
   Serial.print('2');
   delay(500);
   ls=SPR;
   rs=SPR;   
   Timer1.attachInterrupt(timerPulse);
   while(1){
    Serial.print('\t');
    Serial.println(StepR);////////////////なぜかシリアルプリントであたいを表示するとif文が機能する？
     if(StepR>Step){
      break;
      }
    }
     Timer1.detachInterrupt(); 
     digitalWrite(CWCCW_R,LOW);
     Serial.print('3');
     delay(1000);
} 
 void tansaku(int spl,int Step){
   Timer1.detachInterrupt();
   Serial.print('1');
   delay(500);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(500);
   ls=spl;
   rs=spl;   
   Timer1.attachInterrupt(timerPulse);
   while(1){
    Serial.print('\t');
    Serial.println(StepL);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
     if(StepL>Step){
      break;
      }
      
    }
     Timer1.detachInterrupt(); 
     digitalWrite(CWCCW_L,HIGH);
     Serial.print('3');
     delay(1000); 
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
  digitalWrite(CWCCW_L,HIGH);
  digitalWrite(CWCCW_R,LOW);//回転方向/////////////////////////////////////////////////////////////
  digitalWrite(MOTOR_ONOFFL,LOW);
  digitalWrite(MOTOR_ONOFFR,LOW);//電源出力
  
  digitalWrite(13,HIGH);
  
  
   //タイマー割り込み設定
  Timer1.initialize(500);//500マイクロ秒単位で設定（250000で1秒に1パルス）
  Timer1.attachInterrupt(timerPulse);
  
}//void setup END...
    
void loop() {
//switch()文でフェーズごとに作成
trace();
if(count==3){
  senkaiL(1.0,360);
  senkaiR(10.0,750);//(スピード,ステップ数
  
  }

}
//void End...
