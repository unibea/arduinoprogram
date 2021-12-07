#include <TimerOne.h>
#include <FlexiTimer2.h>

//VarSpeedServo myservo0;

//繰り返しモードの設定用
short int  flag = 0;//動作パターン(FLAG)
bool flag_check = 1;//動作パターンのカウント操作
bool FLAG_check = 0;
short int flag_SWITCH = 0;//繰り返しモード
short int FLAG = 0;

/***リミットスイッチ***/
bool LSW0_check=1;
bool LSW1_check=1;
/***1ステップ制御***/
//ステップ数
int step_count1=0;
int step_count2=0;
//1ステップ用のカウント
int Onestep_count1;
int Onestep_count2;
int StepCount_sum;
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
//切り替えカウント変数
int Ai=0;
int Bi=0;
//波長:λ
int r=0;

//スイッチ用のint
//前のスイッチ
bool S1=0;
bool S2=0;
//横のスイッチ
bool S3=0;
bool S4=0;
///吸引器のスイッチ
bool S5=0;

//吸引器用のサーボpin
bool K1=0;

//赤外線センサー用のpin
bool L1=0;

//ステップ計算用
double Mph_dt=0;
double MPh_setpR1=0;
double MPh_setpL1=0;
double MPh_d=0;
double MPh_log=0;//////201906121729
double Phi=0;
double Phi_log=0;//////201906121729
short int P=0;
double ishu=0;

//位置のチェック&モーター速度&角度の変数
char Position_check=0;
int step_check=0;
int dir=0;
int axis=0;
int Gspeed_Hz=0;

void setup(){
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

  Serial.begin(9600);
   //タイマー割込み設定
     Timer1.initialize(T);
//     Timer1.attachInterrupt(timerPulse);
     FlexiTimer2::set(500, flash); // 500ms period
     FlexiTimer2::start();

}

void loop(){
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");
  Serial.print(S3);
  Serial.print("\t");
  Serial.print(S4);
  Serial.print("\t");
  Serial.print(flag);
  Serial.print("\t");
  Serial.println(ishu);
  //条件が満たされるまで拘束
  if(flag_check==0){
    //動作パターン
    switch(flag){
      //動作パターン⓪:リミットスイッチが２つ押さるまで連続回転(前進)
      case 0:
      axis=0;
      dir=0;
      Gspeed_Hz=5000;
      motor_runA(axis,dir,Gspeed_Hz);
      Position_check=0;
      break;
      //動作パターン①:指定した距離に進むまで回転(後進)
      case 1:
      axis=0;
      dir=1;
      Gspeed_Hz=5000;
      motor_runA(axis,dir,Gspeed_Hz);
      //条件設定//
      Position_check=1;
      P=45;//現在距離から45cm指定//
      break;

      //動作パターン②:リミットスイッチが２つ押さるまで連続回転(前進)
      /*case 2:
      axis=1;
      dir=0;
      Gspeed_Hz=5000;
      motor_runA(axis,dir,Gspeed_Hz);
      //条件設定//
      Position_check=0;
      break;    

      //動作パターン③:リミットスイッチが２つ押さるまで連続回転(前進)
      case 3:
      axis=1;
      dir=1;
      Gspeed_Hz=5000;
      motor_runA(axis,dir,Gspeed_Hz);
      //条件設定//
      Position_check=1;
      P=45;//現在距離から45cm指定//
      step_stop();
      break;  
      }
      //動作パターンの命令を拘束
      flag_check=1;*/
  }}

/*********センサ・距離・角度などの条件モード***********/
  if(Position_check==0){
    /*******リミットスイッチが押された時の処理文********/
    switch(dir){
      case 0:
      LSW0_check=0;
      break;
      case 1:
      LSW1_check=0;
      break;
    }
  }
  else if(Position_check==1){
    //指定距離Pで止まる処理文
    depth_Position(P);
  }
  else if(Position_check==2){
    angle(P);
  }
}
