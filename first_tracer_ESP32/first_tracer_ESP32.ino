#define  SPD 100 //スピードを決めているものでこの数値を変えることにより全体の割り込みのタイミングを変えることができる
boolean outputl = LOW;//左パルス変数　実際にこの変数をHIGHとLOW切り替えることによりパルスを作る
boolean outputr = LOW;//右パルス変数　実際にこの変数をHIGHとLOW切り替えることによりパルスを作る

//boolean kaisusitei = LOW;  //パルス数指定モードHIGH＝ON、LOW＝OFF

boolean cwccw_L = LOW;//回転方向用変数CWCCWRに入力（モーターの回転方向決める変数LOW：前回転　HIGH：後ろ回転）
boolean cwccw_R = HIGH;//回転方向用変数CWCCWLに入力（モーターの回転方向決める変数HIGH：後ろ回転　LOW：前回転）

hw_timer_t *timer0 = NULL;//タイマー設定に使用(タイマーの個数だけ必要)

hw_timer_t *timer1 = NULL;//タイマー設定に使用(タイマーの個数だけ必要)

int S1=0,S2=0,S3=0,S4=0,S5=0;//センサー値格納する変数

int S=0 , T=0;//ラインを読むための変数下部参照

void IRAM_ATTR timerPulse0(){//タイマーで繰り返す関数
      digitalWrite(22, outputl);//outputlを２２番ピンに入力
      outputl = !outputl;//出力信号を反転
      }//End_of_IRAM_ATTR timerPulse0()
void IRAM_ATTR timerPulse1(){
      digitalWrite(23, outputr);//outputrを２３番ピンに入力
      outputr = !outputr;//出力信号を反転              
      }//End_of_IRAM_ATTR timerPulse1()
      
void setup() {
     Serial.begin(115200); //シリアル通信設定
      
     pinMode(5,OUTPUT_OPEN_DRAIN);//ピンの動作設定（５番ピンをオープンドレインに設定）
     pinMode(16,OUTPUT_OPEN_DRAIN);
     pinMode(17,OUTPUT_OPEN_DRAIN);
     pinMode(18,OUTPUT_OPEN_DRAIN);

     pinMode(13,OUTPUT);//モーターON　OFF
     pinMode(14,OUTPUT);//R回転方向
     pinMode(27,OUTPUT);//L回転方向
     pinMode(23,OUTPUT);//R回転ぱるす
     pinMode(22,OUTPUT);//L回転パルス

     pinMode(21,OUTPUT);//アラーム

     pinMode(33,INPUT_PULLUP);//プルアップデジタル入力//ボタン関係ピンの動作設定ここまで

     digitalWrite(5,HIGH);//5番ピンになにを入力するかを設定する（５番ピンをHIGHに設定する）
     digitalWrite(16,HIGH);
     digitalWrite(17,HIGH);
     digitalWrite(18,HIGH);//ここまでLED出力

     digitalWrite(13,LOW);
     
     digitalWrite(14,LOW);
     digitalWrite(27,LOW);
     digitalWrite(23,LOW);
     digitalWrite(22,LOW);

     //割り込み設定timer0タイマーの動作設定
     timer0 = timerBegin(0,800,true);//timer0に設定する= (0番のタイマーを使用,　800mhz(10us)で設定,増加)
     timerAttachInterrupt(timer0, &timerPulse0, true);//(timer0について設定する,実際に割り込み処理を行う関数を指定する。今回であればtimerPulse0,増加)
     timerAlarmWrite(timer0,SPD,true);//(timer0に設定するということ,割り込みのタイミングを指定する。今回はSPDなので100,増加)
     
     //割り込み設定timer1
     timer1 = timerBegin(1,800,true);//タイマー０を使用　10usで設定？　増加？
     timerAttachInterrupt(timer1, &timerPulse1, true);//割り込みタイプエッジ？true
     timerAlarmWrite(timer1,SPD,true);//1000usごとに割り込み
     

      digitalWrite(21,HIGH);//電源確認のブザーHIGHで鳴らす
      delay(1000);          //1000ms間待機
      digitalWrite(21,LOW);//LOWで止める
     
     timerAlarmEnable(timer0);//タイマー0開始
     timerAlarmEnable(timer1);//タイマー1開始 
 
}//setup() END...

