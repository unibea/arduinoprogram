void color_recognition(){
    //測光時間用の可変抵抗器の読み込み（アナログ入力:０番ピン）
  int val=analogRead(0);

  //Gate,CK端子をLowに設定
  digitalWrite(GATE,LOW);
  digitalWrite(CK,LOW);
  delayMicroseconds(2000);//2000マイクロ秒待機

  //感度設定（HIGH:高感度に設定）  
  digitalWrite(RANGE,HIGH);

  //測光開始（光量の積算を開始） 
  digitalWrite(GATE,HIGH);

  //測光時間（valを代入し可変的に設定）
  delay(val+1);
  
  //測光終了（光量の積算を終了） 
  digitalWrite(GATE,LOW);
  delayMicroseconds(4);//4マイクロ秒待機
  
  //数値の整理をする
  red = shiftIn();    //赤の処理
  green = shiftIn();    //緑の処理
  blue = shiftIn();   //青の処理
/////////////////////////////////////////////////////////////////////////////
  //取得できた値を原点から伸びる位置ベクトルとして表現するために配列に格納する
  //[0] = red, [1] = green, [2] = blue
  get_vec[0] = red; 
  get_vec[1] = green; 
  get_vec[2] = blue;
  //配列の値を渡して判別色を判定する関数
  
//  color_judgement();
//////////////////////////////////////////////////////////////////////////
  //結果をprintする
  Serial.println("//-- Get value --//");
  Serial.print("Red   = "); Serial.println(get_vec[0]);
  Serial.print("Green = "); Serial.println(get_vec[1]);
  Serial.print("Blue  = "); Serial.println(get_vec[2]);
  Serial.print("R_to_getColor"); Serial.println(R_to_getColor);
  Serial.print("B_to_getColor"); Serial.println(B_to_getColor);
  Serial.print("Y_to_getColor"); Serial.println(Y_to_getColor);


  analogWrite(11, 250);
  analogWrite(12, 250);
  analogWrite(13, 250); 

  //Gate端子をHighに戻す  
  digitalWrite(GATE,HIGH);

  }
