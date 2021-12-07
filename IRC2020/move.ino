void turn90R(){
    Timer1.detachInterrupt();//割り込み停si
    digitalWrite(10,LOW);
    delay(1);
    
    //（HIGH＝後転、LOW＝前転）
    ls=5.0;
    rs=5.0;
    delay(1);
    
    outcvr = !outcvr;
    digitalWrite(CWCCW_R ,outcvr);
    delay(1000);
    
    Timer1.attachInterrupt(timerPulse);
    delay(1);
    
    digitalWrite(10,HIGH);
    delay(3000);
    
    digitalWrite(10,LOW);
    delay(1);
    
    outcvr = !outcvr;
    digitalWrite(CWCCW_R ,outcvr);
    delay(1000);
    
    }//90度右旋回
    
void turn90L(){
    Timer1.detachInterrupt();//割り込み停si
    digitalWrite(10,LOW);
    ls=5.0;
    rs=5.0;
    delay(1);
    
    outcvl=!outcvl;//（HIGH＝後転、LOW＝前転）
    digitalWrite(CWCCW_L ,outcvl);
    delay(1000);
    
    Timer1.attachInterrupt(timerPulse);
    delay(1);
    
    digitalWrite(10,HIGH);
    delay(2000);
    
    digitalWrite(10,LOW);
    delay(1);
    
    outcvl = !outcvl;
    digitalWrite(CWCCW_L ,outcvl);
    delay(1000);
    
    }//90度左旋回
    
void back(){
  Timer1.detachInterrupt();
  digitalWrite(10,LOW);
  ls=5.0;
  rs=5.0;
  delay(1);
  
  outcvl = !outcvl;
  outcvr = !outcvr;
  digitalWrite(CWCCW_R ,outcvr);
  digitalWrite(CWCCW_L ,outcvl);
  delay(5000);
  
  Timer1.attachInterrupt(timerPulse);
  delay(1);
  
  digitalWrite(10,HIGH);
  delay(1500);
  
  digitalWrite(10,LOW);
  delay(1);
  
  outcvl = !outcvl;
  outcvr = !outcvr;
  digitalWrite(CWCCW_R ,outcvr);
  digitalWrite(CWCCW_L ,outcvl);
  delay(1000);
  
  
  }//ボール落とすためのバック
  

 
