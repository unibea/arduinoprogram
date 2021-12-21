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

int ppsL=0; //Lのﾊﾟﾙｽ周波数(ｽﾃｯﾌﾟ速度step/s)
int ppsR=0; //Rのﾊﾟﾙｽ周波数(ｽﾃｯﾌﾟ速度step/s)
///////////////////////////////////////////////カラーセンサー
#define RANGE 9   // 8番ピンをRange端子に設定
#define GATE  11   // 9番ピンをGate端子に設定
#define CK   10    //10番ピンをCK端子に設定
#define DOUT 8    //11番ピンをDout端子に設定

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
void timerPulse1() {
    digitalWrite(CLOCK_R,outputr);
    outputr = !outputr;
    Pulse2++;
    if(Pulse2==2){
        StepR++;
        Pulse2=0;
      }//ｽﾃｯﾌﾟ数の算出と距離の測定
    
}

void timerPulse2() {
  digitalWrite(CLOCK_L,outputl);
  outputl = !outputl;
  Pulse1++;
  if(Pulse1==2){
        StepL++;
        Pulse1=0;
      }
}


//シフト㊞カラーセンサー関数に使用

int shiftIn(){
  int result=0;   //検出結果用の変数を用意（0：初期化）
  Serial.print("shiftIn開始");
  //12ビット分の繰り返し処理
  for(int i=0;i<12;i++){
    digitalWrite(CK,HIGH);//1ビット分のクロックパルス出力（HIGH）
    delayMicroseconds(1);//1マイクロ秒待機

    //Dout端子からの出力がHighの場合
    if(digitalRead(DOUT)==HIGH){
      result+=(1<<i);   //12ビットのi桁目に1を代入（i桁分だけ左にシフト）
    }

    digitalWrite(CK,LOW);   //1ビット分のクロックパルス出力（LOW）
    delayMicroseconds(1);   //1マイクロ秒待機
  }

  delayMicroseconds(3);   //3マイクロ秒待機
  return result;    //結果を出力  //2進数での結果を10進数の0~4095に変換している？
}

int color_judgement(){
  int judge_color = 0;    //red -> 1, blue -> 2 yellow -> 3
  Serial.print("color_jadgement開始");
  /*
   取得できた色のベクトルと基準となる各色ベクトルとの距離を調べる
   */
  //取得できた色のベクトルと赤ベクトルとの距離を調べる
  R_to_getColor = sqrt( (R_vec[0] - get_vec[0]) * (R_vec[0] - get_vec[0])
                       +(R_vec[1] - get_vec[1]) * (R_vec[1] - get_vec[1])
                       +(R_vec[2] - get_vec[2]) * (R_vec[2] - get_vec[2]) );

  //取得できた色のベクトルと青ベクトルとの距離を調べる
  B_to_getColor = sqrt( (B_vec[0] - get_vec[0]) * (B_vec[0] - get_vec[0])
                       +(B_vec[1] - get_vec[1]) * (B_vec[1] - get_vec[1])
                       +(B_vec[2] - get_vec[2]) * (B_vec[2] - get_vec[2]) );

  //取得できた色のベクトルと黄ベクトルとの距離を調べる
  Y_to_getColor = sqrt( (Y_vec[0] - get_vec[0]) * (Y_vec[0] - get_vec[0])
                       +(Y_vec[1] - get_vec[1]) * (Y_vec[1] - get_vec[1])
                       +(Y_vec[2] - get_vec[2]) * (Y_vec[2] - get_vec[2]) );
  
  /*
    ベクトルの距離を比較し，一番大きいベクトルを示す数値を変数に格納する
   */
  /*
  if(R_to_getColor < B_to_getColor){
    judge_color = 1;
  }
  else{
    judge_color = 2;
  }
  */
  if(Y_to_getColor < B_to_getColor&&Y_to_getColor<R_to_getColor){
    judge_color = 8;//黄色
  }else{
    if(R_to_getColor<B_to_getColor){
      judge_color = 6;//赤
      }else{
        judge_color = 7;//青
        }  
  }
  
  //print the result....
  Serial.print("Ball color is ");
  switch(judge_color) {
  case 6:
    Serial.println("Red");
    break;
  case 7:
    Serial.println("Blue");
    break;
  case 8:
    Serial.println("yellow");
    break;
  }
  return judge_color;//6番なら赤7番なら青8番なら黄色
}

