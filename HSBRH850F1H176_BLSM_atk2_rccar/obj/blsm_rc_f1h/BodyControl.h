/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2014-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *		ボディ制御プログラムヘッダファイル
 */

#ifndef BODY_CONTROL_H
#define BODY_CONTROL_H

/*
 *  ポートの最大・最小値
 */
#define TLU01_PORT_MIN 0
#define TLU01_PORT_MAX 7

/*
 *  ボディ系の起動周期
 */
#define BODY_CYCLE_MS		100U

/*
 *  制御系 : 起動時のボディー系の接続テスト用設定
 *           点滅回数，点滅周期
 */
#define INIT_BODY_BLINK_COUNT		3
#define INIT_BODY_BLINK_CYCLE_MS	500

/*
 *  ウィンカーの点滅周期
 */
#define WINKER_INTERVAL_MS	500U

/*
 *  ブザー用アラームコールバックの周期
 */
#define BUZZER_CYCLE_NS		125U

/*
 *  ブザーの周期
 */
#define BUZZER_INTERVAL_MS	500U

/*
 *  ブザーの操作定義
 */
#define BUZZER_OFF			0U
#define BUZZER_ON			1U
#define BUZZER_INTERVAL_ON	2U

/*
 *  ウィンカーの操作定義
 */
#define WINKER_OFF		0U
#define WINKER_L_ON		1U
#define WINKER_R_ON		2U
#define WINKER_LR_ON	3U
#define WINKER_L_BLINK	4U
#define WINKER_R_BLINK	5U
#define WINKER_LR_BLINK	6U

/*
 *  tSetEachLEDで指示する値
 */
#define WINKER_L_NO		0
#define WINKER_R_NO		1
#define BREAK_LAMP_NO	2
#define HEAD_LAMP_NO	3
#define FOG_LAMP_NO		4
#define BACK_LAMP_NO	5

/*
 *  個別のLEDの設定用マクロ
 */
#define WINKER_L_SET(on)		tSetEachLED(WINKER_L_NO, (on))
#define WINKER_R_SET(on)		tSetEachLED(WINKER_R_NO, (on))
#define HEAD_LAMP_SET(on)		tSetEachLED(HEAD_LAMP_NO,  (on))
#define FOG_LAMP_SET(on)		tSetEachLED(FOG_LAMP_NO, (on))
#define BRAKE_LAMP_SET(on)		tSetEachLED(BREAK_LAMP_NO, (on))
#define BACK_LAMP_SET(on)		tSetEachLED(BACK_LAMP_NO, (on))

/*
 *  ボディー系への状態指示構造体
 */
typedef struct {
	boolean headlamp;
	boolean foglamp;
	boolean brakelamp;
	boolean backlamp;
	uint8   winker;
	uint8   buzzer;
}BODYCMD;

/*
 *  ボディー系への状態指示用変数
 */
extern volatile BODYCMD g_bodycmd;

/*
 *  BlinLed用の初期化(blinkled.c)
 */
extern void LedBlinkerInit(void);

/*
 *  ボディ系の初期化(BodyControl.c)
 */
extern void BodyControlInit(void);


#endif /* BODY_CONTROL_H */
