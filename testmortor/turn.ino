/*
void turnR(){//180度回転右
  //short Step=0;
  //short turnStep=500;
 for(count=0;count<1;count++){
   Timer1.detachInterrupt(); //割り込み禁止
    digitalWrite(10,LOW);  //モータOFF 10番ピン(LED)にLOWを出力する(HIGH>ON LOW>OFF)
    outcvl =! outcvl;//出力信号反転
    delay(2000);
    digitalWrite(8, outcvl); //回転方向　8番(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(11, outcvr); //回転方向　11番(LED)に出力する(HIGH>ON LOW>OFF)
    Timer1.attachInterrupt(timerPulse); //再スタート 
    outcvl =! outcvl;//出力信号反転
    digitalWrite(10,HIGH);
    delay(800);
    digitalWrite(10,LOW);
    delay(1000);
    }
 }
      Timer1.detachInterrupt(); //割り込み禁止
    digitalWrite(10,LOW);  //モータOFF 10番ピン(LED)にLOWを出力する(HIGH>ON LOW>OFF)
    outcvl =!outcvl;
    delay(2000);
     Timer1.attachInterrupt(timerPulse); //再スタート
     digitalWrite(10,HIGH);
    
    

void turnL(){//180度回転左
  for(count=0;count<1;count++){
   Timer1.detachInterrupt(); //割り込み禁止
    digitalWrite(10,LOW);  //モータOFF 10番ピン(LED)にLOWを出力する(HIGH>ON LOW>OFF)
    outcvr =! outcvr;//出力信号反転
    delay(2000);
    digitalWrite(8, outcvl); //回転方向　8番(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(11, outcvr); //回転方向　11番(LED)に出力する(HIGH>ON LOW>OFF)
    Timer1.attachInterrupt(timerPulse); //再スタート 
    outcvr =! outcvr;//出力信号反転
    digitalWrite(10,HIGH);
    delay(800);
    digitalWrite(10,LOW);
    delay(1000);
    }
  
  }

  void TURN_R(){//180度回転右
  //short Step=0;
  for(count=0;count<1;count++){
   Timer1.detachInterrupt(); //割り込み禁止
    digitalWrite(10,LOW);  //モータOFF 10番ピン(LED)にLOWを出力する(HIGH>ON LOW>OFF)
    outcvl =! outcvl;//出力信号反転
    delay(2000);
    digitalWrite(8, outcvl); //回転方向　8番(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(11, outcvr); //回転方向　11番(LED)に出力する(HIGH>ON LOW>OFF)
    Timer1.attachInterrupt(timerPulse); //再スタート 
    outcvl =! outcvl;//出力信号反転
    digitalWrite(10,HIGH);
    delay(380);
    digitalWrite(10,LOW);
    delay(1000);
    }
}
void TURN_L(){//180度回転左
  for(count=0;count<1;count++){
   Timer1.detachInterrupt(); //割り込み禁止
    digitalWrite(10,LOW);  //モータOFF 10番ピン(LED)にLOWを出力する(HIGH>ON LOW>OFF)
    outcvr =! outcvr;//出力信号反転
    delay(2000);
    digitalWrite(8, outcvl); //回転方向　8番(LED)に出力する(HIGH>ON LOW>OFF)
    digitalWrite(11, outcvr); //回転方向　11番(LED)に出力する(HIGH>ON LOW>OFF)
    Timer1.attachInterrupt(timerPulse); //再スタート 
    outcvr =! outcvr;//出力信号反転
    digitalWrite(10,HIGH);
    delay(380);
    digitalWrite(10,LOW);
    delay(1000);
    }
  }
  */
