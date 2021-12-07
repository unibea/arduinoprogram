#include <TimerThree.h>
#include <TimerOne.h>

//ブザー関数 se(周波数,長さ);
void se(double noteNum, double len){
  int ms;
  double f,temp;
  f = noteNum;
   temp = ( 1 / f * pow(10,6) / 2);
  ms = (int)temp;
  
  if(noteNum == 145){
 for(long i = 0; i < 500000 * len; i += ms*2){
  digitalWrite(12,LOW);
        delayMicroseconds(ms);
        delayMicroseconds(ms);
    }
  }else{
    for(long i = 0; i < 500000 * len; i += ms*2){
        digitalWrite(12,HIGH);
        delayMicroseconds(ms);
        digitalWrite(12,LOW);
        delayMicroseconds(ms);
    }
  }
}

/*
  if(il>ls){//ここを何秒で実行する という風にへんこうする?割り込みは500usなのでそれを基準に設定
    digitalWrite(CLOCK_L,outputl);
    outputl = !outputl;
    il=0;
    Pulse1++;
  }
 
  if(ir>rs){
    digitalWrite(CLOCK_R,outputr);
    outputr = !outputr;
    ir=0;
    Pulse2++;
  }
 il++;
 ir++;
  if(Pulse1==2){
        StepL++;
        Pulse1=0;
      }//ｽﾃｯﾌﾟ数の算出と距離の測定
  if(Pulse2==2){
        StepR++;
        Pulse2=0;
      }
       */
