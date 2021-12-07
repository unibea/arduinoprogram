void collect(){
//前センサーのサーボを上げる
  while(Z<20){
    digitalWrite(46 ,HIGH);
    delayMicroseconds(2500);//仮
    digitalWrite(46,LOW);
    delayMicroseconds(2500);
    Z++;
    }
   //巻取りサーボを回しながらゆっくり前進してボールを回収する（ボール止めサーボを起動直後に後ろON前OFFにしておく）
     ls=20.0;
     rs=20.0;
     while(Y<100){
      digitalWrite(10,HIGH);
      while(V<20){
        digitalWrite(30,HIGH);
        delayMicroseconds(1000);
        digitalWrite(30,LOW);
        delayMicroseconds(1000);
        V++;
        Serial.println(V);
        }
        while(W<20){
          digitalWrite(21,HIGH);
          delayMicroseconds(2000);
          digitalWrite(21,LOW);
          delayMicroseconds(2000);
          W++;
          Serial.println(W);
          }
          V=0;
          W=0;
          Y++;
          if(sensor>600){
            through++;   
          }//ボール判定
     }
      digitalWrite(10,LOW);
        //S=S+1;//S=3になったら一度ボールを入れに行く
          if(through>=0){
            ls=4.0;
            rs=4.0;
            outcvl = LOW;
            outcvr = HIGH;
            digitalWrite(CWCCW_R ,outcvr);
            digitalWrite(CWCCW_L ,outcvl);
            delay(1);
            digitalWrite(10,HIGH);
            while(B<700){//(仮)
               if(S1>700&&S2<300){//(仮)
                     ls=2.0;
                     rs=4.0;
                }
    
                if(S1<300&&S2>700){ //（仮）
                     ls=4.0;
                     rs=2.0;
               }
              B++;
            }
            digitalWrite(10,LOW);
            delay(2000);
          }
}
