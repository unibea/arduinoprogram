
// センサ入力テスト
// ESP32用 ステッピングモータ回転テスト
// CLOCK 25,26番ピン、CW/CCW 27,14番ピン ON/OFF 13番ピン
// 2020/7/17 SWのテスト機能を追加

#define   SPD        100    // デフォルト起動間隔 10x100=1000us毎 100 150
#define   PIDPARA    300    // 微調整用倍率  

  int n=0;
  int il=0,ir=0;  // パルス幅調整用
  int ls=6, rs=6; // パルス周波数用
  boolean outputl=LOW;    // パルス用変数
  boolean outputr=LOW;    // パルス用変数
  boolean cw=LOW;      // 回転方向用変数
  boolean ccw=HIGH;    // 回転方向用変数
  hw_timer_t *timer1=NULL;  // ﾀｲﾏｰ設定に使用
  hw_timer_t *timer2=NULL;  // ﾀｲﾏｰ設定に使用

  int sd1,sd2,sd3,sd4,sd5;	//センサー読取用 7/17
//  int sd1,sd2,sd3,sd4;	//センサー読取用
//  int f1,f2,f3,f4;	// モータコントロール用
  char srr;		// 前方センサー右   2->3
  char srl;		// 前方センサー左   3->4
  char str;		// 前方センサー右外 1->2
  char stl;		// 前方センサー左外 4->5
  char stg;		// ゴールセンサー      1 GPI36 7/17
  unsigned char Gsenfrot;	// 前方全センサーの値 
  // PID制御用  2018/10/19
  int integral =0;
  int diff[2] ={0,0};
  int s_angle;			// 曲がり具合 
  int sp;
  int si;
  int sd;
  char sf;		// センサー値よりの判断フラグ
  int target =0;	// 目標値 とりあえず[直進は 0  右+ 左-] 
  int delta_t = 1;	// 処理周期 ミリ秒単位 初期は1ms  計算しやすく1/10に
  int kp_t = 5000;	// 比例 
  int ki_t = 200;	// 積分 
  int kd_t = 500;	// 微分 


// パルス出力用割込み関数 us単位で呼び出される IRAM_ATTR属性でRAM配置
void IRAM_ATTR timerPulse1(){

     digitalWrite(22, outputl);      // 22番ピン(L)に出力する(HIGH>ON LOW>OFF)
     outputl = !outputl;    // 出力信号反転
}
void IRAM_ATTR timerPulse2(){

     digitalWrite(23, outputr);      // 23番ピン(R)に出力する(HIGH>ON LOW>OFF)
     outputr = !outputr;    // 出力信号反転
}

