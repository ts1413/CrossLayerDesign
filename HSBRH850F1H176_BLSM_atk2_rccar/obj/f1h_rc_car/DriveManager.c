/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
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
 *		制御系関連
 */

#include "Os.h"
#include "ModelCarControl.h"
#include "t_syslog.h"
#include "target_sysmod.h"

extern sint16 blsm_speed;

/*
 *  ステアリングのニュートラル値
 */
sint8	steer_neutral = STEER_NEUTRAL_INIT;


/*
 *  ステアリングニュートラルや車速ゲイン調整時のACK通知時間
 */
#define ACK_CNT_MAX		8U

/*
 *  ステアリングニュートラルや車速ゲイン調整時のACK通知用変数
 */
static uint8	acklr_cnt = 0U;
static uint8	ackr_cnt = 0U;
static uint8	ackl_cnt = 0U;

/*
 *  速度の加減速量
 */
#define BRAKE_STEP_SIZE 500
#define SPEED_STEP_SIZE 100

/*
 *  TAUB関連レジスタ
*/
#define TAUB_BASE(n)	((uint32) (0xffe30000U + (n * 0x1000U)))
#define TAUB_TPS(n)		(TAUB_BASE(n) + 0x240U)
#define TAUB_CDR(n, m)	(TAUB_BASE(n) + 0x00U + (m * 0x04U))
#define TAUB_CNT(n, m)	(TAUB_BASE(n) + 0x80U + (m * 0x04U))
#define TAUB_CMOR(n, m)	(TAUB_BASE(n) + 0x200U + (m * 0x04U))
#define TAUB_CMUR(n, m)	(TAUB_BASE(n) + 0xc0U + (m * 0x04U))
#define TAUB_CSR(n, m)	(TAUB_BASE(n) + 0x140U + (m * 0x04U))
#define TAUB_CSC(n, m)	(TAUB_BASE(n) + 0x180U + (m * 0x04U))
#define TAUB_TS(n)		(TAUB_BASE(n) + 0x1c4U)
#define TAUB_TE(n)		(TAUB_BASE(n) + 0x1c0U)
#define TAUB_TT(n)		(TAUB_BASE(n) + 0x1c8U)
#define TAUB_TOE(n)		(TAUB_BASE(n) + 0x5CU)
#define TAUB_TO(n)		(TAUB_BASE(n) + 0x58U)
#define TAUB_TOM(n)		(TAUB_BASE(n) + 0x248U)
#define TAUB_TOC(n)		(TAUB_BASE(n) + 0x24cU)
#define TAUB_TOL(n)		(TAUB_BASE(n) + 0x040U)
#define TAUB_TDE(n)		(TAUB_BASE(n) + 0x250U)
#define TAUB_TDL(n)		(TAUB_BASE(n) + 0x54U)
#define TAUB_RDE(n)		(TAUB_BASE(n) + 0x260U)
#define TAUB_RDM(n)		(TAUB_BASE(n) + 0x264U)
#define TAUB_RDS(n)		(TAUB_BASE(n) + 0x268U)
#define TAUB_RDC(n)		(TAUB_BASE(n) + 0x26cU)
#define TAUB_RDT(n)		(TAUB_BASE(n) + 0x44U)
#define TAUB_RSF(n)		(TAUB_BASE(n) + 0x48U)
#define TAUB_EMU(n)		(TAUB_BASE(n) + 0x290U)

