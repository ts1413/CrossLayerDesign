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
#include "driver_if.h"
#include "rlin3x.h"
#include "hsbrh850f1l_device.h"
#include "taub_pwm.h"
#include "prc_sil.h"

/*
 *  IO�ݡ��ȷ�ͳ����³���줿TLU01������
 */

/*
 * TLU01-1 : P10_11
 * TLU01-2 : P10_10
 * TLU01-3 : P10_9
 * TLU01-4 : P10_8
 * TLU01-5 : P10_7
 * TLU01-6 : P10_6
 * TLU01-7 : P9_6
 * TLU01-8 : P9_5
 */

#define TLU01_P10  0x0FC0 /* P10_11, P10_10, P10_9, P10_8, P10_7, P10_6 */
#define TLU01_P9   0x0060 /* P9_6, P9_5 */

/*
 *  �ܥǥ����Ϥν����
 */
void
tInitBody(void)
{
	uint16 wk;

	/* PMC10 ���� */
	wk = sil_reh_mem((void *) PMC(10));
	wk &= ~TLU01_P10;
	sil_wrh_mem((void *) PMC(10), wk);

	/* PM10 ���� */
	wk = sil_reh_mem((void *) PM(10));
	wk &= ~TLU01_P10;
	sil_wrh_mem((void *) PM(10), wk);

	wk = sil_reh_mem((void *) P(10));
	wk &= ~TLU01_P10;
	sil_wrh_mem((void *) P(10), wk);

	/* PMC9 ���� */
	wk = sil_reh_mem((void *) PMC(9));
	wk &= ~TLU01_P9;
	sil_wrh_mem((void *) PMC(9), wk);

	/* PM9 ���� */
	wk = sil_reh_mem((void *) PM(9));
	wk &= ~TLU01_P9;
	sil_wrh_mem((void *) PM(9), wk);

	wk = sil_reh_mem((void *) P(9));
	wk &= ~TLU01_P9;
	sil_wrh_mem((void *) P(9), wk);
}

#define TLU01_PORT_MIN 0
#define TLU01_PORT_MAX 7

static const uint32 tlu01_pinfo[8] ={
	0x0A0200, /* WINKER_L   */
	0x0A0400, /* WINKER_R   */
	0x0A0080, /* BREAK_LAMP */
	0x0A0800, /* HEAD_LAMP  */
	0x0A0100, /* FOG_LAMP   */
	0x0A0040, /* BACK_LAMP  */
	0x090040, /* BUZZER     */
	0x090020, /* Reserve    */
};

void
tlu01_set(uint8 no, boolean on)
{
	uint16 wk;
	uint16 pattern;
	uint32 pinfo;
	uint8  portno;

	if (!(no <= TLU01_PORT_MAX)) {
		return;
	}

	pinfo = tlu01_pinfo[no];
	pattern =  pinfo & 0xffff;
	portno  =  (tlu01_pinfo[no] >> 16);

	wk = sil_reh_mem((void *) P(portno));
	if (on) {
		wk |=  pattern;
	}
	else {
		wk &= ~pattern;
	}
	sil_wrh_mem((void *) P(portno), wk);
}

/*
 *  ���̤�LED������
 */
void
tSetEachLED(uint8 no, boolean on)
{
	tlu01_set(no, on);
}

/*
 *  ��³LED�κ����
 */
#define MAX_LED_NO		6

/*
 *  LED������
 */
void
tSetLED(uint32 pattern)
{
	int loop;

	for(loop = 0; loop <= MAX_LED_NO; loop++){
		if((pattern & (1 << loop)) != 0){
			tlu01_set(loop, TRUE);
		}
		else{
			tlu01_set(loop, FALSE);
		}
	}
}

void
tSetBuzzer(boolean onoff){
	uint16 wk;

	wk = sil_reh_mem((void *) P(9));
	if(onoff == TRUE){
		wk |= 1 << 6;
	}
	else {
		wk &= ~(1 << 6);
	}
	sil_wrh_mem((void *) P(9), wk);
}

/*
 *  SDBDT��³��UART�ν����
 */
void
tInitSBDBTUart(void)
{
	rlin3x_init();
}

/*
 *  SDBDT��³��UART����μ���
 */
boolean
tReceiveSBDBTUart(unsigned char *p_str, uint32 len)
{
	return rlin3x_receive(p_str, len);
}

/*
 *  SDBDT��³��UART������
 */
boolean
tSendSBDBTUart(unsigned char *p_str)
{
	uint16	len = strlen(p_str);
	return rlin3x_send(p_str, len);
}

/*
 *  PWM��Ϣ�ν����
 */
void
tInitPWM(uint32 cycle_us, uint32 servo_duty_us, uint32 esc_duty_us)
{
	taub_init_pwm(cycle_us, servo_duty_us, esc_duty_us);
}

/*
 *  �����ܤΥǥ塼�ƥ�����ѹ�
 */
void
tChangeServoDuty(uint32 servo_duty_us)
{
	taub_change_servo_duty(servo_duty_us);
}

/*
 *  ESC�Υǥ塼�ƥ�����ѹ�
 */
void
tChangeESCDuty(uint32 esc_duty_us)
{
	taub_change_esc_duty(esc_duty_us);
}

/*
 *  ESC�Υǥ塼�ƥ�����ѹ�
 */
void
tChangeESCDuty100NS(uint32 esc_duty_100ns)
{
	taub_change_esc_duty_100ns(esc_duty_100ns);
}
