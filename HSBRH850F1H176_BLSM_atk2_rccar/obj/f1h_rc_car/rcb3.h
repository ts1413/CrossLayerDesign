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
 *  $Id: rcb3.h 122 2015-07-17 01:31:28Z ertl-honda $
 */
/*
 *		RCB3��ʸ���Ͻ����������
 */
#ifndef _RCB3_H_
#define _RCB3_H_

/*
 *  RCB3 Message format
 */
#define RCB3_MSG_LEN 8
#define RCB3_MSG_HEADER_1 0x80

#define RCB3_MSG_OFFSET_BUTTON_H 1
#define RCB3_MSG_OFFSET_BUTTON_L 2
#define RCB3_MSG_OFFSET_L_ANALOG_LR  3
#define RCB3_MSG_OFFSET_L_ANALOG_UD  4
#define RCB3_MSG_OFFSET_R_ANALOG_LR  5
#define RCB3_MSG_OFFSET_R_ANALOG_UD  6

#define RCB3_MSG_BUTTON_UP        0x0001
#define RCB3_MSG_BUTTON_DOWN      0x0002
#define RCB3_MSG_BUTTON_RIGHT     0x0004
#define RCB3_MSG_BUTTON_LEFT      0x0008
#define RCB3_MSG_BUTTON_TRIANGLE  0x0010
#define RCB3_MSG_BUTTON_CROSS     0x0020
#define RCB3_MSG_BUTTON_NOUGHT    0x0040
#define RCB3_MSG_BUTTON_SQUARE    0x0100
#define RCB3_MSG_BUTTON_L1        0x0200
#define RCB3_MSG_BUTTON_L2        0x0400
#define RCB3_MSG_BUTTON_R1        0x0800
#define RCB3_MSG_BUTTON_R2        0x1000
#define RCB3_MSG_BUTTON_START     0x0003
#define RCB3_MSG_BUTTON_SELECT    0x000C

#define RCB3_MSG_ANALOG_MAX      127
#define RCB3_MSG_ANALOG_MIN        1
#define RCB3_MSG_ANALOG_NEUTRAL   64


/*
 * �����֤ȸ��߾��֤򼨤��ݥ���
 */
extern boolean  *p_g_pre_ps3button;
extern boolean  *p_g_cur_ps3button;
extern sint8    *p_g_pre_ps3analog;
extern sint8    *p_g_cur_ps3analog;
extern uint8     g_rcb3_rmsg[RCB3_MSG_LEN];

/*
 *  �ƥܥ�����֤��ֹ�
 */
#define NO_PS3BUTTON_UP        0
#define NO_PS3BUTTON_DOWN      1
#define NO_PS3BUTTON_LEFT      3
#define NO_PS3BUTTON_RIGHT     2
#define NO_PS3BUTTON_TRIANGLE  4
#define NO_PS3BUTTON_CROSS     5
#define NO_PS3BUTTON_NOUGHT    6
#define NO_PS3BUTTON_SQUARE    7
#define NO_PS3BUTTON_L1        8
#define NO_PS3BUTTON_L2        9
#define NO_PS3BUTTON_R1        10
#define NO_PS3BUTTON_R2        11
#define NO_PS3BUTTON_START     12
#define NO_PS3BUTTON_SELECT    13

#define NO_PS3ANALOG_L_LR  0
#define NO_PS3ANALOG_L_UD  1
#define NO_PS3ANALOG_R_LR  2
#define NO_PS3ANALOG_R_UD  3

/*
 *  ��������� 
 */
void rcb3_Init(void);

/*
 *  RCB3���ޥ�ɤβ���
 * 
 *  ��������1�Х�����˰����˻��ꤷ�ƸƤӽФ���
 *  ��ʸ�κǸ��TRUE���֤���
 */
extern boolean rcb3_AddReceivedByte(uint8 c);

/*
 *  ���顼���Υ�����Хå��ؿ��ʥ桼�����ե�����������
 */
extern void rcb3_ErrorCb(uint8 calc_sum, uint8 recv_sum, uint8 *p_rcb3_rmsg);

#endif /* _RCB3_H_ */
