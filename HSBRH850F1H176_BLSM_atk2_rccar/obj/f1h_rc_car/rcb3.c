/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Software
 *
 *  Copyright (C) 2015 by Center for Embedded Computing Systems
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
 *  $Id: rcb3.c 122 2015-07-17 01:31:28Z ertl-honda $
 */
/*
 *		RCB3��ʸ���Ͻ���
 */
#include "Os.h"
#include "rcb3.h"

/*
 *  PS3����ȥ���Υܥ���ο�
 */
#define PS3_NUM_OF_BUTTON 14
#define PS3_NUM_OF_ANALOG 4

/*
 *  ��ʸ�ե����ޥåȤΥ�����
 */
enum rcb3_st {
	RCB3_ST_HEADER_1,
	RCB3_ST_BUTTON_1,
	RCB3_ST_BUTTON_2,
	RCB3_ST_ANALOG_1,
	RCB3_ST_ANALOG_2,
	RCB3_ST_ANALOG_3,
	RCB3_ST_ANALOG_4,
	RCB3_ST_SUM,
};

/*
 *  ������å����������ѥ����Х��ѿ�
 */
static enum rcb3_st s_rcb3_st = RCB3_ST_HEADER_1;
static int			s_rcb3_rmsg_cnt;
uint8				g_rcb3_rmsg[RCB3_MSG_LEN];

/*
 *  �ܥ����å�����Ƚ���ѥ����Х��ѿ�
 */
static const uint16 msg_button_pattern[PS3_NUM_OF_BUTTON] = {
	RCB3_MSG_BUTTON_UP,
	RCB3_MSG_BUTTON_DOWN,
	RCB3_MSG_BUTTON_LEFT,
	RCB3_MSG_BUTTON_RIGHT,
	RCB3_MSG_BUTTON_TRIANGLE,
	RCB3_MSG_BUTTON_CROSS,
	RCB3_MSG_BUTTON_NOUGHT,
	RCB3_MSG_BUTTON_SQUARE,
	RCB3_MSG_BUTTON_L1,
	RCB3_MSG_BUTTON_L2,
	RCB3_MSG_BUTTON_R1,
	RCB3_MSG_BUTTON_R2,
	RCB3_MSG_BUTTON_START,
	RCB3_MSG_BUTTON_SELECT
};

/*
 *  �ܥ�������ݻ��ѥ����Х��ѿ�
 *   ���֥�Хåե��Ȥ��������֤ȸ��߾��֤���¸����
 */
static boolean	s_ps3button[2][PS3_NUM_OF_BUTTON];
static sint8	 s_ps3analog[2][PS3_NUM_OF_ANALOG];

/*
 * �����֤ȸ��߾��֤򼨤��ݥ���
 */
boolean	*p_g_pre_ps3button;
boolean	*p_g_cur_ps3button;
sint8	*p_g_pre_ps3analog;
sint8	*p_g_cur_ps3analog;


void
rcb3_Init(void)
{
	/* Init button */
	p_g_pre_ps3button = s_ps3button[0];
	p_g_cur_ps3button = s_ps3button[1];
	p_g_pre_ps3analog = s_ps3analog[0];
	p_g_cur_ps3analog = s_ps3analog[1];
}

/*
 *  �ܥ�������2�Х��ȤȤʤäƤ��뤿��2�Х��ȥǡ����Ȥ����᤹
 */
static uint16
rcb3_GetReceivedButton()
{
	uint16 button_cmd;

	button_cmd = (g_rcb3_rmsg[RCB3_MSG_OFFSET_BUTTON_H] << 8) | g_rcb3_rmsg[RCB3_MSG_OFFSET_BUTTON_L];

	return button_cmd;
}

/*
 *  �ܥ�����֤ι���
 */
static void
rcb3_UpdateStatus(void)
{
	boolean *tmp_button;
	sint8 *tmp_analog;
	int i;
	uint16 button = rcb3_GetReceivedButton();

	/*
	 *  �����֤ȸ��߾��֤Υݥ��󥿤򹹿�
	 */
	tmp_button = p_g_pre_ps3button;
	tmp_analog = p_g_pre_ps3analog;

	p_g_pre_ps3button = p_g_cur_ps3button;
	p_g_pre_ps3analog = p_g_cur_ps3analog;

	p_g_cur_ps3button = tmp_button;
	p_g_cur_ps3analog = tmp_analog;

	/*
	 *  ���ߤΥǥ�����ܥ���ξ��֤򹹿�
	 */
	for(i = 0; i < PS3_NUM_OF_BUTTON; i++) {
		if ((button & msg_button_pattern[i])) {
			p_g_cur_ps3button[i] = TRUE;
		}
		else {
			p_g_cur_ps3button[i] = FALSE;
		}
	}

	/*
	 *  ���ߤΥ��ʥ��ܥ���ξ��֤򹹿�
	 */
	for(i = 0; i < PS3_NUM_OF_ANALOG; i++) {
		uint8 val = g_rcb3_rmsg[RCB3_MSG_OFFSET_L_ANALOG_LR + i];
		p_g_cur_ps3analog[i] = ((sint8)val - RCB3_MSG_ANALOG_NEUTRAL);
	}
}

/*
 *  RCB3���ޥ�ɤβ���
 *
 *  ��������1�Х�����˰����˻��ꤷ�ƸƤӽФ���
 *  ��ʸ�κǸ��TRUE���֤���
 */
boolean
rcb3_AddReceivedByte(uint8 c)
{
	boolean rval = FALSE;
	int i;
	uint8 sum = 0;

	switch (s_rcb3_st) {
	case RCB3_ST_HEADER_1:
		if (c == RCB3_MSG_HEADER_1) {
			s_rcb3_rmsg_cnt = 0;
			g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
			s_rcb3_st = RCB3_ST_BUTTON_1;
		}
		break;
	case RCB3_ST_BUTTON_1:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		s_rcb3_st = RCB3_ST_BUTTON_2;
		break;
	case RCB3_ST_BUTTON_2:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		s_rcb3_st = RCB3_ST_ANALOG_1;
		break;
	case RCB3_ST_ANALOG_1:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		s_rcb3_st = RCB3_ST_ANALOG_2;
		break;
	case RCB3_ST_ANALOG_2:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		s_rcb3_st = RCB3_ST_ANALOG_3;
		break;
	case RCB3_ST_ANALOG_3:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		s_rcb3_st = RCB3_ST_ANALOG_4;
		break;
	case RCB3_ST_ANALOG_4:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		s_rcb3_st = RCB3_ST_SUM;
		break;
	case RCB3_ST_SUM:
		g_rcb3_rmsg[s_rcb3_rmsg_cnt++] = c;
		for(i = 1; i < (RCB3_MSG_LEN - 1); i++) {
			sum += g_rcb3_rmsg[i];
		}
		sum &= 0x7f;
		if (c == sum) {
			rcb3_UpdateStatus();
			rval = TRUE;
		}
		else{
			rcb3_ErrorCb(sum, c, g_rcb3_rmsg);
		}
		s_rcb3_st = RCB3_ST_HEADER_1;
		break;
	}
	return rval;
}
