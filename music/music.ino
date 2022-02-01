#include <SevSeg.h>
SevSeg digit; 
/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
 int i=0,count=0;
 const int base_time=16*150;
#define buzzera 5
#define buzzerb 6
#define loudspeaker_pin 8
void setup(){
  pinMode( 13, INPUT_PULLUP );
  Serial.begin(9600);

 byte numDigits = 1;
    byte digitPins[] = {0};
    byte segmentPins[] = {1,2,3,4,5,6,7,9};
 
    bool resistorsOnSegments = true;
 
    byte hardwareConfig = COMMON_ANODE; 
    digit.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    digit.setBrightness(90);

  
  // tone(ブザーの刺さっているピン番号,周波数、時間（ミリ秒）)
  tone(buzzerb, 1000, 1000);
  
  
}
void loop(){ 
  for (int i = 0;i < 10;i++){
        digit.setNumber(i);
        digit.refreshDisplay();
        delay(1000);
    }
    /*
  i=digitalRead(13);
  Serial.println(count);
  
  if(i==0){
    count++;
    while(1){
      i=digitalRead(13);
      delay(1);
      if(i==1){break;}
      }
    }*/
lemon();
  
  /*tone(buzzerb,277,500);//ド#
  delay(1000);
  tone(buzzerb,311,500);//レ#
  delay(1000);
  tone(buzzerb,277,500);//ド#
  delay(1000);
  tone(buzzerb,247,500);//シ
  delay(1000);
  tone(buzzerb,415,500);//ソ#
  delay(1000);
  tone(buzzerb,247,500);//シ
  delay(1000);
  tone(buzzerb,311,500);//レ#
  delay(1000);
  tone(buzzerb,370,500);//ファ#
  delay(1000);
  tone(buzzerb,277,500);//ド#
  delay(1000);
  tone(buzzerb,247,500);//シ
  delay(1000);
  */
}
