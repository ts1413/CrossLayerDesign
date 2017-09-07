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

/*
 *		�ܥǥ�����ץ����إå��ե�����
 */

#ifndef BODY_CONTROL_H
#define BODY_CONTROL_H

/*
 *  �ݡ��Ȥκ��硦�Ǿ���
 */
#define TLU01_PORT_MIN 0
#define TLU01_PORT_MAX 7

/*
 *  �ܥǥ��Ϥε�ư����
 */
#define BODY_CYCLE_MS		100U

/*
 *  ����� : ��ư���Υܥǥ����Ϥ���³�ƥ���������
 *           ���ǲ�������Ǽ���
 */
#define INIT_BODY_BLINK_COUNT		3
#define INIT_BODY_BLINK_CYCLE_MS	500

/*
 *  �����󥫡������Ǽ���
 */
#define WINKER_INTERVAL_MS	500U

/*
 *  �֥����ѥ��顼�ॳ����Хå��μ���
 */
#define BUZZER_CYCLE_NS		125U

/*
 *  �֥����μ���
 */
#define BUZZER_INTERVAL_MS	500U

/*
 *  �֥�����������
 */
#define BUZZER_OFF			0U
#define BUZZER_ON			1U
#define BUZZER_INTERVAL_ON	2U

/*
 *  �����󥫡���������
 */
#define WINKER_OFF		0U
#define WINKER_L_ON		1U
#define WINKER_R_ON		2U
#define WINKER_LR_ON	3U
#define WINKER_L_BLINK	4U
#define WINKER_R_BLINK	5U
#define WINKER_LR_BLINK	6U

/*
 *  tSetEachLED�ǻؼ�������
 */
#define WINKER_L_NO		0
#define WINKER_R_NO		1
#define BREAK_LAMP_NO	2
#define HEAD_LAMP_NO	3
#define FOG_LAMP_NO		4
#define BACK_LAMP_NO	5

/*
 *  ���̤�LED�������ѥޥ���
 */
#define WINKER_L_SET(on)		tSetEachLED(WINKER_L_NO, (on))
#define WINKER_R_SET(on)		tSetEachLED(WINKER_R_NO, (on))
#define HEAD_LAMP_SET(on)		tSetEachLED(HEAD_LAMP_NO,  (on))
#define FOG_LAMP_SET(on)		tSetEachLED(FOG_LAMP_NO, (on))
#define BRAKE_LAMP_SET(on)		tSetEachLED(BREAK_LAMP_NO, (on))
#define BACK_LAMP_SET(on)		tSetEachLED(BACK_LAMP_NO, (on))

/*
 *  �ܥǥ����Ϥؤξ��ֻؼ���¤��
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
 *  �ܥǥ����Ϥؤξ��ֻؼ����ѿ�
 */
extern volatile BODYCMD g_bodycmd;

/*
 *  BlinLed�Ѥν����(blinkled.c)
 */
extern void LedBlinkerInit(void);

/*
 *  �ܥǥ��Ϥν����(BodyControl.c)
 */
extern void BodyControlInit(void);


#endif /* BODY_CONTROL_H */