void blsm_motor_init(void)
{
	uint16 wk;

	//出力端子設定
	//P1_5  : U(P)
	//P2_2  : U(N)
	//P1_6  : V(P)
	//P2_3  : V(N)
	//P1_14 : W(P)
	//P2_5  : W(N)

	/* PMC1 設定 */
	wk = sil_reh_mem((void *) PMC(1));
	wk &= ~0x4060;
	sil_wrh_mem((void *) PMC(1), wk);

	/* PM1 設定 */
	wk = sil_reh_mem((void *) PM(1));
	wk &= ~0x4060;
	sil_wrh_mem((void *) PM(1), wk);

	/* PMC2 設定 */
	wk = sil_reh_mem((void *) PMC(2));
	wk &= ~0x002c;
	sil_wrh_mem((void *) PMC(2), wk);

	/* PM2 設定 */
	wk = sil_reh_mem((void *) PM(2));
	wk &= ~0x002c;
	sil_wrh_mem((void *) PM(2), wk);


	//ホールセンサ設定
	//P20_1 : Sensor1 (HS1)
	//P20_4 : Sensor2 (HS2)
	//P9_2  : Sensor3 (HS3)

	/* PM20 設定 */
	wk = sil_reh_mem((void *) PM(20));
	wk |= 0x0012;
	sil_wrh_mem((void *) PM(20), wk);

	/* PIBC20 設定 */
	wk = sil_reh_mem((void *) PIBC(20));
	wk |= 0x0012;
	sil_wrh_mem((void *) PIBC(20), wk);

	/* PM9 設定 */
	wk = sil_reh_mem((void *) PM(9));
	wk |= 0x0012;
	sil_wrh_mem((void *) PM(9), wk);

	/* PIBC9 設定 */
	wk = sil_reh_mem((void *) PIBC(9));
	wk |= 0x0004;
	sil_wrh_mem((void *) PIBC(9), wk);


	//TAUB PWM port
	//P0_8 TAUB0O2 4th out
	//P0_9 TAUB0O4 4th out

	/* PM0 設定 */
	wk = sil_reh_mem((void *) PM(0));
	wk &= ~0x0300;
	sil_wrh_mem((void *) PM(0), wk);

	/* PFCAE0 設定 */
	wk = sil_reh_mem((void *) PFCAE(0));
	wk &= ~0x0300;
	sil_wrh_mem((void *) PFCAE(0), wk);

	/* PFCE0 設定 */
	wk = sil_reh_mem((void *) PFCE(0));
	wk |= 0x0300;
	sil_wrh_mem((void *) PFCE(0), wk);

	/* PFC0 設定 */
	wk = sil_reh_mem((void *) PFC(0));
	wk |= 0x0300;
	sil_wrh_mem((void *) PFC(0), wk);

	/* PMC0 設定 */
	wk = sil_reh_mem((void *) PMC(0));
	wk |= 0x0300;
	sil_wrh_mem((void *) PMC(0), wk);


	//TAUB0 PWM
	sil_wrh_mem((void *) TAUB_TPS(0), 0x0000); //分周1:1

	sil_wrh_mem((void *) TAUB_CDR(0, 0), 960); //MASTER 60MHz 960 -> 周期16us
	sil_wrh_mem((void *) TAUB_CDR(0, 4), 960); //SLAVE  40MHz 961 -> H期間16us duty=961/(960+1)=100% QU
	sil_wrh_mem((void *) TAUB_CDR(0, 2), 360); //SLAVE  40MHz 360 -> H期間6us duty=360/(960+1)=37.5% QL

	sil_wrh_mem((void *) TAUB_CMOR(0, 0), 0x0801);
	sil_wrb_mem((void *) TAUB_CMUR(0, 0), 0x00);

	sil_wrh_mem((void *) TAUB_CMOR(0, 2), 0x0409);
	sil_wrb_mem((void *) TAUB_CMUR(0, 2), 0x00);

	sil_wrh_mem((void *) TAUB_CMOR(0, 4), 0x0409);
	sil_wrb_mem((void *) TAUB_CMUR(0, 4), 0x00);

	sil_wrb_mem((void *) TAUB_TOE(0), 0x14); //ch2,4=1
	sil_wrb_mem((void *) TAUB_TOM(0), 0x14); //ch2,4=1
	sil_wrb_mem((void *) TAUB_TOC(0), 0x0);
	sil_wrb_mem((void *) TAUB_TOL(0), 0x0);  //正論理

	sil_wrb_mem((void *) TAUB_RDE(0), 0x15); //ch0,2,4 = 1 一斉書き換え許可
	sil_wrb_mem((void *) TAUB_RDM(0), 0x0);  //マスタがカウントを開始すると一斉書き換えトリガ信号を生成
}

void blsm_motor_start(void)
{
	sil_wrb_mem((void *) TAUB_TS(0), 0x15);  //ch0,2,4スタート
}

void blsm_motor_stop(void)
{
	uint16 wk;

	wk = sil_reh_mem((void *) P(11));
	wk &= ~0x00fc;
	sil_wrh_mem((void *) P(11), wk);

	wk = sil_reh_mem((void *) P(1));
	wk &= ~0x4060;
	sil_wrh_mem((void *) P(1), wk);

	wk = sil_reh_mem((void *) P(2));
	wk &= ~0x003c;
	sil_wrh_mem((void *) P(2), wk);

	sil_wrb_mem((void *) TAUB_TT(0), 0x15);  //ch0,2,4ストップ
}


/*
 * ステアリング初期化用API
 */
void
SteerInit(void)
{
		pwm_diag_init(PWM_CYCLE_US, SERVO_N_US, 0);
}

/*
 * ステアリング値設定用API
 */
void
SetSteerAngle(sint8 angle_val)
{
  static sint32 servo_pwm = SERVO_N_US;

  /*
   * ステアリング制御
   */
  if ((angle_val <= STEER_ANGLE_MAX) && (angle_val >= STEER_ANGLE_MIN)) {
    if (angle_val >= 0) {
      servo_pwm = SERVO_N_US - (((SERVO_N_US - SERVO_LMAX_US) * 100) * angle_val / (64 * 100));
    }
    else {
      servo_pwm = SERVO_N_US + (((SERVO_RMAX_US - SERVO_N_US) * 100) * (-angle_val) / (64 * 100));
    }
    /* ステアリング値設定 */
    pwm_diag_change_servo_duty(servo_pwm);
  }
}

