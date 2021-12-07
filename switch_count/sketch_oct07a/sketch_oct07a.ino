const int DIN_PIN = 8;

void setup(){
    pinMode( DIN_PIN, INPUT_PULLUP );
    Serial.begin( 9600 );
}
int a=0,b=0;
int check=0;
/*
void loop(){
    if(digitalRead( DIN_PIN )==0){
      a++;
      while(digitalRead( DIN_PIN )==0){}        
      }
    Serial.println(a);
    delay(100);
}*/
void loop(){
  if(check==0){
    if(digitalRead( DIN_PIN )==0){
      a++;
      check=1;
        }
    }
Serial.println(a);
  if(digitalRead(DIN_PIN)==1){
  check=0;
   }  
}
