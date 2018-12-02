/*
 * color_control.cpp
 *
 * Created: 2018/08/23 18:28:32
 *  Author: shun2
 */ 
/*
PF0:RANGE
PF1:CK
PF2:GATE
PF3:DOUT
*/
#include "color_control.hpp"

uint16_t red=0, green=0, blue=0;	//値
uint16_t high=800, low=500;	//しきい値 白>800  700<銀<=800

void init_color(void){	//initialize
	PORTF.DIRSET = PIN0_bm|PIN1_bm|PIN2_bm;
}
void color_read(void){	//センサーの値をgree,red,blueに代入してくれる
	red=0, green=0, blue=0;
	PORTF.OUTCLR = PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;
	_delay_us(2000);
	PORTF.OUTSET = RANGE;
	PORTF.OUTSET = GATE;
	_delay_ms(20);	//光検出delay
	PORTF.OUTCLR = GATE;
	_delay_us(4);
	red=color_shift();
	green=color_shift();
	blue=color_shift();
	PORTF.OUTSET = GATE;
}

uint16_t color_shift(void){//12ビット分のパルス送信と読み込み処理
	uint16_t result=0;//検出結果用の変数を用意（0：初期化）
	for(uint8_t i=0;i<12;i++){//12ビット分の繰り返し処理
		PORTF.OUTSET = CK;//1ビット分のクロックパルス出力（HIGH）
		_delay_us(1);
		if((PORTF.IN & DOUT)!=0){//Dout端子からの出力がHighの場合
			result+=(1<<i);//12ビットのi桁目に1を代入（i桁分だけ左にシフト）
		}
		PORTF.OUTCLR = CK;//1ビット分のクロックパルス出力（LOW）
		_delay_us(1);
	}
	_delay_us(3);//3マイクロ秒待機
	return result;//結果を出力
}

uint8_t color_check(void){	//カラーチェック 0:白 1:クロ 2:銀（チェックポイント）　　returnで帰ってきます。  //まだ値定まらない！
	color_read();
	if(blue+green+red>700){
		return 2;
	}
	//else if(blue+green+red>=100){
		//return 0;
	//}
	else{
		return 1;
	}
}