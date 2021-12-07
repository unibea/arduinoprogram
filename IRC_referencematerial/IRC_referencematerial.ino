//=========================================================
//2012知能ロボコンプログラム
//=========================================================

#include <Servo.h>
//カラーセンサ端子名定義
#define RANGE 26
#define GATE  22
#define CK    24
#define DOUT  28
//フォトリフレクタ端子名定義
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
//距離センサ端子名定義
#define KYORI1 A6
#define KYORI2 A7
//発射台用モーター名定義
#define MOTOR 53
//音楽用ピン
#define SONG 49

#define HAND_R_OPEN  170                //右手opneの値を定義
#define HAND_R_CLOSE  57                //右手closeの値を定義
#define HAND_L_OPEN  10                 //左手openの値を定義
#define HAND_L_CLOSE  118               //左手closeの値を定義
#define ARM_UP  10                      //アームupの値を定義
#define ARM_DOWN  165                   //アームdownの値を定義

int dis_a, dis_b, dis1, dis2;            //距離センサー平均値格納
int red, green, blue;                    //カラーセンサ値格納
int val1, val2, val3, val4;              //フォトリフレクタ値格納用
int Delay, v_up, v_down;                 //ステッピングモーター速度調節
int reserve1, reserve2;                  //ボール反応時のステップ数格納
int div1, div2;                          //ボール反応時のステップ数固定
int goal_step;                           //ゴールへ向く時に必要なステップ数
int new_dis, last_dis;                   //ボール探索時の距離センサの和を格納
int count5_start = 0;                    //line_count=5の時の動作分け

Servo arm;                               //アーム根元サーボ名定義
Servo hand_r;                            //手先右サーボ名定義
Servo hand_l;                            //手先左サーボ名定義

//ステッピングモーター動作用
//last_phase_aは0～7まで変化し、8以上になったら0に戻る。
//last_phase_bは7～0まで変化し、8以上になったら0に戻る。
int last_phase_a = 0;
int last_phase_b = 7;
int new_phase_a;
int new_phase_b;

//ステッピングモーター1_2相励磁励磁用配列(偶数列のみ使用で2相励磁)
int coil_a[8]     = {HIGH, LOW, LOW, LOW,   LOW, LOW, HIGH, HIGH};
int coil_b[8]     = {HIGH, HIGH, HIGH, LOW,   LOW, LOW, LOW, LOW};
int coil_a_bar[8] = { LOW, LOW, HIGH, HIGH,  HIGH, LOW, LOW, LOW};
int coil_b_bar[8] = { LOW, LOW, LOW, LOW,  HIGH, HIGH, HIGH, LOW};

int START = 0;                          //スタート時の場合分け
int black = 0;                          //黒線の読み取り
int state = 0;                          //ラインとの位置
int line_count = 0;                     //ラインを数える
int adv_count = 0;                      //count5における，ライン上を前進した回数を数える
int free_ball = 0;
int ball_catch_count = 0;

int Y_blue[30];                         //現在地から青ゴールまでの角度
int Y_yellow[30];                       //現在地から黄ゴールまでの角度
int Y_red[30];                          //現在地から赤ゴールまでの角度

//--------------------------------------------------
//セットアップ
//---------------------------------------------------

