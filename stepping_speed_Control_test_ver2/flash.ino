/**********割り込み関数*********/
void flash(){
 /****センサーの読み取り****/
  //スイッチ
  S1=digitalRead(14);
  S2=digitalRead(15);
  S3=digitalRead(17);
  S4=digitalRead(18);
//リミットスイッチの条件文
  if(LSW0_check==0){
    if(S1==0&&S2==0){
      flag++;//次の動作の指定
      flag_check=0;//解除
    }
    LSW0_check=1;//拘束
  }
  if(LSW1_check==0){
    if(S3==0&&S4==0){
      flag++;//次の動作の指定
      flag_check=0;//解除
    }
    LSW1_check=1;//拘束
  }

  //吸引器に使用するpin
  K1=digitalRead(2);

  //赤外線センサー
  L1=analogRead(A0);
}
