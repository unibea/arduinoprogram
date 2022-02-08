
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
//カラーセンサー関数
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
                       +(Y_vec[2] - get_vec[2]) * (Y_vec[2] - get_vec[2]) )+1500;
  
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
    se(277,0.5);
    se(330,0.5);
    se(370,0.5);
    se(330,0.5);
    se(440,0.5);
    break;
  case 7:
    Serial.println("Blue");
    se(783,1.0);
    se(659,1.5);
    break;
  case 8:
    Serial.println("yellow");
    se(349,0.2);
    se(440,0.2);
    se(523,0.2);
    se(349,0.2);
    se(440,0.2);
    se(523,0.2);
    se(349,0.2);
    se(440,0.2);
    se(523,0.2);        
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

//ブザー関数
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
//P制御　トレース関数
void trace (float Kp){
  Timer1.attachInterrupt(timerPulse1);
  Timer3.attachInterrupt(timerPulse2);
  Timer1.initialize(HzSPL);//R
  Timer3.initialize(HzSPR);//L
 /* 
 //1ｽﾃｯﾌﾟで0.6535mm進む
  Serial.print(S0);
  Serial.print("\t");
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S3);
  Serial.print("\t");*/
  Serial.print(count);
  Serial.print("\t");
  Serial.println();


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
   HzSPL=spchL(420);
   HzSPR=spchR(420);
    }
    //何もなければ直進
      //ラインカウンタ
      if(point==0){
          if(S3<500&&Sfrag==0){
            count++;
            point=1;
            se(900,0.2);
            }else if(S3<500&&Sfrag==1){
              count--;
            point=1;
            se(900,0.2);
              }
       }
       if(S3>600){
        point=0;
        }

 }

void tracet (float Kp){
  Timer1.attachInterrupt(timerPulse1);
  Timer3.attachInterrupt(timerPulse2);
  Timer1.initialize(HzSPL);//R
  Timer3.initialize(HzSPR);//L

  S0=analogRead(LEFT);//0番ﾋﾟﾝ
  S1=analogRead(MLEFT);//1番ﾋﾟﾝ
  S2=analogRead(MRIGHT);//2番ﾋﾟﾝ
  S3=analogRead(RIGHT);//3番ﾋﾟﾝ

  diffL=targetL-S1;
  diffR=targetR-S2;

  outL=Kp*diffL+bias;
  outR=Kp*diffR+bias;

   HzSPL=spchL(350+outL);
   HzSPR=spchR(350+outR);


  if(S1>850&&S2>850){
   HzSPL=spchL(350);
   HzSPR=spchR(350);
    }

 }
