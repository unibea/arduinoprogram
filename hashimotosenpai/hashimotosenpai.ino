#define RANGE 9   // 8番ピンをRange端子に設定
#define GATE  11   // 9番ピンをGate端子に設定
#define CK   10    //10番ピンをCK端子に設定
#define DOUT 8    //11番ピンをDout端子に設定

int red, green, blue;   //RGB三色の変数を用意
//////////////////////////////////////////////////////////////
//各基準色を原点から伸びる位置ベクトルで表現する, 0~4095の16色
double R_vec[3] = {4095.0, 0.0, 0.0}; //red
double B_vec[3] = {0.0, 0.0, 4095.0}; //blue
double Y_vec[3] = {4095.0, 4095.0, 0.0}; //yellow

//取得できた色情報を原点から伸びる位置ベクトルで表現する
int get_vec[3] = {};  //初期値0, 0~4095の実数

//基準色ベクトルと取得色ベクトルとの距離を示す変数
double R_to_getColor;
double B_to_getColor;
double Y_to_getColor;
///////////////////////////////////////////////////////////////
void setup(){
  //Range,Gate,CK端子をデジタル出力に設定
  pinMode(RANGE,OUTPUT);
  pinMode(GATE,OUTPUT);
  pinMode(CK,OUTPUT);
  pinMode(13,OUTPUT);

  //Dout端子をデジタル入力に設定
  pinMode(DOUT,INPUT);

  //シリアル通信設定
  Serial.begin(9600);
}

void loop(){ //測光時間用の可変抵抗器の読み込み（アナログ入力:０番ピン）
  digitalWrite(13,HIGH);
  int val=analogRead(4);//おそらく測定できなかったのはアナログ４番ピンに繋がってなかったため。//12番と13番のピンの接触？

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
  
  color_judgement();
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
 // analogWrite(12, 250);
  //analogWrite(13, 250); //????????????????????よくわからない

  //Gate端子をHighに戻す  
  digitalWrite(GATE,HIGH);

  if(Serial.available()>0){
    Serial.print(red,DEC);
    Serial.print(",");
    Serial.print(green,DEC);
    Serial.print(",");
    Serial.println(blue,DEC);
    Serial.read();
  }

}
 
//12ビット分のパルス送信と読み込み処理
int shiftIn(){
  int result=0;   //検出結果用の変数を用意（0：初期化）

  //12ビット分の繰り返し処理
  for(int i=0;i<12;i++){
    digitalWrite(CK,HIGH);//1ビット分のクロックパルス出力（HIGH）
    delayMicroseconds(1);//1マイクロ秒待機

    //Dout端子からの出力がHighの場合
    if(digitalRead(DOUT)==HIGH){
      result+=(1<<i);   //12ビットのi桁目に1を代入（i桁分だけ左にシフト）
    }

    digitalWrite(CK,LOW);   //1ビット分のクロックパルス出力（LOW）
    delayMicroseconds(1);   //1マイクロ秒待機
  }

  delayMicroseconds(3);   //3マイクロ秒待機
  return result;    //結果を出力  //2進数での結果を10進数の0~4095に変換している？//出力は１２ビットのデジタル（0,1）出力されてる？でもresultは0〜4095になってる？
}
//////////////////////////////////////////////////////////////////////////////////
void color_judgement(void){
  int judge_color = 0;    //red -> 1, blue -> 2 yellow -> 3
  
  /*
   取得できた色のベクトルと基準となる各色ベクトルとの距離を調べる
   */
  //取得できた色のベクトルと赤ベクトルとの距離を調べる
  R_to_getColor = sqrt( (R_vec[0] - get_vec[0]) * (R_vec[0] - get_vec[0])
                       +(R_vec[1] - get_vec[1]) * (R_vec[1] - get_vec[1])
                       +(R_vec[2] - get_vec[2]) * (R_vec[2] - get_vec[2]) );

  //取得できた色のベクトルと青ベクトルとの距離を調べる
  B_to_getColor = sqrt( (B_vec[0] - get_vec[0]) * (B_vec[0] - get_vec[0])
                       +(B_vec[1] - get_vec[1]) * (B_vec[1] - get_vec[1])
                       +(B_vec[2] - get_vec[2]) * (B_vec[2] - get_vec[2]) );

  //取得できた色のベクトルと黄ベクトルとの距離を調べる
  Y_to_getColor = sqrt( (Y_vec[0] - get_vec[0]) * (Y_vec[0] - get_vec[0])
                       +(Y_vec[1] - get_vec[1]) * (Y_vec[1] - get_vec[1])
                       +(Y_vec[2] - get_vec[2]) * (Y_vec[2] - get_vec[2]) )+1500;
  
  /*
    ベクトルの距離を比較し，一番大きいベクトルを示す数値を変数に格納する
   */
  /*
  if(R_to_getColor < B_to_getColor){
    judge_color = 1;
  }
  else{
    judge_color = 2;
  }
  */
if(Y_to_getColor < B_to_getColor&&Y_to_getColor<R_to_getColor){
    judge_color = 3;
  }else{
    if(R_to_getColor<B_to_getColor){
      judge_color = 1;
      }else{
        judge_color = 2;
        }
    
  }
  

  //print the result....
  Serial.print("Ball color is ");
  switch(judge_color) {
  case 1:
    Serial.println("Red");
    break;
  case 2:
    Serial.println("Blue");
    break;
  case 3:
    Serial.println("yellow");
    break;
  }
  
  return 0;
}