void setup()
{
  //カラーセンサ各端子入出力
  pinMode(RANGE, OUTPUT);   //出力設定
  pinMode(GATE, OUTPUT);
  pinMode(CK, OUTPUT);
  pinMode(DOUT, INPUT);     //入力設定
  //発射台用DCモーター端子出力
  pinMode(MOTOR, OUTPUT);
  //音楽用ピン出力設定
  pinMode(SONG, OUTPUT);
  //サーボ接続端子設定
  hand_r.attach(48);
  hand_l.attach(46);
  arm.attach(50);
  //サーボ初期位置設定
  arm.write(ARM_DOWN);
  hand_r.write(HAND_R_OPEN - 30);
  hand_l.write(HAND_L_OPEN + 30);
  //ステッピングモーター全接続端子出力設定
  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);

  //line_count = 4;
  //Delay=3000;

  //adv_count = 14;
  //count5_start=2;

  //line_count=5以降の縦のラインをY軸として、車体からそれぞれのゴールの位置までの差分を設定する。
  //例えば、line_count=5になった時の車体から青ゴールまでのY軸方向の差分の距離はラインセンサから車軸までの値を+85,
  //5本目のラインから青ゴールまでのY方向の距離を-250とし、つまり車体と青ゴールまでのY方向の差分の距離は-250+85となる。
  //それから車体は一回につき約71mm進むので、一回進むごとにその分の距離を加えてやれば青ゴールまでのY方向の差分距離は算出できる。
  for (int count = 0; count < 30; count++)
  {
    if (count < 15) //行き 15段階に分けてライン上を移動
    {
      Y_blue[count]   = -250 - 71 * count + 85 + 2 * (14 - count); //補正あり
      Y_yellow[count] = 400 - 71 * count + 85;
      Y_red[count]    = 1050 - 71 * count + 85 - 5 * (14 - count) * 2; //補正あり
    }
    else//折り返し　謎補正あり
    {
      Y_blue[count]   = -Y_blue[29 - count] - 15;
      Y_yellow[count] = -Y_yellow[29 - count] - 15;
      Y_red[count]    = -Y_red[29 - count] - 10;
    }
  }
  //DCモーター動作チェック
  digitalWrite(MOTOR, HIGH);
  delay(500);
  digitalWrite(MOTOR, LOW);

  delay(3000);  //電源が入るまでの余裕時間
}
//////////////////////////////////////////////////////
//基本関数
//////////////////////////////////////////////////////

//-------------------------------------------------
//////////////ステッピングモーター////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------

int motor_act(int phase_ma, int phase_mb )//ステッピングモーター回転
{

  digitalWrite(23, coil_a[     phase_ma] );    //phase_maが+1になるごとに、行列cail_a[ X ]によってHIGH,LOWが切り替わる。
  digitalWrite(25, coil_b[     phase_ma] );    //以下も同じようにHIGH,LOWが切り替わる。
  digitalWrite(27, coil_a_bar[ phase_ma] );
  digitalWrite(29, coil_b_bar[ phase_ma] );

  digitalWrite(31, coil_a[     phase_mb] );
  digitalWrite(33, coil_b[     phase_mb] );
  digitalWrite(35, coil_a_bar[ phase_mb] );
  digitalWrite(37, coil_b_bar[ phase_mb] );

  return 0;
}

//-------------------------

int plus_1_2sou(int last_phase) // 1-2相励磁，1ステップ正回転
{
  int new_phase;

  new_phase = last_phase + 1;

  if ( new_phase > 7 )
  {
    new_phase = 0;
  }

  return new_phase;
}

// -------------------------

int minus_1_2sou(int last_phase) // 1－2相励磁，1ステップ負回転
{
  int new_phase;

  new_phase = last_phase - 1;

  if ( new_phase < 0 )
  {
    new_phase = 7;
  }

  return new_phase;
}

// -------------------------

int plus_2sou(int last_phase) // 2相励磁，1ステップ正回転
{
  int new_phase;

  if ( last_phase % 2 == 1 )  {      //もし奇数なら+1して偶数に直す
    new_phase = last_phase + 1;
  }
  else {
    new_phase = last_phase + 2;
  }

  if ( new_phase > 7 ) {
    new_phase = 0;
  }

  return new_phase;
}

// -------------------------

int minus_2sou(int last_phase) // 2相励磁，1ステップ負回転
{
  int new_phase;

  if ( last_phase % 2 == 1 )  {
    new_phase = last_phase - 1;
  }
  else {
    new_phase = last_phase - 2;
  }

  if ( new_phase < 0 ) {
    new_phase = 6;
  }

  return new_phase;

}
//---------------------------