/*
 * 制御系に関する初期化
 */
void
DriveInit(void)
{
	SteerInit();
    blsm_motor_init();
    blsm_motor_start();
}

/*
 *  操舵角と車速の決定
 */
void
DrivingControl(void)
{

	sint32 speed;
	static sint32 pre_speed=0;

	/* ステアリングニュートラル調整 */
	switch (g_contcmd.steern) {
	  case CONTCMD_STEERN_I:
		steer_neutral = STEER_NEUTRAL_INIT;
		acklr_cnt = ACK_CNT_MAX;
		break;
	  case CONTCMD_STEERN_R:
		/* 右(-)へ調整 */
		if (steer_neutral > STEER_NEUTRAL_MIN) {
			steer_neutral--;
		}
		ackr_cnt = ACK_CNT_MAX;
		break;
	  case CONTCMD_STEERN_L:
		/* 左(+)へ調整 */
		if (steer_neutral < STEER_NEUTRAL_MAX) {
			steer_neutral++;
		}
		ackl_cnt = ACK_CNT_MAX;
		break;
	  default:
		/* 要求なし */
		break;
	}

	/* 操舵角決定 */
	SetSteerAngle(g_contcmd.angle + steer_neutral);

	/* さらに200倍(Max 12000rpm程度) */
	speed = g_contcmd.speed * 200;

	if(g_contcmd.brake){ //ブレーキ時
		/* 指令値はBRAKE_STEP_SIZE rpmずつ遅くする */
		if(BRAKE_STEP_SIZE > speed && speed > -1 * BRAKE_STEP_SIZE) speed = 0;
		else if(speed > 0) speed -= BRAKE_STEP_SIZE;
		else speed += BRAKE_STEP_SIZE;
	}
	else{
		/* 指令値はSPEED_STEP_SIZE rpmずつ変更 */
		if(speed - pre_speed > SPEED_STEP_SIZE){ //加速時
			if(0 > pre_speed && pre_speed > -1 * SPEED_STEP_SIZE) speed = 0;
			else speed = pre_speed + SPEED_STEP_SIZE;
		}
		else if(speed - pre_speed < -1* SPEED_STEP_SIZE){ //減速時
			if(SPEED_STEP_SIZE > pre_speed && pre_speed > 0) speed = 0;
			else speed = pre_speed - SPEED_STEP_SIZE;
		}
	}

	blsm_speed = speed;
	pre_speed = speed;
}


/*
 *  初回起動時のボディ系の確認の点滅終了までのカウント値
 */
#define START_CNT_MAX	(INIT_BODY_BLINK_CYCLE_MS*INIT_BODY_BLINK_COUNT*2)/DRIVEMANAGER_TASK_CYCLE_MS

