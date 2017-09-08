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


#include "Os.h"
#include "t_syslog.h"
#include "sysmod/serial.h"
#include "sysmod/banner.h"
#include "ModelCarControl.h"

#include "iodefine.h"

sint32 main(void);
void port_init(void);

/*
 *  制御系へのコマンド更新通知用変数(同一ECU時)
 */
volatile boolean UpDateContCmd = FALSE;

/*
 *  制御系への指示用構造体
 */
volatile CONTCMD g_contcmd;

/*
 *  ボディ系への状態指示用変数
 */
volatile BODYCMD g_bodycmd;

/*
 * TOPPERS/ATK2 main function
 */
sint32
main (void)
{
  StatusType ercd;
  CoreIdType i;
  CoreIdType coreid = GetCoreID ();

  if (coreid == OS_CORE_ID_MASTER) {
    for (i = 0; i < TNUM_HWCORE; i++) {
      if (i != OS_CORE_ID_MASTER)
	StartCore (i, &ercd);
    }
    StartOS (BlsmControl_AppMode);
  } else {
    StartOS (BlsmControl_AppMode);
  }
  while(1){}
  return 0;
}

/*
 * Startup Hook
 */
void StartupHook(void)
{
  CoreIdType coreid = GetCoreID ();

  if (coreid == OS_CORE_ID_MASTER) {
    syslog_initialize ();
    syslog_msk_log (LOG_UPTO(LOG_INFO));
    InitSerial ();
    print_banner ();

    port_init();

    OperationInit();
    DriveInit();
    BodyControlInit();

  } else {
    InitSerial ();
  }

  syslog(LOG_EMERG, "StartupHook @ core%d", coreid);
}

void port_init(void)
{
  //LED出力設定
  AP1 |= 0xf000;    //AP1_12~15: H
  APM1 &= ~0xf000;  //AP1_12~15: out

  //SW入力設定
  APIBC1 |= 0x07e;  //AP1_1~6:DIP-SW, PUSH-SW 入力バッファ有効

}