//blsm.c

/*

Copyright (c) 2016 HOKUTODENSHI CO;LTD

This software is released under the MIT License.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


//駆動磁界変更（定数定義：磁界のかけ方を変更）
//#define D_BLSM_POS_PHASE_CONTROL_METHOD

#include "iodefine.h"
#include "blsm.h"
#include "blsm_temp_table.h"

//定数定義
#define D_BLSM_LED_USE

//ポート定義
#define _PORT_S_0  0x0001
#define _PORT_S_1  0x0002
#define _PORT_S_2  0x0004
#define _PORT_S_3  0x0008
#define _PORT_S_4  0x0010
#define _PORT_S_5  0x0020
#define _PORT_S_6  0x0040
#define _PORT_S_7  0x0080
#define _PORT_S_8  0x0100
#define _PORT_S_9  0x0200
#define _PORT_S_10 0x0400
#define _PORT_S_11 0x0800
#define _PORT_S_12 0x1000
#define _PORT_S_13 0x2000
#define _PORT_S_14 0x4000
#define _PORT_S_15 0x8000

//電流をU→Wに流す設定, P1_5 : U(P) -> P2_5 : W(N)
#define D_BLSM_U_W_DIRECTION_P1 _PORT_S_5
#define D_BLSM_U_W_DIRECTION_P2 _PORT_S_5

//電流をU→Vに流す設定, P1_5 : U(P) -> P2_3 : V(N)
#define D_BLSM_U_V_DIRECTION_P1 _PORT_S_5
#define D_BLSM_U_V_DIRECTION_P2 _PORT_S_3

//電流をW→Vに流す設定, P1_14 : W(P) -> P2_3 : V(N)
#define D_BLSM_W_V_DIRECTION_P1 _PORT_S_14
#define D_BLSM_W_V_DIRECTION_P2 _PORT_S_3

//電流をW→Uに流す設定, P1_14 : W(P) -> P2_2 : U(N)
#define D_BLSM_W_U_DIRECTION_P1 _PORT_S_14
#define D_BLSM_W_U_DIRECTION_P2 _PORT_S_2

//電流をV→Uに流す設定, P1_6 : V(P) -> P2_2 : U(N)
#define D_BLSM_V_U_DIRECTION_P1 _PORT_S_6
#define D_BLSM_V_U_DIRECTION_P2 _PORT_S_2

//電流をV→Wに流す設定, P1_6 : V(P) -> P2_5 : W(N)
#define D_BLSM_V_W_DIRECTION_P1 _PORT_S_6
#define D_BLSM_V_W_DIRECTION_P2 _PORT_S_5

//電流を止める設定 P11_2 ~ P11_7=L
#define D_BLSM_OFF_CURRENT_P1 ~0x4060
#define D_BLSM_OFF_CURRENT_P2 ~0x002C

//エラーコード定義
#define D_BLSM_ERR_OVER_CURRENT 0x1
#define D_BLSM_ERR_OVER_TEMP 0x2


//過電流検出
#define D_BLSM_OVER_CURRENT_CONTROL 1				//1:過電流検出停止を行う
#define D_BLSM_OVER_CURRENT_COUNT_THRESHOLD 8000	//100us毎にチェックを行い1s間に8000回超過した場合停止

//過熱検出
#define D_BLSM_TEMPARATURE_CONTROL 1				//1:過熱検出停止を行う
#define D_BLSM_TEMPARATURE_THRESHOLD 50				//50[℃]

//回転数制御
#define D_BLSM_ROTATION_SPEED_CONTROL 1				//1:回転数制御を行う
#define D_BLSM_DUTY_MAX 750							//duty設定の最大値(/960)
#define D_BLSM_DUTY_MIN 100							//duty設定の最小値(/960)
#define D_BLSM_RPM_MAX 12000						//設定可能な回転数の最大値
#define D_BLSM_RPM_MIN 2000							//設定可能な回転数の最大値
#define D_BLSM_RPM_FEEDBACK_CONST 0.1				//duty変動幅（10%ずつ目標に近づけていく）
#define D_BLSM_RPM_FEEDBACK_THRESHOLD 0.1			//目標回転数と10%以内であればdutyの変更しない

//始動制御
#define D_BLSM_CONTROL_DUTY0 0						//ステージ0（停止時）のduty

#define D_BLSM_CONTROL_THRESHOLD1 100				//ステージ1を抜ける回転数[rpm]
#define D_BLSM_CONTROL_DUTY1_START (165-5)			//ステージ1の開始duty(110)
#define D_BLSM_CONTROL_DUTY1_MAX 180				//ステージ1のdutyの最大値
#define D_BLSM_CONTROL_CYCLE1 200					//100u*200=20[ms] 1回の始動制御持続時間
#define D_BLSM_CONTROL_CYCLE1_PRE 80				//100u*80=8[ms] プリテンションを加える時間

#define D_BLSM_CONTROL_THRESHOLD2 500				//ステージ2を抜ける回転数[rpm]
#define D_BLSM_CONTROL_DUTY2 225					//ステージ2のduty

#define D_BLSM_CONTROL_THRESHOLD3 1000				//ステージ3を抜ける回転数[rpm]
#define D_BLSM_CONTROL_DUTY3 300					//ステージ3のduty

//ブレーキ
#define D_BLSM_CONTROL_DUTY5 50						//ステージ5（ブレーキ）のduty
#define D_BLSM_CONTROL_DUTY5A 180					//ステージ5（ブレーキ）のduty　積極的に逆方向の磁界をかけるとき
#define D_BLSM_BREAK_RPM_THRESH	1000				//1000rpm以下の時は電流を減らす

//グローバル変数定義
unsigned char g_blsm_prev_pos;

//回転方向制御
char g_blsm_target_direction=0;	//char rotation=-1;	//0:stop, 1:反時計回り（前進）, -1:時計回り（後進）//2:ブレーキ

//回転数制御
unsigned short g_blsm_target_rpm=5000;				//目標回転数

//回転数、回転方向モニタ
unsigned short g_blsm_rpm;							//現在の回転数
unsigned short g_blsm_rpm_hist[16];					//過去の回転数
unsigned char g_blsm_rpm_hist_index=0;				//過去の回転数のインデックス
unsigned short g_blsm_rpm_ave;						//現在の回転数（平均）
char g_blsm_direction;								//現在の回転方向

//電圧、電流モニタ
double Vu, Vv, Vw;
unsigned short Vu_ad, Vv_ad, Vw_ad;
double Iu, Iv, Iw;
unsigned short Iu_ad, Iv_ad, Iw_ad;

//過電流検出
unsigned short g_blsm_over_current_count=0;			//電流が閾値を超えた回数
unsigned char g_blsm_over_current_check_start=0;	//モニタスタート

//温度検出
unsigned short g_blsm_temparature_ad_val;			//温度センサーのA/D変換値
short g_blsm_temparature;							//温度センサーの温度

//エラーコード
unsigned char g_blsm_error_code;					//エラーが起きた場合原因を保存


//制御アルゴリズム
unsigned char g_blsm_control_method=0; 			//ステージ 0:stop, 1:stage1, 2:stage2, 3:stage3, 4:normal
unsigned char g_blsm_control_method_prev=0;		//1つ前のステージ
unsigned long g_blsm_control_counter=0;			//100us割込みカウンタ
unsigned long g_blsm_control_counter2=0;		//10ms割込みカウンタ
unsigned char g_blsm_control_start_method=0;	//始動時のテンションを加える方向
unsigned short g_blsm_control_duty1=D_BLSM_CONTROL_DUTY1_START;	//ステージ1の開始duty
unsigned short g_blsm_control_start_wait=0;		//始動時のプリテンションを与える時間をカウントする変数

//デバッグ（始動制御）
//debug// unsigned long g_blsm_control_method_count[5] = {0,0,0,0,0};	//各ステージ偏移回数
//debug// unsigned long g_blsm_control_start_method_count[2]={0,0};	//始動時試行回数
//debug// unsigned char g_blsm_control_start_method_last;				//始動時にどちらの方向のテンションから始動したか

//デバッグ（位置検出回数カウント）
//debug// unsigned long g_blsm_pos_error=0;
//debug// unsigned long g_blsm_posx[6] = {0, 0, 0, 0, 0, 0};
//debug// unsigned long g_blsm_posxr[6] = {0, 0, 0, 0, 0, 0};
//debug// unsigned long g_blsm_pos_error2=0;
//debug// unsigned long g_blsm_posx2[6] = {0, 0, 0, 0, 0, 0};
//debug// unsigned long g_blsm_posxr2[6] = {0, 0, 0, 0, 0, 0};

//デバッグ（温度履歴）
//debug// short g_blsm_temparature_hist[10];					//0.1秒毎の温度値の履歴
//debug// unsigned short g_blsm_temparature_hist_index=0;

//デバッグ（過電流モニタ）
//unsigned short g_blsm_over_current_count_max=0;


void blsm_motor_init(void)
{
	unsigned char i;

	//出力端子設定
	//P1_5  : U(P)
	//P2_2  : U(N)
	//P1_6  : V(P)
	//P2_3  : V(N)
	//P1_14 : W(P)
	//P2_5  : W(N)

	PM1    &= ~0x4060; //ポートグループ1の各端子の入出力モード設定（つまりP1_5,6,14を出力に設定)
	//PPCMD1 =   0xA5;
	//PDSC1  =   0x4060;
	//PDSC1  =  ~0x4060;
	//PDSC1  =   0x4060;
	PMC1   &= ~0x4060; //ポートグループ1の各端子をソフトウェアI/O制御兼用モードに

	PM2    &= ~0x002C;
	//PPCMD1 =   0xA5;
	//PDSC2  =   0x002C;
	//PDSC2  =  ~0x002C;
	//PDSC2  =   0x002C;
	PMC2   &= ~0x002C;

	//ホールセンサ設定
	//P20_1 : Sensor1 (HS1)
	//P20_4 : Sensor2 (HS2)
	//P9_2  : Sensor3 (HS3)
	PM20  |= _PORT_S_1;
	PM20  |= _PORT_S_4;
	PM9   |= _PORT_S_2;
	PIBC20 |= _PORT_S_1;	//入力バッファ有効
	PIBC20 |= _PORT_S_4;	//入力バッファ有効
	PIBC9  |= _PORT_S_2;	//入力バッファ有効

#if 0

	//過電流検出
	//P20_2(INTP4) 1st/input : *INT
	PM20 |= _PORT_S_2;

	//PFCAE20 &= ~_PORT_S_2;
	PFCE20  &= ~_PORT_S_2;
	PFC20   &= ~_PORT_S_2;

	PMC20 |= _PORT_S_2;

	//アナログフィルタタイプA, エッジ設定
	FCLA0CTL4_INTPL = 0x2;		//fall edge

	//割込み設定
	ICP4 =  0x0044;	//エッジ, 割り込み許可（テーブル参照方式、優先度4）
#endif

	PIBC20 |= _PORT_S_2;	//入力バッファ有効（必要？）

	//ADC設定
	//仮想チャネルの設定
#if 1
	//ADC0-7を使用する設定
	ADCA0.VCR00.UINT32 = 0;		//VC0 = ADC0I0  AD0(VU)
	ADCA0.VCR01.UINT32 = 1;		//VC1 = ADC0I1  AD1(VV)
	ADCA0.VCR02.UINT32 = 4;		//VC2 = ADC0I4  AD2(VW)
	ADCA0.VCR03.UINT32 = 5;		//VC3 = ADC0I5  AD3(IU)
	ADCA0.VCR04.UINT32 = 8;		//VC4 = ADC0I8  AD4(IV)
	ADCA0.VCR05.UINT32 = 9;		//VC5 = ADC0I9  AD5(IW)
	ADCA0.VCR06.UINT32 = 12;	//VC6 = ADC0I12 AD6（温度）
	ADCA0.VCR07.UINT32 = 13;	//VC7 = ADC0I13 AD003(電源電圧）
	ADCA0.SGVCSP1.UINT32 = 0;	//スキャングループ1 開始仮想チャンネル
	ADCA0.SGVCEP1.UINT32 = 7;	//スキャングループ1 終了仮想チャンネル
#endif
	//ADC6のみを使用する設定
#if 0
	ADCA0.VCR00.UINT32 = 12;	//VC0 = ADC0I12 AD6（温度）
	ADCA0.SGVCSP1.UINT32 = 0;	//スキャングループ1 開始仮想チャンネル
	ADCA0.SGVCEP1.UINT32 = 0;	//スキャングループ1 終了仮想チャンネル
#endif
	ADCA0.ADCR.UINT32 = 0x0000;	//A/D制御レジスタ, 12bitモード
	ADCA0.THER.UINT32 = 0x0000;	//トラッキング&ホールド ディスエーブル

#if 0
	//TAUD0 : 60MHz/16 = 3.75MHz
//	奇数チャネルは同時に2つのCPUに割り込めないので，チャンネル2に変更
//	//ch0,ch1 INTERVAL
	//ch0,ch2 INTERVAL
	TAUD0.TPS = 0x0004;			//分周1:16
	TAUD0.CDR0 = (37500-1);		//3.75MHz -> 266.67ns 266.67ns * 37,500 = 10ms
//	TAUD0.CDR1 = (375-1);		//3.75MHz -> 266.67ns 266.67ns *    375 = 100us
	TAUD0.CDR2 = (375-1);		//3.75MHz -> 266.67ns 266.67ns *    375 = 100us

	//ch0
	TAUD0.CMOR0 = 0x0001;	//開始時に割り込みを発生させない
	TAUD0.CMUR0 = 0x00;

//	//ch1
//	TAUD0.CMOR1 = 0x0001;	//開始時に割り込みを発生させない
//	TAUD0.CMUR1 = 0x00;

	//ch2
	TAUD0.CMOR2 = 0x0001;	//開始時に割り込みを発生させない
	TAUD0.CMUR2 = 0x00;

	//ch0割込み設定
	ICTAUD0I0 = 0x0046;	//割り込み許可（テーブル参照方式、優先度6）

//	//ch1割込み設定
//	ICTAUD0I1 = 0x0045;	//割り込み許可（テーブル参照方式、優先度5）
//	IBDTAUD0I1 = 0x10002;	//割り込みCPU指定 PE1

	//ch2割込み設定
	ICTAUD0I2 = 0x0045;	//割り込み許可（テーブル参照方式、優先度5）

#endif

	//TAUB PWM port
	//P0_8 TAUB0O2 4th out
	PM0 &= ~_PORT_S_8;

	PFCAE0 &= ~_PORT_S_8;
	PFCE0  |= _PORT_S_8;
	PFC0   |= _PORT_S_8;

	PMC0  |= _PORT_S_8;

	//P0_9 TAUB0O4 4th out
	PM0 &= ~_PORT_S_9;

	PFCAE0 &= ~_PORT_S_9;
	PFCE0  |= _PORT_S_9;
	PFC0   |= _PORT_S_9;

	PMC0  |= _PORT_S_9;

	//TAUB0 PWM
	TAUB0.TPS = 0x0000;	//分周1:1

	TAUB0.CDR0 = 960;	//MASTER 60MHz 960 -> 周期16us
	TAUB0.CDR4 = 961;	//SLAVE  40MHz 961 -> H期間16us duty=961/(960+1)=100% QU
	TAUB0.CDR2 = 360;	//SLAVE  40MHz 360 -> H期間6us duty=360/(960+1)=37.5% QL

	//TAUB0.CDR2 = 0;

	//MASTER
	TAUB0.CMOR0 = 0x0801;
	TAUB0.CMUR0 = 0x00;

	//SLAVE
	TAUB0.CMOR2 = 0x0409;
	TAUB0.CMUR2 = 0x00;

	//SLAVE
	TAUB0.CMOR4 = 0x0409;
	TAUB0.CMUR4 = 0x00;

	TAUB0.TOE = 0x14;	//ch2,4=1
	TAUB0.TOM = 0x14;	//ch2,4=1
	TAUB0.TOC = 0x0;
	TAUB0.TOL = 0x0;	//正論理

	TAUB0.RDE = 0x15;	//ch0,2,4 = 1 一斉書き換え許可
	TAUB0.RDM = 0x0;	//マスタがカウントを開始すると一斉書き換えトリガ信号を生成

#if 0

	//TAUJ0 ch0,ch2 カウンタ     ch2は時間測定用
	TAUJ0.TPS = 0x0000;		//分周1:1
	TAUJ0.CMOR0 = 0x0004;	//ワンカウント, カウント中のスタートを許可
	TAUJ0.CMUR0 = 0x00;
	TAUJ0.CMOR2 = 0x0004;	//ワンカウント, カウント中のスタートを許可
	TAUJ0.CMUR2 = 0x00;

	//速度の平均初期化
	for(i=0; i<16; i++) g_blsm_rpm_hist[i]=0;
	g_blsm_rpm_hist_index=0;
#endif

}

void blsm_motor_start(void)
{
	//TAUDnTS : TAUDnチャネルスタートトリガレジスタ（1に定義したチャネルのカウンタ動作を許可）
	//今回は0x3なので2進数で表すと0000000000000011→ch0,1のカウンタスタート
	//TAUJnTS,TAUBnTSも同様
	//0x1→00000001→ch0スタート
	//0x15→0000000000010101→ch0,2,4スタート

//	TAUD0.TS = 0x3;		//ch0,1スタート
//	TAUD0.TS = 0x5;		//ch0,2スタート
//	TAUJ0.TS = 0x5;		//ch0,2スタート
	TAUB0.TS = 0x15;	//ch0,2,4スタート
}

void blsm_motor_stop(void)
{
	unsigned char i;

	__DI();

	P11 &= ~0x00fc;		//UVW,pn=L
	P1 &= D_BLSM_OFF_CURRENT_P1;
	P2 &= D_BLSM_OFF_CURRENT_P2;

	//TAUDnTT : TAUDnチャネルストップトリガレジスタ（1に定義したチャネルのカウンタ動作を停止）
	//TAUJnTT,TAUBnTTも同様

//	TAUD0.TT = 0x3;		//ch0,1ストップ
	TAUD0.TT = 0x5;		//ch0,2ストップ
	TAUJ0.TT = 0x5;		//ch0,2ストップ
	TAUB0.TT = 0x15;	//ch0,2,4ストップ

	g_blsm_target_direction=0;
	g_blsm_over_current_count=0;
	g_blsm_over_current_check_start=0;

	g_blsm_control_method=0;
	g_blsm_control_method_prev=0;
	g_blsm_control_counter=0;
	g_blsm_control_counter2=0;
	g_blsm_control_start_method=0;
	g_blsm_control_duty1=D_BLSM_CONTROL_DUTY1_START;
	g_blsm_control_start_wait=0;
	g_blsm_error_code=0;

	//速度の平均初期化
	for(i=0; i<16; i++) g_blsm_rpm_hist[i]=0;
	g_blsm_rpm_hist_index=0;

}

//現在未使用
void blsm_int_p0(void)
{
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
}

//現在未使用
void blsm_int_p2(void)
{
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
}

//現在未使用
void blsm_int_p11(void)
{
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
}

//割り込みで使用中(ただし中身は空)
void blsm_int_p4(void)
{
	//過電流停止（1回のパルスで止める場合）
	//P1 &= D_BLSM_OFF_CURRENT_P1;
	//P2 &= D_BLSM_OFF_CURRENT_P2;
	//g_blsm_target_direction = 0;
	//g_blsm_error_code |= D_BLSM_ERR_OVER_CURRENT;

}

//割り込みで使用中(10ms毎) 主にエラー検出と回転数制御の際のduty比の決定
void blsm_int_taud0i0(void)	//10ms割込み
{
	unsigned short target_rpm;
	unsigned short new_duty;
	float rpm_rate;
	float new_rpm_rate;
	float abs_diff;
	unsigned long rpm_sum;

	unsigned char i;

	volatile unsigned long wait;

	//10ms割込み回数カウント
	g_blsm_control_counter2++;

	//過電流検出
	if(D_BLSM_OVER_CURRENT_CONTROL == 1)  //過電流検出を行うかどうか
	{
		//g_blsm_over_current_countはblsm_int_taud0i1（割り込み100us毎）内で過電流が検出された回数
		//これがD_BLSM_OVER_CURRENT_COUNT_THRESHOLD(8000回)を超えた場合停止する
		if((g_blsm_over_current_check_start == 1) && (g_blsm_over_current_count >= D_BLSM_OVER_CURRENT_COUNT_THRESHOLD))
		{
			//過電流停止
			//インバータへの指令の送り方はEXCEL参照
			P1 &= D_BLSM_OFF_CURRENT_P1;	//上アーム、UVW OFF
			P2 &= D_BLSM_OFF_CURRENT_P2;	//下アーム、UVW OFF
			g_blsm_target_direction = 0;	//停止
			g_blsm_error_code |= D_BLSM_ERR_OVER_CURRENT;  //エラー原因を保存
#ifdef D_BLSM_LED_USE  //LED点灯
			while(1)
			{
			 	AP1 &= ~0x1000;	//D2=ON
			 	for(wait=0; wait<1000000; wait++);
			 	AP1 |=  0x1000;	//D2=OFF
			 	for(wait=0; wait<1000000; wait++);
			}
#endif
		}
	}

	//debug// if((g_blsm_over_current_check_start == 1) && (g_blsm_over_current_count_max < g_blsm_over_current_count))
	//debug// {
	//debug//  	g_blsm_over_current_count_max = g_blsm_over_current_count;
	//debug// }

	//1s毎に過電流計測カウンタをリセット
	if((g_blsm_control_counter2 % 100) == 0)
	{
		g_blsm_over_current_count=0;
	}



	//過熱検出
	ADCA0.SGSTCR1.UINT32 = 0x1;	//スキャングループ1のA/D変換を開始
	while( (ADCA0.SGSTR.UINT32 & 0x200) == 0x200 );	//スキャングループ1(bit9)が変換終了まで待つ
	//g_blsm_temparature_ad_valに温度センサーからの値を代入
	g_blsm_temparature_ad_val = (unsigned short)(ADCA0.DR06.UINT32 & 0x0000ffff);




	if(D_BLSM_TEMPARATURE_CONTROL == 1)  //過熱検出を行うかどうか
	{
		//blsm_tmp_table.hで定義されているAD変換値から温度の変換表(g_blsm_ad_val_tableとg_blsm_temp_table)を使い、センサーの値から温度を取得
		for(i=0; i<80; i++)
		{
			if(g_blsm_temparature_ad_val < g_blsm_ad_val_table[i]) break;
		}
		g_blsm_temparature = g_blsm_temp_table[i];

		//温度が閾値以上なら停止
		if(g_blsm_temparature >= D_BLSM_TEMPARATURE_THRESHOLD)
		{
			//過熱停止
			//インバータへの指令の送り方はEXCEL参照
			P1 &= D_BLSM_OFF_CURRENT_P1;	//上アーム、UVW OFF
			P2 &= D_BLSM_OFF_CURRENT_P2;	//下アーム、UVW OFF
			g_blsm_target_direction = 0;	//停止
			g_blsm_error_code |= D_BLSM_ERR_OVER_TEMP;  //エラー原因を保存
#ifdef D_BLSM_LED_USE  //LED点灯
			while(1)
			{
			 	AP1 &= ~0x2000;	//D3=ON
			 	for(wait=0; wait<1000000; wait++);
			 	AP1 |=  0x2000;	//D3=OFF
			 	for(wait=0; wait<1000000; wait++);
			}
		}
#endif
		//debug// if((g_blsm_control_counter2 % 10) == 0)
		//debug// {
		//debug// 	g_blsm_temparature_hist[g_blsm_temparature_hist_index++] = g_blsm_temparature;
		//debug// 	if(g_blsm_temparature_hist_index == 10) g_blsm_temparature_hist_index=0;
		//debug// }
	}

	//回転数制御を行う場合
	//g_blsm_control_method=4なので回転数が1000rpmを超えたときのみ
	if((D_BLSM_ROTATION_SPEED_CONTROL == 1) && (g_blsm_control_method == 4))
	{
		//g_blsm_target_rpm : 目標回転数 main.cにてSWからの指令により決定される
		target_rpm = g_blsm_target_rpm;

		//回転数の最大値や最小値を超えていたら修正
		if(target_rpm > D_BLSM_RPM_MAX) target_rpm=D_BLSM_RPM_MAX;
		if(target_rpm < D_BLSM_RPM_MIN) target_rpm=D_BLSM_RPM_MIN;

		//直近の回転数16回の平均を求める
		//g_blsm_rpm_hist[i]には過去16回分の速度が保存されている(割り込みblsm_int_taud0i1にて保存)
		rpm_sum=0;
		for(i=0; i<16; i++) rpm_sum+= g_blsm_rpm_hist[i];
		g_blsm_rpm_ave = (unsigned short)(rpm_sum >> 4);

		//回転数の平均値で回転数の制御を行う
		//目標値に対する現在の速度との比を計算
		rpm_rate =  (float)target_rpm / (float)g_blsm_rpm_ave;

		if(rpm_rate > 1.0) abs_diff = rpm_rate - 1.0;
		else abs_diff = 1.0 - rpm_rate;

		//目標値と現在の速度がD_BLSM_RPM_FEEDBACK_THRESHOLD(10%)以上ずれていたらduty比をD_BLSM_RPM_FEEDBACK_CONST(10%)ずつ変更する
		if( abs_diff > D_BLSM_RPM_FEEDBACK_THRESHOLD)
		{
			//新しい比の計算
			new_rpm_rate =  ((rpm_rate - 1.0) * D_BLSM_RPM_FEEDBACK_CONST) +1.0;

			//比より新しいduty比を計算
			//TAUB0.CDR2は現在のduty比
			new_duty = (unsigned short)((float)TAUB0.CDR2 * new_rpm_rate);
			if(new_duty < D_BLSM_DUTY_MIN) new_duty = D_BLSM_DUTY_MIN;
			if(new_duty > D_BLSM_DUTY_MAX) new_duty = D_BLSM_DUTY_MAX;
#ifdef D_BLSM_LED_USE  //適宜、LED点灯
			if(TAUB0.CDR2 == new_duty)		//duty更新なし
			{
			 	AP1 |= 0xC000;	//D4, D5=OFF
			}
			else if(TAUB0.CDR2 > new_duty)	//duty down
			{
			 	AP1 &= ~0x4000;	//D4=ON
			 	AP1 |=  0x8000;	//D5=OFF
			}
			else if(TAUB0.CDR2 < new_duty)	//duty up
			{
				AP1 |=  0x4000;	//D4=OFF
				AP1 &= ~0x8000;	//D5=ON
			}
#endif
			//duty比を新しいduty比変更
			TAUB0.CDR2 = new_duty;
			//↓これを打つことでduty比の指令が更新される
			TAUB0.RDT = 0x015;
		}
		else
		{
#ifdef D_BLSM_LED_USE
			//AP1 |= 0xC000;	//D4, D5=OFF
#endif
		}
	}

	//回転数平均化（10ms×16 = 160ms）
	//g_blsm_rpm_hist[g_blsm_rpm_hist_index++]=g_blsm_rpm;
	//if(g_blsm_rpm_hist_index >=16) g_blsm_rpm_hist_index=0;
	//rpm_sum=0;
	//for(i=0; i<16; i++) rpm_sum+= g_blsm_rpm_hist[i];
	//g_blsm_rpm_ave = (unsigned short)(rpm_sum >> 4);

}

void blsm_int_taud0i1(void)	//100us割込み
{
	//100us割込み回blsm_int_taud0i1数カウント
	g_blsm_control_counter++;

	blsm_rpm_calc();	//回転数,方向検出 g_blsm_rpm(回転数),g_blsm_direction(回転方向)に値が保存される

	//電流・電圧検出

	ADCA0.SGSTCR1.UINT32 = 0x1;	//スキャングループ1のA/D変換を開始
	while( (ADCA0.SGSTR.UINT32 & 0x200) == 0x200 );	//スキャングループ1(bit9)が変換終了まで待つ

	Vu_ad = (unsigned short)(ADCA0.DR00.UINT32 & 0x0000ffff); // U相電圧
	Vv_ad = (unsigned short)(ADCA0.DR00.UINT32 >> 16); // V相電圧
	Vw_ad = (unsigned short)(ADCA0.DR02.UINT32 & 0x0000ffff); // W相電圧

	Iu_ad = (unsigned short)(ADCA0.DR02.UINT32 >> 16); // U相駆動電流
	Iv_ad = (unsigned short)(ADCA0.DR04.UINT32 & 0x0000ffff); // V相駆動電流
	Iw_ad = (unsigned short)(ADCA0.DR04.UINT32 >> 16); // W相駆動電流

	//この後AD変換値を電圧・電流値に変換する必要がある
	Vu = (double)3/2 * 5.0 * Vu_ad/4095; //U相電圧
	Vv = (double)3/2 * 5.0 * Vv_ad/4095; //V相電圧
	Vw = (double)3/2 * 5.0 * Vw_ad/4095; //W相電圧

	Iu = 4 * 5.0 * Iu_ad/4095; //U相駆動電流
	Iv = 4 * 5.0 * Iv_ad/4095; //V相駆動電流
	Iw = 4 * 5.0 * Iw_ad/4095; //W相駆動電流


	//回転数平均化（直近の16回を保存）
	g_blsm_rpm_hist[g_blsm_rpm_hist_index++]=g_blsm_rpm;
	if(g_blsm_rpm_hist_index >=16) g_blsm_rpm_hist_index=0;

#ifndef D_BLSM_PORT_CONTROL_INTERRUPT_METHOD	//現状未定義↓下の部分有効
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD			//現状未定義↓無効,elseの部分が有効
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	//1000rpmを超えている場合、blsm_motor_control()で通常制御を行う
	//この時のduty比は10ms割り込み(blsm_int_taud0i1)の回転数制御の部分で決定される
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
#endif

	//過電流検出
	if((PPR0 & _PORT_S_13) != _PORT_S_13)
	{
		g_blsm_over_current_count++;		//P0_13=Lのとき 実際の停止処理はblsm_int_taud0i1で行う
	}
	else
	{
		g_blsm_over_current_check_start=1;
	}

	//モータの状態に応じて制御モードを決定
	if(g_blsm_target_direction == 0)  //停止
	{
		g_blsm_control_method=0;	//モード0（停止）
	}
	else if(g_blsm_target_direction == 2)	//ブレーキ
	{
		g_blsm_control_method=5;	//モード5（ブレーキ）
	}
	else
	{
		if(g_blsm_target_direction == g_blsm_direction)	//回転方向がターゲットと一致していれば
		{
			//回転数に応じてモードを決める
			if(g_blsm_rpm > D_BLSM_CONTROL_THRESHOLD3) g_blsm_control_method=4;			//1000rpm以上
			else if(g_blsm_rpm > D_BLSM_CONTROL_THRESHOLD2) g_blsm_control_method=3;	//500rpm以上
			else if(g_blsm_rpm > D_BLSM_CONTROL_THRESHOLD1) g_blsm_control_method=2;	//100rpm以上
		}
		else g_blsm_control_method=1;					//始動モード（モード1）
	}

	//debug// if(g_blsm_control_method != g_blsm_control_method_prev)
	//debug// {
	//debug// 	g_blsm_control_method_count[g_blsm_control_method]++;	//モード遷移モニタ
	//debug// }

	//制御モードに応じたduty比指令を入力]

	switch(g_blsm_control_method)
	{
		case 0:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY0;		//duty : 0
			TAUB0.RDT = 0x15;						//dutyの更新
			break;
		case 1:
			if(g_blsm_control_method_prev != 1) g_blsm_control_duty1 = D_BLSM_CONTROL_DUTY1_START;	//初期duty : 160	16.6%
			//g_blsm_control_method_prevの初期値は0だが始動制御が始まると1になる(上のif文は無効に)
			//始動制御の中でg_blsm_control_duty1は徐々に上昇
			TAUB0.CDR2 = g_blsm_control_duty1;
			TAUB0.RDT = 0x15;
			break;
		case 2:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY2;		//duty : 225	23.4%
			TAUB0.RDT = 0x15;
			break;
		case 3:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY3;		//duty : 300	31.2%
			TAUB0.RDT = 0x15;
			break;
		case 5:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY5;		//duty : 50		5.2%
			TAUB0.RDT = 0x15;
			break;
	}

	//モード2か3なら通常制御
	if((g_blsm_control_method == 2) || (g_blsm_control_method == 3))
	{
		blsm_motor_control();			//duty固定で一般の回転制御
	}
	//モード5ならブレーキをかける方向に電流を流す制御
	else if(g_blsm_control_method == 5)	//ブレーキ
	{
		blsm_motor_reverse_control();	//duty固定でブレーキの回転制御
	}
	//モード1なら始動制御
	else if(g_blsm_control_method == 1)	//始動制御
	{

		//正転逆転方向に順にテンションをかける
		switch(g_blsm_control_start_method)		//初期値0
		{
			case 0:
				if((g_blsm_control_counter % D_BLSM_CONTROL_CYCLE1) == 0)		//プレ処理 D_BLSM_CONTROL_CYCLE1(200)は一回の始動制御継続時間(200*100us=20ms) カウンタが200の倍数の時だけ動作
				{
					//duty（電流）を増加させる
					g_blsm_control_duty1+=5;
					if(g_blsm_control_duty1 > D_BLSM_CONTROL_DUTY1_MAX) g_blsm_control_duty1=D_BLSM_CONTROL_DUTY1_MAX;

					//センサー位置と回転方向に応じて、1ステップ先の磁界をかける
					blsm_motor_control_pos_phase();

					//debug// g_blsm_control_start_method_count[0]++;
					//debug// g_blsm_control_start_method_last=0;
					g_blsm_control_start_wait=D_BLSM_CONTROL_CYCLE1_PRE; 	//テンション機関のカウンタ値設定 D_BLSM_CONTROL_CYCLE1_PRE(80)はテンションを加える時間(80*100us=8ms)
					g_blsm_control_start_method++;	//現在0なので1にする(次はcase1)
				}
				break;
			case 1:
				if((g_blsm_control_counter % D_BLSM_CONTROL_CYCLE1) == 0)		//プレ処理
				{
					//センサー位置と回転方向に応じて、逆方向の磁界をかける
					blsm_motor_control_neg_phase();

					//debug// g_blsm_control_start_method_count[1]++;
					//debug// g_blsm_control_start_method_last=1;
					g_blsm_control_start_wait=D_BLSM_CONTROL_CYCLE1_PRE;
					g_blsm_control_start_method++;	//現在1なので2にする(次はdefault)
				}
				break;

			default:
				g_blsm_control_start_method=0;	//次はcase0
		}

		//一回の始動制御20msのうちテンションを加える期間8msが終わったら残りは通常制御
		if(g_blsm_control_start_wait == 0)
		{
			blsm_motor_control();			//プレテンション期間が終われば通常の回転制御
		}
		//まだ8ms立ってなかったらカウンタ減算
		else
		{
			g_blsm_control_start_wait--;
		}
	}

	g_blsm_control_method_prev = g_blsm_control_method;		//現在のモードをprevに保存

}

//位置センサーと回転方向に応じて磁界をかける関数
void blsm_motor_control(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;


	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;	//p1_regをP1に代入すれば上アームALLOFF
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;	//p2_regをP2に代入すれば下アームALLOFF

	//ホールセンサーの値の読み取り(EXCEL参照)
	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//posにホールセンサーの値(1～6)を格納(EXCEL参照)
	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);


	if(g_blsm_target_direction == 0)			//停止
	{
#ifdef D_BLSM_LED_USE //LED OFF
		AP1 |= 0x1000;		//D2=OFF
		AP1 |= 0x2000;		//D3=OFF
#endif
		//インバータ停止命令
		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}
	else if(g_blsm_target_direction == 1)		//前進
	{
#ifdef D_BLSM_LED_USE //LED2ON
		AP1 &= ~0x1000;		//D2=ON
		AP1 |= 0x2000;		//D3=OFF
#endif

		//前進方向に磁界をかける(電流を流す向き,指令の送り方はEXCEL参照)
		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
	//後進方向に磁界をかける
	else if(g_blsm_target_direction == -1)	//後進
	{
#ifdef D_BLSM_LED_USE //LED3ON
		AP1 &= ~0x2000;		//D3=ON
		AP1 |= 0x1000;		//D2=OFF
#endif

		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posxr2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posxr2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posxr2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posxr2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posxr2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posxr2[5]++;
				break;
			default:
				//debug// _blsm_pos_error2++;
				break;
		}
	}
}

void blsm_motor_control_pos_phase(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;

	//ホールセンサの読み取り(blsm_motor_controlと同様)
	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(g_blsm_target_direction == 0)			//停止
	{
#ifdef D_BLSM_LED_USE
		AP1 |= 0x1000;		//D2=OFF
		AP1 |= 0x2000;		//D3=OFF
#endif

		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}
	else if(g_blsm_target_direction == 1)		//前進
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x1000;		//D2=ON
		AP1 |= 0x2000;		//D3=OFF
#endif
		//1ステップ先の磁界をかける(blsm_motor_controlの時とは一つ磁界がずれてる)
		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// _blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
	else if(g_blsm_target_direction == -1)	//後進
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x2000;		//D3=ON
		AP1 |= 0x1000;		//D2=OFF
#endif

		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// _blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
}

void blsm_motor_control_neg_phase(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;

	//ホールセンサの読み取り(blsm_motor_controlと同様)
	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(g_blsm_target_direction == 0)			//停止
	{
#ifdef D_BLSM_LED_USE
		AP1 |= 0x1000;		//D2=OFF
		AP1 |= 0x2000;		//D3=OFF
#endif

		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}
	else if(g_blsm_target_direction == 1)		//前進
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x1000;		//D2=ON
		AP1 |= 0x2000;		//D3=OFF
#endif
		//逆方向の磁界をかける(blsm_motor_controlの時とは電流の向きが逆)
		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// _blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// _blsm_pos_error2++;
				break;
		}
	}
	else if(g_blsm_target_direction == -1)	//後進
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x2000;		//D3=ON
		AP1 |= 0x1000;		//D2=OFF
#endif

		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// _blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
}

void blsm_motor_reverse_control(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;

	//ホールセンサの読み取り(blsm_motor_controlと同様)
	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(g_blsm_target_direction == 0)			//停止
	{
#ifdef D_BLSM_LED_USE
		AP1 |= 0x1000;		//D3=OFF
		AP1 |= 0x2000;		//D2=OFF
#endif

		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}

	//5ms以内にセンサーの遷移を観測出来た場合、かつ所定の回転数以上のとき、かつ回転方向が検出できるとき積極的にブレーキを掛ける
	if((TAUJ0.CNT0 < 200000) && (g_blsm_rpm > D_BLSM_BREAK_RPM_THRESH) && (g_blsm_direction != 0) && (g_blsm_direction != 127))
	{
		TAUB0.CDR2 = D_BLSM_CONTROL_DUTY5A;		//制御電流を増やす(180)
		TAUB0.RDT = 0x15;

		if(g_blsm_direction == -1)		//前進方向の回転のとき(後進してるとき前進方向に電流を流す？)
		{
			switch(pos)
			{
				case 5:	//pos0
					P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
					break;
				case 1:	//pos1
					P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
					break;
				case 3:	//pos2
					P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
					break;
				case 2:	//pos3
					P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
					break;
				case 6:	//pos4
					P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
					break;
				case 4:	//pos5
					P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
					break;
				default:
					break;
			}
		}
		else if(g_blsm_direction == -1)	//後進方向の回転のとき(前進してるとき後進方向に電流を流す？だとしたら1の間違い？)
		{
			switch(pos)
			{
				case 5:	//pos0
					P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
					break;
				case 1:	//pos1
					P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
					break;
				case 3:	//pos2
					P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
					break;
				case 2:	//pos3
					P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
					break;
				case 6:	//pos4
					P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
					break;
				case 4:	//pos5
					P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
					break;
				default:
					break;
			}
		}
	}
	else
	{
		TAUB0.CDR2 = D_BLSM_CONTROL_DUTY5;	//電流を減らす(50)
		TAUB0.RDT = 0x15;

		P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
		P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
		//P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
		//P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;

	}

}

//現在の回転速度と回転方向をホールセンサーの値から計測
void blsm_rpm_calc(void)
{

	unsigned short pos;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;
	float rpm_tmp;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(pos != g_blsm_prev_pos)
	{
		if((1 <= pos) && (pos <= 6))
		{
			rpm_tmp = 4.0e8 / (float)TAUJ0.CNT0;	//rpm = 1/(T*25n)*60/6 = 2.4e9/T
			g_blsm_rpm = (unsigned short)rpm_tmp;
			TAUJ0.TS = 0x1;		//TAUJ0 ch0リスタート

			g_blsm_direction=127;	//127:不正値
			switch(pos)
			{
				case 5:	//pos0
					if(g_blsm_prev_pos == 4) g_blsm_direction=1;	//pos5
					if(g_blsm_prev_pos == 1) g_blsm_direction=-1;	//pos1
					break;
				case 1:	//pos1
					if(g_blsm_prev_pos == 5) g_blsm_direction=1;	//pos0
					if(g_blsm_prev_pos == 3) g_blsm_direction=-1;	//pos2
					break;
				case 3:	//pos2
					if(g_blsm_prev_pos == 1) g_blsm_direction=1;	//pos1
					if(g_blsm_prev_pos == 2) g_blsm_direction=-1;	//pos3
					break;
				case 2:	//pos3
					if(g_blsm_prev_pos == 3) g_blsm_direction=1;	//pos2
					if(g_blsm_prev_pos == 6) g_blsm_direction=-1;	//pos4
					break;
				case 6:	//pos4
					if(g_blsm_prev_pos == 2) g_blsm_direction=1;	//pos3
					if(g_blsm_prev_pos == 4) g_blsm_direction=-1;	//pos5
					break;
				case 4:	//pos5
					if(g_blsm_prev_pos == 6) g_blsm_direction=1;	//pos4
					if(g_blsm_prev_pos == 5) g_blsm_direction=-1;	//pos0
					break;
			}
		}
	}
	if(TAUJ0.CNT0 > 20000000)	//500msセンサーの遷移を観測できない場合
	{
		g_blsm_rpm=0;
		g_blsm_direction=0;
		TAUJ0.TS = 0x1;		//TAUJ0 ch0リスタート
	}

	g_blsm_prev_pos=pos;


}

