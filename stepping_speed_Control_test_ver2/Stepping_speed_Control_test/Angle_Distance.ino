/*角度・距離*/
int pi=3.1415;
void caliv(){
  //１ステップあたりの計算
  Mph_dt=(60*pi)/200;
  //左右の観点距離
  MPh_setpR1=Mph_dt*step_count1;
  MPh_setpL1=Mph_dt*step_count2;
  //角度の算出
  MPh_d=MPh_setpR1-MPh_setpL1;
  Phi=(MPh_d*180)/(110*pi);
}

void depth_Position(int P){
  //指定距離>現在の移動距離
  if(P>=MPh_d){
    //モーター停止
    step_stop();
    //初期
    flag_check=0;
    Position_check=0;
    P=0;
  }
}

void angle(int P){
  if(P>=Phi){
    //モーター停止
    step_check=0;
    Position_check=0;
    P=0;
  }
}