int go_straight()//2相励磁直進
{
  new_phase_a = plus_2sou(last_phase_a);
  new_phase_b = minus_2sou(last_phase_b);

  motor_act(new_phase_a, new_phase_b);

  last_phase_a = new_phase_a;
  last_phase_b = new_phase_b;

  return 0;
}

//---------------------------

int go_straight_1_2()//1-2相励磁直進
{
  new_phase_a = plus_1_2sou(last_phase_a);
  new_phase_b = minus_1_2sou(last_phase_b);

  motor_act(new_phase_a, new_phase_b);

  last_phase_a = new_phase_a;
  last_phase_b = new_phase_b;

  return 0;
}

//---------------------------

int go_back()//2相励磁後退
{
  new_phase_a = minus_2sou(last_phase_a);
  new_phase_b = plus_2sou(last_phase_b);

  motor_act(new_phase_a, new_phase_b);

  last_phase_a = new_phase_a;
  last_phase_b = new_phase_b;

  return 0;
}

//-----------------------------

int go_back_1_2()//1-2相励磁後退
{
  new_phase_a = minus_1_2sou(last_phase_a);
  new_phase_b = plus_1_2sou(last_phase_b);

  motor_act(new_phase_a, new_phase_b);

  last_phase_a = new_phase_a;
  last_phase_b = new_phase_b;

  return 0;
}

//-----------------------------

int go_straight_long(int steps)    //一度に複数ステップ直進(1-2相励磁)
{
  int kcount = 0;
  for (kcount = 0; kcount < steps; kcount++) {
    go_straight_1_2();
    delayMicroseconds(Delay);
  }
  return 0;
}

//--------------------------------

int go_back_long(int steps)      //一度に複数ステップ後退(1-2相励磁)
{
  int lcount = 0;
  for (lcount = 0; lcount < steps; lcount++) {
    go_back_1_2();
    delayMicroseconds(Delay);
  }
  return 0;
}

//--------------------------------

int carve_r(int steps)//右曲がり 1-2相励磁
{
  for (int rcount = 0; rcount < steps; rcount++)
  {
    if (rcount % 5 == 0)
    {
      new_phase_a = plus_1_2sou(last_phase_a);  //5回に1回は右車輪も1ステップ回転
    }
    new_phase_b = minus_1_2sou(last_phase_b);

    motor_act(new_phase_a, new_phase_b);

    last_phase_b = new_phase_b;
    if (steps % 5 == 0)
    {
      last_phase_a = new_phase_a;  //次のステップへの移行処理
    }

    delayMicroseconds(Delay);
  }
  return 0;

}

//-----------------------------

int carve_l(int steps)//左曲がり 1-2相励磁
{
  for (int lcount = 0; lcount < steps; lcount++)
  {
    if (lcount % 5 == 0)
    {
      new_phase_b = minus_1_2sou(last_phase_b);
    }
    new_phase_a = plus_1_2sou(last_phase_a);

    motor_act(new_phase_a, new_phase_b);

    last_phase_a = new_phase_a;
    if (lcount % 5 == 0)
    {
      last_phase_b = new_phase_b;
    }

    delayMicroseconds(Delay);
  }
  return 0;
}

//----------------------------

int senkai_l(int steps)//左旋回X度
{

  int icount = 0;
  int senkai_v_up;
  int scount = 0;
  senkai_v_up = steps / 2;
  Delay = 9000;            //初期のdelayMicrosecondsの値を9000に設定
  for (icount = 0; icount < steps; icount++)
  {

    new_phase_a = plus_1_2sou(last_phase_a);
    new_phase_b = plus_1_2sou(last_phase_b);

    motor_act(new_phase_a, new_phase_b);

    last_phase_a = new_phase_a;
    last_phase_b = new_phase_b;

    delayMicroseconds(Delay);
    if (icount <= senkai_v_up) {
      Delay -= 20;   //設定したステップ数の半分の値まで旋回するまでdelayMicrosecondsの値を20ずつ減らす(加速)
    }
    else {
      Delay += 20;   //delayMicrosecondsの値を20ずつ増やす(減速)
    }
  }
  return 0;
}