//サーボ関数
  void servo3(int p){
  digitalWrite(38,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(p); 
  digitalWrite(38,LOW); //digitalWrite(1,LOW);
  delayMicroseconds(20000-p);
  }//固定ｻｰﾎﾞ一番前

  void servo2(int p){
  digitalWrite(40,HIGH); //digitalWrite(1,HIGH);
  delayMicroseconds(p); 
  digitalWrite(40,LOW); //digitalWrite(1,LOW);//サーボのピン番号確認
  delayMicroseconds(20000-p);
  }//固定ｻｰﾎﾞ真ん中

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



 
  //直進
int tyokusin(int spl,int Step)  {
  Serial.print("直進");
    Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   //delay(50);
   digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   //delay(50);
   Timer1.initialize(spchR(spl));//右
   Timer3.initialize(spchL(spl-10)); //左
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
     //delay(10); 
     
  }


  int tyokusinS()  {
  Serial.print("直進");
    Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   //delay(50);
   digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   //delay(50);
   Timer1.initialize(spchR(600));
   Timer3.initialize(spchL(600-10)); 
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
            se(900,0.2);
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
     //delay(10); 
     
  }

  //後進
int kousin(int spl,int Step)  {
  Serial.print("後進します");
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   //delay(50);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   digitalWrite(CWCCW_R,HIGH);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   //delay(50);
   Timer1.initialize(spchR(spl));
   Timer3.initialize(spchL(spl-10)); 
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
     //delay(10); 
  }
  
  //旋回L　左旋回
int senkaiL(int spl,int Step){
 Serial.print("左旋回");
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   //delay(50);
   digitalWrite(CWCCW_L,HIGH);//回転方向変更
   digitalWrite(CWCCW_R,HIGH);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   //delay(50);
   Timer1.initialize(spchR(spl));
   Timer3.initialize(spchL(spl)); 
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
     //delay(10); 
}

//旋回R　右旋回
void senkaiR(int spl,int Step){
  Serial.print("右旋回");
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   //delay(50);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   digitalWrite(CWCCW_R,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   //delay(50);
   Timer1.initialize(spchR(spl));
   Timer3.initialize(spchL(spl)); 
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
     //delay(10); 
  }

//探索
 int tansaku(){
  int counts=0,Flag=0;
  senkaiL(500,357);
   Timer1.detachInterrupt();
   Timer3.detachInterrupt();
   Serial.print('1');
   //delay(50);
   digitalWrite(CWCCW_L,LOW);//回転方向変更
   StepL=0;
   StepR=0;//ｽﾃｯﾌﾟ数ﾘｾｯﾄ
   Serial.print('2');
   //delay(500);
   //ls=10.0;
   //rs=10.0;
   Timer1.initialize(spchR(200));
   Timer3.initialize(spchL(200));    
   Timer1.attachInterrupt(timerPulse1);
   Timer3.attachInterrupt(timerPulse2);
   while(1){
    Flag++;
    dst1=5.0*analogRead(7)/1023;
    dst2=26.549*pow(dst1,-1.2091);
    if(dst2>100.00){dst2=100.00; }//距離センサのcmに変換
    Serial.print('\t');
    Serial.print(dst2);////////////////なぜかｼﾘｱﾙﾌﾟﾘﾝﾄであたいを表示するとif文が機能する?
   /* Serial.print('\t');
    Serial.print(Flag);
    Serial.print('\t');
    Serial.println(counts);*/
    if(dst2<19.0){
      counts++;
      }         
      if(counts>=2){
      DFL=StepL;
     // DFR=StepR; 
      counts=0;   
      break;
        } 
    if(Flag==3){counts=0;Flag=0;}  
    if(StepL>729){
      counts=0;
      break;
      }  
      
   }    
     Timer1.detachInterrupt();
     Timer3.detachInterrupt();
     kyori=dst2;
     digitalWrite(CWCCW_L,HIGH);
     if(dst2<19.0){ 
      //DFL=StepL;
      //DFR=StepR;
     for(int i=0;i<80;i++){
      servo3(1700);
      servo1(1000);
    }
    return 4;
  }
  senkaiL(500,357);
  tyokusin(300,100);
  return 3;
 }
 
//回収
void kaisyuu(){
     int ST=0,Flag=0;
    Timer1.initialize(spchR(100));
    Timer3.initialize(spchL(100));  
    Timer1.detachInterrupt();
    Timer3.detachInterrupt();
    Serial.print("11");
    //delay(500);
    //ls=10.0;
    //rs=10.0;
    StepL=0;
    StepR=0;
    Timer1.attachInterrupt(timerPulse1);
    Timer3.attachInterrupt(timerPulse2);    
    while(1){
       servoloopL();       
       servoloopR();
      T0=analogRead(7);
      Serial.print(StepL);
      Serial.print(" ");
      Serial.println(T0);
      
      if(300<T0&&T0<450){
        Timer1.detachInterrupt();
        Timer3.detachInterrupt();
        Flag=1;
        rd=StepL;
        }
       if(Flag==1){
          ST++;
          }
       if(ST>800){break;}   
        
      }
      digitalWrite(CWCCW_L,LOW);
      digitalWrite(CWCCW_R,HIGH);//バック

      
    Timer1.initialize(spchR(500));
    Timer3.initialize(spchL(500));
            
      StepL=0;
      //delay(500);
      Timer1.attachInterrupt(timerPulse1);
      Timer3.attachInterrupt(timerPulse2);
      while(1){
        Serial.print(StepL);
        Serial.print(" ");
        Serial.println(rd);
        if(rd-15<StepL){//これでバック////////////////////////////////////////////////////////////////////////////////////////////////////////
          break;
          }  
        }
        Timer1.detachInterrupt();
        Timer3.detachInterrupt();
        GET++;
        //delay(1500);
   }
 //ボール排出
 void OUT(){
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
            }//前サーボ開く
  }
 void OUTL(){
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
            }//前サーボ開く
  } 
  //２個目のボール検出メモ参照
 void sortb(){
    senkaiL(500,360);
    Sfrag=0;//カウント進む
    tyokusin(500,100);
    while(1){
      trace(1.0);
      if(count==3){
        phase=6;
        break;
        }
     }
  }
 void sortg(int DX){
      senkaiL(500,720);
      Sfrag=1;//カウント戻る
      tyokusin(500,100);
      while(1){
        trace(1.0);
        if(count==DX){
          senkaiL(500,372);
          phase=6;
          break;
          }
        }
  }
 void sortRY(){
    senkaiL(500,360);
    Sfrag=0;//カウント進む
    tyokusin(500,100);
    while(1){
        trace(1.0);
        if(count==2){
          senkaiR(500,372);
          phase=6;
          break;
          }
      }
  }

 void sortYR(){
    senkaiR(500,360);
    Sfrag=1;//カウント戻る
    tyokusin(500,100);
    while(1){
        trace(1.0);
        if(count==1){
          senkaiL(500,372);
          phase=6;
          break;
          }
      }
  }  
       
