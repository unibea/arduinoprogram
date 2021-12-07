//シリアルライブラリを取り込む
import processing.serial.*;
//シリアル通信用変数portを用意
Serial port;

//読み込み値（三色）の変数を用意
int r,g,b;

void setup(){
  //画面サイズ設定
  size(400,200);
  //ポート設定
  port = new Serial(this,"/dev/cu.usbmodem11201",9600);
  //「10」（ラインフィード）までを読み込む設定にする
  port.bufferUntil(10);
  //外形線なし
  noStroke(); 
}

void draw(){
  //背景色（黒）
  background(0);
  
  fill(r,0,0);//値を塗色の赤に代入
  rect(0,0,100,100);//赤の矩形
  fill(0,g,0);//値を塗色の緑に代入
  rect(100,0,100,100);//緑の矩形
  fill(0,0,b);//値を塗色の青に代入
  rect(200,0,100,100);//青の矩形
  fill(r,g,b);//三つの値を塗色のRGBに代入
  rect(300,0,100,100);//RGB合成した矩形
}

//シリアル通信
void serialEvent(Serial p){
  //文字列の変数stringDataを用意し、
  //「10」(ラインフィード)が来るまで読み込む
  String stringData=port.readStringUntil(10);

  //文字列データが空ではないとき
  if(stringData!=null){
    //文字列データに含まれる改行記号を取り除く
    stringData=trim(stringData);

    //整数型の配列data[]を用意し、
    //コンマ記号をもとに文字列データを区切って
    //配列data[]に整数化して入れておく
    int data[]=int(split(stringData,','));

    //配列data[]内のデータが３つなら、
    if(data.length==3){
      //三つの値をそれぞれマッピングして代入
      r=int(map(data[0],0,4095,0,255));
      g=int(map(data[1],0,4095,0,255));
      b=int(map(data[2],0,4095,0,255));

      //合図用データ送信
      port.write(65);
    }
  }  
}

//マウスが押されたら通信開始
void mousePressed(){
  //開始用データ送信
  port.write(65);
}
