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
 *  $Id: prc_insn.h 549 2015-12-30 10:06:17Z ertl-honda $
 */

/*
 *		プロセッサの特殊命令のインライン関数定義（V850用）
 */
#ifndef TOPPERS_PRC_INSN_H
#define TOPPERS_PRC_INSN_H

#ifndef TOPPERS_MACRO_ONLY

#include <v800_ghs.h>

/*
 *  関数のアライン単位
 *  GHSでは2バイト境界にも配置する．
 */
#define CHECK_FUNC_ALIGN	2

/*
 *  メモリ再読込が必要なことをコンパイラに伝えるためのダミー関数
 */
extern void v850_memory_changed(void);

/*
 *  メモリの再読み込みが必要なことをコンパイラに伝える
 */
#define V850_MEMORY_CHANGED	v850_memory_changed();

LOCAL_INLINE void
disable_int(void)
{
	__DI();
}

LOCAL_INLINE void
enable_int(void)
{
	__EI();
}

#ifdef __v850e2v3__

LOCAL_INLINE uint32
current_psw(void)
{
	__LDSR(31, 0);     /* Select CPU function grp */
	return(__GETSR());
}

LOCAL_INLINE void
set_psw(uint32 psw)
{
	__LDSR(31, 0);     /* Select CPU function grp */
	__SETSR(psw);
}

#elif defined(__v850e3v5__)

LOCAL_INLINE uint32
current_psw(void)
{
	return(__STSR(5, 0));
}

LOCAL_INLINE void
set_psw(uint32 psw)
{
	__LDSR(5, 0, psw);
}

/*
 *  V850E3V5用の割込みコントローラ操作ルーチン
 */
LOCAL_INLINE void
set_pmr(uint16 pmr)
{
	uint32 psw;

	/* PMR must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__LDSR(11, 2, pmr);

	set_psw(psw);

}

LOCAL_INLINE uint16
get_ispr(void)
{
	return(__STSR(10, 2));
}

LOCAL_INLINE void
clear_ispr(void)
{
	uint32 psw;

	/* ISPR must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__LDSR(13, 2, 1);   /* INTCFG = 1; ISPR を書き換え可能に */
	__LDSR(10, 2, 0);   /* ISPR = 0 */
	__LDSR(13, 2, 0);   /* INTCFG = 0; ISPR を書き換え禁止に(自動更新に) */

	set_psw(psw);
}

LOCAL_INLINE void
set_intbp(uint32 intbp)
{
	uint32 psw;

	/* INTBP must be set in di sate(PSW.ID = 1) */
	psw = current_psw();
	disable_int();

	__LDSR(4, 1, intbp);

	set_psw(psw);
}

LOCAL_INLINE uint32
current_peid(void)
{
	uint32 htcfg0_val;
	htcfg0_val = __STSR(0, 2);
	return(((htcfg0_val >> 16) & 0x03));
}

LOCAL_INLINE uint32
get_eiic(void)
{
	uint32 eiic;
	eiic = __STSR(13, 0);
	return(eiic);
}

LOCAL_INLINE boolean
acquire_lock_ldlstc(uint32 *p_lock)
{
	uint32 locked;
	/*
	 * lockedを事前にC言語で0にする方法もあるが，
	 * GHSのインラインアセンブラに不具合があり，
	 * r1にlockedもp_lockも割り付けるため，
	 * あえて両方のパスの最後で設定している．
	 */
	Asm("1: ldl.w [%1], r21 \n"
		"   cmp   r0, r21   \n"
		"   bnz   2f        \n"
		"   mov   1, r21    \n"
		"   stc.w r21, [%1] \n"
		"   cmp   r0, r21   \n"
		"   be    2f        \n"
		"   mov   1, %0     \n"
		"   br    3f        \n"
		"2:                 \n"
		"   mov   0, %0     \n"
		"3:                 \n"
		: "=r" (locked)
		: "r" (p_lock)
		: "cc", "r21");

	return(locked == 1);
}

LOCAL_INLINE void
release_lock_ldlstc(uint32 *p_lock)
{
	*p_lock = 0;
	/* スピンロック取得解放のタイトループで同期しないように一定時間待つ */
	Asm("snooze");
}

LOCAL_INLINE void
set_rbase(uint32 addr)
{
	__LDSR(2, 1, (unsigned int) addr);
}

#else /* __v850e3v5__ */
#error please define ether __v850e2v3__ or __v850e3v5__
#endif /* __v850e2v3__ */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_PRC_INSN_H */
