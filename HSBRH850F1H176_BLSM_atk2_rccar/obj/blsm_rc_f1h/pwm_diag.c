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
 *  $Id: tauj_pwm.c 122 2015-07-17 01:31:28Z ertl-honda $
 */
/*
 *		モータ・サーボPWM制御用ドライバ
 */
#include "Os.h"
#include "v850_gcc/rh850_f1h.h"
#include "pwm_diag.h"
#include "target_sysmod.h"

/*
 * 使用するユニット
 */
#define PWBA_UNIT        1
#define PWGA_UNIT        21

/*
 *  Port 9 Configration for PWGA21
 *   P9_3 : PWGA21O : 第1兼用
 */
#define PWM_P9_MASK			((uint16) 0x0008)
#define PWM_PIPC9_INIT		((uint16) 0x0000)
#define PWM_PMC9_INIT		((uint16) 0x0008)
#define PWM_PFCAE9_INIT		((uint16) 0x0000)
#define PWM_PFCE9_INIT		((uint16) 0x0000)
#define PWM_PFC9_INIT		((uint16) 0x0000)
#define PWM_PM9_INIT		((uint16) 0x0000)

/*
 * 出力ポートを有効に
 */
static void
init_pwm_port(void)
{
	uint16 wk;

	/*
	 * PORT9(PWM)
	 */
	/* PIPC9 設定 */
	wk = sil_reh_mem((void *) PIPC(9));
	wk &= ~PWM_P9_MASK;
	wk |= (PWM_PIPC9_INIT & PWM_P9_MASK);
	sil_wrh_mem((void *) PIPC(9), wk);

	/* PFCAE9 設定 */
	wk = sil_reh_mem((void *) PFCAE(9));
	wk &= ~PWM_P9_MASK;
	wk |= (PWM_PFCAE9_INIT & PWM_P9_MASK);
	sil_wrh_mem((void *) PFCAE(9), wk);

	/* PFCE9 設定 */
	wk = sil_reh_mem((void *) PFCE(9));
	wk &= ~PWM_P9_MASK;
	wk |= (PWM_PFCE9_INIT & PWM_P9_MASK);
	sil_wrh_mem((void *) PFCE(9), wk);

	/* PFC9 設定 */
	wk = sil_reh_mem((void *) PFC(9));
	wk &= ~PWM_P9_MASK;
	wk |= (PWM_PFC9_INIT & PWM_P9_MASK);
	sil_wrh_mem((void *) PFC(9), wk);

	/* PMC9 設定 */
	wk = sil_reh_mem((void *) PMC(9));
	wk &= ~PWM_P9_MASK;
	wk |= (PWM_PMC9_INIT & PWM_P9_MASK);
	sil_wrh_mem((void *) PMC(9), wk);

	/* PM9 設定 */
	wk = sil_reh_mem((void *) PM(9));
	wk &= ~PWM_P9_MASK;
	wk |= (PWM_PM9_INIT & PWM_P9_MASK);
	sil_wrh_mem((void *) PM(9), wk);
}

/*
 *  PWBA関連レジスタ
*/
#define PWBA_BASE		((uint32) (0xffe72800U))
#define PWBA_BRS(m)		(PWBA_BASE + (m * 0x04U))
#define PWBA_TE			(PWBA_BASE + 0x10U)
#define PWBA_TS			(PWBA_BASE + 0x14U)
#define PWBA_TT			(PWBA_BASE + 0x18U)
#define PWBA_EMU		(PWBA_BASE + 0x1CU)

/*
 *  PWGA関連レジスタ
*/
#define PWGA_BASE(n)	((uint32) (0xffe71000U + (n * 0x40U)))
#define PWGA_CSDR(n)	(PWGA_BASE(n) + 0x00U)
#define PWGA_CRDR(n)	(PWGA_BASE(n) + 0x04U)
#define PWGA_CTDR(n)	(PWGA_BASE(n) + 0x08U)
#define PWGA_RDT(n)		(PWGA_BASE(n) + 0x0CU)
#define PWGA_RSF(n)		(PWGA_BASE(n) + 0x10U)
#define PWGA_CNT(n)		(PWGA_BASE(n) + 0x14U)
#define PWGA_CTL(n)		(PWGA_BASE(n) + 0x20U)
#define PWGA_CSBR(n)	(PWGA_BASE(n) + 0x24U)
#define PWGA_CRBR(n)	(PWGA_BASE(n) + 0x28U)
#define PWGA_CTBR(n)	(PWGA_BASE(n) + 0x2CU)
#define SLPWGA(k)		((uint32) (0xFFE73000 + (k * 0x04U)))

#define US_TO_COUNT(usec)	    ((usec * (TIMER_CLOCK_HZ/1000)) / 1000)
#define M100NS_TO_COUNT(usec)	(((usec * (TIMER_CLOCK_HZ/1000)) / 1000) / 10)

/*
 *  クロックはPWMCLK1を使用
 */
#define PWGA_PWMCLK						0

#define TIMER_CLOCK_HZ		((uint32) 205479)

/*
 *  PWM-Diagの初期化
 */
void
pwm_diag_init(uint32 cycle_us, uint32 servo_duty_us, uint32 esc_duty_us)
{
	uint16 wk;

	/* IPERI2 : PWBA,PWGA  CPUCLK2 = 60MHz */

	/* PWGA出力を停止 */
	sil_wrw_mem((void *) SLPWGA(0), 0x00000000);

	/* クロックの停止 */
	sil_wrb_mem((void *) PWBA_TT, 0x0f);

	/* クロック周期を設定 CPUCKL2 / (2 * 146) = 205479Hz */
	/* PWMの周期 4096 / 205479 = 19.93ms ≒ 20ms */
	sil_wrh_mem((void *) PWBA_BRS(0), 0x092);

	/* 出力セット条件 */
	sil_wrh_mem((void *) PWGA_CSDR(PWGA_UNIT), 0x00);
	sil_wrh_mem((void *) PWGA_CRDR(PWGA_UNIT), US_TO_COUNT(servo_duty_us));
	sil_wrb_mem((void *) PWGA_CTL(PWGA_UNIT), 0x00);

	/* クロックの開始 */
	sil_wrh_mem((void *) PWBA_TS, (0x01 << PWGA_PWMCLK));

	/* PWGA出力を開始 */
	sil_wrw_mem((void *) SLPWGA(0), (0x01 << PWGA_UNIT));

	init_pwm_port();

}

/*
 *  サーボのデューティー比を変更
 */
void
pwm_diag_change_servo_duty(uint32 usec)
{
	sil_wrh_mem((void *) PWGA_CRDR(PWGA_UNIT), US_TO_COUNT(usec));
	sil_wrb_mem((void *) PWGA_RDT(PWGA_UNIT), 0x01);
}
