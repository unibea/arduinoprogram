void timerPulse(){
  if(Ci>=C){
    //クロック周波数出すピン
    digitalWrite(12,output_L1);
    //出力逆転
    output_L1=!output_L1;
    //リセット
    Ci=0;
    Onestep_count1++;
  }

  if(Bi>=B){
    //クロック周波数出すピン
    digitalWrite(9,output_R1);
    //出力逆転
    output_R1=!output_R1;
    //リセット
    B=0;
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

  if(Ci>=C){
    //クロック周波数出すピン
    digitalWrite(4,output_L2);
    //出力逆転
    output_L2=!output_L2;
    //リセット
    Ci=0;
    Onestep_count1++;
  }

  if(Bi>=B){
    //クロック周波数出すピン
    digitalWrite(7,output_R2);
    //出力逆転
    output_R2=!output_R2;
    //リセット
    B=0;
    Onestep_count1++;
  }
  

  if(Onestep_count2>=1){
    switch(dir){
      case 0:
      step_count3++;
      step_count4++;
      break;
      case 1:
      step_count3--;
      step_count4--;
      break;
    }
  }
}