void setup() {
     pinMode(5,OUTPUT_OPEN_DRAIN) ;  //ピンをオープンドレイン出力に設定
     pinMode(16,OUTPUT_OPEN_DRAIN) ;  //ピンをオープンドレイン出力に設定
     pinMode(17,OUTPUT_OPEN_DRAIN) ;  //ピンをオープンドレイン出力に設定
     pinMode(18,OUTPUT_OPEN_DRAIN) ;  //ピンをオープンドレイン出力に設定

//     pinMode(13,OUTPUT_OPEN_DRAIN) ;  //ピンをデジタル出力に設定
     pinMode(13,OUTPUT) ;  //ピンをデジタル出力に設定
     pinMode(14,OUTPUT) ;  //ピンをデジタル出力に設定
     pinMode(25,OUTPUT) ;  //ピンをデジタル出力に設定
     pinMode(26,OUTPUT) ;  //ピンをデジタル出力に設定
     pinMode(27,OUTPUT) ;  //ピンをデジタル出力に設定

     pinMode(19,OUTPUT) ;  //ピンをデジタル出力に設定 センサLED 7/17
     pinMode(21,OUTPUT) ;  //ピンをデジタル出力に設定
     pinMode(22,OUTPUT) ;  //ピンをデジタル出力に設定
     pinMode(23,OUTPUT) ;  //ピンをデジタル出力に設定
//     pinMode(33,OUTPUT) ;  //ピンをデジタル出力に設定

     pinMode(33,INPUT_PULLUP) ;  //ピンをプルアップ,デジタル入力に設定 7/17

//     pinMode(A0,INPUT) ;    //アナログピンをデジタル入力に設定(36)A0)
//     pinMode(A3,INPUT) ;    //アナログピンをデジタル入力に設定(39)A3)
//     pinMode(A4,INPUT) ;    //アナログピンをデジタル入力に設定(32)A4)
//     pinMode(A6,INPUT) ;    //アナログピンをデジタル入力に設定(34)A6)
//     pinMode(A7,INPUT) ;    //アナログピンをデジタル入力に設定(35)A7)

     pinMode(36,ANALOG) ;    //アナログピンをデジタル入力に設定(36)A0)
     pinMode(39,ANALOG) ;    //アナログピンをデジタル入力に設定(39)A3)
     pinMode(32,ANALOG) ;    //アナログピンをデジタル入力に設定(32)A4)
     pinMode(34,ANALOG) ;    //アナログピンをデジタル入力に設定(34)A6)
     pinMode(35,ANALOG) ;    //アナログピンをデジタル入力に設定(35)A7)

    digitalWrite(5, HIGH);      // 5番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(16, HIGH);      // 16番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(17, HIGH);      // 17番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(18, HIGH);      // 18番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(13, LOW);      // 出力する(HIGH>ON LOW>OFF)
//    digitalWrite(14, LOW);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(25, LOW);      // に出力する(HIGH>ON LOW>OFF)
    digitalWrite(26, LOW);      // 出力する(HIGH>ON LOW>OFF)
//    digitalWrite(27, LOW);      // 出力する(HIGH>ON LOW>OFF)

    digitalWrite(19, HIGH);      // センサLED点灯(HIGH>ON LOW>OFF) 7/17
//    digitalWrite(19, LOW);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(21, LOW);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(22, HIGH);      // に出力する(HIGH>ON LOW>OFF)
    digitalWrite(23, HIGH);      // 出力する(HIGH>ON LOW>OFF)
//    digitalWrite(33, LOW);      // 出力する(HIGH>ON LOW>OFF)

//     analogReference(DEFAULT);  // 入力基準電圧を電源電圧(5V)に設定
//     analogReadResolution();

  // PCとの通信を開始
  
  Serial.begin(115200);
  Serial.print("sensar");
  Serial.println();


  // タイマ割込み設定
  timer1 = timerBegin(0,800,true);	// ﾀｲﾏｰ0使用、80MHz/800=10usで設定、増加
  timerAttachInterrupt(timer1, &timerPulse1, true);	// 割込みタイプエッジ(true)
  timerAlarmWrite(timer1,SPD, true);			// 10x50=500us毎に起動、周期起動

  timer2 = timerBegin(1,800,true);	// ﾀｲﾏｰ1使用、80MHz/800=10usで設定、増加
  timerAttachInterrupt(timer2, &timerPulse2, true);	// 割込みタイプエッジ(true)
  timerAlarmWrite(timer2,SPD, true);			// 10x50=500us毎に起動、周期起動

  timerAlarmEnable(timer1);		// タイマー開始
  timerAlarmEnable(timer2);		// タイマー開始

}

void loop() {
   sd1 = analogRead(36);
   sd2 = analogRead(39);
   sd3 = analogRead(34);
   sd4 = analogRead(35);
   sd5 = analogRead(32);	// 前方センサー左外 GPI32 7/17

   Serial.print("sensar :");
   Serial.printf("%04d ,%04d ,%04d ,%04d ,%04d ",sd1,sd2,sd3,sd4,sd5);
//   Serial.printf("%04d ,%04d ,%04d ,%04d ",sd1,sd2,sd3,sd4);
   Serial.println();
//   Serial.print("timer : ");
//   Serial.printf("%06d ,%06d ,%06d ",il,ir,n);


  if(digitalRead(33) == LOW ){	// ボタンがおされたら処理  ON:LOW 7/17
    digitalWrite(5,LOW);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(16,LOW);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(17,LOW);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(18,LOW);      // 出力する(HIGH>ON LOW>OFF)
      sei1(0);					//直進 
//      lefght1(10);			// 左回転 
//      right1(10);			// 右回転 

    delay(100);			//ON時間を確保
  }else{
    digitalWrite(5,HIGH);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(16,HIGH);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(17,HIGH);      // 出力する(HIGH>ON LOW>OFF)
    digitalWrite(18,HIGH);      // 出力する(HIGH>ON LOW>OFF)
  } 

 timerAlarmWrite(timer1,100, true);    // 10xlsp us毎に起動、周期起動 L
 timerAlarmWrite(timer2,100, true);    // 10xrsp us毎に起動、周期起動 R

if(sd4<1000&&sd3>3000){
  timerAlarmWrite(timer1,120, true);    // 10xlsp us毎に起動、周期起動 L
  timerAlarmWrite(timer2,100, true);    // 10xrsp us毎に起動、周期起動 R
  }

if(sd4<1000&&sd3>3000){
  timerAlarmWrite(timer1,120, true);    // 10xlsp us毎に起動、周期起動 L
  timerAlarmWrite(timer2,100, true);    // 10xrsp us毎に起動、周期起動 R
  }

delay(10);

}