//-----------------------------

int senkai_r(int steps)//右旋回 184stepで90度
{

  int jcount = 0;
  int senkai_v_up;
  senkai_v_up = steps / 2;
  Delay = 9000;
  for (jcount = 0; jcount < steps; jcount++)
  {

    new_phase_a = minus_1_2sou(last_phase_a);
    new_phase_b = minus_1_2sou(last_phase_b);

    motor_act(new_phase_a, new_phase_b);

    last_phase_a = new_phase_a;
    last_phase_b = new_phase_b;
    delayMicroseconds(Delay);
    if (jcount <= senkai_v_up) {
      Delay -= 20;
    }
    else {
      Delay += 20;
    }
  }

  return 0;
}

//----------------------------

int search_l(int steps)//ボール探索&左旋回
{
  int icount = 0;
  int senkai_v_up;
  senkai_v_up = steps / 2; //旋回するstep数の半分の値を格納
  Delay = 9000;
  for (icount = 0; icount < steps; icount++)
  {
    new_phase_a = plus_1_2sou(last_phase_a);
    new_phase_b = plus_1_2sou(last_phase_b);

    motor_act(new_phase_a, new_phase_b);

    last_phase_a = new_phase_a;
    last_phase_b = new_phase_b;

    delayMicroseconds(Delay);
    if (icount <= senkai_v_up) {
      Delay -= 50;   //旋回するstep数の半分の値になるまで加速しながら旋回
    }
    else {
      Delay += 50;   //旋回しながら減速
    }

    distance_ave();            //距離センサの値を読む

    if (adv_count != 14) {             //14回直進を行っていない場合
      if (dis_a > 350 && dis_b > 350) { //距離センサの値が両方350を超えていない場合
        new_dis = dis_a + dis_b;     //距離センサの値を合計
        if (new_dis > last_dis) {    //合計した値が前回の値よりも大きい場合
          dis1 = dis_a;          //dis1にdis_aの値を格納
          dis2 = dis_b;          //dis2にdis_bの値を格納
          reserve1 = icount;     //ボールに反応した時の旋回したステップ数を格納
          div1++;
          last_dis = new_dis;    //距離センサの合計値をlast_disに格納し、より近いボールに反応するように比較対象として残す。
        }
      }
    }
    else {                             //14回直進した場合
      if (icount > 90) {               //90ステップまでは距離センサは読まない。(壁に反応しないようにするため)
        if (dis_a > 310 && dis_b > 310) { //同じ動作を行う
          new_dis = dis_a + dis_b;
          if (new_dis > last_dis) {
            dis1 = dis_a;
            dis2 = dis_b;
            reserve1 = icount;
            div1++;
            last_dis = new_dis;
          }
        }
      }
    }
  }
  return 0;
}

//-----------------------------

int search_r(int steps)//ボール探索&右旋回
{
  //search_l()と同様の動作を行う
  int jcount = 0;
  int senkai_v_up;
  senkai_v_up = steps / 2;
  Delay = 9000;
  for (jcount = 0; jcount < steps; jcount++)
  {
    new_phase_a = minus_1_2sou(last_phase_a);
    new_phase_b = minus_1_2sou(last_phase_b);

    motor_act(new_phase_a, new_phase_b);

    last_phase_a = new_phase_a;
    last_phase_b = new_phase_b;

    delayMicroseconds(Delay);
    if (jcount <= senkai_v_up) {
      Delay -= 50;
    }
    else {
      Delay += 50;
    }

    distance_ave();
    if (adv_count != 14 ) {
      if (dis_a > 350 && dis_b > 350) {
        new_dis = dis_a + dis_b;
        if (new_dis > last_dis) {
          dis1 = dis_a;
          dis2 = dis_b;
          reserve2 = jcount;
          div2++;
          last_dis = new_dis;
        }
      }
    }
    else {
      if (jcount > 90) {
        if (dis_a > 310 && dis_b > 310) {
          new_dis = dis_a + dis_b;
          if (new_dis > last_dis) {
            dis1 = dis_a;
            dis2 = dis_b;
            reserve2 = jcount;
            div2++;
            last_dis = new_dis;
          }
        }
      }
    }
  }
  return 0;
}