//色認識関数
int  colorcheck(){
  Serial.print("colorcheck開始");
  
   int val=analogRead(4);
   int color=0;

  //Gate,CK端子をLowに設定
  digitalWrite(GATE,LOW);
  digitalWrite(CK,LOW);
  delayMicroseconds(2000);//2000マイクロ秒待機

  //感度設定（HIGH:高感度に設定）  
  digitalWrite(RANGE,HIGH);

  //測光開始（光量の積算を開始） 
  digitalWrite(GATE,HIGH);

  //測光時間（valを代入し可変的に設定）
  delay(val+1);
  
  //測光終了（光量の積算を終了） 
  digitalWrite(GATE,LOW);
  delayMicroseconds(4);//4マイクロ秒待機
  
  //数値の整理をする
  red = shiftIn();    //赤の処理
  green = shiftIn();    //緑の処理
  blue = shiftIn();   //青の処理
  ///////////////////////////////////////////////////////////////////////////
  //取得できた値を原点から伸びる位置ベクトルとして表現するために配列に格納する
  //[0] = red, [1] = green, [2] = blue
  get_vec[0] = red; 
  get_vec[1] = green; 
  get_vec[2] = blue;
  //配列の値を渡して判別色を判定する関数  
  color=color_judgement();
  ////////////////////////////////////////////////////////////////////////
  //結果をprintする
  Serial.println("//-- Get value --//");
  Serial.print("Red   = "); Serial.println(get_vec[0]);
  Serial.print("Green = "); Serial.println(get_vec[1]);
  Serial.print("Blue  = "); Serial.println(get_vec[2]);
  Serial.print("R_to_getColor"); Serial.println(R_to_getColor);
  Serial.print("B_to_getColor"); Serial.println(B_to_getColor);
  Serial.print("Y_to_getColor"); Serial.println(Y_to_getColor);
  

  //analogWrite(11, 250);
  //analogWrite(12, 250);
  //analogWrite(13, 250);//????????????????????????????????????????????????????????????????????????????????/音の原因 

  //Gate端子をHighに戻す  
  digitalWrite(GATE,HIGH);
  
/*
  //シリアル通信でProcessingへ三色の値を文字列で送信  
  if(Serial.available()>0){
    Serial.print(red,DEC);    Serial.print(",");
    Serial.print(green,DEC);   Serial.print(",");
    Serial.println(blue,DEC);    
    
    Serial.read();
    }
    */
    
    return color;
 }
 
//ブザー関数 se(周波数,長さ);
void se(double noteNum, double len){
  int ms;
  double f,temp;
  f = noteNum;
   temp = ( 1 / f * pow(10,6) / 2);
  ms = (int)temp;
  
  if(noteNum == 145){
 for(long i = 0; i < 500000 * len; i += ms*2){
  digitalWrite(12,LOW);
        delayMicroseconds(ms);
        delayMicroseconds(ms);
    }
  }else{
    for(long i = 0; i < 500000 * len; i += ms*2){
        digitalWrite(12,HIGH);
        delayMicroseconds(ms);
        digitalWrite(12,LOW);
        delayMicroseconds(ms);
    }
  }
  
}
//パルス周波数を割り込み速度を変換
double spchL(double warikomiL){
    return (1/(2*warikomiL))*1000000;
  }

double spchR(double warikomiR){
    return (1/(2*warikomiR))*1000000;
  }

//割り込みスピード用変数
int HzSPL=spchL(1000),HzSPR=spchR(1000);

//float Kp=0.7;
const int targetL=550;
const int targetR=550;
const int bias=0;

int diffL=0,diffR=0;
int outL=0,outR=0;

