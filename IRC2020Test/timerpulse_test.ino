void timerPulse() {

  if(il>ls){
    digitalWrite(9,outputl);
    outputl = !outputl;
    il=0;
  }
  
  if(ir>rs){
    digitalWrite(12,outputr);
    outputr = !outputr;
    ir=0;
  }
    
  il++;
  ir++;  
}
