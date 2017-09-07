/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2012-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012-2015 by FUJI SOFT INCORPORATED, JAPAN
 *  Copyright (C) 2012-2013 by Spansion LLC, USA
 *  Copyright (C) 2012-2015 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2012-2015 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2012-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2012-2015 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2012-2015 by TOSHIBA CORPORATION, JAPAN
 *  Copyright (C) 2012-2015 by Witz Corporation
 *  Copyright (C) 2014-2015 by AISIN COMCRUISE Co., Ltd., JAPAN
 *  Copyright (C) 2014-2015 by eSOL Co.,Ltd., JAPAN
 *  Copyright (C) 2014-2015 by SCSK Corporation, JAPAN
 *  Copyright (C) 2015 by SUZUKI MOTOR CORPORATION
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
 *  $Id: spinlock.h 425 2015-12-07 08:06:19Z witz-itoyo $
 */

/*
 *		スピンロック機能
 */

#ifndef TOPPERS_SPINLOCK_H
#define TOPPERS_SPINLOCK_H

/*
 *  スピンロックIDからスピンロック管理ブロックを取り出すためのマクロ
 *   スピンロックIDは1オリジンなので-1する
 */
#define get_spncb(spnid)	(&(spncb_table[(spnid) - 1U]))

/*
 *  スピンロックIDからスピンロック初期化ブロックを取り出すためのマクロ
 *   スピンロックIDは1オリジンなので-1する
 */
#define get_spninib(spnid)	(&(spninib_table[(spnid) - 1U]))

/*
 *  スピンロック初期化ブロック
 */
typedef struct spinlock_initialization_block {
	StatusType (*susint)(void);                                 /* 割込み禁止関数(option) */
	FunctionRefType								resint;         /* 割込み許可関数(option) */
	const struct spinlock_initialization_block	*p_nextspn;     /* 次に取得するスピンロック */
} SPNINIB;

/*
 *  スピンロック管理ブロック
 */
typedef struct spinlock_control_block {
	const SPNINIB					*p_spninib;     /* 初期化ブロックへのポインタ */
	CCB								*p_ccb;         /* ロックを取得しているコア */
	void							*p_holdcb;      /* ロックを取得しているタスク，C2ISRまたはプロテクションフック */
	struct spinlock_control_block	*p_prevspncb;   /* 前に獲得したスピンロック */
#if TTYPE_SPN == NATIVE_SPN
#if TTYPE_KLOCK == G_KLOCK
#error Can not use NATIVE_SPN with G_KLOCK. Use EMULATE_SPN or C_KLOCK.
#endif /* TTYPE_KLOCK == G_KLOCK */
	LockType spn_lock;
#endif /* TTYPE_SPN == NATIVE_SPN */
} SPNCB;

/*
 *  スピンロック初期化ブロックのエリア（Os_Lcfg.c）
 */
extern const SPNINIB		spninib_table[];

/*
 *  スピンロック管理ブロックのエリア（Os_Lcfg.c）
 */
extern SPNCB				spncb_table[];

/*
 *  スピンロック機能の初期化
 */
extern void spinlock_initialize(void);

/*
 *  スピンロック数を保持する変数の宣言（Os_Lcfg.c）
 */
extern const SpinlockIdType	tnum_spinlock;  /* スピンロックの数 */

/*
 *  スピンロックをラッピングする全割込み禁止
 */
extern StatusType wrap_sus_all_int(void);

/*
 *  スピンロックをラッピングする全割込み禁止解除
 */
extern void wrap_res_all_int(void);

/*
 *  スピンロックをラッピングするOS割込み禁止
 */
extern StatusType wrap_sus_os_int(void);

/*
 *  スピンロックをラッピングするOS割込み禁止解除
 */
extern void wrap_res_os_int(void);

/*
 *  スピンロック全解放
 */
extern void force_release_spinlocks(SPNCB *p_lastspncb);

#endif /* TOPPERS_SPINLOCK_H */
