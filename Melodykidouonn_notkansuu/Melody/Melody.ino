#define BEAT 300          // ���̒������w��
#define PINNO 12          // ���d�X�s�[�J��ڑ������s���ԍ�

void setup() {
     pinMode(12,OUTPUT) ; // ���d�X�s�[�J��ڑ������s�����f�W�^���o�͂Ŏw��
}

void loop() {
     melody(262,BEAT) ;   
     melody(294,BEAT) ;   
     melody(330,BEAT) ;   
     melody(349,BEAT) ;   
     melody(392,BEAT) ;   
     melody(440,BEAT) ;  
     melody(494,BEAT) ;   
     melody(523,BEAT) ;   
     delay(3000) ;
}

void melody(int frequency, int duration)
{
     long i ;
     int  tones ;

     tones = 1000000L / (2 * frequency) ;
    
     for (i = 0; i < duration * 1000L; i += tones * 2) {
          digitalWrite(PINNO, HIGH) ;
          delayMicroseconds(tones) ;
          digitalWrite(PINNO, LOW) ;
          delayMicroseconds(tones) ;
     }
}