//-----------------------------

int kasoku(int which, int DELAYS, int minus)    //2相励磁 加速 kasoku(前進(0)or後退(1), 加速後の速度, 加速の速度)
{
  for (v_up = 9000; v_up >= DELAYS; v_up -= minus) //1stepごとに定めた値だけdelayの間隔を小さくする
  {
    if (which == 0) {
      go_straight();
    }
    else {
      go_back();
    }
    delayMicroseconds(v_up);
  }
  Delay = v_up;
  return 0;
}

//--------------------------------

int kasoku_1_2(int which, int DELAYS, int minus)    //1-2相励磁 加速 kasoku_1_2(前進(0)or後退(1), 加速後の速度, 加速の速度)
{
  for (v_up = 9000; v_up > DELAYS; v_up -= minus) //1stepごとに定めた値だけdelayの間隔を小さくする
  {
    if (which == 0) {
      go_straight_1_2();
    }
    else {
      go_back_1_2();
    }
    delayMicroseconds(v_up);
  }
  Delay = v_up;
  return 0;
}

//------------------------------------

int gensoku(int which, int plus)    //2相励磁  減速 gensoku(前進(0)or後退(1), 加速後の速度, 加速の速度)
{
  for (v_down = Delay; v_down <= 9000; v_down += plus) //1stepごとに定めた値だけdelayの間隔を大きくする
  {
    if (which == 0) {
      go_straight();
    }
    else {
      go_back();
    }
    delayMicroseconds(v_down);
  }
  Delay = v_down;
  return 0;
}

//------------------------------------

