void timerPulse(){
  if(Ai>=A){
    //クロック周波数出すピン
    digitalWrite(12,output_L1);
    //出力逆転
    output_L1=!output_L1;
    //リセット
    Ai=0;
    Onestep_count1++;
  }

  if(Bi>=B){
    //クロック周波数出すピン
    digitalWrite(9,output_R1);
    //出力逆転
    output_R1=!output_R1;
    //リセット
    Bi=0;
    Onestep_count1++;
  }
  
  if(Onestep_count1>=1){
    switch(dir){
      case 0:
      step_count1++;
      step_count2++;
      break;
      case 1:
      step_count1--;
      step_count2--;
      break;
    }
  }

  caliv();//計算
}
