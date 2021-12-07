//一つ一つの動作を他のところで移しテストを行う。
#include <TimerOne.h>

//カラーセンサー用端子
#define RANGE 4 //6番ピンをRANGEとする
#define GATE  2 //4番ピンをGATEとする
#define CK    3 //5番ピンをCKとする
#define DOUT  5//7番ピンをDOUTとする

//前方センサー端子（仮）
#define Sensor 15

//モータードライバー用端子 
#define CLOCK_L  12
#define CLOCK_R  9
#define CWCCW_R 8
#define CWCCW_L 11 
#define MOTOR_ONOFF 10

//トレースセンサー用端子

#define LEFT   0//左側のセンサーを0番ピンとする
#define MLEFT  1//真ん中左側のセンサーを1番ピンとする
#define MRIGHT 2//真ん中右側のセンサーを2番ピンとする
#define RIGHT  3//右側のセンサーを3番ぴんとする

//サーボ制御端子
#define makitoriL 0//巻取り用のサーボの左側
#define makitoriR 1//巻取り用のサーボの右側
#define servoF    2//ボール止めサーボの前側
#define servoB    3//ボール止めサーボの後ろ側　//デジタルピンでの制御に変わったので変更の必要あり

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

//前方センサー用変数
int sensor=0;

//カラーセンサー関連変数
int red,green,blue;//色の値を格納する変数
int get_vec[3]={};//手に入れた色の値をベクトル上に載せる
//赤青黄色の基準色ベクトルを配列で格納する
double R_vec[3]={4095.0,0.0,0.0};
double B_vec[3]={0.0,0.0,4095.0};
double Y_vec[3]={4095.0,4095.0,0.0};
//赤青黄色の基準とget_vecの距離を示す。//一番短いものをその時の色とする
double R_to_getColor=0;
double B_to_getColor=0;
double Y_to_getColor=0;

void timerPulse();//?

//動作コード用変数//////////////////////////////////////////////////////////////////////////動作コード
int post=0; 

//時間確認のための変数（探索）
int T=0;
int S=0;//ボール回収の回数変数
int X=1;

//ラインカウンター用変数
int Line_counter=0;
int Line_check=0;

int t=0;
int s=0;
int u=0;

int through ;//回収の際ボールが通ったか確認のため

int V=0;//サーボ時間管理　配列で書き換え
int W=0;
int Y=0;
int Z=0;
int U=0;
int A=0;
int B=0;
void setup() {
  Serial.begin(9600);
  //モータードライバー関連

  pinMode(CLOCK_L,OUTPUT);
  pinMode(CLOCK_R,OUTPUT);
  pinMode(CWCCW_L,OUTPUT);
  pinMode(CWCCW_R,OUTPUT);
  pinMode(MOTOR_ONOFF,OUTPUT);
  pinMode(13,OUTPUT);//モータードライバー用電源５V

  pinMode(30,OUTPUT);//サーボ用デジタルピン0
  pinMode(34,OUTPUT);//ピン番号（仮）

  digitalWrite(30,LOW);//サーボ用デジタルピン1
  digitalWrite(21,LOW);//ピン番号（仮）

  digitalWrite(MOTOR_ONOFF,LOW);
  digitalWrite(CWCCW_R,LOW);
  digitalWrite(CLOCK_L,LOW);
  digitalWrite(13,HIGH);/////////////////////////////////////////////変更点

  //カラーセンサー関連
  pinMode(RANGE,OUTPUT);
  pinMode(GATE,OUTPUT);
  pinMode(CK,OUTPUT);
  pinMode(DOUT,INPUT);//DOUT端子をデジタル入力

  //シリアル通信
  Serial.begin(9600);

  //タイマー割り込み設定
  Timer1.initialize(500);//500マイクロ秒単位で設定（250000で1秒に1パルス）
  Timer1.attachInterrupt(timerPulse);

  //巻取りサーボチェック
  while(Y<10){
    while(V<20){
      digitalWrite(30,HIGH);
      delayMicroseconds(1000);
      digitalWrite(30,LOW);
      delayMicroseconds(1000);
      V++;
      Serial.println(V);
    } //end of while(V<20)
    
    while(W<20){
      digitalWrite(34,HIGH);
      delayMicroseconds(2000);
      digitalWrite(34,LOW);
      delayMicroseconds(2000);
      W++;
      Serial.println(W);
    } //end of while(W<20)
    
    W=0;
    V=0;
    Y++;
  } //end of while(Y<10)
  
  //サーボ500マイクロで0度　2500マイクロで180度　2000　1000
  while(Z<20){
    digitalWrite(19,HIGH);
    delayMicroseconds(1450);//前センサーのちょうどよい角度
    digitalWrite(19,LOW);
    delayMicroseconds(1450);
    Z++;
    Serial.println(Z);
  }
  
  while(U<10){
    digitalWrite(34,HIGH);
    delayMicroseconds(1500);
    digitalWrite(34,LOW);
    delayMicroseconds(1500);
    U++;
    Serial.println(U);
  }
  
  //サーボの時間変数リセット
  V=0;
  W=0;
  Y=0;
  Z=0;
  S=0;
  A=0;
  //配列に書き換える
} //end of setup()

void loop() {

 
  //color_recognition();
  //トレース関連  
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
  Serial.print("\t");
  Serial.print(Line_counter);
  Serial.print("\t");
  Serial.print(Line_check);
  Serial.print("\t");
  Serial.print(post);
  Serial.print("\t");
  Serial.println(sensor);
  

  //前方センサー関連
  sensor=analogRead(15);//


  digitalWrite(8, outcvl);//outcvlに入力されている方向を8番ピンに出力する(実際に実行する)
  digitalWrite(11,outcvr);//outcvrに入力されている方向を11番ピンに出力する(実際に実行する)
  digitalWrite(10,HIGH);
  delay(1);//////////////////////////////////////////////////////////////////
  ls=3.0;
  rs=3.0;
      //ライントレース関連
  if(S1>700&&S2<300){//(仮)
    ls=2.0;
    rs=4.0;
  }
    
  if(S1<300&&S2>700){ //（仮）
    ls=4.0;
    rs=2.0;
  }
   

  if(S0<300&&S1>500){
    ls=50.0;
    rs=2.0;
  }
  /*    
  if(S0>700&&S1<300){ 
    ls=50.0;
    rs=2.0;
  }
  */  
  if(S2>500&&S3<300){
    ls=2.0;
    rs=50.0;
  }
   /* 
  if(S2<300&&S3>700){ 
    ls=50.0;
    rs=2.0;
    }
  */
//   color_recognition();

  //なぜか上の関数を呼び出すとlsやrsに入力された速度と違う速度が出力される
//  Line_count();
 /*
  switch(post){
    case 0:
      if(Line_counter==2){
        delay(1);
        turn90R();
        back();
        delay(1);
        turn90L();
        Line_counter=0; 
        delay(1);
        post=1;
      }
      break;
 /*     
    case 1:
      if(Line_counter==3){
        digitalWrite(10,LOW);
        delay(1000);
        tansaku();
        post=2;
      }
      break;

    case 2:
      //delay(5000);
      break;*/
  // } end of switch(post)
 

} //end of loop()
