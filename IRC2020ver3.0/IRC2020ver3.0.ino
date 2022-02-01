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

#define RANGE 9   // 8番ピンをRange端子に設定
#define GATE  11   // 9番ピンをGate端子に設定
#define CK   10    //10番ピンをCK端子に設定
#define DOUT 8    //11番ピンをDout端子に設定


int phase=0;//動作の段階
/* case 0:スタート直後からコース１本目の線まで直進
   case 1:90度旋回して自由ボールを落とす
   case 2:P制御で進みつつ回収場所まで進んだら位置調整を行う
   case 3:ボール探索を行う（ボールを発見したらサーボを上げるまで）
   case 4:ボール回収を行う（ボールを回収し進んだ分だけ戻る部分まで）
   case 5:(回転してor壁当てしてから回転)して後ろを向きボールの色認識を行う。
   case 6:赤色のゴールにボールを運びcount値をリセットしてcase2に戻る
   case 7:青色のゴールにボールを運びcount値をリセットしてcase2に戻る
   case 8:黄色のゴールにボールを運びcount値をリセットしてcase2に戻る
   */

//ﾄﾚｰｽｾﾝｻｰ用変数
int S0=0;
int S1=0;
int S2=0;
int S3=0;

float dst1=0,dst2=0;//前センサcm表示変数


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
int Pulse2=0;//割り込み数

int StepL=0;//ステップ数：割り込み数２つで１つの凹凸
int StepR=0;

int rd=0;//ボール回収の際に進んだステップ数を記録する変数
 
int GET=0;//ボールを回収した数（回収フェーズが作動した数）

//ﾊﾟﾙｽ周波数=ｽﾃｯﾌﾟ速度(1秒間に何ｽﾃｯﾌﾟ)
//500usそのままだった場合1秒間に1000ｽﾃｯﾌﾟ

int ppsL=0; //Lのﾊﾟﾙｽ周波数(ｽﾃｯﾌﾟ速度step/s)
int ppsR=0; //Rのﾊﾟﾙｽ周波数(ｽﾃｯﾌﾟ速度step/s)

///////////////////////////////////////////////カラーセンサー
int red, green, blue;   //RGB三色の変数を用意

int colorti=0;

//各基準色を原点から伸びる位置ベクトルで表現する, 0~4095の16色
double R_vec[3] = {4095.0, 0.0, 0.0}; //red
double B_vec[3] = {0.0, 0.0, 4095.0}; //blue
double Y_vec[3] = {4095.0, 4095.0, 0.0}; //yellow

//取得できた色情報を原点から伸びる位置ベクトルで表現する
int get_vec[3] = {};  //初期値0, 0~4095の実数

//基準色ベクトルと取得色ベクトルとの距離を示す変数
double R_to_getColor;
double B_to_getColor;
double Y_to_getColor;

const int signal = 12;//buzzer

int sen=5;

////////////////////////////////////////////////////////////////////////
void timerPulse1();//pt宣言
void timerPulse2();//pt宣言
//パルス周波数を割り込み速度を変換
double spchL(double warikomiL){
    return (1/(2*warikomiL))*1000000;
  }

double spchR(double warikomiR){
    return (1/(2*warikomiR))*1000000;
  }


//シフト㊞カラーセンサー関数に使用
int shiftIn();
int color_judgement();
int  colorcheck();

void se(); 
//ブザー関数 se(周波数,長さ);


//割り込みスピード用変数仮の初期値を代入
int HzSPL=spchL(1000),HzSPR=spchR(1000);

//float Kp=0.7;P制御目標値とバイアス値の変数
const int targetL=550;
const int targetR=550;
const int bias=0;
int diffL=0,diffR=0;//目標値との差の変数
int outL=0,outR=0;//制御量出力変数

void trace ();
void tracet(); 


//ここからｻｰﾎﾞﾓｰﾀｰ関数
void servo3();
void servo1();  
void servoloopL();   
void servoloopR();   
//ここまでは確認済み//

void senkaiR();
int tyokusin();
int tyokusinS();
int kousin();
int senkaiL();  

int DFL=0;//探索時の旋回量記録変数


int d=0,s=0,Frag=0;//Frag回数指定フラグ
//距離センサ距離の変数
float kyori=0;

