/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2014-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *
 *  �ܥ��եȥ������ϡ�AUTOSAR��AUTomotive Open System ARchitecture�˻�
 *  �ͤ˴�Ť��Ƥ��롥�嵭�ε����ϡ�AUTOSAR����Ū�⻺������������Τ�
 *  �Ϥʤ���AUTOSAR�ϡ�AUTOSAR���ͤ˴�Ť������եȥ�����������Ū����
 *  �Ѥ���Ԥ��Ф��ơ�AUTOSAR�ѡ��ȥʡ��ˤʤ뤳�Ȥ���Ƥ��롥
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
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
 *  ����ϤؤΥ��ޥ�ɹ����������ѿ�(Ʊ��ECU��)
 */
volatile boolean UpDateContCmd = FALSE;

/*
 *  ����Ϥؤλؼ��ѹ�¤��
 */
volatile CONTCMD g_contcmd;

/*
 *  �ܥǥ��Ϥؤξ��ֻؼ����ѿ�
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
  //LED��������
  AP1 |= 0xf000;    //AP1_12~15: H
  APM1 &= ~0xf000;  //AP1_12~15: out

  //SW��������
  APIBC1 |= 0x07e;  //AP1_1~6:DIP-SW, PUSH-SW ���ϥХåե�ͭ��

}