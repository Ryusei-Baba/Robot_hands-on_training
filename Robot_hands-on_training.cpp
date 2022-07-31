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
Motor motor2(MOTOR21, MOTOR22);
/*
  AnalogInクラスを使うことでA/D変換による
  センサ値の取得ができます．
  下記のようにすると，
  ad[0] が AD1 ピンの入力に
  ad[1] が AD2 ピンの入力にそれぞれ対応します．
  AD1～AD5まで使えます．adrobo.h 参照 */
AnalogIn ad[] = {AD1, AD2};
/* サーボモータを使うときは PwmOut クラスを
   利用します．下記のようにすると，SERVO1に
   接続したサーボモータを制御するための
   servo1という変数が宣言されます．
   SERVO1～SERVO4まで使えます．adrobo.h参照 */
PwmOut servo1(SERVO1);


int main() {
 
	float value;
	 /* サーボモータは，PWMという方式の信号を
     使って位置を指定します．
     次の命令はPWMの周期を設定するための
     もので，はじめに1度呼んでおく必要があります．
     以下では周期を20ミリ秒に設定しています． */
  servo1.period_ms(20);
  int i;

  while(1) {
		motor1 = -0.3f; /*速度を上げるなら絶対値を大きく*/
		motor2 = 0.3f;  /*速度を下げるなら絶対値小さく*/
		
		/* 実際に数値を読み込むときは，
       下記のようにします．
       センサからの入力値はfloat型の0.0～1.0の
       値として戻されます．
       adの添え字は，読み取りたいピンに応じて
       変更します． */
    value = ad[0].read();
    printf("%f\r\n", value);
		
	if (value >= 0.9){ /*サーボモータ（腕を上げる）*/
		motor1 = 0.0f;
		motor2 = 0.0f;
		i = 800;
		servo1.pulsewidth_us(i);
		}
	
	else if (value < 0.9){ /*サーボモータ（腕を下げる）*/
		i = 1400;
    servo1.pulsewidth_us(i);
  }
	wait(0.5);
  }
}
