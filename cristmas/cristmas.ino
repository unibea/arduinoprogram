int S0=0;
int S1=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A0, INPUT);
pinMode(8, OUTPUT);
pinMode(A1, INPUT);
pinMode(7,OUTPUT);
}


void loop() {

  S0=analogRead(0);
  S1=analogRead(1);
  Serial.print(S0);
  Serial.print("\t");
  Serial.print(S1);
  Serial.println("\t");
 
  if(S0>420){
    digitalWrite(8,HIGH);
   
    }else{
      digitalWrite(8,LOW);
      
      }
/*  if(S1>51 0){
    digitalWrite(7,HIGH);
    }else{
      digitalWrite(7,LOW);
      }
  
  // put your main code here, to run repeatedly:
*/
}
