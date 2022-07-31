例1
/* 
  この開発環境ではC++（C言語の拡張版）を利用
  しているので，一部C言語では使えない機能が
  使われています．
*/
#include "mbed.h"
#include "adrobo.h"

/* 
  LEDやGPIOの出力は，下記のようにBusOut型の
  変数を宣言することで利用できます．
  下記のようにすると，ledの1ビット目は
  LED1，2ビット目はLED2，のようにそれぞれ
  対応します．LED1～LED5とLEDは，ボード上や
  マイコン上のLEDに対応します．adrobo.h参照
 */
BusOut led(LED1,LED2,LED3,LED4,LED5,LED);
/*
  ボード上のスイッチや，GPIOの入力は，下記の
  ようにBusIn型の変数を宣言することで利用できます．
  下記のようにすると，swの1ビット目はSW1，
  2ビット目はSW2に対応します．adrobo.h参照
*/
BusIn sw(SW1, SW2);

int main() {
  /* ボード上のスイッチを利用する際には
     プルアップモードにする必要があります．
     最初に1度だけ下記を呼ぶ必要があります */
  sw.mode(PullUp);

  while(1) {
    /* スイッチは押されているとき0,
       押されてないときに1になります．
       したがって，SW1が押されているときは，
       変数swの1ビット目が立ってない（1でない）
       ことを確認する必要があります */
    if ((sw & 0x01) == 0) {
      /* SW1が押されていればLED1を点灯する */
      led = led | 0x01;
    } else {
      /* SW1が押されてなければLED1を消灯する */
      led = led & 0xfe;
    }
    wait(0.1);
  }
}
例2
#include "mbed.h"
#include "adrobo.h"

BusOut led(LED1,LED2,LED3,LED4,LED5,LED);
/*
  AnalogInクラスを使うことでA/D変換による
  センサ値の取得ができます．
  下記のようにすると，
  ad[0] が AD1 ピンの入力に
  ad[1] が AD2 ピンの入力にそれぞれ対応します．
  AD1～AD5まで使えます．adrobo.h 参照 */
AnalogIn ad[] = {AD1, AD2};

int main() {
  float value;
  while(1) {
    /* 実際に数値を読み込むときは，
       下記のようにします．
       センサからの入力値はfloat型の0.0～1.0の
       値として戻されます．
       adの添え字は，読み取りたいピンに応じて
       変更します． */
    value = ad[0].read();
    printf("%f\r\n", value);
    wait(0.1);
  }
}
例3
#include "mbed.h"
#include "adrobo.h"
/* DCモータを使う場合は Motor.h を
   読み込む必要があります． */
#include "Motor.h"

BusOut led(LED1,LED2,LED3,LED4,LED5,LED);
/* 下記のように変数motor1を宣言することで，
   CN1に接続したDCモータを制御することが
   可能になります． */
Motor motor1(MOTOR11, MOTOR12);
/* DCモータは計2個同時に制御できます．
   CN2に接続したDCモータも制御したい場合は
   以下の様にします */
/* Motor motor2(MOTOR21, MOTOR22); */

int main() {
  /* valueがモータに与える値（速度に相当）．
     deltaがvalueを時間に応じてどれだけ変化
     させるかを表した値 */
  float value = 0.0f, delta = 0.1f;

  while(1) {
    /* value に delta を足しこむ */
    value += delta;
    /* valueの絶対値（fabs(value)が
       1.0以上になったら delta の符号を
       反転させる．こうすることで，
       valueは -1 ～ 1 の間をいったり
       きたりする． */
    if (fabs(value) >= 1.0f) {
      delta *= -1.0f;
    }
    printf("%f\r\n", value);
    
    /* motor1 に数値を代入するとその値に
       応じた速度で回転する．
       -1～1の値を与える必要がある．
       絶対値が大きいほど速く回転する．
       符号は回転の方向を表す．
     */
    motor1 = value;
    wait(0.2);
  }
}

例4
#include "mbed.h"
#include "adrobo.h"

BusOut led(LED1,LED2,LED3,LED4,LED5,LED);
/* サーボモータを使うときは PwmOut クラスを
   利用します．下記のようにすると，SERVO1に
   接続したサーボモータを制御するための
   servo1という変数が宣言されます．
   SERVO1～SERVO4まで使えます．adrobo.h参照 */
PwmOut servo1(SERVO1);

int main() {
  /* サーボモータは，PWMという方式の信号を
     使って位置を指定します．
     次の命令はPWMの周期を設定するための
     もので，はじめに1度呼んでおく必要があります．
     以下では周期を20ミリ秒に設定しています． */
  servo1.period_ms(20);
  int i;
  
  while(1) {
    for(i = 400; i <= 2000; i += 100) {
      /* 実際にサーボモータに位置を指令する際には，
         パルス幅というものを設定する必要があります．
         次の命令はそれを行うためのものです．
         iの値に応じて400マイクロ秒～2000マイクロ秒に
         設定しています．
         初期位置は個体差があるので注意してください．
      */
      servo1.pulsewidth_us(i);
      wait(0.2);
    }
  }
}