//-------------------------------------------------------------------------
//  右回転 
//-------------------------------------------------------------------------
void right1(int angle)			// 左右差分
{
  int lsp,rsp;
  lsp = SPD - angle;	// 左の起動間隔を短く   速くする
  rsp = SPD + angle;	// 右の起動間隔を伸ばす 遅くする

//  timerEnd(timer1);
//  timerEnd(timer2);
  digitalWrite(27, ccw);      // 回転方向 27番ピン(L)に出力する(HIGH>ON LOW>OFF)
  digitalWrite(14, cw);     // 回転方向 14番ピン(R)に出力する(HIGH>ON LOW>OFF)
  digitalWrite(13, HIGH);      // モータON 13番ピンにHIGHを出力する(HIGH>ON LOW>OFF)

  timerAlarmWrite(timer1,lsp, true);		// 10xlsp us毎に起動、周期起動 L
  timerAlarmWrite(timer2,rsp, true);		// 10xrsp us毎に起動、周期起動 R

//  timerAlarmEnable(timer1);		// タイマー開始
//  timerAlarmEnable(timer2);		// タイマー開始

  return;
}

//-------------------------------------------------------------------------
//  左回転  
//-------------------------------------------------------------------------
void lefght1(int angle)			// 左右差分
{
  int lsp,rsp;
  lsp = SPD + angle;	// 左の起動間隔を伸ばす 遅くする
  rsp = SPD - angle;	// 右の起動間隔を短く   速くする

//  timerEnd(timer1);
//  timerEnd(timer2);
  digitalWrite(27, ccw);      // 回転方向 27番ピン(L)に出力する(HIGH>ON LOW>OFF)
  digitalWrite(14, cw);     // 回転方向 14番ピン(R)に出力する(HIGH>ON LOW>OFF)
  digitalWrite(13, HIGH);      // モータON 13番ピンにHIGHを出力する(HIGH>ON LOW>OFF)

  timerAlarmWrite(timer1,lsp, true);		// 10xlsp us毎に起動、周期起動 L
  timerAlarmWrite(timer2,rsp, true);		// 10xrsp us毎に起動、周期起動 R

//  timerAlarmEnable(timer1);		// タイマー開始
//  timerAlarmEnable(timer2);		// タイマー開始

  return;
}

//-------------------------------------------------------------------------
// 直進 
//-------------------------------------------------------------------------
void sei1(int speed)			// 速度変更値
{
  int lsp,rsp;
  lsp = SPD + speed;
  rsp = SPD + speed;

//  timerEnd(timer1);
//  timerEnd(timer2);
  digitalWrite(27, ccw);      // 回転方向 27番ピン(L)に出力する(HIGH>ON LOW>OFF)
  digitalWrite(14, cw);     // 回転方向 14番ピン(R)に出力する(HIGH>ON LOW>OFF)
  digitalWrite(13, HIGH);      // モータON 13番ピンにHIGHを出力する(HIGH>ON LOW>OFF)

  timerAlarmWrite(timer1,lsp, true);		// 10xlsp us毎に起動、周期起動 L
  timerAlarmWrite(timer2,rsp, true);		// 10xrsp us毎に起動、周期起動 R

//  timerAlarmEnable(timer1);		// タイマー開始
//  timerAlarmEnable(timer2);		// タイマー開始

  return;
}

//-------------------------------------------------------------------------
//  停止モジュール
//-------------------------------------------------------------------------
void com_stop( void )
{

    digitalWrite(13, LOW);      // モータOFF 13番ピンにLOWを出力する(HIGH>ON LOW>OFF)

  delay(100);                  // 0.1秒モータを停止
}
