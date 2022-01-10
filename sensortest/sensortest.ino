int S0=0;
int S1=0;
int S2=0;
int S3=0,S9=0;
float dst1,dst2;

void setup() {
  Serial.begin(9600);

}
void loop() {

  
  S0=analogRead(0);//一番右
  S1=analogRead(1);
  S2=analogRead(2);
  S3=analogRead(7);//一番左

 //0~1023
 dst1=5.0*analogRead(7)/1023;
 dst2=26.549*pow(dst1,-1.2091);
 if(dst2>100.00){dst2=100.00;}
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(dst2);
  Serial.println("\t");
   

}
 
    