void trace (float Kp){
  Timer1.attachInterrupt(timerPulse1);
  Timer3.attachInterrupt(timerPulse2);
  Timer1.initialize(HzSPL);//R
  Timer3.initialize(HzSPR);//L
  
/* 1ｽﾃｯﾌﾟで0.6535mm進む
  Serial.print(S0);
  Serial.print("\t");
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");
  Serial.print(diffR);
  Serial.print("\t");
  Serial.print(diffL);
  Serial.print("\t");
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


  if(S1>850&&S2>850){
   HzSPL=spchL(450);
   HzSPR=spchR(450);
    }
    //何もなければ直進
      //ラインカウンタ
      if(point==0){
          if(S3<500){
            count++;
            point=1;
            }
       }
       if(S3>600){
        point=0;
        }

 }


 
  //・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・改善ポイント

//ここからｻｰﾎﾞﾓｰﾀｰ関数

int i=-100;//ｻｰﾎﾞの出力時間調整変数

void servo3(int p){
  digitalWrite(38,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(p); 
  digitalWrite(38,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(20000-p);
  }//固定ｻｰﾎﾞ一番前
void servo1(int s){
  digitalWrite(32,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(s); 
  digitalWrite(32,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(20000-s);
  }//固定ｻｰﾎﾞ一番後ろ 
void servoloopL(){
  digitalWrite(30,HIGH);
  delayMicroseconds(1670);
  digitalWrite(30,LOW);
  delayMicroseconds(1670);
  }//無限ｻｰﾎﾞ前向いて左側  
void servoloopR(){
  digitalWrite(34,HIGH);
  delayMicroseconds(1180);
  digitalWrite(34,LOW);
  delayMicroseconds(1180);
  }//無限ｻｰﾎﾞ前向いて左側


//旋回R
void senkaiR(int spl,int Step){
  Serial.print("右旋回");
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   delay(50);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(50);
   Timer1.initialize(spchL(spl));
   Timer3.initialize(spchR(spl)); 
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    Serial.print('\t');
    Serial.println(StepL);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
     if(StepL>Step){
      break;
      }
    }
     Timer1.detachInterrupt();
     Timer3.detachInterrupt(); 
     digitalWrite(CWCCW_L,HIGH);
     Serial.print('3');
     delay(10); 
  }
 
  //直進
int tyokusin(int spl,int Step)  {
  Serial.print("直進");
    Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   delay(50);
   digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(50);
   Timer1.initialize(spchL(spl));
   Timer3.initialize(spchR(spl)); 
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    Serial.print('\t');
    Serial.println(StepL);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
     if(StepL>Step){
      break;
      }
    }
     Timer1.detachInterrupt();
     Timer3.detachInterrupt();

     Serial.print('3');
     delay(10); 
     
  }


  int tyokusinS()  {
  Serial.print("直進");
    Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   delay(50);
   digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(50);
   Timer1.initialize(spchL(500));
   Timer3.initialize(spchR(500)); 
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    S0=analogRead(LEFT);//0番ﾋﾟﾝ
    S1=analogRead(MLEFT);//1番ﾋﾟﾝ
    S2=analogRead(MRIGHT);//2番ﾋﾟﾝ
    S3=analogRead(RIGHT);//3番ﾋﾟﾝ
     //ラインカウンタ
      if(point==0){
          if(S0<500&&S3<500&&S1<500){
            count++;
            point=1;
            }
       }
       if(S0>600&&S3>600){
        point=0;
        }
     if(count==2){
      phase=1;
      break;
      }
    }
     Timer1.detachInterrupt();
     Timer3.detachInterrupt();

     Serial.print('3');
     delay(10); 
     
  }

  
  //後進
int kousin(int spl,int Step)  {
  Serial.print("後進します");
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   delay(50);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   digitalWrite(CWCCW_R,HIGH);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(50);
   Timer1.initialize(spchL(spl));
   Timer3.initialize(spchR(spl)); 
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    Serial.print('\t');
    Serial.println(StepL);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
     if(StepL>Step){
      break;
      }
    }
     Timer1.detachInterrupt();
     Timer3.detachInterrupt();
     digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更 
     Serial.print('3');
     delay(10); 
  }
  
  //旋回L
int senkaiL(int spl,int Step){
 Serial.print("左旋回");
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   delay(50);
   digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,HIGH);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(50);
   Timer1.initialize(spchL(spl));
   Timer3.initialize(spchR(spl)); 
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    Serial.print('\t');
    Serial.println(StepL);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
     if(StepL>Step){
      break;
      }
    }
     Timer1.detachInterrupt();
     Timer3.detachInterrupt(); 
     digitalWrite(CWCCW_R,LOW);
     Serial.print('3');
     delay(10); 
}

int DFL=0;
int DFR=0; 

int j=-500;

//探索
 int tansaku(){
  senkaiL(500,360);
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   Serial.print('1');
   delay(50);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   delay(500);
   ls=10.0;
   rs=10.0;
   Timer1.initialize(spchL(200));
   Timer3.initialize(spchR(200));    
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    T0=analogRead(7);
    Serial.print('\t');
    Serial.println(T0);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
    if(T0>550){
      DFL=StepL;
      DFR=StepR;    
      break;
      }
    if(StepL>710){
      break;
      }
   }    
     Timer1.detachInterrupt();
     Timer3.detachInterrupt();
     digitalWrite(CWCCW_L,HIGH);
     if(T0>550){ 
      //DFL=StepL;
      //DFR=StepR;
     for(int i=0;i<80;i++){
      servo3(1700);
      servo1(1000);
    }
    return 4;
  }
  senkaiL(500,360);
  tyokusin(500,100);
  return 3;
 }
 
//回収
void kaisyuu(){
     
    Timer1.initialize(spchL(100));
    Timer3.initialize(spchR(100));  
    Timer1.detachInterrupt();
    Timer3.detachInterrupt();
    Serial.print("11");
    delay(500);
    ls=10.0;
    rs=10.0;
    StepL=0;
    StepR=0;
    Timer1.attachInterrupt(timerPulse1);
    Timer3.attachInterrupt(timerPulse2);
    
    for(int i=-1000;i<0;i++){
       servoloopL();       
       servoloopR();
      T0=analogRead(7);
      Serial.print(StepL);
      Serial.print(" ");
      Serial.println(T0);
      
      if(300<T0&&T0<450){
        Timer1.detachInterrupt();
        Timer3.detachInterrupt();
        rd=StepL;
        } 
      }
      digitalWrite(CWCCW_L,LOW);
      digitalWrite(CWCCW_R,HIGH);
      
      StepL=0;
      delay(500);
      Timer1.attachInterrupt(timerPulse1);
      Timer3.attachInterrupt(timerPulse2);
      while(1){
        Serial.print(StepL);
        Serial.print(" ");
        Serial.println(rd);
        if(rd<StepL){//これでバックすることが可能に？////////////////////////////////////////////////////////////////////////////////////////////////////////
          break;
          }  
        }
        Timer1.detachInterrupt();
        Timer3.detachInterrupt();
        delay(1500);
   }

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

  //Range,Gate,CK端子をデジタル出力に設定
  pinMode(RANGE,OUTPUT);
  pinMode(GATE,OUTPUT);
  pinMode(CK,OUTPUT);

  //Dout端子をデジタル入力に設定
  pinMode(DOUT,INPUT);

  se(987,0.2);
  se(1318,0.2);
  
  Serial.print("buzer");
  delay(500);///////////////////////////

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
  i=-300;//時間調整変数初期化
  
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
  delay(500);///////////////////////////
  
  digitalWrite(MOTOR_ONOFFL,LOW);//ﾓｰﾀｰへのﾊﾟﾙｽ出力 LOWで出力 HIGHで停止
  digitalWrite(MOTOR_ONOFFR,LOW);//ﾓｰﾀｰへのﾊﾟﾙｽ出力
  /*      //ﾀｲﾏｰ割り込み設定
 Timer1.initialize(500);//ﾏｲｸﾛ秒単位で設定(500000us=0.5sで1秒に1ﾊﾟﾙｽ)
  Timer1.attachInterrupt(timerPulse1);

 Timer3.initialize(500);//仮
 Timer3.attachInterrupt(timerPulse2);*/
}//void setup END...
int d=0,s=0;
void loop() {

  switch(phase){
    case 0:
        Serial.print("スタートしてとりあえず直進しますよ〜〜〜");
        Serial.println(count);
        tyokusinS();
       // if(count==2){phase=1;}
      break;
    case 1:
        Serial.println("ここで自由ボールを落としますよ〜");
        senkaiR(500,360);//90度旋回
        delay(500);
        kousin(500,300);
        delay(500);
        tyokusin(500,300);
        delay(500);
        senkaiL(500,360);
        delay(500);
        phase=2;
      break;
    case 2:
       // Serial.println("ボールのある場所まで移動しますよ〜〜〜〜〜〜〜〜〜〜〜");
        trace(0.7);
        if(count==5){
            Serial.println("if文の中ですよ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜");
            kousin(500,1000);
            tyokusin(500,800);
            delay(750);
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
         phase=5;
       break;
     case 5:
          Serial.println("ボールを持ってゴールまで向かいますよ〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜");
          Serial.println(DFL);
          senkaiR(500,740-DFL);//ズレすぎ？760
          delay(10);
          senkaiR(500,330);//変更点360で90度
          delay(10);
          tyokusin(500,2000);
          delay(10);
          kousin(500,250);//もとは300
          delay(10);
          senkaiL(500,360);//もとは360
          //delay(10);
          //colorti=colorcheck();
          //Serial.println(colorti);
          //digitalWrite(MOTOR_ONOFFL,HIGH);//ﾓｰﾀｰへのﾊﾟﾙｽ出力 LOWで出力 HIGHで停止
          //digitalWrite(MOTOR_ONOFFR,HIGH);//ﾓｰﾀｰへのﾊﾟﾙｽ出力
          delay(1000);//長時間停止　この時にシリアルプリントで値確認
          count=0;
          s=0,j=-500;         
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
            s++;}
          //Serial.print("赤色が選択されました");
          trace(0.65);
          if(count==3){
            senkaiL(500,360);
            delay(100);
            kousin(500,200);//落ちないか確認
            while(j<0){
           servo1(1600);////////servo1(1600)で開く//////////////////////////////////////////////////////////////////////////////
           j++;
          }
          j=-100;
          while(j<0){
           servo1(1000);////////servo1(1150)で閉じる//////////////////////////////////////////////////////////////////////////////
           j++;
          }        
            delay(100);      
            tyokusin(500,200);
            delay(1);
            senkaiL(500,360);
            tyokusin(500,100);
            count=2;
            }
          
        break;
      case 7://青を認識した
            while(s<1){
            se(783,1.0);
            se(659,1.5);
            count=0;
            s++;}
           // Serial.print("青色が選択されました");
          trace(0.5);//ゲインを下げる 
          if(count==1){
            senkaiL(500,720);
            delay(100);
            kousin(500,500);//落ちないか確認
            while(j<0){
           servo1(1600);////////servo1(1600)で開く//////////////////////////////////////////////////////////////////////////////
           j++;
          } 
          j=0;
          while(j<1000){
           servo1(1000);////////servo1(1150)で閉じる//////////////////////////////////////////////////////////////////////////////
           j++;
          }i=-50;
          while(i<0){
          servo3(800);
          i++;
          }       
            delay(100);
            tyokusin(500,580);
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
          trace(0.7s);
          if(count==2){
            senkaiL(500,360);
            delay(100);
            kousin(500,200);//落ちないか確認
            
            while(j<0){
           servo1(1600);////////servo1(1600)で開く//////////////////////////////////////////////////////////////////////////////
           j++;
          }
          j=-300;
          while(j<0){
           servo1(1000);////////servo1(1150)で閉じる//////////////////////////////////////////////////////////////////////////////
           j++;
          }         
          Serial.print("黄色のボールを落としました。");
            delay(100);
             tyokusin(500,200);
             senkaiL(500,360);
             tyokusin(500,100);
             count=3;
             phase=2;     
            }          
          break;  
                               
    default:
      while(1){
         digitalWrite(MOTOR_ONOFFL,HIGH);//ﾓｰﾀｰへのﾊﾟﾙｽ出力 LOWで出力 HIGHで停止
  digitalWrite(MOTOR_ONOFFR,HIGH);//ﾓｰﾀｰへのﾊﾟﾙｽ出力
        }
      break;   
  }
 }//void 
