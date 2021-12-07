int redled   =8;
int yellowled=7;
int greenled =6;
int Switch1  =5;
int Switch2  =4;
int Switch3  =3;
int resetSwitch=2;
int BUT1;
int BUT2;
int BUT3;
int RBUT;
void setup() {
  Serial.begin(9600);
pinMode(redled,OUTPUT);
pinMode(yellowled,OUTPUT);
pinMode(greenled,OUTPUT);
pinMode(Switch1,INPUT);
pinMode(Switch2,INPUT);
pinMode(Switch3,INPUT);
pinMode(resetSwitch,INPUT);


}

void loop() {
  BUT1=digitalRead(Switch1);
  BUT2=digitalRead(Switch2);
BUT3=digitalRead(Switch3);
RBUT=digitalRead(resetSwitch);
if(BUT1==HIGH){
    digitalWrite(redled,HIGH);
    }else{
      digitalWrite(redled,LOW);
      }
    Serial.println(BUT1);
    //Serial.print()
   // Serial.println(redled);
  
}
