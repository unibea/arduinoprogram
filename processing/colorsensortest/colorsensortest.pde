//シリアルライブラリを取り込む
import processing.serial.*;
//シリアル通信用変数portを用意
Serial port;

//読み込み値（三色）の変数を用意
int r,g,b;

void setup(){
  //画面サイズ設定
  size(500,300);
  //ポート設定
  port = new Serial(this,"/dev/cu.usbmodem1401",9600);
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
  fill(3000,4095,0);//三つの値を塗色のRGBに代入
  rect(0,200,100,100);//RGB合成した矩形
}

void serialEvent(Serial p){

  String stringData=port.readStringUntil(10);

 
  if(stringData!=null){

    stringData=trim(stringData);


    int data[]=int(split(stringData,','));


    if(data.length==3){
  
      r=int(map(data[0],0,4095,0,255));
      g=int(map(data[1],0,4095,0,255));
      b=int(map(data[2],0,4095,0,255));

      
      port.write(65);
    }
  }  
}


void mousePressed(){
  //開始用データ送信
  port.write(65);
}