void loop() {
  
     S1=analogRead(36);//ゴールセンサー　S1にセンサーの値を格納する analogReadはアナログピンの値を読み取る
     S2=analogRead(39);//右外
     S3=analogRead(34);//右
     S4=analogRead(35);//左
     S5=analogRead(32);//左外   
     Serial.printf("%d\t,%d\t,%d\t,%d\t,%d,\t%d.\t%d",S1,S2,S3,S4,S5,outputl,outputr);//センサーの値表示
     Serial.println();//改行
                          
     digitalWrite(27, cwccw_L);//回転方向27番ピン（L）cwccw_Lの値（HIGHかLOW）を出力
     digitalWrite(14, cwccw_R);//回転方向14番ピン（R）cwccw_Rの値（HIGHかLOW）を出力
     
     digitalWrite(13,HIGH);  //回転スタートモーターON

      if(S3<1700&&S4<1700&&S2<1700&&S1<1700){//もしこれらの変数がこの条件の場合
       timerAlarmWrite(timer0,SPD,true);//このような速度に変更する    
       timerAlarmWrite(timer1,SPD,true);
       digitalWrite(18,HIGH);digitalWrite(17,HIGH);//ここは搭載されているLEDをどうするかを書いている。今回はすべて白線を踏んでない場合 
     digitalWrite(16,HIGH);digitalWrite(5,HIGH);//すべてのLEDを消灯するように設定している
      }
     if(S3>3000&&S4<1700){
       timerAlarmWrite(timer0,SPD,true);    
       timerAlarmWrite(timer1,SPD+50,true); //ここではSPD+50つまりSPDを150に設定している
       digitalWrite(17,LOW);digitalWrite(18,HIGH);digitalWrite(5,HIGH);digitalWrite(16,HIGH);//ここでは17番ピンに接続されているLEDのみ点灯
      }
     if(S4>3000&&S3<1700){
       timerAlarmWrite(timer0,SPD+50,true);//1000usごとに割り込み左のモータ +する
       timerAlarmWrite(timer1,SPD,true);//1000usごとに割り込み右のモータ    -する
       digitalWrite(16,LOW);digitalWrite(18,HIGH);digitalWrite(5,HIGH);digitalWrite(17,HIGH);
       delay(10);//if文認識タイミングをずらすため
      }
     if(S2>3000&&S3<1700){
       timerAlarmWrite(timer0,SPD,true);//1000usごとに割り込み左のモータ    +する
       timerAlarmWrite(timer1,SPD+100,true);//1000usごとに割り込み右のモータ -する
       digitalWrite(18,LOW);digitalWrite(5,HIGH);digitalWrite(17,HIGH);digitalWrite(16,HIGH);
      }
     if(S5>3000&&S4<1700){
       timerAlarmWrite(timer0,SPD+100,true);//1000usごとに割り込み左のモータ +する
       timerAlarmWrite(timer1,SPD,true);//1000usごとに割り込み右のモータ    -する
       digitalWrite(5,LOW);digitalWrite(17,HIGH);digitalWrite(18,HIGH);digitalWrite(16,HIGH);
       delay(10);//if文認識タイミングをずらすため
      }
      //ラインカウント
      if(S==0){     
        if(S1>3000){//S1は一番端のセンサーでそこに白線が通るとカウントをする
          T++;
          S=1;
            }
      }
      if(S1<1700){S=0;}  

      if(T==5){digitalWrite(13,LOW); delay(5000);
      }//5回線を読めば5秒間停止する

      
}//loop() END...