void
BodyControl(void)
{
	static uint8	start_cnt = 0U;
	static boolean	toggle_headlamp = FALSE;
	static boolean	toggle_foglamp = FALSE;
	static boolean	toggle_hazard = FALSE;
	static boolean	toggle_winker_r = FALSE;
	static boolean	toggle_winker_l = FALSE;
	static boolean	pre_headlamp = FALSE;
	static boolean	pre_foglamp = FALSE;
	static boolean	pre_hazard = FALSE;
	static boolean	pre_winker_r = FALSE;
	static boolean	pre_winker_l = FALSE;
	static uint32	canmsg_snd_cycle = 0;

	/* 初回起動時処理 */
	if (start_cnt <= START_CNT_MAX) {
		/* 左右のWinker，全Light点滅(head，fog，brake，back) */
		if ((((start_cnt / (START_CNT_MAX/(INIT_BODY_BLINK_COUNT*2))) % 2U) == 0U)
			&& (start_cnt != START_CNT_MAX)) {
			g_bodycmd.headlamp  = TRUE;
			g_bodycmd.foglamp   = TRUE;
			g_bodycmd.brakelamp = TRUE;
			g_bodycmd.backlamp  = TRUE;
			g_bodycmd.winker    = WINKER_LR_ON;
			g_bodycmd.buzzer    = BUZZER_ON;
			canmsg_snd_cycle = 0;
		}
		else {
			g_bodycmd.headlamp  = FALSE;
			g_bodycmd.foglamp   = FALSE;
			g_bodycmd.brakelamp = FALSE;
			g_bodycmd.backlamp  = FALSE;
			g_bodycmd.winker    = WINKER_OFF;
			g_bodycmd.buzzer    = BUZZER_OFF;
			canmsg_snd_cycle = 0;
		}
		/* 起動カウントインクリメント */
		start_cnt++;
	}
	/* 通常起動時処理 */
	else {
		/* FALSE→TRUE遷移時に各トグル切り替え */
		if ((pre_headlamp == FALSE) && (g_contcmd.headlamp == TRUE)) {
			toggle_headlamp = (toggle_headlamp == FALSE) ? TRUE : FALSE;
		}
		if ((pre_foglamp == FALSE) && (g_contcmd.foglamp == TRUE)) {
			toggle_foglamp = (toggle_foglamp == FALSE) ? TRUE : FALSE;
		}
		if ((pre_hazard == FALSE) && (g_contcmd.hazard == TRUE)) {
			toggle_hazard = (toggle_hazard == FALSE) ? TRUE : FALSE;
		}
		if ((pre_winker_r == FALSE) && (g_contcmd.winker_r == TRUE)) {
			toggle_winker_r = (toggle_winker_r == FALSE) ? TRUE : FALSE;
		}
		if ((pre_winker_l == FALSE) && (g_contcmd.winker_l == TRUE)) {
			toggle_winker_l = (toggle_winker_l == FALSE) ? TRUE : FALSE;
		}

		/*
		 *  ランプ状態判定
		 */
		/* ヘッドランプ状態判定 */
		if (toggle_headlamp == TRUE) {
			g_bodycmd.headlamp = TRUE;
		}
		else {
			g_bodycmd.headlamp = FALSE;
		}

		/* フォグランプ状態判定 */
		if (toggle_foglamp == TRUE) {
			g_bodycmd.foglamp = TRUE;
		}
		else {
			g_bodycmd.foglamp = FALSE;
		}

		/* ブレーキランプ状態判定 */
		if (g_contcmd.brake == TRUE) {
			g_bodycmd.brakelamp = TRUE;
		}
		else {
			g_bodycmd.brakelamp = FALSE;
		}

		/* バックランプ状態判定 */
		if (g_contcmd.speed < 0) {
			g_bodycmd.backlamp = TRUE;
		}
		else {
			g_bodycmd.backlamp = FALSE;
		}

		/*
		 *  ウィンカー状態判定
		 */
		/* ゲイン，ニュートラル調整中は方向指示 */
		if ((acklr_cnt != 0U) || (ackr_cnt != 0U) || (ackl_cnt != 0U)) {
			if (acklr_cnt != 0U) {
				acklr_cnt--;
				g_bodycmd.winker = (acklr_cnt == 0)? WINKER_OFF : WINKER_LR_ON;
			}
			if (ackr_cnt != 0U) {
				ackr_cnt--;
				g_bodycmd.winker = (ackr_cnt == 0)? WINKER_OFF : WINKER_R_ON;
			}
			if (ackl_cnt != 0U) {
				ackl_cnt--;
				g_bodycmd.winker = (ackl_cnt == 0)? WINKER_OFF : WINKER_L_ON;
			}
		}
		else {
			/* 左右点滅 */
			if ((toggle_hazard == TRUE) ||
				((toggle_winker_r == TRUE) && (toggle_winker_l == TRUE))) {
				g_bodycmd.winker = WINKER_LR_BLINK;
			}
			/* 右のみ点滅 */
			else if (toggle_winker_r == TRUE) {
				g_bodycmd.winker = WINKER_R_BLINK;
			}
			/* 左のみ点滅 */
			else if (toggle_winker_l == TRUE) {
				g_bodycmd.winker = WINKER_L_BLINK;
			}
			/* 消灯 */
			else {
				g_bodycmd.winker = WINKER_OFF;
			}
		}

		/*
		 * ブザー状態判定
		 */
		if (g_contcmd.speed < 0) {
			g_bodycmd.buzzer = BUZZER_INTERVAL_ON;
		}
		else {
			g_bodycmd.buzzer = BUZZER_OFF;
		}

		/* 前回値保存 */
		pre_headlamp = g_contcmd.headlamp;
		pre_foglamp  = g_contcmd.foglamp;
		pre_hazard   = g_contcmd.hazard;
		pre_winker_r = g_contcmd.winker_r;
		pre_winker_l = g_contcmd.winker_l;
	}

}

/*
 *  制御タスク
 */
TASK(DriveManagerTask)
{
	static	boolean executed = FALSE;
	uint8 can_rx_data[6];
	uint8 can_rx_size;

	/*  初回起動時のみ実行 */
	if (executed == FALSE) {
		syslog(LOG_INFO, "DriveManagerTask : Start!");
		executed = TRUE;
	}

	/* 駆動系の制御 */
	DrivingControl();

	/*  ボディ系の制御  */
	BodyControl();

	TerminateTask();
}
