//blsm.h

#ifndef _BLSM_H_
#define _BLSM_H_

void blsm_motor_init(void);
void blsm_motor_start(void);
void blsm_motor_stop(void);
void blsm_motor_control(void);
void blsm_motor_control_pos_phase(void);
void blsm_motor_control_neg_phase(void);
void blsm_motor_reverse_control(void);
void blsm_rpm_calc(void);

void blsm_int_p0(void);
void blsm_int_p2(void);
void blsm_int_p5(void);
void blsm_int_p11(void);
void blsm_int_tauj0i0(void);
void blsm_int_tauj0i1(void);
void blsm_int_taud0i0(void);
void blsm_int_taud0i1(void);


#endif