void step_stop(){
  Timer1.detachInterrupt();
  //モーターの電源を落とす
  digitalWrite(5,LOW);
  digitalWrite(10,LOW);
  //delay(100);
  //次の動作のための拘束を解除
  //flag_check=0;
}

void step_stop2(){
  Timer1.detachInterrupt();
  //モーターの電源を落とす
  digitalWrite(5,LOW);
  digitalWrite(10,LOW);
  delay(100);
  //次の動作のための拘束を解除
  flag_check=1;
}
