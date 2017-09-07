/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012-2014 by FUJISOFT INCORPORATED, JAPAN
 *  Copyright (C) 2012-2013 by Spansion LLC, USA
 *  Copyright (C) 2012-2013 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2012-2014 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2012-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2012-2014 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2012-2014 by TOSHIBA CORPORATION, JAPAN
 *  Copyright (C) 2012-2014 by Witz Corporation, JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  $Id: prc_config.c 549 2015-12-30 10:06:17Z ertl-honda $
 */

/*
 *		プロセッサ依存モジュール（V850用）
 */
#include "kernel_impl.h"
#include "mc.h"

/*
 *  CPU例外要因情報保持変数宣言
 */
#ifdef CFG_USE_PROTECTIONHOOK
uint32			v850_cpu_exp_no_table[TNUM_HWCORE];
uint32			v850_cpu_exp_pc_table[TNUM_HWCORE];
uint32			v850_cpu_exp_sp_table[TNUM_HWCORE];
#endif /* CFG_USE_PROTECTIONHOOK */

/*
 *  起動時同期用の変数
 */
volatile uint32 core_state_table[TNUM_HWCORE];

/*
 *  SILのスピンロック用変数
 */
uint32			TOPPERS_spn_var;

/* 割り込み要求マスクテーブル */
const uint16	pmr_setting_tbl[] =
{
#if TNUM_INTPRI == 16
	0xffff, /* ISR2 LEVEL -16 MASK */
	0xfffe, /* ISR2 LEVEL -15 MASK */
	0xfffc, /* ISR2 LEVEL -14 MASK */
	0xfff8, /* ISR2 LEVEL -13 MASK */
	0xfff0, /* ISR2 LEVEL -12 MASK */
	0xffe0, /* ISR2 LEVEL -11 MASK */
	0xffc0, /* ISR2 LEVEL -10 MASK */
	0xff80, /* ISR2 LEVEL -9 MASK */
	0xff00, /* ISR2 LEVEL -8 MASK */
	0xfe00, /* ISR2 LEVEL -7 MASK */
	0xfc00, /* ISR2 LEVEL -6 MASK */
	0xf800, /* ISR2 LEVEL -5 MASK */
	0xf000, /* ISR2 LEVEL -4 MASK */
	0xe000, /* ISR2 LEVEL -3 MASK */
	0xc000, /* ISR2 LEVEL -2 MASK */
	0x8000, /* ISR2 LEVEL -1 MASK */
	0x0000  /* Dummy			  */
#elif TNUM_INTPRI == 8
	0x00ff, /* ISR2 LEVEL -8 MASK */
	0x00fe, /* ISR2 LEVEL -7 MASK */
	0x00fc, /* ISR2 LEVEL -6 MASK */
	0x00f8, /* ISR2 LEVEL -5 MASK */
	0x00f0, /* ISR2 LEVEL -4 MASK */
	0x00e0, /* ISR2 LEVEL -3 MASK */
	0x00c0, /* ISR2 LEVEL -2 MASK */
	0x0080, /* ISR2 LEVEL -1 MASK */
	0x0000  /* Dummy			  */
#else
#error TNUM_INTPRI is illegal.
#endif /* TNUM_INTPRI == 16 */
};

/*
 *  起動時のハードウェア初期化
 */
void
prc_hardware_initialize(void)
{
	/*
	 *  SILのスピンロック用変数の初期化
	 */
	TOPPERS_spn_var = 0U;
}

/*
 *  リセットベクタ(prc_support.S)
 */
extern void _reset(void);

/*
 *  テーブル参照方式のINTBPアドレス(Os_Lcfg.c)
 */
extern const uint32 intbp_table[TotalNumberOfCores];

/*
 *  プロセッサ依存の初期化
 */
void
prc_initialize(void)
{
	TCCB *p_tccb = &(get_my_p_ccb()->target_ccb);

	/*
	 *  カーネル起動時は非タスクコンテキストとして動作させるため1に
	 */
	p_tccb->except_nest_cnt = 1U;

	/*
	 * 割込み優先度マスクの初期値は最低優先度
	 */
	p_tccb->current_iintpri = INT_IPM(0);

	p_tccb->trusted_hook_savedsp = 0U;

	/* テーブル参照方式のINTBPアドレス設定 */
	set_intbp((uint32) intbp_table[x_core_id()]);
}

/*
 *  プロセッサ依存の終了処理
 */
void
prc_terminate(void)
{
	/* 割り込み処理中で終了した場合を想定してISPRをクリアする */
	clear_ispr();

	/* 例外処理から終了した場合を想定してNP/EPビットをクリアする */
	set_psw(current_psw() & ~(0x0080 | 0x0040));

	/* コアが停止していることを示す */
	core_state_table[x_core_id()] = 0U;
}

/*
 *  割込み要求ラインの属性の設定
 */
void
x_config_int(InterruptNumberType intno, AttributeType intatr, PriorityType intpri, CoreIdType coreid)
{
	uint32 eic_address;

	ASSERT(VALID_INTNO(intno));

	eic_address = EIC_ADDRESS(INTNO_MASK(intno));

	/*
	 *  割込みのマスク
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	(void) x_disable_int(INTNO_MASK(intno));

	/*
	 *  割込み優先度の設定
	 */
	sil_wrh_mem((uint16 *) eic_address,
				((sil_reh_mem((uint16 *) eic_address) & ~0x0f) | INT_IPM(intpri)));

	/* テーブル参照方式 */
	sil_wrh_mem((uint16 *) eic_address,
				(sil_reh_mem((uint16 *) eic_address) | (1 << 6)));

	if ((intatr & ENABLE) != 0U) {
		/*
		 *  割込みのマスク解除
		 */
		(void) x_enable_int(INTNO_MASK(intno));
	}
}

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 *  未定義の割込みが入った場合の処理
 */
void
default_int_handler(void)
{
	target_fput_str("Unregistered Interrupt occurs.");
	ASSERT_NO_REACHED;
}
#endif /* OMIT_DEFAULT_INT_HANDLER */

/*
 *  無限ループ処理
 *  (デバッグ時の無限ループ到達確認用 / 最適化防止のためここで定義する)
 */
void
infinite_loop(void)
{
	while (1) {
	}
}

/*
 *	コア間割込みの実体定義
 */
ISR(target_ici_handler0)
{
	ici_handler_main();
}
#if TotalNumberOfCores > 1
ISR(target_ici_handler1)
{
	ici_handler_main();
}
#endif /* TotalNumberOfCores > 1 */
#if TotalNumberOfCores > 2
ISR(target_ici_handler2)
{
	ici_handler_main();
}
#endif /* TotalNumberOfCores > 2 */
#if TotalNumberOfCores > 3
ISR(target_ici_handler3)
{
	ici_handler_main();
}
#endif /* TotalNumberOfCores > 3 */

/*
 *  特定の割込み要求ラインの有効/無効を制御可能かを調べる処理
 */
boolean
target_is_int_controllable(InterruptNumberType intno)
{
	/*
	 *  V850では全ての割込み要求ラインに対して
	 *  有効/無効を制御可能であるため，常にtrueを返す
	 */
	return(TRUE);
}
