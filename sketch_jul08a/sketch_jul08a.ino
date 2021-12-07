void setup() {
  // put your setup code here, to run once:


pinMode(13,OUTPUT);
pinMode(A0,INPUT);
Serial.begin(9600);
}
void loop() {
int a;
  a=analogRead(A0);
  Serial.println(a);
  if(a>0){
    digitalWrite(13,HIGH);
    }else{digitalWrite(13,LOW);
    }

}
