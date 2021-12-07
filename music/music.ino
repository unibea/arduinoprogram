/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
 int i=0;
 const int base_time=16*150;
#define buzzera 5
#define buzzerb 6
#define loudspeaker_pin 12
void setup()
{
  Serial.begin(9600);
  // tone(ブザーの刺さっているピン番号,周波数、時間（ミリ秒）)
  tone(buzzera, 1000, 1000); 
  tone(buzzerb, 1000, 1000);
}
void loop(){ 
  
tone(loudspeaker_pin, 554);
delay(base_time / 16);
tone(loudspeaker_pin, 622);
delay(base_time / 16);
tone(loudspeaker_pin, 554);
delay(base_time / 16);
tone(loudspeaker_pin, 494);
delay(base_time / 16);
tone(loudspeaker_pin, 415);
delay(base_time / 8);
tone(loudspeaker_pin, 494);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 622);
delay(base_time / 8);
tone(loudspeaker_pin, 740);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 554);
delay(base_time / 8);
tone(loudspeaker_pin, 494);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 554);
delay(base_time / 16);
tone(loudspeaker_pin, 622);
delay(base_time / 16);
tone(loudspeaker_pin, 554);
delay(base_time / 16);
tone(loudspeaker_pin, 494);
delay(base_time / 16);
tone(loudspeaker_pin, 415);
delay(base_time / 8);
tone(loudspeaker_pin, 494);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 622);
delay(base_time / 8);
tone(loudspeaker_pin, 740);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 554);
delay(base_time / 8);
tone(loudspeaker_pin, 494);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 554);
delay(base_time / 16);
tone(loudspeaker_pin, 622);
delay(base_time / 16);
tone(loudspeaker_pin, 554);
delay(base_time / 16);
tone(loudspeaker_pin, 494);
delay(base_time / 16);
tone(loudspeaker_pin, 415);
delay(base_time / 8);
tone(loudspeaker_pin, 494);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 622);
delay(base_time / 8);
//tone(loudspeaker_pin, 740);
//delay(base_time / 4);
tone(loudspeaker_pin, 740);
delay(base_time / 8);
tone(loudspeaker_pin, 831);
delay(base_time / 8);
tone(loudspeaker_pin, 740);
delay(base_time * 3 / 2 / 8);
//tone(loudspeaker_pin, 740);
//delay(base_time / 8);
tone(loudspeaker_pin, 988);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 932);
delay(base_time / 8);
tone(loudspeaker_pin, 740);
delay(base_time * 3 / 2 / 8);
tone(loudspeaker_pin, 622);
delay(base_time / 8);
tone(loudspeaker_pin, 740);
delay(base_time / 4);
tone(loudspeaker_pin, 554);
delay(base_time * 3 / 2 / 8);
  
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
