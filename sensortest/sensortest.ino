int S0=0;
int S1=0;
int S2=0;
int S3=0;

void setup() {
  Serial.begin(9600);

}
void loop() {

  
  S0=analogRead(0);//一番右
  S1=analogRead(1);
  S2=analogRead(2);
  S3=analogRead(7);//一番左
 
 Serial.println(S0);
  /*Serial.print("\t");
  Serial.print(S1);
  Serial.print("\t");
  Serial.print(S2);
  Serial.print("\t");
  Serial.print(S3);
  Serial.println("\t");
  
*/

 

}
 
    
