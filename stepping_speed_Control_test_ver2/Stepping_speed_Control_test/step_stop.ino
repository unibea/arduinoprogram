void step_stop(){
  Timer1.detachInterrupt();
  //モーターの電源を落とす
  digitalWrite(10,LOW);
  digitalWrite(5,LOW);
  delay(100);
  //次の動作のための拘束を解除
  //Timer1.attachInterrupt(timerPulse);
  flag_check=0;
}

void step_stop1(){
  Timer1.detachInterrupt();
  //モーターの電源を落とす
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  //次の動作のための拘束を解除
  Timer1.attachInterrupt(timerPulse);
  flag_check=0;
}

void step_stop2(){
  Timer1.detachInterrupt();
  //モーターの電源を落とす
  digitalWrite(5,LOW);
  digitalWrite(11,LOW);
  //次の動作のための拘束を解除
  Timer1.attachInterrupt(timerPulse);
  flag_check=0;
}
