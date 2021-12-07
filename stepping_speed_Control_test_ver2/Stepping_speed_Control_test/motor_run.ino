void motor_runA(short int axis,short int dir,short int speed){
  
  //スピードを計算
  int motor_speed(speed);
  if(dir==0){
    //回転方向の出力
    digitalWrite(8,LOW);
    digitalWrite(11,HIGH);
    }
    else if(dir==1){
    //回転方向の出力
    digitalWrite(8,HIGH);
    digitalWrite(11,LOW);
    }

    if(axis==0){
      //モーター電源ON
      digitalWrite(10,HIGH);
      }
      else if(axis==1){
      //モーター電源ON
      digitalWrite(10,LOW);
    }
   
    
}

void motor_runB(short int axis,short int dir,short int speed){
  
  //スピードを計算
  int motor_speed(speed);
  if(dir==0){
    //回転方向の出力
    digitalWrite(6,HIGH);
    digitalWrite(3,LOW);
    }
    else if(dir==1){
    //回転方向の出力
    digitalWrite(6,LOW);
    digitalWrite(3,HIGH);
    }

    if(axis==0){
      //モーター電源ON
      digitalWrite(5,HIGH);
      }
      else if(axis==1){
      //モーター電源ON
      digitalWrite(5,LOW);
    }
   
    
}
