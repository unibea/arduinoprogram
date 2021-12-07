int shiftIn(){
  int result=0; //検出結果用の変数を用意

  //12ビット分の繰り返し処理
  for(int i=0;i<12;i++){
    digitalWrite(CK,HIGH);//＝5番ピンをHIGHにする
    delayMicroseconds(1);//1マイクロ秒待機

    //Dout端子からの出力がHIGHなら12ビットのi桁目に１を代入
    if(digitalRead(DOUT)==HIGH){
      result+=(1<<i);//result=(result+1を左にiビットシフトさせたもの　　　　　　　　　//http://www.itsenka.com/contents/development/c/operator.html
      }

      digitalWrite(CK,LOW);//1ビット分のクロックパルス出力（LOW）
      delayMicroseconds(1);
    }
    delayMicroseconds(3);
    return result;  //結果を出力　2進数での結果を10進数の0~4095に変換している？
  }
