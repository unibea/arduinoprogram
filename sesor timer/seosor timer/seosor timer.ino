void setup() {
  // put your setup code here, to run once:

//pinMode(13,OUTPUT);
pinMode(A0,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  


int a=0;
  a=analogRead(0);
 // b=analogRead(A1);
  //c=analogRead(A2);
 // d=analogRead(A3);
  
  Serial.print(a);
  Serial.println("\t");
  /*
  if(a>2){
    digitalWrite(12,HIGH);
    }else{digitalWrite(12,LOW);
    }
    
    if(b>2){
    digitalWrite(10,HIGH);
    }else{digitalWrite(10,LOW);
    }
    if(c>2){
    digitalWrite(8,HIGH);
    }else{digitalWrite(8,LOW);
    }
    
    if(d>2){
    digitalWrite(7,HIGH);
    }else{digitalWrite(7,LOW);
    }  */  }
    