int gensoku_1_2(int which, int plus)      //1-2相励磁  減速 gensoku_1_2(前進(0)or後退(1), 加速後の速度, 加速の速度)
{
  for (v_down = Delay; v_down <= 9000; v_down += plus) //1stepごとに定めた値だけdelayの間隔を大きくする
  {
    if (which == 0) {
      go_straight_1_2();
    }
    else {
      go_back_1_2();
    }
    delayMicroseconds(v_down);
  }
  Delay = v_down;
  return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////モーター関連ここまで
//----------------------------------------------------
//カラーセンサ
//----------------------------------------------------

void color_check()      //カラーセンサ読み取り
{
  digitalWrite(GATE, LOW);
  digitalWrite(CK, LOW);
  delayMicroseconds(2000);

  digitalWrite(RANGE, HIGH);

  digitalWrite(GATE, HIGH);
  delay(100);

  digitalWrite(GATE, LOW);
  delayMicroseconds(4);

  red = shiftIn();
  green = shiftIn();
  blue = shiftIn();

  digitalWrite(GATE, HIGH);

}

//-----------------------------

int shiftIn()
{
  int result = 0;
  for (int i = 0; i < 12; i++)
  {
    digitalWrite(CK, HIGH);

    delayMicroseconds(1);
    if (digitalRead(DOUT) == HIGH)
    {
      result += (1 << i);
    }

    digitalWrite(CK, LOW);
    delayMicroseconds(1);
  }

  delayMicroseconds(3);

  return result;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------
//ラインセンサ
//-------------------------------------------------------

void line_check()          //フォトリフレクタからの値の読み込み
{
  //stateの合計値を決め、どのフォトリフレクタが反応しているかをかを数値で表わす
  state = 0;
  val1 = analogRead(SENSOR1);
  val2 = analogRead(SENSOR2);
  val3 = analogRead(SENSOR3);
  val4 = analogRead(SENSOR4);

  if (val1 > 830) {
    state += 1;   //SENSOR1のフォトリフレクタが反応した場合+1
  }
  if (val2 > 830) {
    state += 2;   //SENSOR1のフォトリフレクタが反応した場合+2
  }
  if (val3 > 830) {
    state += 4;   //SENSOR1のフォトリフレクタが反応した場合+4
  }
  if (val4 > 850) {
    state += 8;   //SENSOR1のフォトリフレクタが反応した場合+8
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------
//距離センサ(平均値)
//-----------------------------------------------------

void distance_ave()
{
  dis_a = 0;
  dis_b = 0;
  int dis_count;
  for (dis_count = 0; dis_count < 10; dis_count++) { //距離センサの値を10回読み込み平均する。
    dis_a += analogRead(KYORI1);
    dis_b += analogRead(KYORI2);
  }
  dis_a = dis_a / 10;
  dis_b = dis_b / 10;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////動作関数///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//---------------------------------------------------------
//ライントレース
//---------------------------------------------------------

void line_counter()    //通過した黒線をカウント
{
  if (state == 15) { //フォトリフレクタがすべて黒を認識した場合
    if (black == 0) {
      line_count++;   //black=0ならば線の数を+1とする。
    }
    black = 1;
  }
  else {
    black = 0;
  }
}

//-----------------------------

void line_trace()
{
  switch (state) //stateの数をもとに行動を設定する。
  {
    case 0:
      go_straight_1_2();
      delayMicroseconds(Delay);
      break;
    case 1:
      carve_l(10);
      break;
    case 2:
      go_straight_1_2();
      delayMicroseconds(Delay);
      break;
    case 3:
      carve_l(5);
      break;
    case 4:
      go_straight_1_2();
      delayMicroseconds(Delay);
      break;
    case 6:
      go_straight_1_2();
      delayMicroseconds(Delay);
      break;
    case 8:
      carve_r(10);
      break;
    case 12:
      carve_r(5);
      break;
    default:
      go_straight_1_2();
      delayMicroseconds(Delay);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------
//ボール探索
//-----------------------------------------------------

void ball_search()  //ボール探索
{
  //必要な変数を設定
  div1 = 0;
  div2 = 0;
  dis1 = 0;
  dis2 = 0;
  reserve1 = 0;
  reserve2 = 0;
  new_dis = 0;
  last_dis = 0;

  search_r(160);  //車体から右側を右旋回でボールをさがす
  delay(500);
  senkai_l(160 - reserve2); //左旋回しボールに反応していればその位置でストップ
  if (div2 == 0) {  //ボールに反応していなければ
    search_l(160);  //今度は左側を左旋回でボールをさがす
    delay(500);
    senkai_r(160 - reserve1);
  }
  delay(200);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------
//ボール捕獲
//---------------------------------

void ball_catch()
{
  hand_r.write(HAND_R_CLOSE);  //手を閉じる
  hand_l.write(HAND_L_CLOSE);

  delay(1000);
  color_check();    //カラーセンサーで色識別
  /*
    red=500;
    blue=1500;
    green=1000;
  */
  if (red < 800 && green < 1250 && blue < 1500) //カラーセンサーが一定以上の値を示さないときボールを取ってないと判断
  {
    dis_a = 0;    //距離センサの値を初期化しキャッチを無効化
    dis_b = 0;
    arm.write(ARM_DOWN - 40);
    delay(100);
  }
  else    //ボールを取ったと判断した場合
  {
    ball_catch_count++;    //ボールの数+1
    arm.write(ARM_UP);     //アームを上げる
    delay(100);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------
//ボールを発射
//-------------------------------

void ball_shoot()
{
  digitalWrite(MOTOR, HIGH); //1秒間車輪を回転
  delay(1500);

  hand_r.write(HAND_R_CLOSE + 40); //両手を少し開いてボールを落とす
  hand_l.write(HAND_L_CLOSE - 40);

  digitalWrite(MOTOR, HIGH);     //2秒間車輪を回転(この間にボールはスロープを下りて発射される)
  delay(2000);

  digitalWrite(MOTOR, LOW);     //車輪を止める

  hand_r.write(HAND_R_OPEN);    //両手、アームを初期位置に
  hand_l.write(HAND_L_OPEN);
  arm.write(ARM_DOWN);
  delay(1000);

  if (adv_count < 15) {
    senkai_r(goal_step);   //車体の向きを初期位置に(車体の向きをライン上に合わせる)
  }
  else {
    senkai_l(goal_step);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------
//ゴールまでの旋回ステップ数を計算
//------------------------------------
int calculate(int Y, int X)    //atan2(Y,X)を用いて現在地からゴールまでの角度を計算、さらにそれを旋回分のstep数に直す。
{
  int steps = 0;
  double wheel = 90;                          //車輪直径
  double between = 168;                       //車輪刊長さ
  double degree = atan2(Y, X) * 180 / PI; //回転角度（ラジアンからdegree(°)に直す)

  steps = (PI * between * (degree + 90) / 360) / (PI * wheel * 0.9 / 360);

  return steps;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------
//ゴールへ向けて方向転換
//--------------------------------
void GOAL()
{
  int X = 1400;//1340→1440 ゴールまでのX方向の距離(これはすべてのゴールにおいて一定)

  if (blue > 1900) {
    goal_step = calculate( Y_blue[adv_count], X );   //青のゴールまでのY方向の距離
  }
  else {
    if (red > green) {
      goal_step = calculate( Y_red[adv_count], X );   //赤のゴールまでのY方向の距離
    }
    else {
      goal_step = calculate( Y_yellow[adv_count], X );   //黄のゴールまでのY方向の距離
    }
  }

  delayMicroseconds(1000);

  int senkai;
  if (adv_count < 15) {
    if (div2 != 0 ) { //div2=1つまり車体の右側でボールを感知した場合
      senkai = goal_step + reserve2;
      senkai_l(senkai);
    }
    else {         //div2=0でdiv1=1つまり車体の右側でボールを感知した場合
      senkai = goal_step - reserve1;
      if (senkai >= 0) {
        senkai_l(senkai);
      }
      else {
        senkai = senkai * (-1);
        senkai_r(senkai);
      }
    }
  }
  else {
    if (div1 != 0) {
      senkai = goal_step + reserve1;
      senkai_r(senkai);
    }
    else {
      senkai = goal_step - reserve2;
      if (senkai >= 0) {
        senkai_r(senkai);
      }
      else {
        senkai = senkai * (-1);
        senkai_l(senkai);
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------
//count=5の時のライン復帰
//------------------------------------------

void line_return()
{
  while (1) {
    line_check();
    if (state == 6 || state == 2 || state == 4 || state == 15) {
      break;
    }
    else {
      if (state < 6) {
        senkai_l(1);
      }
      else {
        senkai_r(1);
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------
//カウント5全体の動作
//---------------------------------------------

void COUNT5_WORK()
{
  if (count5_start == 0) {
    senkai_r(12);
    hand_r.write(HAND_R_OPEN);
    hand_l.write(HAND_L_OPEN);
    digitalWrite(SONG, HIGH);
    digitalWrite(SONG, LOW);
    count5_start++;
  }

  if (ball_catch_count == 5) {
    digitalWrite(SONG, HIGH);
    digitalWrite(SONG, LOW);
  }
  else if (ball_catch_count == 10) {
    digitalWrite(SONG, HIGH);
    digitalWrite(SONG, LOW);
    delayMicroseconds(100);
    digitalWrite(SONG, HIGH);
    digitalWrite(SONG, HIGH);
  }

  delay(500);

  ball_search();

  if (div1 == 0 && div2 == 0 )
  {
    if (adv_count != 14) {
      kasoku_1_2(0, 6000, 60);
      gensoku_1_2(0, 60);
    }
    adv_count++;
  }
  else
  {
    hand_r.write(HAND_R_OPEN - 65);
    hand_l.write(HAND_L_OPEN + 65);
    delay(500);

    if (dis1 > 560 && dis2 > 560)
    {
      kasoku_1_2(0, 5500, 100);
      gensoku_1_2(0, 100);
      delay(100);

      distance_ave();
      if (dis_a > 150 && dis_b > 150) {
        ball_catch();
      }

      kasoku_1_2(1, 5500, 100);
      gensoku_1_2(1, 100);

      if (dis_a > 150 && dis_b > 150) {
        GOAL();
        ball_shoot();
      }
      else {
        hand_r.write(HAND_R_OPEN);
        hand_l.write(HAND_L_OPEN);
        delay(500);
        arm.write(ARM_DOWN);

        if (div2 != 0) {
          senkai_l(reserve2);
        }
        else {
          senkai_r(reserve1);
        }
      }
    }
    else if (dis1 <= 415 && dis1 <= 415)
    {
      kasoku_1_2(0, 5800, 40);
      gensoku_1_2(0, 40);
      delay(100);

      distance_ave();
      if (dis_a > 150 && dis_b > 150) {
        ball_catch();
      }

      kasoku_1_2(1, 5800, 40);
      gensoku_1_2(1, 40);

      if (dis_a > 150 && dis_b > 150) {
        GOAL();
        ball_shoot();
      }
      else {
        hand_r.write(HAND_R_OPEN);
        hand_l.write(HAND_L_OPEN);
        delay(500);
        arm.write(ARM_DOWN);

        if (div2 != 0) {
          senkai_l(reserve2);
        }
        else {
          senkai_r(reserve1);
        }
      }
    }
    else
    {
      kasoku_1_2(0, 5700, 60);
      gensoku_1_2(0, 60);
      delay(100);

      distance_ave();
      if (dis_a > 150 && dis_b > 150) {
        ball_catch();
      }

      kasoku_1_2(1, 5700, 60);
      gensoku_1_2(1, 60);

      if (dis_a > 150 && dis_b > 150) {
        GOAL();
        ball_shoot();
      }
      else {
        hand_r.write(HAND_R_OPEN);
        hand_l.write(HAND_L_OPEN);
        delay(500);
        arm.write(ARM_DOWN);
        if (div2 != 0) {
          senkai_l(reserve2);
        }
        else {
          senkai_r(reserve1);
        }
      }
    }
  }

  line_return();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------
////////////////////////////////////////////////////////////////メインループ/////////////////////////////////////////////////////////////////
//------------------------------------------------------------

void loop()
{
  line_check();
  line_counter();

  if (line_count < 2)
  {
    if (START == 0) {
      kasoku_1_2(0, 3000, 100);
      START++;
    } 
    else {
      go_straight_1_2();
      delayMicroseconds(Delay);
    }
  }
  else if (line_count == 2)
  {
    if (free_ball == 0) {
      gensoku_1_2(0, 115);
      delay(500);
      senkai_l(170);
      delay(500);
      kasoku_1_2(0, 3000, 50);
      gensoku_1_2(0, 50);
      delay(500);
      kasoku_1_2(1, 3000, 50);
      gensoku_1_2(1, 50);
      delay(500);
      senkai_r(170);
      delay(500);
      free_ball++;
      hand_r.write(HAND_R_OPEN);
      hand_l.write(HAND_L_OPEN);
      kasoku_1_2(0, 3000, 100);
    }
    line_trace();
  }
  else if (line_count > 2 && line_count < 5)
  {
    line_trace();
  }
  else
  {
    COUNT5_WORK();
    if (adv_count == 15 && count5_start == 1) {
      senkai_r(370);
      count5_start++;
    }
    else if (adv_count == 30 && count5_start == 2) {
      senkai_r(370);
      count5_start = 0;
      adv_count = 0;
    }
  }

}