//ｻｰﾎﾞの出力時間調整変数
int i=-50;
int j=-100;
int tansaku();
void kaisyuu();


void setup() {/////////////////////////////////////////////////////////////////////setup()/////////////////
  //ｼﾘｱﾙ通信
  Serial.begin(9600);
  
  pinMode(CLOCK_L,OUTPUT);
  pinMode(CLOCK_R,OUTPUT);//パルス出力ピン
  pinMode(CWCCW_L,OUTPUT);
  pinMode(CWCCW_R,OUTPUT);//モータ回転方向出力ピン
  pinMode(MOTOR_ONOFFL,OUTPUT);
  pinMode(MOTOR_ONOFFR,OUTPUT);//モーター電源ピン
  pinMode(13,OUTPUT);//ﾓｰﾀｰﾄﾞﾗｲﾊﾞｰ用電源5V

  pinMode(30,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(34,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(40,OUTPUT);//ｻｰﾎﾞﾓｰﾀｰﾋﾟﾝ  

  pinMode(12,OUTPUT);//ブザーピン
  
  digitalWrite(CWCCW_L,HIGH); //回転方向
  digitalWrite(CWCCW_R,LOW);//回転方向//この状態で直進
  
  digitalWrite(MOTOR_ONOFFL,HIGH);//ﾓｰﾀｰへの電源出力 LOWで出力 HIGHで停止
  digitalWrite(MOTOR_ONOFFR,HIGH);//ﾓｰﾀｰへの電源出力
  
  digitalWrite(13,HIGH);//ﾓｰﾀｰﾄﾞﾗｲﾊﾞｰ用電源5V

  //カラーセンサー用
  pinMode(RANGE,OUTPUT);
  pinMode(GATE,OUTPUT);
  pinMode(CK,OUTPUT);
  pinMode(DOUT,INPUT);

  se(987,0.2);
  se(1318,0.2);//起動音
  Serial.print("buzer");
 
  //事前作業
  while(i<0){
    servo1(1000);//1150
    i++;
  }//後方ｻｰﾎﾞﾃｽﾄと初期位置の出力
  
  i=-80;//時間調整変数初期化
  
  while(i<0){
    servo3(800);
    i++;
  }//前方ｻｰﾎﾞﾃｽﾄと初期位置の出力
  i=-150;//時間調整変数初期化
  
  while(i<0){
    servoloopL();
    servoloopR();
    i++;
  }//ﾙｰﾌﾟｻｰﾎﾞﾃｽﾄ
    
  while(1){
    T0=analogRead(7);
    if(T0>500){
      break;
    }
  }//ｾﾝｻｰに手を近づけるとｽﾀｰﾄ(setupから抜ける)
  se(1318,0.4);
  se(1567,0.4);
  se(2093,0.4);
   
  digitalWrite(MOTOR_ONOFFL,LOW);//ﾓｰﾀｰへの電源出力 LOWで出力 HIGHで停止
  digitalWrite(MOTOR_ONOFFR,LOW);//ﾓｰﾀｰへの電源出力

}//void setup END...

void loop() {

  switch(phase){
    case 0:
        Serial.print("スタート");
        Serial.println(count);
        tyokusinS();
        // senkaiL(500,357);//90度旋回試験用
        if(count==2){phase=1;}
      break;
    case 1:
        Serial.println("ここで自由ボールを落とす");
        senkaiR(500,380);//90度旋回
        //delay(500);
        kousin(500,300);
        //delay(500);
        tyokusin(500,290);
        //delay(500);
        senkaiL(500,366);
        //delay(500);       
        phase=2;
      break;
    case 2:
        trace(1.0);
        StepL=0;
        if(count==5){
          while(1){
            tracet(1.3);
            if(GET<=4&&StepL>400){Serial.print("GET1,GET=");Serial.println(GET);break;}
            else if(GET>=5&&GET<=8&&StepL>600){Serial.println("GET2,GET=");Serial.println(GET);break;}
            else if(GET>=9&&GET<=12&&StepL>1000){Serial.print("GET3,GET=");Serial.println(GET);break;}
            else if(GET>=13&&StepL>1500){Serial.print("GET4,GET=");Serial.println(GET);break;}       
          }
          kousin(400,350);//後ろの壁当てをすると長距離走ることになりずれるため出来るだけライン上からはなれないようトレースして少し進み戻る。
          phase=3;
       }
       break;   
     case 3:
         Serial.println("ボールを探索しますよ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜"); 
         phase=tansaku();
        break;
     case 4:
         Serial.println("ボールを回収しますよ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜");
         kaisyuu();
         Frag=0;
         phase=5;
       break;
     case 5:
          Serial.println("ボールを持ってゴールまで向かいますよ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜");
          Serial.println(DFL);
          while(1){
            Serial.print("そのまま");
            senkaiR(500,727-DFL);//ズレすぎ？760
            if(GET==3||GET==6||GET==9||GET==12||GET==15||kyori>15.0){break;}
            senkaiR(500,360);//変更点360で90度
            Frag=1;
            break;
          }
          while(1){
            Serial.print("壁当て");
            if(Frag==1){break;}
            kousin(500,1100);
            tyokusin(500,785);
            senkaiR(500,360);
            break;
          }
          kousin(500,100);
          count=0;
          s=0,j=-100,i=-50;         
          phase=colorcheck();     
          Serial.println("ここで色判定をしますよ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜");
          Serial.print("色判定結果");
          Serial.println(phase);//６：赤色、７：青色、８：黄色
       break;
      case 6://赤を認識した場合
          while(s<1){
            se(277,0.5);
            se(330,0.5);
            se(370,0.5);
            se(330,0.5);
            se(440,0.5);
            Serial.print("s=");
            Serial.println(s);
            s++;
            }
          //Serial.print("赤色が選択されました");
          trace(1.0);
          if(count==4){
            senkaiL(500,372);
            //delay(100);
            //kousin(500,200);//落ちないか確認
          while(j<0){
              servo1(1600);////////servo1(1600)で開く//////////////////////////////////////////////////////////////////////////////
              j++;
            }
            kousin(500,200);//落ちないか確認
            tyokusin(500,185);
            j=-100;
          while(j<0){
            servo1(1000);////////servo1(1150)で閉じる//////////////////////////////////////////////////////////////////////////////
            j++;
            }
          while(i<0){
            servo3(800);
            i++;
            }        

            senkaiL(500,372);
            tyokusin(500,100);
            count=2;
            phase=2;
          }
          
        break;
      case 7://青を認識した
            while(s<1){
            se(783,1.0);
            se(659,1.5);
            count=0;
            s++;}
          trace(0.9);//ゲインを下げる 
          if(count==2){
            senkaiL(500,720);
            //delay(100);           
            while(j<0){
           servo1(1600);////////servo1(1600)で開く//////////////////////////////////////////////////////////////////////////////
           j++;
          }
           kousin(500,550);//落ちないか確認 
           tyokusin(500,580);
          j=-100;
          while(j<0){
           servo1(1000);////////servo1(1150)で閉じる//////////////////////////////////////////////////////////////////////////////
           j++;
          }
          while(i<0){
          servo3(800);
          i++;
          }       
           // delay(100);           
            count=4;
            phase=2;
              }      
            //phase=9;
          break;
      case 8://黄色を認識した
          while(s<1){
          se(349,0.2);
            se(440,0.2);
            se(523,0.2);
            se(349,0.2);
            se(440,0.2);
            se(523,0.2);
            se(349,0.2);
            se(440,0.2);
            se(523,0.2);
            s++;}
          //Serial.print("黄色が選択されました");
          trace(1.0);
          if(count==3){//変更点111
            senkaiL(500,372);
           // delay(100);
            //kousin(500,200);//落ちないか確認
            
            while(j<0){
           servo1(1600);////////servo1(1600)で開く//////////////////////////////////////////////////////////////////////////////
           j++;
          }
           kousin(500,200);//落ちないか確認
           tyokusin(500,183);
          j=-100;
          while(j<0){
           servo1(1000);////////servo1(1150)で閉じる//////////////////////////////////////////////////////////////////////////////
           j++;
          }
          while(i<0){
          servo3(800);
          i++;
          }            
          Serial.print("黄色のボールを落としました。");
            //delay(100);
             
             senkaiL(500,372);
             tyokusin(500,100);
             count=3;
             phase=2;     
            }          
          break;  
  }                       
 }//void 