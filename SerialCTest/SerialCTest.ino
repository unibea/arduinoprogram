void setup() {
  Serial.begin( 9600 );     // シリアル通信を初期化する。通信速度は9600bps

  Serial.println( "Hello Arduino!" );   // 最初に1回だけメッセージを表示する
}

void loop() {
  char key;     // 受信データを格納するchar型の変数

  // 受信データがあった時だけ、処理を行う
  if ( Serial.available() ) {       // 受信データがあるか？
    key = Serial.read();            // 1文字だけ読み込む
    Serial.write( key );            // 1文字送信。受信データをそのまま送り返す。
  }
}
