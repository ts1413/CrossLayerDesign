//blsm.c

/*

Copyright (c) 2016 HOKUTODENSHI CO;LTD

This software is released under the MIT License.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


//�쓮���E�ύX�i�萔��`�F���E�̂�������ύX�j
//#define D_BLSM_POS_PHASE_CONTROL_METHOD

#include "iodefine.h"
#include "blsm.h"
#include "blsm_temp_table.h"

//�萔��`
#define D_BLSM_LED_USE

//�|�[�g��`
#define _PORT_S_0  0x0001
#define _PORT_S_1  0x0002
#define _PORT_S_2  0x0004
#define _PORT_S_3  0x0008
#define _PORT_S_4  0x0010
#define _PORT_S_5  0x0020
#define _PORT_S_6  0x0040
#define _PORT_S_7  0x0080
#define _PORT_S_8  0x0100
#define _PORT_S_9  0x0200
#define _PORT_S_10 0x0400
#define _PORT_S_11 0x0800
#define _PORT_S_12 0x1000
#define _PORT_S_13 0x2000
#define _PORT_S_14 0x4000
#define _PORT_S_15 0x8000

//�d����U��W�ɗ����ݒ�, P1_5 : U(P) -> P2_5 : W(N)
#define D_BLSM_U_W_DIRECTION_P1 _PORT_S_5
#define D_BLSM_U_W_DIRECTION_P2 _PORT_S_5

//�d����U��V�ɗ����ݒ�, P1_5 : U(P) -> P2_3 : V(N)
#define D_BLSM_U_V_DIRECTION_P1 _PORT_S_5
#define D_BLSM_U_V_DIRECTION_P2 _PORT_S_3

//�d����W��V�ɗ����ݒ�, P1_14 : W(P) -> P2_3 : V(N)
#define D_BLSM_W_V_DIRECTION_P1 _PORT_S_14
#define D_BLSM_W_V_DIRECTION_P2 _PORT_S_3

//�d����W��U�ɗ����ݒ�, P1_14 : W(P) -> P2_2 : U(N)
#define D_BLSM_W_U_DIRECTION_P1 _PORT_S_14
#define D_BLSM_W_U_DIRECTION_P2 _PORT_S_2

//�d����V��U�ɗ����ݒ�, P1_6 : V(P) -> P2_2 : U(N)
#define D_BLSM_V_U_DIRECTION_P1 _PORT_S_6
#define D_BLSM_V_U_DIRECTION_P2 _PORT_S_2

//�d����V��W�ɗ����ݒ�, P1_6 : V(P) -> P2_5 : W(N)
#define D_BLSM_V_W_DIRECTION_P1 _PORT_S_6
#define D_BLSM_V_W_DIRECTION_P2 _PORT_S_5

//�d�����~�߂�ݒ� P11_2 ~ P11_7=L
#define D_BLSM_OFF_CURRENT_P1 ~0x4060
#define D_BLSM_OFF_CURRENT_P2 ~0x002C

//�G���[�R�[�h��`
#define D_BLSM_ERR_OVER_CURRENT 0x1
#define D_BLSM_ERR_OVER_TEMP 0x2


//�ߓd�����o
#define D_BLSM_OVER_CURRENT_CONTROL 1				//1:�ߓd�����o��~���s��
#define D_BLSM_OVER_CURRENT_COUNT_THRESHOLD 8000	//100us���Ƀ`�F�b�N���s��1s�Ԃ�8000�񒴉߂����ꍇ��~

//�ߔM���o
#define D_BLSM_TEMPARATURE_CONTROL 1				//1:�ߔM���o��~���s��
#define D_BLSM_TEMPARATURE_THRESHOLD 50				//50[��]

//��]������
#define D_BLSM_ROTATION_SPEED_CONTROL 1				//1:��]��������s��
#define D_BLSM_DUTY_MAX 750							//duty�ݒ�̍ő�l(/960)
#define D_BLSM_DUTY_MIN 100							//duty�ݒ�̍ŏ��l(/960)
#define D_BLSM_RPM_MAX 12000						//�ݒ�\�ȉ�]���̍ő�l
#define D_BLSM_RPM_MIN 2000							//�ݒ�\�ȉ�]���̍ő�l
#define D_BLSM_RPM_FEEDBACK_CONST 0.1				//duty�ϓ����i10%���ڕW�ɋ߂Â��Ă����j
#define D_BLSM_RPM_FEEDBACK_THRESHOLD 0.1			//�ڕW��]����10%�ȓ��ł����duty�̕ύX���Ȃ�

//�n������
#define D_BLSM_CONTROL_DUTY0 0						//�X�e�[�W0�i��~���j��duty

#define D_BLSM_CONTROL_THRESHOLD1 100				//�X�e�[�W1�𔲂����]��[rpm]
#define D_BLSM_CONTROL_DUTY1_START (165-5)			//�X�e�[�W1�̊J�nduty(110)
#define D_BLSM_CONTROL_DUTY1_MAX 180				//�X�e�[�W1��duty�̍ő�l
#define D_BLSM_CONTROL_CYCLE1 200					//100u*200=20[ms] 1��̎n�����䎝������
#define D_BLSM_CONTROL_CYCLE1_PRE 80				//100u*80=8[ms] �v���e���V�����������鎞��

#define D_BLSM_CONTROL_THRESHOLD2 500				//�X�e�[�W2�𔲂����]��[rpm]
#define D_BLSM_CONTROL_DUTY2 225					//�X�e�[�W2��duty

#define D_BLSM_CONTROL_THRESHOLD3 1000				//�X�e�[�W3�𔲂����]��[rpm]
#define D_BLSM_CONTROL_DUTY3 300					//�X�e�[�W3��duty

//�u���[�L
#define D_BLSM_CONTROL_DUTY5 50						//�X�e�[�W5�i�u���[�L�j��duty
#define D_BLSM_CONTROL_DUTY5A 180					//�X�e�[�W5�i�u���[�L�j��duty�@�ϋɓI�ɋt�����̎��E��������Ƃ�
#define D_BLSM_BREAK_RPM_THRESH	1000				//1000rpm�ȉ��̎��͓d�������炷

//�O���[�o���ϐ���`
unsigned char g_blsm_prev_pos;

//��]��������
char g_blsm_target_direction=0;	//char rotation=-1;	//0:stop, 1:�����v���i�O�i�j, -1:���v���i��i�j//2:�u���[�L

//��]������
unsigned short g_blsm_target_rpm=5000;				//�ڕW��]��

//��]���A��]�������j�^
unsigned short g_blsm_rpm;							//���݂̉�]��
unsigned short g_blsm_rpm_hist[16];					//�ߋ��̉�]��
unsigned char g_blsm_rpm_hist_index=0;				//�ߋ��̉�]���̃C���f�b�N�X
unsigned short g_blsm_rpm_ave;						//���݂̉�]���i���ρj
char g_blsm_direction;								//���݂̉�]����

//�d���A�d�����j�^
double Vu, Vv, Vw;
unsigned short Vu_ad, Vv_ad, Vw_ad;
double Iu, Iv, Iw;
unsigned short Iu_ad, Iv_ad, Iw_ad;

//�ߓd�����o
unsigned short g_blsm_over_current_count=0;			//�d����臒l�𒴂�����
unsigned char g_blsm_over_current_check_start=0;	//���j�^�X�^�[�g

//���x���o
unsigned short g_blsm_temparature_ad_val;			//���x�Z���T�[��A/D�ϊ��l
short g_blsm_temparature;							//���x�Z���T�[�̉��x

//�G���[�R�[�h
unsigned char g_blsm_error_code;					//�G���[���N�����ꍇ������ۑ�


//����A���S���Y��
unsigned char g_blsm_control_method=0; 			//�X�e�[�W 0:stop, 1:stage1, 2:stage2, 3:stage3, 4:normal
unsigned char g_blsm_control_method_prev=0;		//1�O�̃X�e�[�W
unsigned long g_blsm_control_counter=0;			//100us�����݃J�E���^
unsigned long g_blsm_control_counter2=0;		//10ms�����݃J�E���^
unsigned char g_blsm_control_start_method=0;	//�n�����̃e���V���������������
unsigned short g_blsm_control_duty1=D_BLSM_CONTROL_DUTY1_START;	//�X�e�[�W1�̊J�nduty
unsigned short g_blsm_control_start_wait=0;		//�n�����̃v���e���V������^���鎞�Ԃ��J�E���g����ϐ�

//�f�o�b�O�i�n������j
//debug// unsigned long g_blsm_control_method_count[5] = {0,0,0,0,0};	//�e�X�e�[�W�Έډ�
//debug// unsigned long g_blsm_control_start_method_count[2]={0,0};	//�n�������s��
//debug// unsigned char g_blsm_control_start_method_last;				//�n�����ɂǂ���̕����̃e���V��������n��������

//�f�o�b�O�i�ʒu���o�񐔃J�E���g�j
//debug// unsigned long g_blsm_pos_error=0;
//debug// unsigned long g_blsm_posx[6] = {0, 0, 0, 0, 0, 0};
//debug// unsigned long g_blsm_posxr[6] = {0, 0, 0, 0, 0, 0};
//debug// unsigned long g_blsm_pos_error2=0;
//debug// unsigned long g_blsm_posx2[6] = {0, 0, 0, 0, 0, 0};
//debug// unsigned long g_blsm_posxr2[6] = {0, 0, 0, 0, 0, 0};

//�f�o�b�O�i���x�����j
//debug// short g_blsm_temparature_hist[10];					//0.1�b���̉��x�l�̗���
//debug// unsigned short g_blsm_temparature_hist_index=0;

//�f�o�b�O�i�ߓd�����j�^�j
//unsigned short g_blsm_over_current_count_max=0;


void blsm_motor_init(void)
{
	unsigned char i;

	//�o�͒[�q�ݒ�
	//P1_5  : U(P)
	//P2_2  : U(N)
	//P1_6  : V(P)
	//P2_3  : V(N)
	//P1_14 : W(P)
	//P2_5  : W(N)

	PM1    &= ~0x4060; //�|�[�g�O���[�v1�̊e�[�q�̓��o�̓��[�h�ݒ�i�܂�P1_5,6,14���o�͂ɐݒ�)
	//PPCMD1 =   0xA5;
	//PDSC1  =   0x4060;
	//PDSC1  =  ~0x4060;
	//PDSC1  =   0x4060;
	PMC1   &= ~0x4060; //�|�[�g�O���[�v1�̊e�[�q���\�t�g�E�F�AI/O���䌓�p���[�h��

	PM2    &= ~0x002C;
	//PPCMD1 =   0xA5;
	//PDSC2  =   0x002C;
	//PDSC2  =  ~0x002C;
	//PDSC2  =   0x002C;
	PMC2   &= ~0x002C;

	//�z�[���Z���T�ݒ�
	//P20_1 : Sensor1 (HS1)
	//P20_4 : Sensor2 (HS2)
	//P9_2  : Sensor3 (HS3)
	PM20  |= _PORT_S_1;
	PM20  |= _PORT_S_4;
	PM9   |= _PORT_S_2;
	PIBC20 |= _PORT_S_1;	//���̓o�b�t�@�L��
	PIBC20 |= _PORT_S_4;	//���̓o�b�t�@�L��
	PIBC9  |= _PORT_S_2;	//���̓o�b�t�@�L��

#if 0

	//�ߓd�����o
	//P20_2(INTP4) 1st/input : *INT
	PM20 |= _PORT_S_2;

	//PFCAE20 &= ~_PORT_S_2;
	PFCE20  &= ~_PORT_S_2;
	PFC20   &= ~_PORT_S_2;

	PMC20 |= _PORT_S_2;

	//�A�i���O�t�B���^�^�C�vA, �G�b�W�ݒ�
	FCLA0CTL4_INTPL = 0x2;		//fall edge

	//�����ݐݒ�
	ICP4 =  0x0044;	//�G�b�W, ���荞�݋��i�e�[�u���Q�ƕ����A�D��x4�j
#endif

	PIBC20 |= _PORT_S_2;	//���̓o�b�t�@�L���i�K�v�H�j

	//ADC�ݒ�
	//���z�`���l���̐ݒ�
#if 1
	//ADC0-7���g�p����ݒ�
	ADCA0.VCR00.UINT32 = 0;		//VC0 = ADC0I0  AD0(VU)
	ADCA0.VCR01.UINT32 = 1;		//VC1 = ADC0I1  AD1(VV)
	ADCA0.VCR02.UINT32 = 4;		//VC2 = ADC0I4  AD2(VW)
	ADCA0.VCR03.UINT32 = 5;		//VC3 = ADC0I5  AD3(IU)
	ADCA0.VCR04.UINT32 = 8;		//VC4 = ADC0I8  AD4(IV)
	ADCA0.VCR05.UINT32 = 9;		//VC5 = ADC0I9  AD5(IW)
	ADCA0.VCR06.UINT32 = 12;	//VC6 = ADC0I12 AD6�i���x�j
	ADCA0.VCR07.UINT32 = 13;	//VC7 = ADC0I13 AD003(�d���d���j
	ADCA0.SGVCSP1.UINT32 = 0;	//�X�L�����O���[�v1 �J�n���z�`�����l��
	ADCA0.SGVCEP1.UINT32 = 7;	//�X�L�����O���[�v1 �I�����z�`�����l��
#endif
	//ADC6�݂̂��g�p����ݒ�
#if 0
	ADCA0.VCR00.UINT32 = 12;	//VC0 = ADC0I12 AD6�i���x�j
	ADCA0.SGVCSP1.UINT32 = 0;	//�X�L�����O���[�v1 �J�n���z�`�����l��
	ADCA0.SGVCEP1.UINT32 = 0;	//�X�L�����O���[�v1 �I�����z�`�����l��
#endif
	ADCA0.ADCR.UINT32 = 0x0000;	//A/D���䃌�W�X�^, 12bit���[�h
	ADCA0.THER.UINT32 = 0x0000;	//�g���b�L���O&�z�[���h �f�B�X�G�[�u��

#if 0
	//TAUD0 : 60MHz/16 = 3.75MHz
//	��`���l���͓�����2��CPU�Ɋ��荞�߂Ȃ��̂ŁC�`�����l��2�ɕύX
//	//ch0,ch1 INTERVAL
	//ch0,ch2 INTERVAL
	TAUD0.TPS = 0x0004;			//����1:16
	TAUD0.CDR0 = (37500-1);		//3.75MHz -> 266.67ns 266.67ns * 37,500 = 10ms
//	TAUD0.CDR1 = (375-1);		//3.75MHz -> 266.67ns 266.67ns *    375 = 100us
	TAUD0.CDR2 = (375-1);		//3.75MHz -> 266.67ns 266.67ns *    375 = 100us

	//ch0
	TAUD0.CMOR0 = 0x0001;	//�J�n���Ɋ��荞�݂𔭐������Ȃ�
	TAUD0.CMUR0 = 0x00;

//	//ch1
//	TAUD0.CMOR1 = 0x0001;	//�J�n���Ɋ��荞�݂𔭐������Ȃ�
//	TAUD0.CMUR1 = 0x00;

	//ch2
	TAUD0.CMOR2 = 0x0001;	//�J�n���Ɋ��荞�݂𔭐������Ȃ�
	TAUD0.CMUR2 = 0x00;

	//ch0�����ݐݒ�
	ICTAUD0I0 = 0x0046;	//���荞�݋��i�e�[�u���Q�ƕ����A�D��x6�j

//	//ch1�����ݐݒ�
//	ICTAUD0I1 = 0x0045;	//���荞�݋��i�e�[�u���Q�ƕ����A�D��x5�j
//	IBDTAUD0I1 = 0x10002;	//���荞��CPU�w�� PE1

	//ch2�����ݐݒ�
	ICTAUD0I2 = 0x0045;	//���荞�݋��i�e�[�u���Q�ƕ����A�D��x5�j

#endif

	//TAUB PWM port
	//P0_8 TAUB0O2 4th out
	PM0 &= ~_PORT_S_8;

	PFCAE0 &= ~_PORT_S_8;
	PFCE0  |= _PORT_S_8;
	PFC0   |= _PORT_S_8;

	PMC0  |= _PORT_S_8;

	//P0_9 TAUB0O4 4th out
	PM0 &= ~_PORT_S_9;

	PFCAE0 &= ~_PORT_S_9;
	PFCE0  |= _PORT_S_9;
	PFC0   |= _PORT_S_9;

	PMC0  |= _PORT_S_9;

	//TAUB0 PWM
	TAUB0.TPS = 0x0000;	//����1:1

	TAUB0.CDR0 = 960;	//MASTER 60MHz 960 -> ����16us
	TAUB0.CDR4 = 961;	//SLAVE  40MHz 961 -> H����16us duty=961/(960+1)=100% QU
	TAUB0.CDR2 = 360;	//SLAVE  40MHz 360 -> H����6us duty=360/(960+1)=37.5% QL

	//TAUB0.CDR2 = 0;

	//MASTER
	TAUB0.CMOR0 = 0x0801;
	TAUB0.CMUR0 = 0x00;

	//SLAVE
	TAUB0.CMOR2 = 0x0409;
	TAUB0.CMUR2 = 0x00;

	//SLAVE
	TAUB0.CMOR4 = 0x0409;
	TAUB0.CMUR4 = 0x00;

	TAUB0.TOE = 0x14;	//ch2,4=1
	TAUB0.TOM = 0x14;	//ch2,4=1
	TAUB0.TOC = 0x0;
	TAUB0.TOL = 0x0;	//���_��

	TAUB0.RDE = 0x15;	//ch0,2,4 = 1 ��ď�����������
	TAUB0.RDM = 0x0;	//�}�X�^���J�E���g���J�n����ƈ�ď��������g���K�M���𐶐�

#if 0

	//TAUJ0 ch0,ch2 �J�E���^     ch2�͎��ԑ���p
	TAUJ0.TPS = 0x0000;		//����1:1
	TAUJ0.CMOR0 = 0x0004;	//�����J�E���g, �J�E���g���̃X�^�[�g������
	TAUJ0.CMUR0 = 0x00;
	TAUJ0.CMOR2 = 0x0004;	//�����J�E���g, �J�E���g���̃X�^�[�g������
	TAUJ0.CMUR2 = 0x00;

	//���x�̕��Ϗ�����
	for(i=0; i<16; i++) g_blsm_rpm_hist[i]=0;
	g_blsm_rpm_hist_index=0;
#endif

}

void blsm_motor_start(void)
{
	//TAUDnTS : TAUDn�`���l���X�^�[�g�g���K���W�X�^�i1�ɒ�`�����`���l���̃J�E���^��������j
	//�����0x3�Ȃ̂�2�i���ŕ\����0000000000000011��ch0,1�̃J�E���^�X�^�[�g
	//TAUJnTS,TAUBnTS�����l
	//0x1��00000001��ch0�X�^�[�g
	//0x15��0000000000010101��ch0,2,4�X�^�[�g

//	TAUD0.TS = 0x3;		//ch0,1�X�^�[�g
//	TAUD0.TS = 0x5;		//ch0,2�X�^�[�g
//	TAUJ0.TS = 0x5;		//ch0,2�X�^�[�g
	TAUB0.TS = 0x15;	//ch0,2,4�X�^�[�g
}

void blsm_motor_stop(void)
{
	unsigned char i;

	__DI();

	P11 &= ~0x00fc;		//UVW,pn=L
	P1 &= D_BLSM_OFF_CURRENT_P1;
	P2 &= D_BLSM_OFF_CURRENT_P2;

	//TAUDnTT : TAUDn�`���l���X�g�b�v�g���K���W�X�^�i1�ɒ�`�����`���l���̃J�E���^������~�j
	//TAUJnTT,TAUBnTT�����l

//	TAUD0.TT = 0x3;		//ch0,1�X�g�b�v
	TAUD0.TT = 0x5;		//ch0,2�X�g�b�v
	TAUJ0.TT = 0x5;		//ch0,2�X�g�b�v
	TAUB0.TT = 0x15;	//ch0,2,4�X�g�b�v

	g_blsm_target_direction=0;
	g_blsm_over_current_count=0;
	g_blsm_over_current_check_start=0;

	g_blsm_control_method=0;
	g_blsm_control_method_prev=0;
	g_blsm_control_counter=0;
	g_blsm_control_counter2=0;
	g_blsm_control_start_method=0;
	g_blsm_control_duty1=D_BLSM_CONTROL_DUTY1_START;
	g_blsm_control_start_wait=0;
	g_blsm_error_code=0;

	//���x�̕��Ϗ�����
	for(i=0; i<16; i++) g_blsm_rpm_hist[i]=0;
	g_blsm_rpm_hist_index=0;

}

//���ݖ��g�p
void blsm_int_p0(void)
{
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
}

//���ݖ��g�p
void blsm_int_p2(void)
{
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
}

//���ݖ��g�p
void blsm_int_p11(void)
{
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
}

//���荞�݂Ŏg�p��(���������g�͋�)
void blsm_int_p4(void)
{
	//�ߓd����~�i1��̃p���X�Ŏ~�߂�ꍇ�j
	//P1 &= D_BLSM_OFF_CURRENT_P1;
	//P2 &= D_BLSM_OFF_CURRENT_P2;
	//g_blsm_target_direction = 0;
	//g_blsm_error_code |= D_BLSM_ERR_OVER_CURRENT;

}

//���荞�݂Ŏg�p��(10ms��) ��ɃG���[���o�Ɖ�]������̍ۂ�duty��̌���
void blsm_int_taud0i0(void)	//10ms������
{
	unsigned short target_rpm;
	unsigned short new_duty;
	float rpm_rate;
	float new_rpm_rate;
	float abs_diff;
	unsigned long rpm_sum;

	unsigned char i;

	volatile unsigned long wait;

	//10ms�����݉񐔃J�E���g
	g_blsm_control_counter2++;

	//�ߓd�����o
	if(D_BLSM_OVER_CURRENT_CONTROL == 1)  //�ߓd�����o���s�����ǂ���
	{
		//g_blsm_over_current_count��blsm_int_taud0i1�i���荞��100us���j���ŉߓd�������o���ꂽ��
		//���ꂪD_BLSM_OVER_CURRENT_COUNT_THRESHOLD(8000��)�𒴂����ꍇ��~����
		if((g_blsm_over_current_check_start == 1) && (g_blsm_over_current_count >= D_BLSM_OVER_CURRENT_COUNT_THRESHOLD))
		{
			//�ߓd����~
			//�C���o�[�^�ւ̎w�߂̑������EXCEL�Q��
			P1 &= D_BLSM_OFF_CURRENT_P1;	//��A�[���AUVW OFF
			P2 &= D_BLSM_OFF_CURRENT_P2;	//���A�[���AUVW OFF
			g_blsm_target_direction = 0;	//��~
			g_blsm_error_code |= D_BLSM_ERR_OVER_CURRENT;  //�G���[������ۑ�
#ifdef D_BLSM_LED_USE  //LED�_��
			while(1)
			{
			 	AP1 &= ~0x1000;	//D2=ON
			 	for(wait=0; wait<1000000; wait++);
			 	AP1 |=  0x1000;	//D2=OFF
			 	for(wait=0; wait<1000000; wait++);
			}
#endif
		}
	}

	//debug// if((g_blsm_over_current_check_start == 1) && (g_blsm_over_current_count_max < g_blsm_over_current_count))
	//debug// {
	//debug//  	g_blsm_over_current_count_max = g_blsm_over_current_count;
	//debug// }

	//1s���ɉߓd���v���J�E���^�����Z�b�g
	if((g_blsm_control_counter2 % 100) == 0)
	{
		g_blsm_over_current_count=0;
	}



	//�ߔM���o
	ADCA0.SGSTCR1.UINT32 = 0x1;	//�X�L�����O���[�v1��A/D�ϊ����J�n
	while( (ADCA0.SGSTR.UINT32 & 0x200) == 0x200 );	//�X�L�����O���[�v1(bit9)���ϊ��I���܂ő҂�
	//g_blsm_temparature_ad_val�ɉ��x�Z���T�[����̒l����
	g_blsm_temparature_ad_val = (unsigned short)(ADCA0.DR06.UINT32 & 0x0000ffff);




	if(D_BLSM_TEMPARATURE_CONTROL == 1)  //�ߔM���o���s�����ǂ���
	{
		//blsm_tmp_table.h�Œ�`����Ă���AD�ϊ��l���牷�x�̕ϊ��\(g_blsm_ad_val_table��g_blsm_temp_table)���g���A�Z���T�[�̒l���牷�x���擾
		for(i=0; i<80; i++)
		{
			if(g_blsm_temparature_ad_val < g_blsm_ad_val_table[i]) break;
		}
		g_blsm_temparature = g_blsm_temp_table[i];

		//���x��臒l�ȏ�Ȃ��~
		if(g_blsm_temparature >= D_BLSM_TEMPARATURE_THRESHOLD)
		{
			//�ߔM��~
			//�C���o�[�^�ւ̎w�߂̑������EXCEL�Q��
			P1 &= D_BLSM_OFF_CURRENT_P1;	//��A�[���AUVW OFF
			P2 &= D_BLSM_OFF_CURRENT_P2;	//���A�[���AUVW OFF
			g_blsm_target_direction = 0;	//��~
			g_blsm_error_code |= D_BLSM_ERR_OVER_TEMP;  //�G���[������ۑ�
#ifdef D_BLSM_LED_USE  //LED�_��
			while(1)
			{
			 	AP1 &= ~0x2000;	//D3=ON
			 	for(wait=0; wait<1000000; wait++);
			 	AP1 |=  0x2000;	//D3=OFF
			 	for(wait=0; wait<1000000; wait++);
			}
		}
#endif
		//debug// if((g_blsm_control_counter2 % 10) == 0)
		//debug// {
		//debug// 	g_blsm_temparature_hist[g_blsm_temparature_hist_index++] = g_blsm_temparature;
		//debug// 	if(g_blsm_temparature_hist_index == 10) g_blsm_temparature_hist_index=0;
		//debug// }
	}

	//��]��������s���ꍇ
	//g_blsm_control_method=4�Ȃ̂ŉ�]����1000rpm�𒴂����Ƃ��̂�
	if((D_BLSM_ROTATION_SPEED_CONTROL == 1) && (g_blsm_control_method == 4))
	{
		//g_blsm_target_rpm : �ڕW��]�� main.c�ɂ�SW����̎w�߂ɂ�茈�肳���
		target_rpm = g_blsm_target_rpm;

		//��]���̍ő�l��ŏ��l�𒴂��Ă�����C��
		if(target_rpm > D_BLSM_RPM_MAX) target_rpm=D_BLSM_RPM_MAX;
		if(target_rpm < D_BLSM_RPM_MIN) target_rpm=D_BLSM_RPM_MIN;

		//���߂̉�]��16��̕��ς����߂�
		//g_blsm_rpm_hist[i]�ɂ͉ߋ�16�񕪂̑��x���ۑ�����Ă���(���荞��blsm_int_taud0i1�ɂĕۑ�)
		rpm_sum=0;
		for(i=0; i<16; i++) rpm_sum+= g_blsm_rpm_hist[i];
		g_blsm_rpm_ave = (unsigned short)(rpm_sum >> 4);

		//��]���̕��ϒl�ŉ�]���̐�����s��
		//�ڕW�l�ɑ΂��錻�݂̑��x�Ƃ̔���v�Z
		rpm_rate =  (float)target_rpm / (float)g_blsm_rpm_ave;

		if(rpm_rate > 1.0) abs_diff = rpm_rate - 1.0;
		else abs_diff = 1.0 - rpm_rate;

		//�ڕW�l�ƌ��݂̑��x��D_BLSM_RPM_FEEDBACK_THRESHOLD(10%)�ȏジ��Ă�����duty���D_BLSM_RPM_FEEDBACK_CONST(10%)���ύX����
		if( abs_diff > D_BLSM_RPM_FEEDBACK_THRESHOLD)
		{
			//�V������̌v�Z
			new_rpm_rate =  ((rpm_rate - 1.0) * D_BLSM_RPM_FEEDBACK_CONST) +1.0;

			//����V����duty����v�Z
			//TAUB0.CDR2�͌��݂�duty��
			new_duty = (unsigned short)((float)TAUB0.CDR2 * new_rpm_rate);
			if(new_duty < D_BLSM_DUTY_MIN) new_duty = D_BLSM_DUTY_MIN;
			if(new_duty > D_BLSM_DUTY_MAX) new_duty = D_BLSM_DUTY_MAX;
#ifdef D_BLSM_LED_USE  //�K�X�ALED�_��
			if(TAUB0.CDR2 == new_duty)		//duty�X�V�Ȃ�
			{
			 	AP1 |= 0xC000;	//D4, D5=OFF
			}
			else if(TAUB0.CDR2 > new_duty)	//duty down
			{
			 	AP1 &= ~0x4000;	//D4=ON
			 	AP1 |=  0x8000;	//D5=OFF
			}
			else if(TAUB0.CDR2 < new_duty)	//duty up
			{
				AP1 |=  0x4000;	//D4=OFF
				AP1 &= ~0x8000;	//D5=ON
			}
#endif
			//duty���V����duty��ύX
			TAUB0.CDR2 = new_duty;
			//�������ł��Ƃ�duty��̎w�߂��X�V�����
			TAUB0.RDT = 0x015;
		}
		else
		{
#ifdef D_BLSM_LED_USE
			//AP1 |= 0xC000;	//D4, D5=OFF
#endif
		}
	}

	//��]�����ω��i10ms�~16 = 160ms�j
	//g_blsm_rpm_hist[g_blsm_rpm_hist_index++]=g_blsm_rpm;
	//if(g_blsm_rpm_hist_index >=16) g_blsm_rpm_hist_index=0;
	//rpm_sum=0;
	//for(i=0; i<16; i++) rpm_sum+= g_blsm_rpm_hist[i];
	//g_blsm_rpm_ave = (unsigned short)(rpm_sum >> 4);

}

void blsm_int_taud0i1(void)	//100us������
{
	//100us�����݉�blsm_int_taud0i1���J�E���g
	g_blsm_control_counter++;

	blsm_rpm_calc();	//��]��,�������o g_blsm_rpm(��]��),g_blsm_direction(��]����)�ɒl���ۑ������

	//�d���E�d�����o

	ADCA0.SGSTCR1.UINT32 = 0x1;	//�X�L�����O���[�v1��A/D�ϊ����J�n
	while( (ADCA0.SGSTR.UINT32 & 0x200) == 0x200 );	//�X�L�����O���[�v1(bit9)���ϊ��I���܂ő҂�

	Vu_ad = (unsigned short)(ADCA0.DR00.UINT32 & 0x0000ffff); // U���d��
	Vv_ad = (unsigned short)(ADCA0.DR00.UINT32 >> 16); // V���d��
	Vw_ad = (unsigned short)(ADCA0.DR02.UINT32 & 0x0000ffff); // W���d��

	Iu_ad = (unsigned short)(ADCA0.DR02.UINT32 >> 16); // U���쓮�d��
	Iv_ad = (unsigned short)(ADCA0.DR04.UINT32 & 0x0000ffff); // V���쓮�d��
	Iw_ad = (unsigned short)(ADCA0.DR04.UINT32 >> 16); // W���쓮�d��

	//���̌�AD�ϊ��l��d���E�d���l�ɕϊ�����K�v������
	Vu = (double)3/2 * 5.0 * Vu_ad/4095; //U���d��
	Vv = (double)3/2 * 5.0 * Vv_ad/4095; //V���d��
	Vw = (double)3/2 * 5.0 * Vw_ad/4095; //W���d��

	Iu = 4 * 5.0 * Iu_ad/4095; //U���쓮�d��
	Iv = 4 * 5.0 * Iv_ad/4095; //V���쓮�d��
	Iw = 4 * 5.0 * Iw_ad/4095; //W���쓮�d��


	//��]�����ω��i���߂�16���ۑ��j
	g_blsm_rpm_hist[g_blsm_rpm_hist_index++]=g_blsm_rpm;
	if(g_blsm_rpm_hist_index >=16) g_blsm_rpm_hist_index=0;

#ifndef D_BLSM_PORT_CONTROL_INTERRUPT_METHOD	//���󖢒�`�����̕����L��
#ifdef D_BLSM_POS_PHASE_CONTROL_METHOD			//���󖢒�`������,else�̕������L��
	if(g_blsm_control_method == 4) blsm_motor_control_pos_phase();
#else
	//1000rpm�𒴂��Ă���ꍇ�Ablsm_motor_control()�Œʏ퐧����s��
	//���̎���duty���10ms���荞��(blsm_int_taud0i1)�̉�]������̕����Ō��肳���
	if(g_blsm_control_method == 4) blsm_motor_control();
#endif
#endif

	//�ߓd�����o
	if((PPR0 & _PORT_S_13) != _PORT_S_13)
	{
		g_blsm_over_current_count++;		//P0_13=L�̂Ƃ� ���ۂ̒�~������blsm_int_taud0i1�ōs��
	}
	else
	{
		g_blsm_over_current_check_start=1;
	}

	//���[�^�̏�Ԃɉ����Đ��䃂�[�h������
	if(g_blsm_target_direction == 0)  //��~
	{
		g_blsm_control_method=0;	//���[�h0�i��~�j
	}
	else if(g_blsm_target_direction == 2)	//�u���[�L
	{
		g_blsm_control_method=5;	//���[�h5�i�u���[�L�j
	}
	else
	{
		if(g_blsm_target_direction == g_blsm_direction)	//��]�������^�[�Q�b�g�ƈ�v���Ă����
		{
			//��]���ɉ����ă��[�h�����߂�
			if(g_blsm_rpm > D_BLSM_CONTROL_THRESHOLD3) g_blsm_control_method=4;			//1000rpm�ȏ�
			else if(g_blsm_rpm > D_BLSM_CONTROL_THRESHOLD2) g_blsm_control_method=3;	//500rpm�ȏ�
			else if(g_blsm_rpm > D_BLSM_CONTROL_THRESHOLD1) g_blsm_control_method=2;	//100rpm�ȏ�
		}
		else g_blsm_control_method=1;					//�n�����[�h�i���[�h1�j
	}

	//debug// if(g_blsm_control_method != g_blsm_control_method_prev)
	//debug// {
	//debug// 	g_blsm_control_method_count[g_blsm_control_method]++;	//���[�h�J�ڃ��j�^
	//debug// }

	//���䃂�[�h�ɉ�����duty��w�߂����]

	switch(g_blsm_control_method)
	{
		case 0:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY0;		//duty : 0
			TAUB0.RDT = 0x15;						//duty�̍X�V
			break;
		case 1:
			if(g_blsm_control_method_prev != 1) g_blsm_control_duty1 = D_BLSM_CONTROL_DUTY1_START;	//����duty : 160	16.6%
			//g_blsm_control_method_prev�̏����l��0�����n�����䂪�n�܂��1�ɂȂ�(���if���͖�����)
			//�n������̒���g_blsm_control_duty1�͏��X�ɏ㏸
			TAUB0.CDR2 = g_blsm_control_duty1;
			TAUB0.RDT = 0x15;
			break;
		case 2:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY2;		//duty : 225	23.4%
			TAUB0.RDT = 0x15;
			break;
		case 3:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY3;		//duty : 300	31.2%
			TAUB0.RDT = 0x15;
			break;
		case 5:
			TAUB0.CDR2 = D_BLSM_CONTROL_DUTY5;		//duty : 50		5.2%
			TAUB0.RDT = 0x15;
			break;
	}

	//���[�h2��3�Ȃ�ʏ퐧��
	if((g_blsm_control_method == 2) || (g_blsm_control_method == 3))
	{
		blsm_motor_control();			//duty�Œ�ň�ʂ̉�]����
	}
	//���[�h5�Ȃ�u���[�L������������ɓd���𗬂�����
	else if(g_blsm_control_method == 5)	//�u���[�L
	{
		blsm_motor_reverse_control();	//duty�Œ�Ńu���[�L�̉�]����
	}
	//���[�h1�Ȃ�n������
	else if(g_blsm_control_method == 1)	//�n������
	{

		//���]�t�]�����ɏ��Ƀe���V������������
		switch(g_blsm_control_start_method)		//�����l0
		{
			case 0:
				if((g_blsm_control_counter % D_BLSM_CONTROL_CYCLE1) == 0)		//�v������ D_BLSM_CONTROL_CYCLE1(200)�͈��̎n������p������(200*100us=20ms) �J�E���^��200�̔{���̎���������
				{
					//duty�i�d���j�𑝉�������
					g_blsm_control_duty1+=5;
					if(g_blsm_control_duty1 > D_BLSM_CONTROL_DUTY1_MAX) g_blsm_control_duty1=D_BLSM_CONTROL_DUTY1_MAX;

					//�Z���T�[�ʒu�Ɖ�]�����ɉ����āA1�X�e�b�v��̎��E��������
					blsm_motor_control_pos_phase();

					//debug// g_blsm_control_start_method_count[0]++;
					//debug// g_blsm_control_start_method_last=0;
					g_blsm_control_start_wait=D_BLSM_CONTROL_CYCLE1_PRE; 	//�e���V�����@�ւ̃J�E���^�l�ݒ� D_BLSM_CONTROL_CYCLE1_PRE(80)�̓e���V�����������鎞��(80*100us=8ms)
					g_blsm_control_start_method++;	//����0�Ȃ̂�1�ɂ���(����case1)
				}
				break;
			case 1:
				if((g_blsm_control_counter % D_BLSM_CONTROL_CYCLE1) == 0)		//�v������
				{
					//�Z���T�[�ʒu�Ɖ�]�����ɉ����āA�t�����̎��E��������
					blsm_motor_control_neg_phase();

					//debug// g_blsm_control_start_method_count[1]++;
					//debug// g_blsm_control_start_method_last=1;
					g_blsm_control_start_wait=D_BLSM_CONTROL_CYCLE1_PRE;
					g_blsm_control_start_method++;	//����1�Ȃ̂�2�ɂ���(����default)
				}
				break;

			default:
				g_blsm_control_start_method=0;	//����case0
		}

		//���̎n������20ms�̂����e���V���������������8ms���I�������c��͒ʏ퐧��
		if(g_blsm_control_start_wait == 0)
		{
			blsm_motor_control();			//�v���e���V�������Ԃ��I���Βʏ�̉�]����
		}
		//�܂�8ms�����ĂȂ�������J�E���^���Z
		else
		{
			g_blsm_control_start_wait--;
		}
	}

	g_blsm_control_method_prev = g_blsm_control_method;		//���݂̃��[�h��prev�ɕۑ�

}

//�ʒu�Z���T�[�Ɖ�]�����ɉ����Ď��E��������֐�
void blsm_motor_control(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;


	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;	//p1_reg��P1�ɑ������Ώ�A�[��ALLOFF
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;	//p2_reg��P2�ɑ������Ή��A�[��ALLOFF

	//�z�[���Z���T�[�̒l�̓ǂݎ��(EXCEL�Q��)
	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//pos�Ƀz�[���Z���T�[�̒l(1�`6)���i�[(EXCEL�Q��)
	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);


	if(g_blsm_target_direction == 0)			//��~
	{
#ifdef D_BLSM_LED_USE //LED OFF
		AP1 |= 0x1000;		//D2=OFF
		AP1 |= 0x2000;		//D3=OFF
#endif
		//�C���o�[�^��~����
		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}
	else if(g_blsm_target_direction == 1)		//�O�i
	{
#ifdef D_BLSM_LED_USE //LED2ON
		AP1 &= ~0x1000;		//D2=ON
		AP1 |= 0x2000;		//D3=OFF
#endif

		//�O�i�����Ɏ��E��������(�d���𗬂�����,�w�߂̑������EXCEL�Q��)
		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
	//��i�����Ɏ��E��������
	else if(g_blsm_target_direction == -1)	//��i
	{
#ifdef D_BLSM_LED_USE //LED3ON
		AP1 &= ~0x2000;		//D3=ON
		AP1 |= 0x1000;		//D2=OFF
#endif

		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posxr2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posxr2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posxr2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posxr2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posxr2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posxr2[5]++;
				break;
			default:
				//debug// _blsm_pos_error2++;
				break;
		}
	}
}

void blsm_motor_control_pos_phase(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;

	//�z�[���Z���T�̓ǂݎ��(blsm_motor_control�Ɠ��l)
	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(g_blsm_target_direction == 0)			//��~
	{
#ifdef D_BLSM_LED_USE
		AP1 |= 0x1000;		//D2=OFF
		AP1 |= 0x2000;		//D3=OFF
#endif

		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}
	else if(g_blsm_target_direction == 1)		//�O�i
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x1000;		//D2=ON
		AP1 |= 0x2000;		//D3=OFF
#endif
		//1�X�e�b�v��̎��E��������(blsm_motor_control�̎��Ƃ͈���E������Ă�)
		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// _blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
	else if(g_blsm_target_direction == -1)	//��i
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x2000;		//D3=ON
		AP1 |= 0x1000;		//D2=OFF
#endif

		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// _blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
}

void blsm_motor_control_neg_phase(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;

	//�z�[���Z���T�̓ǂݎ��(blsm_motor_control�Ɠ��l)
	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(g_blsm_target_direction == 0)			//��~
	{
#ifdef D_BLSM_LED_USE
		AP1 |= 0x1000;		//D2=OFF
		AP1 |= 0x2000;		//D3=OFF
#endif

		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}
	else if(g_blsm_target_direction == 1)		//�O�i
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x1000;		//D2=ON
		AP1 |= 0x2000;		//D3=OFF
#endif
		//�t�����̎��E��������(blsm_motor_control�̎��Ƃ͓d���̌������t)
		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// _blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// _blsm_pos_error2++;
				break;
		}
	}
	else if(g_blsm_target_direction == -1)	//��i
	{
#ifdef D_BLSM_LED_USE
		AP1 &= ~0x2000;		//D3=ON
		AP1 |= 0x1000;		//D2=OFF
#endif

		switch(pos)
		{
			case 5:	//pos0
				P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
				//debug// g_blsm_posx2[0]++;
				break;
			case 1:	//pos1
				P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
				//debug// g_blsm_posx2[1]++;
				break;
			case 3:	//pos2
				P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
				//debug// g_blsm_posx2[2]++;
				break;
			case 2:	//pos3
				P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
				//debug// g_blsm_posx2[3]++;
				break;
			case 6:	//pos4
				P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
				//debug// _blsm_posx2[4]++;
				break;
			case 4:	//pos5
				P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
				P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
				//debug// g_blsm_posx2[5]++;
				break;
			default:
				//debug// g_blsm_pos_error2++;
				break;
		}
	}
}

void blsm_motor_reverse_control(void)
{
	unsigned short pos;
	unsigned short p1_reg;
	unsigned short p2_reg;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;

	//�z�[���Z���T�̓ǂݎ��(blsm_motor_control�Ɠ��l)
	p1_reg = P1 & D_BLSM_OFF_CURRENT_P1;
	p2_reg = P2 & D_BLSM_OFF_CURRENT_P2;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(g_blsm_target_direction == 0)			//��~
	{
#ifdef D_BLSM_LED_USE
		AP1 |= 0x1000;		//D3=OFF
		AP1 |= 0x2000;		//D2=OFF
#endif

		P1 = p1_reg;	//UVW,pn=L
		P2 = p2_reg;	//UVW,pn=L
	}

	//5ms�ȓ��ɃZ���T�[�̑J�ڂ��ϑ��o�����ꍇ�A������̉�]���ȏ�̂Ƃ��A����]���������o�ł���Ƃ��ϋɓI�Ƀu���[�L���|����
	if((TAUJ0.CNT0 < 200000) && (g_blsm_rpm > D_BLSM_BREAK_RPM_THRESH) && (g_blsm_direction != 0) && (g_blsm_direction != 127))
	{
		TAUB0.CDR2 = D_BLSM_CONTROL_DUTY5A;		//����d���𑝂₷(180)
		TAUB0.RDT = 0x15;

		if(g_blsm_direction == -1)		//�O�i�����̉�]�̂Ƃ�(��i���Ă�Ƃ��O�i�����ɓd���𗬂��H)
		{
			switch(pos)
			{
				case 5:	//pos0
					P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
					break;
				case 1:	//pos1
					P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
					break;
				case 3:	//pos2
					P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
					break;
				case 2:	//pos3
					P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
					break;
				case 6:	//pos4
					P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
					break;
				case 4:	//pos5
					P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
					break;
				default:
					break;
			}
		}
		else if(g_blsm_direction == -1)	//��i�����̉�]�̂Ƃ�(�O�i���Ă�Ƃ���i�����ɓd���𗬂��H���Ƃ�����1�̊ԈႢ�H)
		{
			switch(pos)
			{
				case 5:	//pos0
					P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
					break;
				case 1:	//pos1
					P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;
					break;
				case 3:	//pos2
					P1 = p1_reg | D_BLSM_W_U_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_U_DIRECTION_P2;
					break;
				case 2:	//pos3
					P1 = p1_reg | D_BLSM_W_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_W_V_DIRECTION_P2;
					break;
				case 6:	//pos4
					P1 = p1_reg | D_BLSM_U_V_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_V_DIRECTION_P2;
					break;
				case 4:	//pos5
					P1 = p1_reg | D_BLSM_U_W_DIRECTION_P1;
					P2 = p2_reg | D_BLSM_U_W_DIRECTION_P2;
					break;
				default:
					break;
			}
		}
	}
	else
	{
		TAUB0.CDR2 = D_BLSM_CONTROL_DUTY5;	//�d�������炷(50)
		TAUB0.RDT = 0x15;

		P1 = p1_reg | D_BLSM_V_W_DIRECTION_P1;
		P2 = p2_reg | D_BLSM_V_W_DIRECTION_P2;
		//P1 = p1_reg | D_BLSM_V_U_DIRECTION_P1;
		//P2 = p2_reg | D_BLSM_V_U_DIRECTION_P2;

	}

}

//���݂̉�]���x�Ɖ�]�������z�[���Z���T�[�̒l����v��
void blsm_rpm_calc(void)
{

	unsigned short pos;
	unsigned short ppr20_reg;
	unsigned short ppr9_reg;
	float rpm_tmp;

	ppr20_reg = PPR20;
	ppr9_reg = PPR9;

	//		P20_1:b0                          P20_4:b1                        P9_2:b2
	pos = ((ppr20_reg & _PORT_S_1) >> 1) | ((ppr20_reg & _PORT_S_4) >> 3) | (ppr9_reg & _PORT_S_2);

	if(pos != g_blsm_prev_pos)
	{
		if((1 <= pos) && (pos <= 6))
		{
			rpm_tmp = 4.0e8 / (float)TAUJ0.CNT0;	//rpm = 1/(T*25n)*60/6 = 2.4e9/T
			g_blsm_rpm = (unsigned short)rpm_tmp;
			TAUJ0.TS = 0x1;		//TAUJ0 ch0���X�^�[�g

			g_blsm_direction=127;	//127:�s���l
			switch(pos)
			{
				case 5:	//pos0
					if(g_blsm_prev_pos == 4) g_blsm_direction=1;	//pos5
					if(g_blsm_prev_pos == 1) g_blsm_direction=-1;	//pos1
					break;
				case 1:	//pos1
					if(g_blsm_prev_pos == 5) g_blsm_direction=1;	//pos0
					if(g_blsm_prev_pos == 3) g_blsm_direction=-1;	//pos2
					break;
				case 3:	//pos2
					if(g_blsm_prev_pos == 1) g_blsm_direction=1;	//pos1
					if(g_blsm_prev_pos == 2) g_blsm_direction=-1;	//pos3
					break;
				case 2:	//pos3
					if(g_blsm_prev_pos == 3) g_blsm_direction=1;	//pos2
					if(g_blsm_prev_pos == 6) g_blsm_direction=-1;	//pos4
					break;
				case 6:	//pos4
					if(g_blsm_prev_pos == 2) g_blsm_direction=1;	//pos3
					if(g_blsm_prev_pos == 4) g_blsm_direction=-1;	//pos5
					break;
				case 4:	//pos5
					if(g_blsm_prev_pos == 6) g_blsm_direction=1;	//pos4
					if(g_blsm_prev_pos == 5) g_blsm_direction=-1;	//pos0
					break;
			}
		}
	}
	if(TAUJ0.CNT0 > 20000000)	//500ms�Z���T�[�̑J�ڂ��ϑ��ł��Ȃ��ꍇ
	{
		g_blsm_rpm=0;
		g_blsm_direction=0;
		TAUJ0.TS = 0x1;		//TAUJ0 ch0���X�^�[�g
	}

	g_blsm_prev_pos=pos;


}

