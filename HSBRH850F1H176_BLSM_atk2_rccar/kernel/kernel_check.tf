$
$  TOPPERS ATK2
$      Toyohashi Open Platform for Embedded Real-Time Systems
$      Automotive Kernel Version 2
$
$  Copyright (C) 2008-2015 by Center for Embedded Computing Systems
$              Graduate School of Information Science, Nagoya Univ., JAPAN
$  Copyright (C) 2011-2015 by FUJI SOFT INCORPORATED, JAPAN
$  Copyright (C) 2011-2013 by Spansion LLC, USA
$  Copyright (C) 2011-2015 by NEC Communication Systems, Ltd., JAPAN
$  Copyright (C) 2011-2015 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
$  Copyright (C) 2011-2014 by Renesas Electronics Corporation, JAPAN
$  Copyright (C) 2011-2015 by Sunny Giken Inc., JAPAN
$  Copyright (C) 2011-2015 by TOSHIBA CORPORATION, JAPAN
$  Copyright (C) 2011-2015 by Witz Corporation
$  Copyright (C) 2014-2015 by AISIN COMCRUISE Co., Ltd., JAPAN
$  Copyright (C) 2014-2015 by eSOL Co.,Ltd., JAPAN
$  Copyright (C) 2014-2015 by SCSK Corporation, JAPAN
$  Copyright (C) 2015 by SUZUKI MOTOR CORPORATION
$
$  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
$  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
$  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
$  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
$      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
$      スコード中に含まれていること．
$  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
$      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
$      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
$      の無保証規定を掲載すること．
$  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
$      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
$      と．
$    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
$        作権表示，この利用条件および下記の無保証規定を掲載すること．
$    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
$        報告すること．
$  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
$      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
$      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
$      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
$      免責すること．
$
$  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
$  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
$  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
$  用する者に対して，AUTOSARパートナーになることを求めている．
$
$  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
$  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
$  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
$  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
$  の責任を負わない．
$
$  $Id: kernel_check.tf 425 2015-12-07 08:06:19Z witz-itoyo $
$

$INCLUDE "cfg2_out.tf"$

$
$  スタック領域の先頭番地のチェック
$
$IF CHECK_STACK_ALIGN || CHECK_STACK_NONNULL$
$	// タスクのスタック領域の先頭番地のチェック NOS1304
	$tinib = SYMBOL("tinib_table")$
	$FOREACH tskid TSK.ID_LIST$
		$IF USE_TSKINICTXB$
			$stk = GET_STK_TSKINICTXB(tinib)$
		$ELSE$
			$stk = PEEK(tinib + offsetof_TINIB_stk, sizeof_void_ptr)$
		$END$
		$IF CHECK_STACK_ALIGN && (stk & (CHECK_STACK_ALIGN - 1)) != 0$
			$ERROR TSK.TEXT_LINE[tskid]$$FORMAT(_("%1% `%2%\' of `%3%\' is not aligned"), "stk", TSK.STK[tskid], tskid)$$END$
		$END$
		$IF CHECK_STACK_NONNULL && stk == 0$
			$ERROR TSK.TEXT_LINE[tskid]$$FORMAT(_("%1% `%2%\' of `%3%\' is null"), "stk", TSK.STK[tskid], tskid)$$END$
		$END$
		$tinib = tinib + sizeof_TINIB$
	$END$

$	// 非タスクコンテキスト用のスタック領域の先頭番地のチェック NOS1301
	$ostk_table_addr = SYMBOL("_ostk_table")$
	$FOREACH ostkid OSTK.ID_LIST$
		$ostk_addr = PEEK(ostk_table_addr, sizeof_void_ptr)$
		$IF CHECK_STACK_ALIGN && (ostk_addr & (CHECK_STACK_ALIGN - 1)) != 0$
			$ERROR OSTK.TEXT_LINE[ostkid]$$FORMAT(_("%1% `%2%\' is not aligned"), "stk", OSTK.STK[ostkid])$$END$
		$END$
		$IF CHECK_STACK_NONNULL && ostk_addr == 0$
			$ERROR OSTK.TEXT_LINE[ostkid]$$FORMAT(_("%1% `%2%\' is null"), "stk", OSTK.STK[ostkid])$$END$
		$END$
		$ostk_table_addr = ostk_table_addr + sizeof_StackType$
	$END$
$END$
