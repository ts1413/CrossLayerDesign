/*****************************************************/
/*                                                   */
/*  FILE        :Main.c                              */
/*                                                   */
/*****************************************************/
#include "Os.h"
#include "t_syslog.h"
#include "sysmod/serial.h"
#include "sysmod/banner.h"
#include "ModelCarControl.h"

#include "iodefine.h"
#include "blsm.h"

sint32 main(void);
void clock_init(void);
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
    //StartOS (debug_AppMode);
    StartOS (a_controller_AppMode);
  } else {
    //StartOS (debug_AppMode);
    StartOS (a_controller_AppMode);
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

    //clock_init();
    port_init();
    blsm_motor_init();
    blsm_motor_start();

  } else {
    InitSerial ();
  }

  syslog(LOG_EMERG, "StartupHook @ core%d", coreid);
}


void clock_init(void)
{
  //TAPA, PIC0，TAUJ1クロック
  do
  {
    PROTCMD1 = 0xA5;
    CKSC_IPERI2S_CTL = 0x1; //CPUCLK2
    CKSC_IPERI2S_CTL = ~0x1;
    CKSC_IPERI2S_CTL = 0x1;
  }while(PROTS1 != 0);
  while(CKSC_IPERI2S_ACT != 0x1);

#if 0
  //ADC
  do
  {
    PROTCMD0 = 0xA5;
    CKSC_AADCAS_CTL = 0x3;
    CKSC_AADCAS_CTL = ~0x3;
    CKSC_AADCAS_CTL = 0x3;
  }while(PROTS0 != 0);
  while(CKSC_AADCAS_ACT != 0x3);
#endif
}

void port_init(void)
{
  //LED出力設定
  AP1 |= 0xf000;    //AP1_12~15: H
  APM1 &= ~0xf000;  //AP1_12~15: out

  //SW入力設定
  APIBC1 |= 0x07e;  //AP1_1~6:DIP-SW, PUSH-SW 入力バッファ有効

}