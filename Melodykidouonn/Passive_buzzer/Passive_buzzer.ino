
#define buzzer 12
void setup()
{
  Serial.begin(9600);
  // tone(ブザーの刺さっているピン番号,周波数、時間（ミリ秒）)
  tone(buzzer, 1000, 1000); 
}
void loop() 
{
  tone(buzzer,987,130);//シ
  delay(130);
  tone(buzzer,1318,180);//ミ上
  delay(180);
  tone(buzzer,987,130);//シ
  delay(130);
  tone(buzzer,1479,180);//ファ#6
  delay(180);
  tone(buzzer,987,130);//シ
  delay(130);
  tone(buzzer,1661,600);//そ#6
  delay(1000);
  //delay(1000);
/*for(i=1;i<4000;i++){
tone(buzzer,i,500);
delay(10);
Serial.println(i);
*/
}
