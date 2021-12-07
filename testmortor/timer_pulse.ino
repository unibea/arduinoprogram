void timerPulse(){
//パルス出力用割り込み関数 us単位で呼び出される。
  if(il > ls){
    digitalWrite(9,outputl);  //9番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    outputl = ! outputl;      //出力信号反転(0.5秒ごとにon（5V） off(0V)の切り替えを行っている
    il=0;
  }

  if(ir > rs){
    digitalWrite(12,outputr); //12番ピン(LED)に出力する(HIGH>ON LOW>OFF)
    outputr = ! outputr;       //出力信号反転
    ir=0;//
  }
  
  il++;
  ir++;;//+1から+0.1にかえることでより細かく調整できる？
  n++;
    
}
