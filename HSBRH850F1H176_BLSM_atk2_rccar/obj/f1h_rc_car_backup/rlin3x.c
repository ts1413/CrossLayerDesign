/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Software
 *
 *  Copyright (C) 2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 *
 *  �{�\�t�g�E�F�A�́CAUTOSAR�iAUTomotive Open System ARchitecture�j�d
 *  �l�Ɋ�Â��Ă���D��L�̋����́CAUTOSAR�̒m�I���Y��������������̂�
 *  �͂Ȃ��DAUTOSAR�́CAUTOSAR�d�l�Ɋ�Â����\�t�g�E�F�A�����p�ړI�ŗ�
 *  �p����҂ɑ΂��āCAUTOSAR�p�[�g�i�[�ɂȂ邱�Ƃ����߂Ă���D
 *
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 *
 *  $Id: rlin3x.c 122 2015-07-17 01:31:28Z ertl-honda $
 */
/*
 *		�ʐM���W���[���pUART(RLIN)�h���C�o
 */
#include "Os.h"
#include "t_stdlib.h"
#include "target_sysmod.h"
#include "rlin3x.h"

#undef RLIN3x_BASE
#define RLIN3x_BASE	RLIN32_BASE

#define RLIN3xLWBR_B		0x00000001
#define RLIN3xLBRP01_H		0x00000002
#define RLIN3xLBRP0_B		0x00000002
#define RLIN3xLBRP1_B		0x00000003
#define RLIN3xLSTC_B		0x00000004
#define RLIN3xLMD_B			0x00000008
#define RLIN3xLBFC_B		0x00000009
#define RLIN3xLSC_B			0x0000000a
#define RLIN3xLWUP_B		0x0000000b
#define RLIN3xLIE_B			0x0000000c
#define RLIN3xLEDE_B		0x0000000d
#define RLIN3xLCUC_B		0x0000000e
#define RLIN3xLTRC_B		0x00000010
#define RLIN3xLMST_B		0x00000011
#define RLIN3xLST_B			0x00000012
#define RLIN3xLEST_B		0x00000013
#define RLIN3xLDFC_B		0x00000014
#define RLIN3xLIDB_B		0x00000015
#define RLIN3xLCBR_B		0x00000016
#define RLIN3xLUDB0_B		0x00000017
#define RLIN3xLDBR1_B		0x00000018
#define RLIN3xLDBR2_B		0x00000019
#define RLIN3xLDBR3_B		0x0000001a
#define RLIN3xLDBR4_B		0x0000001b
#define RLIN3xLDBR5_B		0x0000001c
#define RLIN3xLDBR6_B		0x0000001d
#define RLIN3xLDBR7_B		0x0000001e
#define RLIN3xLDBR8_B		0x0000001f
#define RLIN3xLUOER_B		0x00000020
#define RLIN3xLUOR1_B		0x00000021
#define RLIN3xLUTDR_H		0x00000024
#define RLIN3xLUTDRL_B		0x00000024
#define RLIN3xLUTDRH_B		0x00000025
#define RLIN3xLURDR_H		0x00000026
#define RLIN3xLURDRL_B		0x00000026
#define RLIN3xLURDRH_B		0x00000027
#define RLIN3xLUWTDR_H		0x00000028
#define RLIN3xLUWTDRL_B		0x00000028
#define RLIN3xLUWTDRH_B		0x00000029

/*
 *  7 : TX
 *  8 : RX
 */

/*
 *  Port 0 Configration for RLIN32
 *   P10_13 : RLIN32RX : ��2���p
 *   P10_14 : RLIN32TX : ��2���p
 */
#define RLIN32_P10_MASK			((uint16) 0x6000)
#define RLIN32_PMC10_INIT		((uint16) 0x6000)
#define RLIN32_PFCAE10_INIT		((uint16) 0x0000)
#define RLIN32_PFCE10_INIT		((uint16) 0x0000)
#define RLIN32_PFC10_INIT		((uint16) 0x6000)
#define RLIN32_PM10_INIT		((uint16) 0x2000)
#define RLIN32_PIBC10_INIT		((uint16) 0x2000)

/*
 *  RLIN32�p�|�[�g�̏�����
 */
void
rlin3x_portinit(void)
{
	uint16 wk;

	/*
	 * PORT10(RLIN32)
	 */
	/* PFCAE10 �ݒ� */
	wk = sil_reh_mem((void *) PFCAE(10));
	wk &= ~RLIN32_P10_MASK;
	wk |= (RLIN32_PFCAE10_INIT & RLIN32_P10_MASK);
	sil_wrh_mem((void *) PFCAE(10), wk);

	/* PFCE10 �ݒ� */
	wk = sil_reh_mem((void *) PFCE(10));
	wk &= ~RLIN32_P10_MASK;
	wk |= (RLIN32_PFCE10_INIT & RLIN32_P10_MASK);
	sil_wrh_mem((void *) PFCE(10), wk);

	/* PFC10 �ݒ� */
	wk = sil_reh_mem((void *) PFC(10));
	wk &= ~RLIN32_P10_MASK;
	wk |= (RLIN32_PFC10_INIT & RLIN32_P10_MASK);
	sil_wrh_mem((void *) PFC(10), wk);

	/* PMC10 �ݒ� */
	wk = sil_reh_mem((void *) PMC(10));
	wk &= ~RLIN32_P10_MASK;
	wk |= (RLIN32_PMC10_INIT & RLIN32_P10_MASK);
	sil_wrh_mem((void *) PMC(10), wk);

	/* PM10 �ݒ� */
	wk = sil_reh_mem((void *) PM(10));
	wk &= ~RLIN32_P10_MASK;
	wk |= (RLIN32_PM10_INIT & RLIN32_P10_MASK);
	sil_wrh_mem((void *) PM(10), wk);

	/* PIBC10 �ݒ� */
	wk = sil_reh_mem((void *) PIBC(10));
	wk &= ~RLIN32_P10_MASK;
	wk |= (RLIN32_PIBC10_INIT & RLIN32_P10_MASK);
	sil_wrh_mem((void *) PIBC(10), wk);
}

///*
// *   �{�[���[�g���W�X�^�ւ̐ݒ�l PCLK=40MHz, 115200bps
// */
//#define RLIN3xLWBR_VAL		0xf2
//#define RLIN3xLBRP01_VAL	10

/*
 *   �{�[���[�g���W�X�^�ւ̐ݒ�l MainOSC=16MHz, 115200bps
 */
#define RLIN3xLWBR_VAL	0xf0    /* 1/1 ���� */
#define RLIN3xLBRP01_VAL	8   /* 9(9+1)����  source clock / �v���X�P�[�� / BRP01 / 16*/


#define RxBUFF_SIZE 256
#define TxBUFF_SIZE 256

static uint8 s_rxbuff[RxBUFF_SIZE];
static uint8 s_txbuff[TxBUFF_SIZE];

static int s_rxbuff_tail;
static int s_rxbuff_head;
static int s_rxbuff_cnt;
static int s_txbuff_tail;
static int s_txbuff_head;
static int s_txbuff_cnt;
static boolean s_do_tx;

/*
 * ������
 */
void
rlin3x_init(void)
{
	s_rxbuff_tail = 0;
	s_rxbuff_head = 0;
	s_rxbuff_cnt  = 0;
	s_txbuff_tail = 0;
	s_txbuff_head = 0;
	s_txbuff_cnt  = 0;
	s_do_tx = FALSE;

	/* Uart Mode ��L��(�m�C�Y�t�B���^���L��) */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLMD_B), 0x31);

	/* �{�[���[�g�ݒ� */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLWBR_B), RLIN3xLWBR_VAL);
	sil_wrh_mem((void *) (RLIN3x_BASE + RLIN3xLBRP01_H), RLIN3xLBRP01_VAL);

	/* �G���[���o���� */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLEDE_B), 0x0d);

	/* �f�[�^ �t�H�[�}�b�g */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLBFC_B), 0x00);

	/* ���Z�b�g���� */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLCUC_B), 0x01);

	/* ���Z�b�g�����҂� */
	while (sil_reb_mem((void *) (RLIN3x_BASE + RLIN3xLMST_B)) == 0x00) {
	}

	/* ����M���싖�� */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLUOER_B), 0x03);

	/* ����M�����݋��� */
	sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLIE_B), 0x03);

	/* �|�[�g�̏����� */
	rlin3x_portinit();

}


