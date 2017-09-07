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
 *		��ǥ롼��������ץ����إå��ե�����
 */

#ifndef TOPPERS_MODEL_CAR_CONTROL_H
#define TOPPERS_MODEL_CAR_CONTROL_H

/*
 *  ���̥إå��ե�����
 */
//#include "driver_if.h"
//#include "can_if.h"

/*
 *  �ѥ�ȥ��Ϣ : �˥塼�ȥ����Υ��ƥ���󥰳��٤ν����
 */
#define STEER_NEUTRAL_INIT	0

/*
 * ��®�����ƥ���󥰺���/�Ǿ���
 */
#define DRIVE_SPEED_MAX		64
#define DRIVE_SPEED_MIN		-63
#define STEER_ANGLE_MAX		64
#define STEER_ANGLE_MIN		-63

/*
 *  ���ƥ���󥰥˥塼�ȥ��Ĵ���ͤκ���/�Ǿ���
 */
#define STEER_NEUTRAL_MIN	-32
#define STEER_NEUTRAL_MAX	32

/*
 *  �ܥǥ��Ϥε�ư����
 */
#define BODY_CYCLE_MS		100U

/*
 *  DriveManager�������ε�ư����
 */
#define DRIVEMANAGER_TASK_CYCLE_MS		20

/*
 *  ����� : ��ư���Υܥǥ����Ϥ���³�ƥ���������
 *           ���ǲ�������Ǽ���
 */
#define INIT_BODY_BLINK_COUNT		3
#define INIT_BODY_BLINK_CYCLE_MS	500

/*
 *  ����� : CAN��������
 */
//#define BODYCMD_CANMSG_SEND_CYCLE_MS	100

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
 *  CAN�˴ؤ������
 */

/*
 *  CAN��Ϣ : �ܥǥ��ϥ��ޥ����CAN��å�����ID
 */
//#define BODYCMD_CANMSG_ID		16

/*
 *  CAN��Ϣ : ����ϥ��ޥ����CAN��å�����ID
 */
//#define CONTCMD_CANMSG_ID		3

/*
 *  �ƷϤ�ư�������Ʊ��ECU��ư��Ƥ����TRUE�Ȥʤ��
 */
//extern boolean IsBodyOnECU;
//extern boolean IsOperatorOnECU;
//extern boolean IsControlerOnECU;

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
 *  �ܥǥ��ϥ��ޥ�ɤ�CAN��å������Ȥ�����������
 */
//extern void SendBodyCmdCanMag(void);

/*
 *  �ܥǥ��ϥ��ޥ�ɤ�CAN��å���������Υ���ѥå�
 */
//extern void UnpackBodyCmdCanMsg(uint8 *p_rx_data);

/*
 *  ����Ϥؤλؼ���¤��
 */
typedef struct {
	sint8	angle;
	sint8	speed;
	sint8	steern;
//	sint8	gain;
	boolean brake;
	boolean headlamp;
	boolean foglamp;
	boolean winker_l;
	boolean winker_r;
	boolean hazard;
}CONTCMD;

/*
 *  ����ϤؤΥ��ƥ���󥰥˥塼�ȥ���Ϣ�λؼ�
 */
#define CONTCMD_STEERN_I	1U
#define CONTCMD_STEERN_R	2U
#define CONTCMD_STEERN_L	3U

/*
 *  ����ϤؤΥ⡼���������Ϣ�λؼ�
 */
#define CONTCMD_GAIN_I	1U
#define CONTCMD_GAIN_U	2U
#define CONTCMD_GAIN_D	3U

/*
 *  ����ϤؤΥ��ޥ�ɹ����������ѿ�(Ʊ��ECU��)
 */
extern volatile boolean UpDateContCmd;

/*
 *  ����Ϥؤλؼ����ѿ�
 */
extern volatile CONTCMD g_contcmd;

/*
 *  ����ϥ��ޥ�ɤ�CAN��å������Ȥ�������
 */
//extern void SendContCmdCanMag(void);

/*
 *  ����ϥ��ޥ�ɤ�CAN��å���������Υ���ѥå�
 */
//extern void UnpackContCmdCanMsg(uint8 *p_rx_data);

/*
 * ���ƥ������������API
 */
extern void SetSteerAngle(sint8 angle);

/*
 *  ��®������API
 */
extern void SetDriveSpeed(sint8 speed);

/*
 *  �֥졼����������API
 */
extern void SetBrake(boolean brake);

/*
 *  BlinLed�Ѥν����(blinkled.c)
 */
extern void LedBlinkerInit(void);

/*
 *  �ܥǥ��Ϥν����(BodyControl.c)
 */
extern void BodyControlInit(void);


/*
 *  ����Ϥν����(RcCar.c)
 */
extern void RcCarInit(void);


/*
 *  ���Ϥν����(RcCar.c)
 */
extern void OperationInit(void);

//tuika
extern void blsm_motor_init(void);

#endif /* TOPPERS_MODEL_CAR_CONTROL_H */
