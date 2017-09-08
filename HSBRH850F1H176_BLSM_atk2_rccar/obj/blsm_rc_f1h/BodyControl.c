/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Software
 *
 *  Copyright (C) 2014-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2014-2015 by FUJI SOFT INCORPORATED, JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id$
 */

/*
 *		ボディ系関連
 */

#include "Os.h"
#include "BodyControl.h"
#include "t_syslog.h"
#include "target_sysmod.h"

/*
 *  IOポート経由で接続されたTLU01の制御
 */

/*
 * TLU01-1 : P8_3
 * TLU01-2 : P8_1
 * TLU01-3 : P8_5
 * TLU01-4 : P8_4
 * TLU01-5 : P8_7
 * TLU01-6 : P8_6
 * TLU01-7 : P8_9
 * TLU01-8 : P8_8
 */

#define TLU01_P8  0x03fa

/*
 *  ボディ系に関する初期化
 */
void
BodyControlInit(void)
{
	uint16 wk;

	/* PMC8 設定 */
	wk = sil_reh_mem((void *) PMC(8));
	wk &= ~TLU01_P8;
	sil_wrh_mem((void *) PMC(8), wk);

	/* PM8 設定 */
	wk = sil_reh_mem((void *) PM(8));
	wk &= ~TLU01_P8;
	sil_wrh_mem((void *) PM(8), wk);

	wk = sil_reh_mem((void *) P(8));
	wk &= ~TLU01_P8;
	sil_wrh_mem((void *) P(8), wk);
}

void
tlu01_set(uint8 no, boolean on)
{
	uint16 wk;
	uint16 pattern;

	if (!(no <= TLU01_PORT_MAX)) {
		return;
	}

	switch(no){
		case 0:	pattern = 0x0020;
				break;
		case 1:	pattern = 0x0002;
				break;
		case 2:	pattern = 0x0080;
				break;
		case 3:	pattern = 0x0008;
				break;
		case 4:	pattern = 0x0010;
				break;
		case 5:	pattern = 0x0040;
				break;
		case 6:	pattern = 0x0200;
				break;
		case 7:	pattern = 0x0100;
				break;
	}

	wk = sil_reh_mem((void *) P(8));
	if (on) {
		wk |=  pattern;
	}
	else {
		wk &= ~pattern;
	}
	sil_wrh_mem((void *) P(8), wk);
}

/*
 *  個別のLEDの設定
 */
void
tSetEachLED(uint8 no, boolean on)
{
	tlu01_set(no, on);
}

void
tSetBuzzer(boolean onoff){
	uint16 wk;

	wk = sil_reh_mem((void *) P(8));
	if(onoff == TRUE){
		wk |= 1 << 9;
	}
	else {
		wk &= ~(1 << 9);
	}
	sil_wrh_mem((void *) P(8), wk);
}

/*
 * ランプに関する処理
 */
void
LampControl(void)
{
	/* ヘッドランプ設定 */
	HEAD_LAMP_SET(g_bodycmd.headlamp);
	/* フォグランプ設定 */
	FOG_LAMP_SET(g_bodycmd.foglamp);
	/* ブレーキランプ設定 */
	BRAKE_LAMP_SET(g_bodycmd.brakelamp);
	/* バックランプ設定 */
	BACK_LAMP_SET(g_bodycmd.backlamp);
}

/*
 *  ウィンカーの点滅周期用カウンタの初期値
 */
#define CNT_BLINK_INIT  (WINKER_INTERVAL_MS/BODY_CYCLE_MS)

/*
 *  ウィンカーに関する処理
 */
void
WinkerControl(void)
{
	static uint8	pre_winker = WINKER_OFF;
	static boolean	pre_winker_lr = FALSE;
	static boolean	pre_winker_l = FALSE;
	static boolean	pre_winker_r = FALSE;
	static uint32	cnt_blink = 0U;

	/*  指示に変化があったタイミングから点滅周期を開始する */
	if (g_bodycmd.winker != pre_winker) {
		cnt_blink = 0U;
	}

	if (cnt_blink == 0U) {
		switch (g_bodycmd.winker) {
		case WINKER_OFF: /* 消灯 */
			WINKER_L_SET(FALSE);
			WINKER_R_SET(FALSE);
			break;
		case WINKER_L_ON: /* 左のみ点灯 */
			WINKER_L_SET(TRUE);
			break;
		case WINKER_R_ON: /* 右のみ点灯 */
			WINKER_R_SET(TRUE);
			break;
		case WINKER_LR_ON: /* 左右点灯 */
			WINKER_L_SET(TRUE);
			WINKER_R_SET(TRUE);
			break;
		case WINKER_L_BLINK: /* 左のみ点滅 */
			if (pre_winker_l == FALSE) {
				WINKER_L_SET(TRUE);
				pre_winker_l = TRUE;
			}
			else {
				WINKER_L_SET(FALSE);
				pre_winker_l = FALSE;
			}
			break;
		case WINKER_R_BLINK: /* 右のみ点滅 */
			if (pre_winker_r == FALSE) {
				WINKER_R_SET(TRUE);
				pre_winker_r = TRUE;
			}
			else {
				WINKER_R_SET(FALSE);
				pre_winker_r = FALSE;
			}
			break;
		case WINKER_LR_BLINK: /* 左右点滅 */
			if (pre_winker_lr == FALSE) {
				WINKER_L_SET(TRUE);
				WINKER_R_SET(TRUE);
				pre_winker_lr = TRUE;
			}
			else {
				WINKER_L_SET(FALSE);
				WINKER_R_SET(FALSE);
				pre_winker_lr = FALSE;
			}
			break;
		default:
			break;
		}
		cnt_blink = CNT_BLINK_INIT;
	}

	cnt_blink--;

	pre_winker = g_bodycmd.winker;
}

/*
 *  ボディ系タスク
 */
TASK(BodyControlTask)
{
	static	boolean executed = FALSE;

	/* 初回起動時のみ実行 */
	if (executed == FALSE) {
		syslog(LOG_NOTICE, "BodyControlTask : Start!");
		BodyControlInit();
		executed = TRUE;
	}

	/* ランプに関する処理 */
	LampControl();

	/* ウィンカーに関する処理 */
	WinkerControl();

	TerminateTask();
}

/*
 *  ブザー用のアラームコールバック
 */
ALARMCALLBACK(BuzzerControlCycAlarmCb){
	static boolean	buzzer_toggle = FALSE;
	static uint16	buzzer_cycle = 0U;
	boolean 		buzzer_on = FALSE;

	switch (g_bodycmd.buzzer) {
	  case BUZZER_OFF:
		/* 消音 */
		buzzer_cycle = 0U;
		break;
	  case BUZZER_ON:
		/* 常時発音 */
		buzzer_on = TRUE;
		break;
	  case BUZZER_INTERVAL_ON:
		/* 一定間隔で発音 */
		buzzer_cycle++;
		if (buzzer_cycle < ((BUZZER_INTERVAL_MS * 1000)/BUZZER_CYCLE_NS)) {
			buzzer_on = TRUE;
		}
		if (buzzer_cycle == ((BUZZER_INTERVAL_MS * 1000)/BUZZER_CYCLE_NS)*2) {
			buzzer_cycle = 0U;
		}
		break;
	  default:
		break;
	}

	if (buzzer_on == TRUE) {
		tSetBuzzer(buzzer_toggle);
		buzzer_toggle = (buzzer_toggle == TRUE) ? FALSE : TRUE;
	} else {
		tSetBuzzer(FALSE);
	}
}