/*
 * ��M�����f�[�^��
 */
int
rlin3x_get_rxcnt(void)
{
	return s_rxbuff_cnt;
}

/*
 *  RLIN32��M
 */
boolean
rlin3x_receive(unsigned char *data, int len)
{
	boolean result = TRUE;
	int loop;

	SuspendOSInterrupts();
	if (s_rxbuff_cnt < len) {
		result = FALSE;
	}else {
		for(loop = 0; loop < len; loop++) {
			*data++ = s_rxbuff[s_rxbuff_head++];
			s_rxbuff_head = s_rxbuff_head % RxBUFF_SIZE;
			s_rxbuff_cnt--;
		}
	}
	ResumeOSInterrupts();

	return result;
}

/*
 *  RLIN32���M
 */
boolean
rlin3x_send(const unsigned char *data, int len)
{
	int loop;
	char c;

	/* �󂫂�������΃G���[ */
	if (s_txbuff_cnt + len > TxBUFF_SIZE) {
		return FALSE;
	}

	/* �����O�o�b�t�@�ɃR�s�[ */
	for(loop = 0; loop < len; loop++){
		s_txbuff[s_txbuff_tail++] = *data++;
		s_txbuff_tail = s_txbuff_tail % TxBUFF_SIZE;
		s_txbuff_cnt++;
	}

	if (!(s_do_tx)) {
		SuspendOSInterrupts();
		/* �����O�o�b�t�@������o�����M */
		c = s_txbuff[s_txbuff_head++];
		s_txbuff_head = s_txbuff_head % TxBUFF_SIZE;
		s_txbuff_cnt--;
		s_do_tx = TRUE;
		sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLUTDRL_B), c);
		ResumeOSInterrupts();
	}

	return TRUE;
}

void
rlin3x_tx_interrupt(void)
{
	char c;

	if (s_txbuff_cnt == 0) {
		/* �o�b�t�@���� */
		s_do_tx = FALSE;
	}
	else {
		/* �o�b�t�@����f�[�^�����o���o�� */
		c = s_txbuff[s_txbuff_head++];
		s_txbuff_head = s_txbuff_head % TxBUFF_SIZE;
		s_txbuff_cnt--;
		sil_wrb_mem((void *) (RLIN3x_BASE + RLIN3xLUTDRL_B), c);
		s_do_tx = TRUE;
	}
}

ISR(RLIN3x_TX_ISR)
{
	rlin3x_tx_interrupt();
}

void
rlin3x_rx_interrupt(void)
{
	char c;

	c = sil_reb_mem((void *) (RLIN3x_BASE + RLIN3xLURDRL_B));

	if (s_rxbuff_cnt < RxBUFF_SIZE) {
		s_rxbuff[s_rxbuff_tail++] = c;
		s_rxbuff_tail = s_rxbuff_tail % RxBUFF_SIZE;
		s_rxbuff_cnt++;
	}
}

ISR(RLIN3x_RX_ISR)
{
	rlin3x_rx_interrupt();
}
