void tansaku(){
  while(X>0){
  turn90L();
  Timer1.detachInterrupt();
  digitalWrite(10,LOW);
  delay(1);
  outcvr=!outcvr;
  digitalWrite(CWCCW_R,outcvr);
  delay(1);
  ls=15.0;
  rs=15.0;;
  delay(1000);
  Timer1.attachInterrupt(timerPulse);
  digitalWrite(10,HIGH);
  while(sensor<600){
  sensor=analogRead(15);
  Serial.print(T);
  Serial.print("\t");
  Serial.println(sensor);
  T=T+1;
  if(T==1200){
    T=0;
    break;
    }
  }
  digitalWrite(10,LOW);
  Timer1.detachInterrupt();
  delay(1);
  outcvr=!outcvr;
  digitalWrite(11,outcvr);
  if(sensor>600){ //ボールを発見したら
    collect();
    }
  delay(1000);
  turn90L();
  Timer1.attachInterrupt(timerPulse);
  delay(1);
  digitalWrite(10,HIGH);
  while(A<1800){
    A++;
    S0=analogRead(LEFT);//0番ピン
S1=analogRead(MLEFT);//1番ピン
S2=analogRead(MRIGHT);//2番ピン
S3=analogRead(RIGHT);//3番ピン
 
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
      }
  A=0;
  digitalWrite(10,LOW);
  if(S==3){ break;}
    }
      }
