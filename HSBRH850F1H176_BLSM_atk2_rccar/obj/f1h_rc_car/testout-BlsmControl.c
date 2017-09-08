/*
 * generate "BlsmControl" code for TOPPERS/ATK2
 */

#include <string.h>

#include "Os.h"
#include "t_syslog.h"
#include "t_stdlib.h"
#include "sysmod/serial.h"
#include "sysmod/syslog.h"
#include "sysmod/banner.h"
#include "target_sysmod.h"
#include "target_serial.h"
#include "target_hw_counter.h"

#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "BlsmControl_types.h"

sint16 blsm_speed;

/*
 * memset
 */
static void *memset(void *s,int c, size_t n)
{
  const unsigned char uc = c;
  unsigned char *p = (unsigned char *)s;

  while (n-- > 0)
    *p++ = uc;

  return (s);
}

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif


/* Real-time Model Data Structure */
struct tag_RTM_BlsmControl_T {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint8_T TID[2];
    } TaskCounters;
  } Timing;
};

#include "BlsmControl_private.h"

#ifndef __GNUC__
#define __inline__
#endif /* __GNUC__ */

#ifndef CORE_TASK_DATA_PREPARE
#define CORE_TASK_DATA_PREPARE(N)	/* none */
#endif /* CORE_TASK_DATA_PREPARE */

#ifndef CORE_TASK_DATA_ATTRIBUTE
#define CORE_TASK_DATA_ATTRIBUTE(N)	/* none */
#endif /* CORE_TASK_DATA_ATTRIBUTE */

#ifndef CORE_TASK_CODE_PREPARE
#define CORE_TASK_CODE_PREPARE(N)	/* none */
#endif /* CORE_TASK_CODE_PREPARE */

#ifndef CORE_TASK_CODE_ATTRIBUTE
#define CORE_TASK_CODE_ATTRIBUTE(N)	/* none */
#endif /* CORE_TASK_CODE_ATTRIBUTE */

#ifndef TASK_DATA_PREPARE
#define TASK_DATA_PREPARE(N)	/* none */
#endif /* TASK_DATA_PREPARE */

#ifndef TASK_DATA_ATTRIBUTE
#define TASK_DATA_ATTRIBUTE(N)	/* none */
#endif /* TASK_DATA_ATTRIBUTE */

#ifndef TASK_CODE_PREPARE
#define TASK_CODE_PREPARE(N)	/* none */
#endif /* TASK_CODE_PREPARE */

#ifndef TASK_CODE_ATTRIBUTE
#define TASK_CODE_ATTRIBUTE(N)	/* none */
#endif /* TASK_CODE_ATTRIBUTE */

/*
 * Channel Macros
 */
#define CH__VEC__CONCAT(A,B)	A##B
#define CH__VEC_CONCAT(A,B)	CH__VEC__CONCAT(A,B)
#define CH__VEC_NAME(N)		CH__VEC_CONCAT(ch__vec_,N)
#define CH__VEC_ELEMENT_TYPE	unsigned long
#define CH__VEC_ELEMENT_BITS	(sizeof (CH__VEC_ELEMENT_TYPE) * 8)
#define CH__VEC_ELEMENT_SIZE(S)	\
  (((S)+CH__VEC_ELEMENT_BITS-1)/CH__VEC_ELEMENT_BITS)
#define CH__VEC_ELEMENT_0	((CH__VEC_ELEMENT_TYPE)0)
#define CH__VEC_ELEMENT_1	((CH__VEC_ELEMENT_TYPE)1)
#define CH__VEC(N,S)	\
  CH__VEC_ELEMENT_TYPE CH__VEC_NAME(N)[CH__VEC_ELEMENT_SIZE(S)] = {0,}
#define CH__VEC_INIT(N)		memset(CH__VEC_NAME(N), 0, sizeof CH__VEC_NAME(N))
#define CH__BITCHECK(N,B)	ch__bitcheck(CH__VEC_NAME(N),B)
#define CH__BITSET(N,B)		ch__bitset(CH__VEC_NAME(N),B)
#define CH__COMPLETED(N,S)	ch__completed(CH__VEC_NAME(N),S)

#define CH__TIME_LT(L,R)	((long)((L)-(R)) < (long)0)
#define CH__TIME_LE(L,R)	((long)((L)-(R)) <= (long)0)
#define CH__SEND_COUNT(N,D,R)	(((N)-(D)+((R)-1))/R)

/*
 * Task Macros for TOPPERS/ATK2
 */

#define SwCnt0_TIMER_CLOCK_HZ TIMER_CLOCK_HZ

#define ATK2_TASK_WAIT_TASKS	BlsmControl_Core0_1_0_Task
#define ATK2_WE_WAIT_TASKS	BlsmControl_CompleteEvent
#define ATK2_SE_WAIT_TASKS	BlsmControl_CompleteEvent
#define ATK2_TNAME_WAIT_TASKS	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_TASK_WAIT_TASKS	"BlsmControl_Core0_1_0_Task"

/* Task: BlsmControl_Core0_1_0_Task */
#define ATK2_TASK_BlsmControl_Core0_1_0_Task	BlsmControl_Core0_1_0_Task
#define ATK2_WE_BlsmControl_Core0_1_0_Task	BlsmControl_WakeupEvent
#define ATK2_SE_BlsmControl_Core0_1_0_Task	BlsmControl_WakeupEvent
#define ATK2_TNAME_TASK_BlsmControl_Core0_1_0_Task	"BlsmControl_Core0_1_0_Task"
#define ATK2_ALARM_BlsmControl_Core0_1_0_Task	BlsmControl_Core0_1_0_Alarm
#define ATK2_TNAME_ALARM_BlsmControl_Core0_1_0_Task	"BlsmControl_Core0_1_0_Alarm"

/* Task_0103: BlsmControl_System_rpm_culc_counter_UnitDelay */
#define ATK2_TASK_0103		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0103	ATK2_TASK_0103
#define ATK2_TASK_OUT_0103	ATK2_TASK_0103
#define ATK2_SE_IN_0103		ATK2_TASK_0103, BlsmControl_InputEvent
#define ATK2_SE_OUT_0103	ATK2_TASK_0103, BlsmControl_OutputEvent
#define ATK2_WE_IN_0103		BlsmControl_InputEvent
#define ATK2_WE_OUT_0103	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0103	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0103	ATK2_TNAME_TASK_0103
#define ATK2_TNAME_OUT_0103	ATK2_TNAME_TASK_0103
#define ATK2_TID_TASK_0103	"0103"
#define ATK2_TID_IN_0103	ATK2_TID_TASK_0103
#define ATK2_TID_OUT_0103	ATK2_TID_TASK_0103
#define ATK2_WE_TASK_0103	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0103	BlsmControl_WakeupEvent

/* Task_0127: BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1 */
#define ATK2_TASK_0127		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0127	ATK2_TASK_0127
#define ATK2_TASK_OUT_0127	ATK2_TASK_0127
#define ATK2_SE_IN_0127		ATK2_TASK_0127, BlsmControl_InputEvent
#define ATK2_SE_OUT_0127	ATK2_TASK_0127, BlsmControl_OutputEvent
#define ATK2_WE_IN_0127		BlsmControl_InputEvent
#define ATK2_WE_OUT_0127	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0127	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0127	ATK2_TNAME_TASK_0127
#define ATK2_TNAME_OUT_0127	ATK2_TNAME_TASK_0127
#define ATK2_TID_TASK_0127	"0127"
#define ATK2_TID_IN_0127	ATK2_TID_TASK_0127
#define ATK2_TID_OUT_0127	ATK2_TID_TASK_0127
#define ATK2_WE_TASK_0127	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0127	BlsmControl_WakeupEvent

/* Task_0121: BlsmControl_System_rpm_culc_if_RelationalOperator */
#define ATK2_TASK_0121		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0121	ATK2_TASK_0121
#define ATK2_TASK_OUT_0121	ATK2_TASK_0121
#define ATK2_SE_IN_0121		ATK2_TASK_0121, BlsmControl_InputEvent
#define ATK2_SE_OUT_0121	ATK2_TASK_0121, BlsmControl_OutputEvent
#define ATK2_WE_IN_0121		BlsmControl_InputEvent
#define ATK2_WE_OUT_0121	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0121	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0121	ATK2_TNAME_TASK_0121
#define ATK2_TNAME_OUT_0121	ATK2_TNAME_TASK_0121
#define ATK2_TID_TASK_0121	"0121"
#define ATK2_TID_IN_0121	ATK2_TID_TASK_0121
#define ATK2_TID_OUT_0121	ATK2_TID_TASK_0121
#define ATK2_WE_TASK_0121	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0121	BlsmControl_WakeupEvent

/* Task_0117: BlsmControl_System_rpm_culc_if_CompareToConstant_Compare */
#define ATK2_TASK_0117		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0117	ATK2_TASK_0117
#define ATK2_TASK_OUT_0117	ATK2_TASK_0117
#define ATK2_SE_IN_0117		ATK2_TASK_0117, BlsmControl_InputEvent
#define ATK2_SE_OUT_0117	ATK2_TASK_0117, BlsmControl_OutputEvent
#define ATK2_WE_IN_0117		BlsmControl_InputEvent
#define ATK2_WE_OUT_0117	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0117	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0117	ATK2_TNAME_TASK_0117
#define ATK2_TNAME_OUT_0117	ATK2_TNAME_TASK_0117
#define ATK2_TID_TASK_0117	"0117"
#define ATK2_TID_IN_0117	ATK2_TID_TASK_0117
#define ATK2_TID_OUT_0117	ATK2_TID_TASK_0117
#define ATK2_WE_TASK_0117	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0117	BlsmControl_WakeupEvent

/* Task_0120: BlsmControl_System_rpm_culc_if_LogicalOperator */
#define ATK2_TASK_0120		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0120	ATK2_TASK_0120
#define ATK2_TASK_OUT_0120	ATK2_TASK_0120
#define ATK2_SE_IN_0120		ATK2_TASK_0120, BlsmControl_InputEvent
#define ATK2_SE_OUT_0120	ATK2_TASK_0120, BlsmControl_OutputEvent
#define ATK2_WE_IN_0120		BlsmControl_InputEvent
#define ATK2_WE_OUT_0120	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0120	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0120	ATK2_TNAME_TASK_0120
#define ATK2_TNAME_OUT_0120	ATK2_TNAME_TASK_0120
#define ATK2_TID_TASK_0120	"0120"
#define ATK2_TID_IN_0120	ATK2_TID_TASK_0120
#define ATK2_TID_OUT_0120	ATK2_TID_TASK_0120
#define ATK2_WE_TASK_0120	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0120	BlsmControl_WakeupEvent

/* Task_0108: BlsmControl_System_rpm_culc_culc_div_num_Switch1 */
#define ATK2_TASK_0108		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0108	ATK2_TASK_0108
#define ATK2_TASK_OUT_0108	ATK2_TASK_0108
#define ATK2_SE_IN_0108		ATK2_TASK_0108, BlsmControl_InputEvent
#define ATK2_SE_OUT_0108	ATK2_TASK_0108, BlsmControl_OutputEvent
#define ATK2_WE_IN_0108		BlsmControl_InputEvent
#define ATK2_WE_OUT_0108	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0108	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0108	ATK2_TNAME_TASK_0108
#define ATK2_TNAME_OUT_0108	ATK2_TNAME_TASK_0108
#define ATK2_TID_TASK_0108	"0108"
#define ATK2_TID_IN_0108	ATK2_TID_TASK_0108
#define ATK2_TID_OUT_0108	ATK2_TID_TASK_0108
#define ATK2_WE_TASK_0108	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0108	BlsmControl_WakeupEvent

/* Task_0095: BlsmControl_System_rpm_culc_Gain */
#define ATK2_TASK_0095		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0095	ATK2_TASK_0095
#define ATK2_TASK_OUT_0095	ATK2_TASK_0095
#define ATK2_SE_IN_0095		ATK2_TASK_0095, BlsmControl_InputEvent
#define ATK2_SE_OUT_0095	ATK2_TASK_0095, BlsmControl_OutputEvent
#define ATK2_WE_IN_0095		BlsmControl_InputEvent
#define ATK2_WE_OUT_0095	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0095	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0095	ATK2_TNAME_TASK_0095
#define ATK2_TNAME_OUT_0095	ATK2_TNAME_TASK_0095
#define ATK2_TID_TASK_0095	"0095"
#define ATK2_TID_IN_0095	ATK2_TID_TASK_0095
#define ATK2_TID_OUT_0095	ATK2_TID_TASK_0095
#define ATK2_WE_TASK_0095	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0095	BlsmControl_WakeupEvent

/* Task_0094: BlsmControl_System_rpm_culc_Divide */
#define ATK2_TASK_0094		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0094	ATK2_TASK_0094
#define ATK2_TASK_OUT_0094	ATK2_TASK_0094
#define ATK2_SE_IN_0094		ATK2_TASK_0094, BlsmControl_InputEvent
#define ATK2_SE_OUT_0094	ATK2_TASK_0094, BlsmControl_OutputEvent
#define ATK2_WE_IN_0094		BlsmControl_InputEvent
#define ATK2_WE_OUT_0094	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0094	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0094	ATK2_TNAME_TASK_0094
#define ATK2_TNAME_OUT_0094	ATK2_TNAME_TASK_0094
#define ATK2_TID_TASK_0094	"0094"
#define ATK2_TID_IN_0094	ATK2_TID_TASK_0094
#define ATK2_TID_OUT_0094	ATK2_TID_TASK_0094
#define ATK2_WE_TASK_0094	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0094	BlsmControl_WakeupEvent

/* Task_0096: BlsmControl_System_rpm_culc_Switch */
#define ATK2_TASK_0096		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0096	ATK2_TASK_0096
#define ATK2_TASK_OUT_0096	ATK2_TASK_0096
#define ATK2_SE_IN_0096		ATK2_TASK_0096, BlsmControl_InputEvent
#define ATK2_SE_OUT_0096	ATK2_TASK_0096, BlsmControl_OutputEvent
#define ATK2_WE_IN_0096		BlsmControl_InputEvent
#define ATK2_WE_OUT_0096	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0096	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0096	ATK2_TNAME_TASK_0096
#define ATK2_TNAME_OUT_0096	ATK2_TNAME_TASK_0096
#define ATK2_TID_TASK_0096	"0096"
#define ATK2_TID_IN_0096	ATK2_TID_TASK_0096
#define ATK2_TID_OUT_0096	ATK2_TID_TASK_0096
#define ATK2_WE_TASK_0096	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0096	BlsmControl_WakeupEvent

/* Task_0075: BlsmControl_System_duty_culc_Zero_OrderHold1 */
#define ATK2_TASK_0075		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0075	ATK2_TASK_0075
#define ATK2_TASK_OUT_0075	ATK2_TASK_0075
#define ATK2_SE_IN_0075		ATK2_TASK_0075, BlsmControl_InputEvent
#define ATK2_SE_OUT_0075	ATK2_TASK_0075, BlsmControl_OutputEvent
#define ATK2_WE_IN_0075		BlsmControl_InputEvent
#define ATK2_WE_OUT_0075	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0075	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0075	ATK2_TNAME_TASK_0075
#define ATK2_TNAME_OUT_0075	ATK2_TNAME_TASK_0075
#define ATK2_TID_TASK_0075	"0075"
#define ATK2_TID_IN_0075	ATK2_TID_TASK_0075
#define ATK2_TID_OUT_0075	ATK2_TID_TASK_0075
#define ATK2_WE_TASK_0075	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0075	BlsmControl_WakeupEvent

/* Task_0066: BlsmControl_System_duty_culc_Switch16 */
#define ATK2_TASK_0066		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0066	ATK2_TASK_0066
#define ATK2_TASK_OUT_0066	ATK2_TASK_0066
#define ATK2_SE_IN_0066		ATK2_TASK_0066, BlsmControl_InputEvent
#define ATK2_SE_OUT_0066	ATK2_TASK_0066, BlsmControl_OutputEvent
#define ATK2_WE_IN_0066		BlsmControl_InputEvent
#define ATK2_WE_OUT_0066	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0066	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0066	ATK2_TNAME_TASK_0066
#define ATK2_TNAME_OUT_0066	ATK2_TNAME_TASK_0066
#define ATK2_TID_TASK_0066	"0066"
#define ATK2_TID_IN_0066	ATK2_TID_TASK_0066
#define ATK2_TID_OUT_0066	ATK2_TID_TASK_0066
#define ATK2_WE_TASK_0066	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0066	BlsmControl_WakeupEvent

/* Task_0067: BlsmControl_System_duty_culc_Switch17 */
#define ATK2_TASK_0067		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0067	ATK2_TASK_0067
#define ATK2_TASK_OUT_0067	ATK2_TASK_0067
#define ATK2_SE_IN_0067		ATK2_TASK_0067, BlsmControl_InputEvent
#define ATK2_SE_OUT_0067	ATK2_TASK_0067, BlsmControl_OutputEvent
#define ATK2_WE_IN_0067		BlsmControl_InputEvent
#define ATK2_WE_OUT_0067	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0067	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0067	ATK2_TNAME_TASK_0067
#define ATK2_TNAME_OUT_0067	ATK2_TNAME_TASK_0067
#define ATK2_TID_TASK_0067	"0067"
#define ATK2_TID_IN_0067	ATK2_TID_TASK_0067
#define ATK2_TID_OUT_0067	ATK2_TID_TASK_0067
#define ATK2_WE_TASK_0067	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0067	BlsmControl_WakeupEvent

/* Task_0071: BlsmControl_System_duty_culc_Switch5 */
#define ATK2_TASK_0071		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0071	ATK2_TASK_0071
#define ATK2_TASK_OUT_0071	ATK2_TASK_0071
#define ATK2_SE_IN_0071		ATK2_TASK_0071, BlsmControl_InputEvent
#define ATK2_SE_OUT_0071	ATK2_TASK_0071, BlsmControl_OutputEvent
#define ATK2_WE_IN_0071		BlsmControl_InputEvent
#define ATK2_WE_OUT_0071	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0071	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0071	ATK2_TNAME_TASK_0071
#define ATK2_TNAME_OUT_0071	ATK2_TNAME_TASK_0071
#define ATK2_TID_TASK_0071	"0071"
#define ATK2_TID_IN_0071	ATK2_TID_TASK_0071
#define ATK2_TID_OUT_0071	ATK2_TID_TASK_0071
#define ATK2_WE_TASK_0071	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0071	BlsmControl_WakeupEvent

/* Task_0049: BlsmControl_System_duty_culc_Subsystem1_UnitDelay12 */
#define ATK2_TASK_0049		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0049	ATK2_TASK_0049
#define ATK2_TASK_OUT_0049	ATK2_TASK_0049
#define ATK2_SE_IN_0049		ATK2_TASK_0049, BlsmControl_InputEvent
#define ATK2_SE_OUT_0049	ATK2_TASK_0049, BlsmControl_OutputEvent
#define ATK2_WE_IN_0049		BlsmControl_InputEvent
#define ATK2_WE_OUT_0049	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0049	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0049	ATK2_TNAME_TASK_0049
#define ATK2_TNAME_OUT_0049	ATK2_TNAME_TASK_0049
#define ATK2_TID_TASK_0049	"0049"
#define ATK2_TID_IN_0049	ATK2_TID_TASK_0049
#define ATK2_TID_OUT_0049	ATK2_TID_TASK_0049
#define ATK2_WE_TASK_0049	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0049	BlsmControl_WakeupEvent

/* Task_0051: BlsmControl_System_duty_culc_Subsystem1_UnitDelay14 */
#define ATK2_TASK_0051		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0051	ATK2_TASK_0051
#define ATK2_TASK_OUT_0051	ATK2_TASK_0051
#define ATK2_SE_IN_0051		ATK2_TASK_0051, BlsmControl_InputEvent
#define ATK2_SE_OUT_0051	ATK2_TASK_0051, BlsmControl_OutputEvent
#define ATK2_WE_IN_0051		BlsmControl_InputEvent
#define ATK2_WE_OUT_0051	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0051	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0051	ATK2_TNAME_TASK_0051
#define ATK2_TNAME_OUT_0051	ATK2_TNAME_TASK_0051
#define ATK2_TID_TASK_0051	"0051"
#define ATK2_TID_IN_0051	ATK2_TID_TASK_0051
#define ATK2_TID_OUT_0051	ATK2_TID_TASK_0051
#define ATK2_WE_TASK_0051	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0051	BlsmControl_WakeupEvent

/* Task_0048: BlsmControl_System_duty_culc_Subsystem1_UnitDelay11 */
#define ATK2_TASK_0048		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0048	ATK2_TASK_0048
#define ATK2_TASK_OUT_0048	ATK2_TASK_0048
#define ATK2_SE_IN_0048		ATK2_TASK_0048, BlsmControl_InputEvent
#define ATK2_SE_OUT_0048	ATK2_TASK_0048, BlsmControl_OutputEvent
#define ATK2_WE_IN_0048		BlsmControl_InputEvent
#define ATK2_WE_OUT_0048	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0048	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0048	ATK2_TNAME_TASK_0048
#define ATK2_TNAME_OUT_0048	ATK2_TNAME_TASK_0048
#define ATK2_TID_TASK_0048	"0048"
#define ATK2_TID_IN_0048	ATK2_TID_TASK_0048
#define ATK2_TID_OUT_0048	ATK2_TID_TASK_0048
#define ATK2_WE_TASK_0048	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0048	BlsmControl_WakeupEvent

/* Task_0047: BlsmControl_System_duty_culc_Subsystem1_UnitDelay10 */
#define ATK2_TASK_0047		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0047	ATK2_TASK_0047
#define ATK2_TASK_OUT_0047	ATK2_TASK_0047
#define ATK2_SE_IN_0047		ATK2_TASK_0047, BlsmControl_InputEvent
#define ATK2_SE_OUT_0047	ATK2_TASK_0047, BlsmControl_OutputEvent
#define ATK2_WE_IN_0047		BlsmControl_InputEvent
#define ATK2_WE_OUT_0047	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0047	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0047	ATK2_TNAME_TASK_0047
#define ATK2_TNAME_OUT_0047	ATK2_TNAME_TASK_0047
#define ATK2_TID_TASK_0047	"0047"
#define ATK2_TID_IN_0047	ATK2_TID_TASK_0047
#define ATK2_TID_OUT_0047	ATK2_TID_TASK_0047
#define ATK2_WE_TASK_0047	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0047	BlsmControl_WakeupEvent

/* Task_0059: BlsmControl_System_duty_culc_Subsystem1_UnitDelay9 */
#define ATK2_TASK_0059		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0059	ATK2_TASK_0059
#define ATK2_TASK_OUT_0059	ATK2_TASK_0059
#define ATK2_SE_IN_0059		ATK2_TASK_0059, BlsmControl_InputEvent
#define ATK2_SE_OUT_0059	ATK2_TASK_0059, BlsmControl_OutputEvent
#define ATK2_WE_IN_0059		BlsmControl_InputEvent
#define ATK2_WE_OUT_0059	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0059	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0059	ATK2_TNAME_TASK_0059
#define ATK2_TNAME_OUT_0059	ATK2_TNAME_TASK_0059
#define ATK2_TID_TASK_0059	"0059"
#define ATK2_TID_IN_0059	ATK2_TID_TASK_0059
#define ATK2_TID_OUT_0059	ATK2_TID_TASK_0059
#define ATK2_WE_TASK_0059	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0059	BlsmControl_WakeupEvent

/* Task_0058: BlsmControl_System_duty_culc_Subsystem1_UnitDelay8 */
#define ATK2_TASK_0058		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0058	ATK2_TASK_0058
#define ATK2_TASK_OUT_0058	ATK2_TASK_0058
#define ATK2_SE_IN_0058		ATK2_TASK_0058, BlsmControl_InputEvent
#define ATK2_SE_OUT_0058	ATK2_TASK_0058, BlsmControl_OutputEvent
#define ATK2_WE_IN_0058		BlsmControl_InputEvent
#define ATK2_WE_OUT_0058	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0058	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0058	ATK2_TNAME_TASK_0058
#define ATK2_TNAME_OUT_0058	ATK2_TNAME_TASK_0058
#define ATK2_TID_TASK_0058	"0058"
#define ATK2_TID_IN_0058	ATK2_TID_TASK_0058
#define ATK2_TID_OUT_0058	ATK2_TID_TASK_0058
#define ATK2_WE_TASK_0058	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0058	BlsmControl_WakeupEvent

/* Task_0057: BlsmControl_System_duty_culc_Subsystem1_UnitDelay7 */
#define ATK2_TASK_0057		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0057	ATK2_TASK_0057
#define ATK2_TASK_OUT_0057	ATK2_TASK_0057
#define ATK2_SE_IN_0057		ATK2_TASK_0057, BlsmControl_InputEvent
#define ATK2_SE_OUT_0057	ATK2_TASK_0057, BlsmControl_OutputEvent
#define ATK2_WE_IN_0057		BlsmControl_InputEvent
#define ATK2_WE_OUT_0057	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0057	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0057	ATK2_TNAME_TASK_0057
#define ATK2_TNAME_OUT_0057	ATK2_TNAME_TASK_0057
#define ATK2_TID_TASK_0057	"0057"
#define ATK2_TID_IN_0057	ATK2_TID_TASK_0057
#define ATK2_TID_OUT_0057	ATK2_TID_TASK_0057
#define ATK2_WE_TASK_0057	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0057	BlsmControl_WakeupEvent

/* Task_0056: BlsmControl_System_duty_culc_Subsystem1_UnitDelay6 */
#define ATK2_TASK_0056		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0056	ATK2_TASK_0056
#define ATK2_TASK_OUT_0056	ATK2_TASK_0056
#define ATK2_SE_IN_0056		ATK2_TASK_0056, BlsmControl_InputEvent
#define ATK2_SE_OUT_0056	ATK2_TASK_0056, BlsmControl_OutputEvent
#define ATK2_WE_IN_0056		BlsmControl_InputEvent
#define ATK2_WE_OUT_0056	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0056	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0056	ATK2_TNAME_TASK_0056
#define ATK2_TNAME_OUT_0056	ATK2_TNAME_TASK_0056
#define ATK2_TID_TASK_0056	"0056"
#define ATK2_TID_IN_0056	ATK2_TID_TASK_0056
#define ATK2_TID_OUT_0056	ATK2_TID_TASK_0056
#define ATK2_WE_TASK_0056	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0056	BlsmControl_WakeupEvent

/* Task_0055: BlsmControl_System_duty_culc_Subsystem1_UnitDelay5 */
#define ATK2_TASK_0055		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0055	ATK2_TASK_0055
#define ATK2_TASK_OUT_0055	ATK2_TASK_0055
#define ATK2_SE_IN_0055		ATK2_TASK_0055, BlsmControl_InputEvent
#define ATK2_SE_OUT_0055	ATK2_TASK_0055, BlsmControl_OutputEvent
#define ATK2_WE_IN_0055		BlsmControl_InputEvent
#define ATK2_WE_OUT_0055	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0055	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0055	ATK2_TNAME_TASK_0055
#define ATK2_TNAME_OUT_0055	ATK2_TNAME_TASK_0055
#define ATK2_TID_TASK_0055	"0055"
#define ATK2_TID_IN_0055	ATK2_TID_TASK_0055
#define ATK2_TID_OUT_0055	ATK2_TID_TASK_0055
#define ATK2_WE_TASK_0055	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0055	BlsmControl_WakeupEvent

/* Task_0054: BlsmControl_System_duty_culc_Subsystem1_UnitDelay4 */
#define ATK2_TASK_0054		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0054	ATK2_TASK_0054
#define ATK2_TASK_OUT_0054	ATK2_TASK_0054
#define ATK2_SE_IN_0054		ATK2_TASK_0054, BlsmControl_InputEvent
#define ATK2_SE_OUT_0054	ATK2_TASK_0054, BlsmControl_OutputEvent
#define ATK2_WE_IN_0054		BlsmControl_InputEvent
#define ATK2_WE_OUT_0054	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0054	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0054	ATK2_TNAME_TASK_0054
#define ATK2_TNAME_OUT_0054	ATK2_TNAME_TASK_0054
#define ATK2_TID_TASK_0054	"0054"
#define ATK2_TID_IN_0054	ATK2_TID_TASK_0054
#define ATK2_TID_OUT_0054	ATK2_TID_TASK_0054
#define ATK2_WE_TASK_0054	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0054	BlsmControl_WakeupEvent

/* Task_0053: BlsmControl_System_duty_culc_Subsystem1_UnitDelay3 */
#define ATK2_TASK_0053		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0053	ATK2_TASK_0053
#define ATK2_TASK_OUT_0053	ATK2_TASK_0053
#define ATK2_SE_IN_0053		ATK2_TASK_0053, BlsmControl_InputEvent
#define ATK2_SE_OUT_0053	ATK2_TASK_0053, BlsmControl_OutputEvent
#define ATK2_WE_IN_0053		BlsmControl_InputEvent
#define ATK2_WE_OUT_0053	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0053	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0053	ATK2_TNAME_TASK_0053
#define ATK2_TNAME_OUT_0053	ATK2_TNAME_TASK_0053
#define ATK2_TID_TASK_0053	"0053"
#define ATK2_TID_IN_0053	ATK2_TID_TASK_0053
#define ATK2_TID_OUT_0053	ATK2_TID_TASK_0053
#define ATK2_WE_TASK_0053	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0053	BlsmControl_WakeupEvent

/* Task_0052: BlsmControl_System_duty_culc_Subsystem1_UnitDelay2 */
#define ATK2_TASK_0052		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0052	ATK2_TASK_0052
#define ATK2_TASK_OUT_0052	ATK2_TASK_0052
#define ATK2_SE_IN_0052		ATK2_TASK_0052, BlsmControl_InputEvent
#define ATK2_SE_OUT_0052	ATK2_TASK_0052, BlsmControl_OutputEvent
#define ATK2_WE_IN_0052		BlsmControl_InputEvent
#define ATK2_WE_OUT_0052	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0052	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0052	ATK2_TNAME_TASK_0052
#define ATK2_TNAME_OUT_0052	ATK2_TNAME_TASK_0052
#define ATK2_TID_TASK_0052	"0052"
#define ATK2_TID_IN_0052	ATK2_TID_TASK_0052
#define ATK2_TID_OUT_0052	ATK2_TID_TASK_0052
#define ATK2_WE_TASK_0052	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0052	BlsmControl_WakeupEvent

/* Task_0046: BlsmControl_System_duty_culc_Subsystem1_UnitDelay1 */
#define ATK2_TASK_0046		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0046	ATK2_TASK_0046
#define ATK2_TASK_OUT_0046	ATK2_TASK_0046
#define ATK2_SE_IN_0046		ATK2_TASK_0046, BlsmControl_InputEvent
#define ATK2_SE_OUT_0046	ATK2_TASK_0046, BlsmControl_OutputEvent
#define ATK2_WE_IN_0046		BlsmControl_InputEvent
#define ATK2_WE_OUT_0046	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0046	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0046	ATK2_TNAME_TASK_0046
#define ATK2_TNAME_OUT_0046	ATK2_TNAME_TASK_0046
#define ATK2_TID_TASK_0046	"0046"
#define ATK2_TID_IN_0046	ATK2_TID_TASK_0046
#define ATK2_TID_OUT_0046	ATK2_TID_TASK_0046
#define ATK2_WE_TASK_0046	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0046	BlsmControl_WakeupEvent

/* Task_0045: BlsmControl_System_duty_culc_Subsystem1_UnitDelay */
#define ATK2_TASK_0045		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0045	ATK2_TASK_0045
#define ATK2_TASK_OUT_0045	ATK2_TASK_0045
#define ATK2_SE_IN_0045		ATK2_TASK_0045, BlsmControl_InputEvent
#define ATK2_SE_OUT_0045	ATK2_TASK_0045, BlsmControl_OutputEvent
#define ATK2_WE_IN_0045		BlsmControl_InputEvent
#define ATK2_WE_OUT_0045	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0045	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0045	ATK2_TNAME_TASK_0045
#define ATK2_TNAME_OUT_0045	ATK2_TNAME_TASK_0045
#define ATK2_TID_TASK_0045	"0045"
#define ATK2_TID_IN_0045	ATK2_TID_TASK_0045
#define ATK2_TID_OUT_0045	ATK2_TID_TASK_0045
#define ATK2_WE_TASK_0045	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0045	BlsmControl_WakeupEvent

/* Task_0073: BlsmControl_System_duty_culc_Switch8 */
#define ATK2_TASK_0073		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0073	ATK2_TASK_0073
#define ATK2_TASK_OUT_0073	ATK2_TASK_0073
#define ATK2_SE_IN_0073		ATK2_TASK_0073, BlsmControl_InputEvent
#define ATK2_SE_OUT_0073	ATK2_TASK_0073, BlsmControl_OutputEvent
#define ATK2_WE_IN_0073		BlsmControl_InputEvent
#define ATK2_WE_OUT_0073	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0073	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0073	ATK2_TNAME_TASK_0073
#define ATK2_TNAME_OUT_0073	ATK2_TNAME_TASK_0073
#define ATK2_TID_TASK_0073	"0073"
#define ATK2_TID_IN_0073	ATK2_TID_TASK_0073
#define ATK2_TID_OUT_0073	ATK2_TID_TASK_0073
#define ATK2_WE_TASK_0073	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0073	BlsmControl_WakeupEvent

/* Task_0060: BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold */
#define ATK2_TASK_0060		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0060	ATK2_TASK_0060
#define ATK2_TASK_OUT_0060	ATK2_TASK_0060
#define ATK2_SE_IN_0060		ATK2_TASK_0060, BlsmControl_InputEvent
#define ATK2_SE_OUT_0060	ATK2_TASK_0060, BlsmControl_OutputEvent
#define ATK2_WE_IN_0060		BlsmControl_InputEvent
#define ATK2_WE_OUT_0060	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0060	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0060	ATK2_TNAME_TASK_0060
#define ATK2_TNAME_OUT_0060	ATK2_TNAME_TASK_0060
#define ATK2_TID_TASK_0060	"0060"
#define ATK2_TID_IN_0060	ATK2_TID_TASK_0060
#define ATK2_TID_OUT_0060	ATK2_TID_TASK_0060
#define ATK2_WE_TASK_0060	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0060	BlsmControl_WakeupEvent

/* Task_0044: BlsmControl_System_duty_culc_Subsystem1_Subtract */
#define ATK2_TASK_0044		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0044	ATK2_TASK_0044
#define ATK2_TASK_OUT_0044	ATK2_TASK_0044
#define ATK2_SE_IN_0044		ATK2_TASK_0044, BlsmControl_InputEvent
#define ATK2_SE_OUT_0044	ATK2_TASK_0044, BlsmControl_OutputEvent
#define ATK2_WE_IN_0044		BlsmControl_InputEvent
#define ATK2_WE_OUT_0044	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0044	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0044	ATK2_TNAME_TASK_0044
#define ATK2_TNAME_OUT_0044	ATK2_TNAME_TASK_0044
#define ATK2_TID_TASK_0044	"0044"
#define ATK2_TID_IN_0044	ATK2_TID_TASK_0044
#define ATK2_TID_OUT_0044	ATK2_TID_TASK_0044
#define ATK2_WE_TASK_0044	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0044	BlsmControl_WakeupEvent

/* Task_0043: BlsmControl_System_duty_culc_Subsystem1_Divide */
#define ATK2_TASK_0043		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0043	ATK2_TASK_0043
#define ATK2_TASK_OUT_0043	ATK2_TASK_0043
#define ATK2_SE_IN_0043		ATK2_TASK_0043, BlsmControl_InputEvent
#define ATK2_SE_OUT_0043	ATK2_TASK_0043, BlsmControl_OutputEvent
#define ATK2_WE_IN_0043		BlsmControl_InputEvent
#define ATK2_WE_OUT_0043	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0043	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0043	ATK2_TNAME_TASK_0043
#define ATK2_TNAME_OUT_0043	ATK2_TNAME_TASK_0043
#define ATK2_TID_TASK_0043	"0043"
#define ATK2_TID_IN_0043	ATK2_TID_TASK_0043
#define ATK2_TID_OUT_0043	ATK2_TID_TASK_0043
#define ATK2_WE_TASK_0043	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0043	BlsmControl_WakeupEvent

/* Task_0072: BlsmControl_System_duty_culc_Switch6 */
#define ATK2_TASK_0072		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0072	ATK2_TASK_0072
#define ATK2_TASK_OUT_0072	ATK2_TASK_0072
#define ATK2_SE_IN_0072		ATK2_TASK_0072, BlsmControl_InputEvent
#define ATK2_SE_OUT_0072	ATK2_TASK_0072, BlsmControl_OutputEvent
#define ATK2_WE_IN_0072		BlsmControl_InputEvent
#define ATK2_WE_OUT_0072	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0072	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0072	ATK2_TNAME_TASK_0072
#define ATK2_TNAME_OUT_0072	ATK2_TNAME_TASK_0072
#define ATK2_TID_TASK_0072	"0072"
#define ATK2_TID_IN_0072	ATK2_TID_TASK_0072
#define ATK2_TID_OUT_0072	ATK2_TID_TASK_0072
#define ATK2_WE_TASK_0072	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0072	BlsmControl_WakeupEvent

/* Task_0070: BlsmControl_System_duty_culc_Switch4 */
#define ATK2_TASK_0070		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0070	ATK2_TASK_0070
#define ATK2_TASK_OUT_0070	ATK2_TASK_0070
#define ATK2_SE_IN_0070		ATK2_TASK_0070, BlsmControl_InputEvent
#define ATK2_SE_OUT_0070	ATK2_TASK_0070, BlsmControl_OutputEvent
#define ATK2_WE_IN_0070		BlsmControl_InputEvent
#define ATK2_WE_OUT_0070	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0070	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0070	ATK2_TNAME_TASK_0070
#define ATK2_TNAME_OUT_0070	ATK2_TNAME_TASK_0070
#define ATK2_TID_TASK_0070	"0070"
#define ATK2_TID_IN_0070	ATK2_TID_TASK_0070
#define ATK2_TID_OUT_0070	ATK2_TID_TASK_0070
#define ATK2_WE_TASK_0070	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0070	BlsmControl_WakeupEvent

/* Task_0069: BlsmControl_System_duty_culc_Switch3 */
#define ATK2_TASK_0069		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0069	ATK2_TASK_0069
#define ATK2_TASK_OUT_0069	ATK2_TASK_0069
#define ATK2_SE_IN_0069		ATK2_TASK_0069, BlsmControl_InputEvent
#define ATK2_SE_OUT_0069	ATK2_TASK_0069, BlsmControl_OutputEvent
#define ATK2_WE_IN_0069		BlsmControl_InputEvent
#define ATK2_WE_OUT_0069	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0069	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0069	ATK2_TNAME_TASK_0069
#define ATK2_TNAME_OUT_0069	ATK2_TNAME_TASK_0069
#define ATK2_TID_TASK_0069	"0069"
#define ATK2_TID_IN_0069	ATK2_TID_TASK_0069
#define ATK2_TID_OUT_0069	ATK2_TID_TASK_0069
#define ATK2_WE_TASK_0069	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0069	BlsmControl_WakeupEvent

/* Task_0068: BlsmControl_System_duty_culc_Switch2 */
#define ATK2_TASK_0068		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0068	ATK2_TASK_0068
#define ATK2_TASK_OUT_0068	ATK2_TASK_0068
#define ATK2_SE_IN_0068		ATK2_TASK_0068, BlsmControl_InputEvent
#define ATK2_SE_OUT_0068	ATK2_TASK_0068, BlsmControl_OutputEvent
#define ATK2_WE_IN_0068		BlsmControl_InputEvent
#define ATK2_WE_OUT_0068	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0068	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0068	ATK2_TNAME_TASK_0068
#define ATK2_TNAME_OUT_0068	ATK2_TNAME_TASK_0068
#define ATK2_TID_TASK_0068	"0068"
#define ATK2_TID_IN_0068	ATK2_TID_TASK_0068
#define ATK2_TID_OUT_0068	ATK2_TID_TASK_0068
#define ATK2_WE_TASK_0068	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0068	BlsmControl_WakeupEvent

/* Task_0036: BlsmControl_System_duty_culc_Product3 */
#define ATK2_TASK_0036		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0036	ATK2_TASK_0036
#define ATK2_TASK_OUT_0036	ATK2_TASK_0036
#define ATK2_SE_IN_0036		ATK2_TASK_0036, BlsmControl_InputEvent
#define ATK2_SE_OUT_0036	ATK2_TASK_0036, BlsmControl_OutputEvent
#define ATK2_WE_IN_0036		BlsmControl_InputEvent
#define ATK2_WE_OUT_0036	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0036	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0036	ATK2_TNAME_TASK_0036
#define ATK2_TNAME_OUT_0036	ATK2_TNAME_TASK_0036
#define ATK2_TID_TASK_0036	"0036"
#define ATK2_TID_IN_0036	ATK2_TID_TASK_0036
#define ATK2_TID_OUT_0036	ATK2_TID_TASK_0036
#define ATK2_WE_TASK_0036	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0036	BlsmControl_WakeupEvent

/* Task_0101: BlsmControl_System_rpm_culc_counter_Sum */
#define ATK2_TASK_0101		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0101	ATK2_TASK_0101
#define ATK2_TASK_OUT_0101	ATK2_TASK_0101
#define ATK2_SE_IN_0101		ATK2_TASK_0101, BlsmControl_InputEvent
#define ATK2_SE_OUT_0101	ATK2_TASK_0101, BlsmControl_OutputEvent
#define ATK2_WE_IN_0101		BlsmControl_InputEvent
#define ATK2_WE_OUT_0101	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0101	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0101	ATK2_TNAME_TASK_0101
#define ATK2_TNAME_OUT_0101	ATK2_TNAME_TASK_0101
#define ATK2_TID_TASK_0101	"0101"
#define ATK2_TID_IN_0101	ATK2_TID_TASK_0101
#define ATK2_TID_OUT_0101	ATK2_TID_TASK_0101
#define ATK2_WE_TASK_0101	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0101	BlsmControl_WakeupEvent

/* Task_0102: BlsmControl_System_rpm_culc_counter_Switch */
#define ATK2_TASK_0102		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0102	ATK2_TASK_0102
#define ATK2_TASK_OUT_0102	ATK2_TASK_0102
#define ATK2_SE_IN_0102		ATK2_TASK_0102, BlsmControl_InputEvent
#define ATK2_SE_OUT_0102	ATK2_TASK_0102, BlsmControl_OutputEvent
#define ATK2_WE_IN_0102		BlsmControl_InputEvent
#define ATK2_WE_OUT_0102	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0102	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0102	ATK2_TNAME_TASK_0102
#define ATK2_TNAME_OUT_0102	ATK2_TNAME_TASK_0102
#define ATK2_TID_TASK_0102	"0102"
#define ATK2_TID_IN_0102	ATK2_TID_TASK_0102
#define ATK2_TID_OUT_0102	ATK2_TID_TASK_0102
#define ATK2_WE_TASK_0102	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0102	BlsmControl_WakeupEvent

/* Task_0126: BlsmControl_System_rpm_culc_update_prev_pos_Switch3 */
#define ATK2_TASK_0126		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0126	ATK2_TASK_0126
#define ATK2_TASK_OUT_0126	ATK2_TASK_0126
#define ATK2_SE_IN_0126		ATK2_TASK_0126, BlsmControl_InputEvent
#define ATK2_SE_OUT_0126	ATK2_TASK_0126, BlsmControl_OutputEvent
#define ATK2_WE_IN_0126		BlsmControl_InputEvent
#define ATK2_WE_OUT_0126	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0126	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0126	ATK2_TNAME_TASK_0126
#define ATK2_TNAME_OUT_0126	ATK2_TNAME_TASK_0126
#define ATK2_TID_TASK_0126	"0126"
#define ATK2_TID_IN_0126	ATK2_TID_TASK_0126
#define ATK2_TID_OUT_0126	ATK2_TID_TASK_0126
#define ATK2_WE_TASK_0126	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0126	BlsmControl_WakeupEvent

/* Task_0186: BlsmControl_duty */
#define ATK2_TASK_0186		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0186	ATK2_TASK_0186
#define ATK2_TASK_OUT_0186	ATK2_TASK_0186
#define ATK2_SE_IN_0186		ATK2_TASK_0186, BlsmControl_InputEvent
#define ATK2_SE_OUT_0186	ATK2_TASK_0186, BlsmControl_OutputEvent
#define ATK2_WE_IN_0186		BlsmControl_InputEvent
#define ATK2_WE_OUT_0186	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0186	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0186	ATK2_TNAME_TASK_0186
#define ATK2_TNAME_OUT_0186	ATK2_TNAME_TASK_0186
#define ATK2_TID_TASK_0186	"0186"
#define ATK2_TID_IN_0186	ATK2_TID_TASK_0186
#define ATK2_TID_OUT_0186	ATK2_TID_TASK_0186
#define ATK2_WE_TASK_0186	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0186	BlsmControl_WakeupEvent

/* Task_0002: BlsmControl_pos */
#define ATK2_TASK_0002		BlsmControl_Core0_1_0_Task
#define ATK2_TASK_IN_0002	ATK2_TASK_0002
#define ATK2_TASK_OUT_0002	ATK2_TASK_0002
#define ATK2_SE_IN_0002		ATK2_TASK_0002, BlsmControl_InputEvent
#define ATK2_SE_OUT_0002	ATK2_TASK_0002, BlsmControl_OutputEvent
#define ATK2_WE_IN_0002		BlsmControl_InputEvent
#define ATK2_WE_OUT_0002	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0002	"BlsmControl_Core0_1_0_Task"
#define ATK2_TNAME_IN_0002	ATK2_TNAME_TASK_0002
#define ATK2_TNAME_OUT_0002	ATK2_TNAME_TASK_0002
#define ATK2_TID_TASK_0002	"0002"
#define ATK2_TID_IN_0002	ATK2_TID_TASK_0002
#define ATK2_TID_OUT_0002	ATK2_TID_TASK_0002
#define ATK2_WE_TASK_0002	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0002	BlsmControl_WakeupEvent

/* Task: BlsmControl_Core1_1_0_Task */
#define ATK2_TASK_BlsmControl_Core1_1_0_Task	BlsmControl_Core1_1_0_Task
#define ATK2_WE_BlsmControl_Core1_1_0_Task	BlsmControl_WakeupEvent
#define ATK2_SE_BlsmControl_Core1_1_0_Task	BlsmControl_WakeupEvent
#define ATK2_TNAME_TASK_BlsmControl_Core1_1_0_Task	"BlsmControl_Core1_1_0_Task"
#define ATK2_ALARM_BlsmControl_Core1_1_0_Task	BlsmControl_Core1_1_0_Alarm
#define ATK2_TNAME_ALARM_BlsmControl_Core1_1_0_Task	"BlsmControl_Core1_1_0_Alarm"

/* Task_0015: BlsmControl_System_duty_culc_Abs */
#define ATK2_TASK_0015		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0015	ATK2_TASK_0015
#define ATK2_TASK_OUT_0015	ATK2_TASK_0015
#define ATK2_SE_IN_0015		ATK2_TASK_0015, BlsmControl_InputEvent
#define ATK2_SE_OUT_0015	ATK2_TASK_0015, BlsmControl_OutputEvent
#define ATK2_WE_IN_0015		BlsmControl_InputEvent
#define ATK2_WE_OUT_0015	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0015	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0015	ATK2_TNAME_TASK_0015
#define ATK2_TNAME_OUT_0015	ATK2_TNAME_TASK_0015
#define ATK2_TID_TASK_0015	"0015"
#define ATK2_TID_IN_0015	ATK2_TID_TASK_0015
#define ATK2_TID_OUT_0015	ATK2_TID_TASK_0015
#define ATK2_WE_TASK_0015	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0015	BlsmControl_WakeupEvent

/* Task_0037: BlsmControl_System_duty_culc_RateTransition2 */
#define ATK2_TASK_0037		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0037	ATK2_TASK_0037
#define ATK2_TASK_OUT_0037	ATK2_TASK_0037
#define ATK2_SE_IN_0037		ATK2_TASK_0037, BlsmControl_InputEvent
#define ATK2_SE_OUT_0037	ATK2_TASK_0037, BlsmControl_OutputEvent
#define ATK2_WE_IN_0037		BlsmControl_InputEvent
#define ATK2_WE_OUT_0037	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0037	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0037	ATK2_TNAME_TASK_0037
#define ATK2_TNAME_OUT_0037	ATK2_TNAME_TASK_0037
#define ATK2_TID_TASK_0037	"0037"
#define ATK2_TID_IN_0037	ATK2_TID_TASK_0037
#define ATK2_TID_OUT_0037	ATK2_TID_TASK_0037
#define ATK2_WE_TASK_0037	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0037	BlsmControl_WakeupEvent

/* Task_0088: BlsmControl_System_pos_to_theta_MultiportSwitch */
#define ATK2_TASK_0088		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0088	ATK2_TASK_0088
#define ATK2_TASK_OUT_0088	ATK2_TASK_0088
#define ATK2_SE_IN_0088		ATK2_TASK_0088, BlsmControl_InputEvent
#define ATK2_SE_OUT_0088	ATK2_TASK_0088, BlsmControl_OutputEvent
#define ATK2_WE_IN_0088		BlsmControl_InputEvent
#define ATK2_WE_OUT_0088	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0088	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0088	ATK2_TNAME_TASK_0088
#define ATK2_TNAME_OUT_0088	ATK2_TNAME_TASK_0088
#define ATK2_TID_TASK_0088	"0088"
#define ATK2_TID_IN_0088	ATK2_TID_TASK_0088
#define ATK2_TID_OUT_0088	ATK2_TID_TASK_0088
#define ATK2_WE_TASK_0088	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0088	BlsmControl_WakeupEvent

/* Task_0087: BlsmControl_System_pos_to_theta_Gain */
#define ATK2_TASK_0087		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0087	ATK2_TASK_0087
#define ATK2_TASK_OUT_0087	ATK2_TASK_0087
#define ATK2_SE_IN_0087		ATK2_TASK_0087, BlsmControl_InputEvent
#define ATK2_SE_OUT_0087	ATK2_TASK_0087, BlsmControl_OutputEvent
#define ATK2_WE_IN_0087		BlsmControl_InputEvent
#define ATK2_WE_OUT_0087	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0087	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0087	ATK2_TNAME_TASK_0087
#define ATK2_TNAME_OUT_0087	ATK2_TNAME_TASK_0087
#define ATK2_TID_TASK_0087	"0087"
#define ATK2_TID_IN_0087	ATK2_TID_TASK_0087
#define ATK2_TID_OUT_0087	ATK2_TID_TASK_0087
#define ATK2_WE_TASK_0087	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0087	BlsmControl_WakeupEvent

/* Task_0182: BlsmControl_System_sector_culc_Zero_OrderHold1 */
#define ATK2_TASK_0182		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0182	ATK2_TASK_0182
#define ATK2_TASK_OUT_0182	ATK2_TASK_0182
#define ATK2_SE_IN_0182		ATK2_TASK_0182, BlsmControl_InputEvent
#define ATK2_SE_OUT_0182	ATK2_TASK_0182, BlsmControl_OutputEvent
#define ATK2_WE_IN_0182		BlsmControl_InputEvent
#define ATK2_WE_OUT_0182	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0182	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0182	ATK2_TNAME_TASK_0182
#define ATK2_TNAME_OUT_0182	ATK2_TNAME_TASK_0182
#define ATK2_TID_TASK_0182	"0182"
#define ATK2_TID_IN_0182	ATK2_TID_TASK_0182
#define ATK2_TID_OUT_0182	ATK2_TID_TASK_0182
#define ATK2_WE_TASK_0182	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0182	BlsmControl_WakeupEvent

/* Task_0160: BlsmControl_System_sector_culc_Gain */
#define ATK2_TASK_0160		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0160	ATK2_TASK_0160
#define ATK2_TASK_OUT_0160	ATK2_TASK_0160
#define ATK2_SE_IN_0160		ATK2_TASK_0160, BlsmControl_InputEvent
#define ATK2_SE_OUT_0160	ATK2_TASK_0160, BlsmControl_OutputEvent
#define ATK2_WE_IN_0160		BlsmControl_InputEvent
#define ATK2_WE_OUT_0160	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0160	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0160	ATK2_TNAME_TASK_0160
#define ATK2_TNAME_OUT_0160	ATK2_TNAME_TASK_0160
#define ATK2_TID_TASK_0160	"0160"
#define ATK2_TID_IN_0160	ATK2_TID_TASK_0160
#define ATK2_TID_OUT_0160	ATK2_TID_TASK_0160
#define ATK2_WE_TASK_0160	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0160	BlsmControl_WakeupEvent

/* Task_0174: BlsmControl_System_sector_culc_Switch24 */
#define ATK2_TASK_0174		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0174	ATK2_TASK_0174
#define ATK2_TASK_OUT_0174	ATK2_TASK_0174
#define ATK2_SE_IN_0174		ATK2_TASK_0174, BlsmControl_InputEvent
#define ATK2_SE_OUT_0174	ATK2_TASK_0174, BlsmControl_OutputEvent
#define ATK2_WE_IN_0174		BlsmControl_InputEvent
#define ATK2_WE_OUT_0174	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0174	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0174	ATK2_TNAME_TASK_0174
#define ATK2_TNAME_OUT_0174	ATK2_TNAME_TASK_0174
#define ATK2_TID_TASK_0174	"0174"
#define ATK2_TID_IN_0174	ATK2_TID_TASK_0174
#define ATK2_TID_OUT_0174	ATK2_TID_TASK_0174
#define ATK2_WE_TASK_0174	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0174	BlsmControl_WakeupEvent

/* Task_0175: BlsmControl_System_sector_culc_Switch3 */
#define ATK2_TASK_0175		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0175	ATK2_TASK_0175
#define ATK2_TASK_OUT_0175	ATK2_TASK_0175
#define ATK2_SE_IN_0175		ATK2_TASK_0175, BlsmControl_InputEvent
#define ATK2_SE_OUT_0175	ATK2_TASK_0175, BlsmControl_OutputEvent
#define ATK2_WE_IN_0175		BlsmControl_InputEvent
#define ATK2_WE_OUT_0175	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0175	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0175	ATK2_TNAME_TASK_0175
#define ATK2_TNAME_OUT_0175	ATK2_TNAME_TASK_0175
#define ATK2_TID_TASK_0175	"0175"
#define ATK2_TID_IN_0175	ATK2_TID_TASK_0175
#define ATK2_TID_OUT_0175	ATK2_TID_TASK_0175
#define ATK2_WE_TASK_0175	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0175	BlsmControl_WakeupEvent

/* Task_0177: BlsmControl_System_sector_culc_Switch5 */
#define ATK2_TASK_0177		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0177	ATK2_TASK_0177
#define ATK2_TASK_OUT_0177	ATK2_TASK_0177
#define ATK2_SE_IN_0177		ATK2_TASK_0177, BlsmControl_InputEvent
#define ATK2_SE_OUT_0177	ATK2_TASK_0177, BlsmControl_OutputEvent
#define ATK2_WE_IN_0177		BlsmControl_InputEvent
#define ATK2_WE_OUT_0177	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0177	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0177	ATK2_TNAME_TASK_0177
#define ATK2_TNAME_OUT_0177	ATK2_TNAME_TASK_0177
#define ATK2_TID_TASK_0177	"0177"
#define ATK2_TID_IN_0177	ATK2_TID_TASK_0177
#define ATK2_TID_OUT_0177	ATK2_TID_TASK_0177
#define ATK2_WE_TASK_0177	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0177	BlsmControl_WakeupEvent

/* Task_0179: BlsmControl_System_sector_culc_Switch7 */
#define ATK2_TASK_0179		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0179	ATK2_TASK_0179
#define ATK2_TASK_OUT_0179	ATK2_TASK_0179
#define ATK2_SE_IN_0179		ATK2_TASK_0179, BlsmControl_InputEvent
#define ATK2_SE_OUT_0179	ATK2_TASK_0179, BlsmControl_OutputEvent
#define ATK2_WE_IN_0179		BlsmControl_InputEvent
#define ATK2_WE_OUT_0179	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0179	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0179	ATK2_TNAME_TASK_0179
#define ATK2_TNAME_OUT_0179	ATK2_TNAME_TASK_0179
#define ATK2_TID_TASK_0179	"0179"
#define ATK2_TID_IN_0179	ATK2_TID_TASK_0179
#define ATK2_TID_OUT_0179	ATK2_TID_TASK_0179
#define ATK2_WE_TASK_0179	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0179	BlsmControl_WakeupEvent

/* Task_0181: BlsmControl_System_sector_culc_Switch9 */
#define ATK2_TASK_0181		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0181	ATK2_TASK_0181
#define ATK2_TASK_OUT_0181	ATK2_TASK_0181
#define ATK2_SE_IN_0181		ATK2_TASK_0181, BlsmControl_InputEvent
#define ATK2_SE_OUT_0181	ATK2_TASK_0181, BlsmControl_OutputEvent
#define ATK2_WE_IN_0181		BlsmControl_InputEvent
#define ATK2_WE_OUT_0181	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0181	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0181	ATK2_TNAME_TASK_0181
#define ATK2_TNAME_OUT_0181	ATK2_TNAME_TASK_0181
#define ATK2_TID_TASK_0181	"0181"
#define ATK2_TID_IN_0181	ATK2_TID_TASK_0181
#define ATK2_TID_OUT_0181	ATK2_TID_TASK_0181
#define ATK2_WE_TASK_0181	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0181	BlsmControl_WakeupEvent

/* Task_0165: BlsmControl_System_sector_culc_Switch11 */
#define ATK2_TASK_0165		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0165	ATK2_TASK_0165
#define ATK2_TASK_OUT_0165	ATK2_TASK_0165
#define ATK2_SE_IN_0165		ATK2_TASK_0165, BlsmControl_InputEvent
#define ATK2_SE_OUT_0165	ATK2_TASK_0165, BlsmControl_OutputEvent
#define ATK2_WE_IN_0165		BlsmControl_InputEvent
#define ATK2_WE_OUT_0165	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0165	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0165	ATK2_TNAME_TASK_0165
#define ATK2_TNAME_OUT_0165	ATK2_TNAME_TASK_0165
#define ATK2_TID_TASK_0165	"0165"
#define ATK2_TID_IN_0165	ATK2_TID_TASK_0165
#define ATK2_TID_OUT_0165	ATK2_TID_TASK_0165
#define ATK2_WE_TASK_0165	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0165	BlsmControl_WakeupEvent

/* Task_0162: BlsmControl_System_sector_culc_Switch */
#define ATK2_TASK_0162		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0162	ATK2_TASK_0162
#define ATK2_TASK_OUT_0162	ATK2_TASK_0162
#define ATK2_SE_IN_0162		ATK2_TASK_0162, BlsmControl_InputEvent
#define ATK2_SE_OUT_0162	ATK2_TASK_0162, BlsmControl_OutputEvent
#define ATK2_WE_IN_0162		BlsmControl_InputEvent
#define ATK2_WE_OUT_0162	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0162	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0162	ATK2_TNAME_TASK_0162
#define ATK2_TNAME_OUT_0162	ATK2_TNAME_TASK_0162
#define ATK2_TID_TASK_0162	"0162"
#define ATK2_TID_IN_0162	ATK2_TID_TASK_0162
#define ATK2_TID_OUT_0162	ATK2_TID_TASK_0162
#define ATK2_WE_TASK_0162	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0162	BlsmControl_WakeupEvent

/* Task_0166: BlsmControl_System_sector_culc_Switch12 */
#define ATK2_TASK_0166		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0166	ATK2_TASK_0166
#define ATK2_TASK_OUT_0166	ATK2_TASK_0166
#define ATK2_SE_IN_0166		ATK2_TASK_0166, BlsmControl_InputEvent
#define ATK2_SE_OUT_0166	ATK2_TASK_0166, BlsmControl_OutputEvent
#define ATK2_WE_IN_0166		BlsmControl_InputEvent
#define ATK2_WE_OUT_0166	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0166	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0166	ATK2_TNAME_TASK_0166
#define ATK2_TNAME_OUT_0166	ATK2_TNAME_TASK_0166
#define ATK2_TID_TASK_0166	"0166"
#define ATK2_TID_IN_0166	ATK2_TID_TASK_0166
#define ATK2_TID_OUT_0166	ATK2_TID_TASK_0166
#define ATK2_WE_TASK_0166	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0166	BlsmControl_WakeupEvent

/* Task_0167: BlsmControl_System_sector_culc_Switch13 */
#define ATK2_TASK_0167		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0167	ATK2_TASK_0167
#define ATK2_TASK_OUT_0167	ATK2_TASK_0167
#define ATK2_SE_IN_0167		ATK2_TASK_0167, BlsmControl_InputEvent
#define ATK2_SE_OUT_0167	ATK2_TASK_0167, BlsmControl_OutputEvent
#define ATK2_WE_IN_0167		BlsmControl_InputEvent
#define ATK2_WE_OUT_0167	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0167	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0167	ATK2_TNAME_TASK_0167
#define ATK2_TNAME_OUT_0167	ATK2_TNAME_TASK_0167
#define ATK2_TID_TASK_0167	"0167"
#define ATK2_TID_IN_0167	ATK2_TID_TASK_0167
#define ATK2_TID_OUT_0167	ATK2_TID_TASK_0167
#define ATK2_WE_TASK_0167	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0167	BlsmControl_WakeupEvent

/* Task_0164: BlsmControl_System_sector_culc_Switch10 */
#define ATK2_TASK_0164		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0164	ATK2_TASK_0164
#define ATK2_TASK_OUT_0164	ATK2_TASK_0164
#define ATK2_SE_IN_0164		ATK2_TASK_0164, BlsmControl_InputEvent
#define ATK2_SE_OUT_0164	ATK2_TASK_0164, BlsmControl_OutputEvent
#define ATK2_WE_IN_0164		BlsmControl_InputEvent
#define ATK2_WE_OUT_0164	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0164	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0164	ATK2_TNAME_TASK_0164
#define ATK2_TNAME_OUT_0164	ATK2_TNAME_TASK_0164
#define ATK2_TID_TASK_0164	"0164"
#define ATK2_TID_IN_0164	ATK2_TID_TASK_0164
#define ATK2_TID_OUT_0164	ATK2_TID_TASK_0164
#define ATK2_WE_TASK_0164	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0164	BlsmControl_WakeupEvent

/* Task_0168: BlsmControl_System_sector_culc_Switch14 */
#define ATK2_TASK_0168		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0168	ATK2_TASK_0168
#define ATK2_TASK_OUT_0168	ATK2_TASK_0168
#define ATK2_SE_IN_0168		ATK2_TASK_0168, BlsmControl_InputEvent
#define ATK2_SE_OUT_0168	ATK2_TASK_0168, BlsmControl_OutputEvent
#define ATK2_WE_IN_0168		BlsmControl_InputEvent
#define ATK2_WE_OUT_0168	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0168	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0168	ATK2_TNAME_TASK_0168
#define ATK2_TNAME_OUT_0168	ATK2_TNAME_TASK_0168
#define ATK2_TID_TASK_0168	"0168"
#define ATK2_TID_IN_0168	ATK2_TID_TASK_0168
#define ATK2_TID_OUT_0168	ATK2_TID_TASK_0168
#define ATK2_WE_TASK_0168	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0168	BlsmControl_WakeupEvent

/* Task_0180: BlsmControl_System_sector_culc_Switch8 */
#define ATK2_TASK_0180		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0180	ATK2_TASK_0180
#define ATK2_TASK_OUT_0180	ATK2_TASK_0180
#define ATK2_SE_IN_0180		ATK2_TASK_0180, BlsmControl_InputEvent
#define ATK2_SE_OUT_0180	ATK2_TASK_0180, BlsmControl_OutputEvent
#define ATK2_WE_IN_0180		BlsmControl_InputEvent
#define ATK2_WE_OUT_0180	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0180	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0180	ATK2_TNAME_TASK_0180
#define ATK2_TNAME_OUT_0180	ATK2_TNAME_TASK_0180
#define ATK2_TID_TASK_0180	"0180"
#define ATK2_TID_IN_0180	ATK2_TID_TASK_0180
#define ATK2_TID_OUT_0180	ATK2_TID_TASK_0180
#define ATK2_WE_TASK_0180	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0180	BlsmControl_WakeupEvent

/* Task_0169: BlsmControl_System_sector_culc_Switch15 */
#define ATK2_TASK_0169		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0169	ATK2_TASK_0169
#define ATK2_TASK_OUT_0169	ATK2_TASK_0169
#define ATK2_SE_IN_0169		ATK2_TASK_0169, BlsmControl_InputEvent
#define ATK2_SE_OUT_0169	ATK2_TASK_0169, BlsmControl_OutputEvent
#define ATK2_WE_IN_0169		BlsmControl_InputEvent
#define ATK2_WE_OUT_0169	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0169	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0169	ATK2_TNAME_TASK_0169
#define ATK2_TNAME_OUT_0169	ATK2_TNAME_TASK_0169
#define ATK2_TID_TASK_0169	"0169"
#define ATK2_TID_IN_0169	ATK2_TID_TASK_0169
#define ATK2_TID_OUT_0169	ATK2_TID_TASK_0169
#define ATK2_WE_TASK_0169	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0169	BlsmControl_WakeupEvent

/* Task_0178: BlsmControl_System_sector_culc_Switch6 */
#define ATK2_TASK_0178		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0178	ATK2_TASK_0178
#define ATK2_TASK_OUT_0178	ATK2_TASK_0178
#define ATK2_SE_IN_0178		ATK2_TASK_0178, BlsmControl_InputEvent
#define ATK2_SE_OUT_0178	ATK2_TASK_0178, BlsmControl_OutputEvent
#define ATK2_WE_IN_0178		BlsmControl_InputEvent
#define ATK2_WE_OUT_0178	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0178	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0178	ATK2_TNAME_TASK_0178
#define ATK2_TNAME_OUT_0178	ATK2_TNAME_TASK_0178
#define ATK2_TID_TASK_0178	"0178"
#define ATK2_TID_IN_0178	ATK2_TID_TASK_0178
#define ATK2_TID_OUT_0178	ATK2_TID_TASK_0178
#define ATK2_WE_TASK_0178	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0178	BlsmControl_WakeupEvent

/* Task_0170: BlsmControl_System_sector_culc_Switch16 */
#define ATK2_TASK_0170		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0170	ATK2_TASK_0170
#define ATK2_TASK_OUT_0170	ATK2_TASK_0170
#define ATK2_SE_IN_0170		ATK2_TASK_0170, BlsmControl_InputEvent
#define ATK2_SE_OUT_0170	ATK2_TASK_0170, BlsmControl_OutputEvent
#define ATK2_WE_IN_0170		BlsmControl_InputEvent
#define ATK2_WE_OUT_0170	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0170	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0170	ATK2_TNAME_TASK_0170
#define ATK2_TNAME_OUT_0170	ATK2_TNAME_TASK_0170
#define ATK2_TID_TASK_0170	"0170"
#define ATK2_TID_IN_0170	ATK2_TID_TASK_0170
#define ATK2_TID_OUT_0170	ATK2_TID_TASK_0170
#define ATK2_WE_TASK_0170	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0170	BlsmControl_WakeupEvent

/* Task_0176: BlsmControl_System_sector_culc_Switch4 */
#define ATK2_TASK_0176		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0176	ATK2_TASK_0176
#define ATK2_TASK_OUT_0176	ATK2_TASK_0176
#define ATK2_SE_IN_0176		ATK2_TASK_0176, BlsmControl_InputEvent
#define ATK2_SE_OUT_0176	ATK2_TASK_0176, BlsmControl_OutputEvent
#define ATK2_WE_IN_0176		BlsmControl_InputEvent
#define ATK2_WE_OUT_0176	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0176	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0176	ATK2_TNAME_TASK_0176
#define ATK2_TNAME_OUT_0176	ATK2_TNAME_TASK_0176
#define ATK2_TID_TASK_0176	"0176"
#define ATK2_TID_IN_0176	ATK2_TID_TASK_0176
#define ATK2_TID_OUT_0176	ATK2_TID_TASK_0176
#define ATK2_WE_TASK_0176	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0176	BlsmControl_WakeupEvent

/* Task_0171: BlsmControl_System_sector_culc_Switch17 */
#define ATK2_TASK_0171		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0171	ATK2_TASK_0171
#define ATK2_TASK_OUT_0171	ATK2_TASK_0171
#define ATK2_SE_IN_0171		ATK2_TASK_0171, BlsmControl_InputEvent
#define ATK2_SE_OUT_0171	ATK2_TASK_0171, BlsmControl_OutputEvent
#define ATK2_WE_IN_0171		BlsmControl_InputEvent
#define ATK2_WE_OUT_0171	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0171	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0171	ATK2_TNAME_TASK_0171
#define ATK2_TNAME_OUT_0171	ATK2_TNAME_TASK_0171
#define ATK2_TID_TASK_0171	"0171"
#define ATK2_TID_IN_0171	ATK2_TID_TASK_0171
#define ATK2_TID_OUT_0171	ATK2_TID_TASK_0171
#define ATK2_WE_TASK_0171	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0171	BlsmControl_WakeupEvent

/* Task_0172: BlsmControl_System_sector_culc_Switch2 */
#define ATK2_TASK_0172		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0172	ATK2_TASK_0172
#define ATK2_TASK_OUT_0172	ATK2_TASK_0172
#define ATK2_SE_IN_0172		ATK2_TASK_0172, BlsmControl_InputEvent
#define ATK2_SE_OUT_0172	ATK2_TASK_0172, BlsmControl_OutputEvent
#define ATK2_WE_IN_0172		BlsmControl_InputEvent
#define ATK2_WE_OUT_0172	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0172	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0172	ATK2_TNAME_TASK_0172
#define ATK2_TNAME_OUT_0172	ATK2_TNAME_TASK_0172
#define ATK2_TID_TASK_0172	"0172"
#define ATK2_TID_IN_0172	ATK2_TID_TASK_0172
#define ATK2_TID_OUT_0172	ATK2_TID_TASK_0172
#define ATK2_WE_TASK_0172	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0172	BlsmControl_WakeupEvent

/* Task_0173: BlsmControl_System_sector_culc_Switch23 */
#define ATK2_TASK_0173		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0173	ATK2_TASK_0173
#define ATK2_TASK_OUT_0173	ATK2_TASK_0173
#define ATK2_SE_IN_0173		ATK2_TASK_0173, BlsmControl_InputEvent
#define ATK2_SE_OUT_0173	ATK2_TASK_0173, BlsmControl_OutputEvent
#define ATK2_WE_IN_0173		BlsmControl_InputEvent
#define ATK2_WE_OUT_0173	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0173	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0173	ATK2_TNAME_TASK_0173
#define ATK2_TNAME_OUT_0173	ATK2_TNAME_TASK_0173
#define ATK2_TID_TASK_0173	"0173"
#define ATK2_TID_IN_0173	ATK2_TID_TASK_0173
#define ATK2_TID_OUT_0173	ATK2_TID_TASK_0173
#define ATK2_WE_TASK_0173	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0173	BlsmControl_WakeupEvent

/* Task_0161: BlsmControl_System_sector_culc_Product */
#define ATK2_TASK_0161		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0161	ATK2_TASK_0161
#define ATK2_TASK_OUT_0161	ATK2_TASK_0161
#define ATK2_SE_IN_0161		ATK2_TASK_0161, BlsmControl_InputEvent
#define ATK2_SE_OUT_0161	ATK2_TASK_0161, BlsmControl_OutputEvent
#define ATK2_WE_IN_0161		BlsmControl_InputEvent
#define ATK2_WE_OUT_0161	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0161	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0161	ATK2_TNAME_TASK_0161
#define ATK2_TNAME_OUT_0161	ATK2_TNAME_TASK_0161
#define ATK2_TID_TASK_0161	"0161"
#define ATK2_TID_IN_0161	ATK2_TID_TASK_0161
#define ATK2_TID_OUT_0161	ATK2_TID_TASK_0161
#define ATK2_WE_TASK_0161	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0161	BlsmControl_WakeupEvent

/* Task_0163: BlsmControl_System_sector_culc_Switch1 */
#define ATK2_TASK_0163		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0163	ATK2_TASK_0163
#define ATK2_TASK_OUT_0163	ATK2_TASK_0163
#define ATK2_SE_IN_0163		ATK2_TASK_0163, BlsmControl_InputEvent
#define ATK2_SE_OUT_0163	ATK2_TASK_0163, BlsmControl_OutputEvent
#define ATK2_WE_IN_0163		BlsmControl_InputEvent
#define ATK2_WE_OUT_0163	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0163	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0163	ATK2_TNAME_TASK_0163
#define ATK2_TNAME_OUT_0163	ATK2_TNAME_TASK_0163
#define ATK2_TID_TASK_0163	"0163"
#define ATK2_TID_IN_0163	ATK2_TID_TASK_0163
#define ATK2_TID_OUT_0163	ATK2_TID_TASK_0163
#define ATK2_WE_TASK_0163	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0163	BlsmControl_WakeupEvent

/* Task_0187: BlsmControl_sector */
#define ATK2_TASK_0187		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0187	ATK2_TASK_0187
#define ATK2_TASK_OUT_0187	ATK2_TASK_0187
#define ATK2_SE_IN_0187		ATK2_TASK_0187, BlsmControl_InputEvent
#define ATK2_SE_OUT_0187	ATK2_TASK_0187, BlsmControl_OutputEvent
#define ATK2_WE_IN_0187		BlsmControl_InputEvent
#define ATK2_WE_OUT_0187	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0187	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0187	ATK2_TNAME_TASK_0187
#define ATK2_TNAME_OUT_0187	ATK2_TNAME_TASK_0187
#define ATK2_TID_TASK_0187	"0187"
#define ATK2_TID_IN_0187	ATK2_TID_TASK_0187
#define ATK2_TID_OUT_0187	ATK2_TID_TASK_0187
#define ATK2_WE_TASK_0187	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0187	BlsmControl_WakeupEvent

/* Task_0004: BlsmControl_speed_command */
#define ATK2_TASK_0004		BlsmControl_Core1_1_0_Task
#define ATK2_TASK_IN_0004	ATK2_TASK_0004
#define ATK2_TASK_OUT_0004	ATK2_TASK_0004
#define ATK2_SE_IN_0004		ATK2_TASK_0004, BlsmControl_InputEvent
#define ATK2_SE_OUT_0004	ATK2_TASK_0004, BlsmControl_OutputEvent
#define ATK2_WE_IN_0004		BlsmControl_InputEvent
#define ATK2_WE_OUT_0004	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0004	"BlsmControl_Core1_1_0_Task"
#define ATK2_TNAME_IN_0004	ATK2_TNAME_TASK_0004
#define ATK2_TNAME_OUT_0004	ATK2_TNAME_TASK_0004
#define ATK2_TID_TASK_0004	"0004"
#define ATK2_TID_IN_0004	ATK2_TID_TASK_0004
#define ATK2_TID_OUT_0004	ATK2_TID_TASK_0004
#define ATK2_WE_TASK_0004	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0004	BlsmControl_WakeupEvent

/* Task: BlsmControl_Core0_100_0_Task */
#define ATK2_TASK_BlsmControl_Core0_100_0_Task	BlsmControl_Core0_100_0_Task
#define ATK2_WE_BlsmControl_Core0_100_0_Task	BlsmControl_WakeupEvent
#define ATK2_SE_BlsmControl_Core0_100_0_Task	BlsmControl_WakeupEvent
#define ATK2_TNAME_TASK_BlsmControl_Core0_100_0_Task	"BlsmControl_Core0_100_0_Task"
#define ATK2_ALARM_BlsmControl_Core0_100_0_Task	BlsmControl_Core0_100_0_Alarm
#define ATK2_TNAME_ALARM_BlsmControl_Core0_100_0_Task	"BlsmControl_Core0_100_0_Alarm"

/* Task_0038: BlsmControl_System_duty_culc_RateTransition3 */
#define ATK2_TASK_0038		BlsmControl_Core0_100_0_Task
#define ATK2_TASK_IN_0038	ATK2_TASK_0038
#define ATK2_TASK_OUT_0038	ATK2_TASK_0038
#define ATK2_SE_IN_0038		ATK2_TASK_0038, BlsmControl_InputEvent
#define ATK2_SE_OUT_0038	ATK2_TASK_0038, BlsmControl_OutputEvent
#define ATK2_WE_IN_0038		BlsmControl_InputEvent
#define ATK2_WE_OUT_0038	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0038	"BlsmControl_Core0_100_0_Task"
#define ATK2_TNAME_IN_0038	ATK2_TNAME_TASK_0038
#define ATK2_TNAME_OUT_0038	ATK2_TNAME_TASK_0038
#define ATK2_TID_TASK_0038	"0038"
#define ATK2_TID_IN_0038	ATK2_TID_TASK_0038
#define ATK2_TID_OUT_0038	ATK2_TID_TASK_0038
#define ATK2_WE_TASK_0038	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0038	BlsmControl_WakeupEvent

/* Task: BlsmControl_Core1_100_0_Task */
#define ATK2_TASK_BlsmControl_Core1_100_0_Task	BlsmControl_Core1_100_0_Task
#define ATK2_WE_BlsmControl_Core1_100_0_Task	BlsmControl_WakeupEvent
#define ATK2_SE_BlsmControl_Core1_100_0_Task	BlsmControl_WakeupEvent
#define ATK2_TNAME_TASK_BlsmControl_Core1_100_0_Task	"BlsmControl_Core1_100_0_Task"
#define ATK2_ALARM_BlsmControl_Core1_100_0_Task	BlsmControl_Core1_100_0_Alarm
#define ATK2_TNAME_ALARM_BlsmControl_Core1_100_0_Task	"BlsmControl_Core1_100_0_Alarm"

/* Task_0076: BlsmControl_System_duty_culc_Zero_OrderHold4 */
#define ATK2_TASK_0076		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0076	ATK2_TASK_0076
#define ATK2_TASK_OUT_0076	ATK2_TASK_0076
#define ATK2_SE_IN_0076		ATK2_TASK_0076, BlsmControl_InputEvent
#define ATK2_SE_OUT_0076	ATK2_TASK_0076, BlsmControl_OutputEvent
#define ATK2_WE_IN_0076		BlsmControl_InputEvent
#define ATK2_WE_OUT_0076	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0076	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0076	ATK2_TNAME_TASK_0076
#define ATK2_TNAME_OUT_0076	ATK2_TNAME_TASK_0076
#define ATK2_TID_TASK_0076	"0076"
#define ATK2_TID_IN_0076	ATK2_TID_TASK_0076
#define ATK2_TID_OUT_0076	ATK2_TID_TASK_0076
#define ATK2_WE_TASK_0076	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0076	BlsmControl_WakeupEvent

/* Task_0077: BlsmControl_System_duty_culc_Zero_OrderHold5 */
#define ATK2_TASK_0077		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0077	ATK2_TASK_0077
#define ATK2_TASK_OUT_0077	ATK2_TASK_0077
#define ATK2_SE_IN_0077		ATK2_TASK_0077, BlsmControl_InputEvent
#define ATK2_SE_OUT_0077	ATK2_TASK_0077, BlsmControl_OutputEvent
#define ATK2_WE_IN_0077		BlsmControl_InputEvent
#define ATK2_WE_OUT_0077	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0077	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0077	ATK2_TNAME_TASK_0077
#define ATK2_TNAME_OUT_0077	ATK2_TNAME_TASK_0077
#define ATK2_TID_TASK_0077	"0077"
#define ATK2_TID_IN_0077	ATK2_TID_TASK_0077
#define ATK2_TID_OUT_0077	ATK2_TID_TASK_0077
#define ATK2_WE_TASK_0077	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0077	BlsmControl_WakeupEvent

/* Task_0032: BlsmControl_System_duty_culc_Divide1 */
#define ATK2_TASK_0032		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0032	ATK2_TASK_0032
#define ATK2_TASK_OUT_0032	ATK2_TASK_0032
#define ATK2_SE_IN_0032		ATK2_TASK_0032, BlsmControl_InputEvent
#define ATK2_SE_OUT_0032	ATK2_TASK_0032, BlsmControl_OutputEvent
#define ATK2_WE_IN_0032		BlsmControl_InputEvent
#define ATK2_WE_OUT_0032	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0032	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0032	ATK2_TNAME_TASK_0032
#define ATK2_TNAME_OUT_0032	ATK2_TNAME_TASK_0032
#define ATK2_TID_TASK_0032	"0032"
#define ATK2_TID_IN_0032	ATK2_TID_TASK_0032
#define ATK2_TID_OUT_0032	ATK2_TID_TASK_0032
#define ATK2_WE_TASK_0032	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0032	BlsmControl_WakeupEvent

/* Task_0063: BlsmControl_System_duty_culc_Sum4 */
#define ATK2_TASK_0063		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0063	ATK2_TASK_0063
#define ATK2_TASK_OUT_0063	ATK2_TASK_0063
#define ATK2_SE_IN_0063		ATK2_TASK_0063, BlsmControl_InputEvent
#define ATK2_SE_OUT_0063	ATK2_TASK_0063, BlsmControl_OutputEvent
#define ATK2_WE_IN_0063		BlsmControl_InputEvent
#define ATK2_WE_OUT_0063	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0063	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0063	ATK2_TNAME_TASK_0063
#define ATK2_TNAME_OUT_0063	ATK2_TNAME_TASK_0063
#define ATK2_TID_TASK_0063	"0063"
#define ATK2_TID_IN_0063	ATK2_TID_TASK_0063
#define ATK2_TID_OUT_0063	ATK2_TID_TASK_0063
#define ATK2_WE_TASK_0063	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0063	BlsmControl_WakeupEvent

/* Task_0034: BlsmControl_System_duty_culc_Product1 */
#define ATK2_TASK_0034		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0034	ATK2_TASK_0034
#define ATK2_TASK_OUT_0034	ATK2_TASK_0034
#define ATK2_SE_IN_0034		ATK2_TASK_0034, BlsmControl_InputEvent
#define ATK2_SE_OUT_0034	ATK2_TASK_0034, BlsmControl_OutputEvent
#define ATK2_WE_IN_0034		BlsmControl_InputEvent
#define ATK2_WE_OUT_0034	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0034	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0034	ATK2_TNAME_TASK_0034
#define ATK2_TNAME_OUT_0034	ATK2_TNAME_TASK_0034
#define ATK2_TID_TASK_0034	"0034"
#define ATK2_TID_IN_0034	ATK2_TID_TASK_0034
#define ATK2_TID_OUT_0034	ATK2_TID_TASK_0034
#define ATK2_WE_TASK_0034	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0034	BlsmControl_WakeupEvent

/* Task_0064: BlsmControl_System_duty_culc_Sum5 */
#define ATK2_TASK_0064		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0064	ATK2_TASK_0064
#define ATK2_TASK_OUT_0064	ATK2_TASK_0064
#define ATK2_SE_IN_0064		ATK2_TASK_0064, BlsmControl_InputEvent
#define ATK2_SE_OUT_0064	ATK2_TASK_0064, BlsmControl_OutputEvent
#define ATK2_WE_IN_0064		BlsmControl_InputEvent
#define ATK2_WE_OUT_0064	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0064	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0064	ATK2_TNAME_TASK_0064
#define ATK2_TNAME_OUT_0064	ATK2_TNAME_TASK_0064
#define ATK2_TID_TASK_0064	"0064"
#define ATK2_TID_IN_0064	ATK2_TID_TASK_0064
#define ATK2_TID_OUT_0064	ATK2_TID_TASK_0064
#define ATK2_WE_TASK_0064	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0064	BlsmControl_WakeupEvent

/* Task_0062: BlsmControl_System_duty_culc_Sum3 */
#define ATK2_TASK_0062		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0062	ATK2_TASK_0062
#define ATK2_TASK_OUT_0062	ATK2_TASK_0062
#define ATK2_SE_IN_0062		ATK2_TASK_0062, BlsmControl_InputEvent
#define ATK2_SE_OUT_0062	ATK2_TASK_0062, BlsmControl_OutputEvent
#define ATK2_WE_IN_0062		BlsmControl_InputEvent
#define ATK2_WE_OUT_0062	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0062	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0062	ATK2_TNAME_TASK_0062
#define ATK2_TNAME_OUT_0062	ATK2_TNAME_TASK_0062
#define ATK2_TID_TASK_0062	"0062"
#define ATK2_TID_IN_0062	ATK2_TID_TASK_0062
#define ATK2_TID_OUT_0062	ATK2_TID_TASK_0062
#define ATK2_WE_TASK_0062	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0062	BlsmControl_WakeupEvent

/* Task_0033: BlsmControl_System_duty_culc_Divide2 */
#define ATK2_TASK_0033		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0033	ATK2_TASK_0033
#define ATK2_TASK_OUT_0033	ATK2_TASK_0033
#define ATK2_SE_IN_0033		ATK2_TASK_0033, BlsmControl_InputEvent
#define ATK2_SE_OUT_0033	ATK2_TASK_0033, BlsmControl_OutputEvent
#define ATK2_WE_IN_0033		BlsmControl_InputEvent
#define ATK2_WE_OUT_0033	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0033	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0033	ATK2_TNAME_TASK_0033
#define ATK2_TNAME_OUT_0033	ATK2_TNAME_TASK_0033
#define ATK2_TID_TASK_0033	"0033"
#define ATK2_TID_IN_0033	ATK2_TID_TASK_0033
#define ATK2_TID_OUT_0033	ATK2_TID_TASK_0033
#define ATK2_WE_TASK_0033	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0033	BlsmControl_WakeupEvent

/* Task_0016: BlsmControl_System_duty_culc_Abs1 */
#define ATK2_TASK_0016		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0016	ATK2_TASK_0016
#define ATK2_TASK_OUT_0016	ATK2_TASK_0016
#define ATK2_SE_IN_0016		ATK2_TASK_0016, BlsmControl_InputEvent
#define ATK2_SE_OUT_0016	ATK2_TASK_0016, BlsmControl_OutputEvent
#define ATK2_WE_IN_0016		BlsmControl_InputEvent
#define ATK2_WE_OUT_0016	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0016	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0016	ATK2_TNAME_TASK_0016
#define ATK2_TNAME_OUT_0016	ATK2_TNAME_TASK_0016
#define ATK2_TID_TASK_0016	"0016"
#define ATK2_TID_IN_0016	ATK2_TID_TASK_0016
#define ATK2_TID_OUT_0016	ATK2_TID_TASK_0016
#define ATK2_WE_TASK_0016	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0016	BlsmControl_WakeupEvent

/* Task_0065: BlsmControl_System_duty_culc_Switch1 */
#define ATK2_TASK_0065		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0065	ATK2_TASK_0065
#define ATK2_TASK_OUT_0065	ATK2_TASK_0065
#define ATK2_SE_IN_0065		ATK2_TASK_0065, BlsmControl_InputEvent
#define ATK2_SE_OUT_0065	ATK2_TASK_0065, BlsmControl_OutputEvent
#define ATK2_WE_IN_0065		BlsmControl_InputEvent
#define ATK2_WE_OUT_0065	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0065	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0065	ATK2_TNAME_TASK_0065
#define ATK2_TNAME_OUT_0065	ATK2_TNAME_TASK_0065
#define ATK2_TID_TASK_0065	"0065"
#define ATK2_TID_IN_0065	ATK2_TID_TASK_0065
#define ATK2_TID_OUT_0065	ATK2_TID_TASK_0065
#define ATK2_WE_TASK_0065	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0065	BlsmControl_WakeupEvent

/* Task_0035: BlsmControl_System_duty_culc_Product2 */
#define ATK2_TASK_0035		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0035	ATK2_TASK_0035
#define ATK2_TASK_OUT_0035	ATK2_TASK_0035
#define ATK2_SE_IN_0035		ATK2_TASK_0035, BlsmControl_InputEvent
#define ATK2_SE_OUT_0035	ATK2_TASK_0035, BlsmControl_OutputEvent
#define ATK2_WE_IN_0035		BlsmControl_InputEvent
#define ATK2_WE_OUT_0035	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0035	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0035	ATK2_TNAME_TASK_0035
#define ATK2_TNAME_OUT_0035	ATK2_TNAME_TASK_0035
#define ATK2_TID_TASK_0035	"0035"
#define ATK2_TID_IN_0035	ATK2_TID_TASK_0035
#define ATK2_TID_OUT_0035	ATK2_TID_TASK_0035
#define ATK2_WE_TASK_0035	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0035	BlsmControl_WakeupEvent

/* Task_0039: BlsmControl_System_duty_culc_Saturation1 */
#define ATK2_TASK_0039		BlsmControl_Core1_100_0_Task
#define ATK2_TASK_IN_0039	ATK2_TASK_0039
#define ATK2_TASK_OUT_0039	ATK2_TASK_0039
#define ATK2_SE_IN_0039		ATK2_TASK_0039, BlsmControl_InputEvent
#define ATK2_SE_OUT_0039	ATK2_TASK_0039, BlsmControl_OutputEvent
#define ATK2_WE_IN_0039		BlsmControl_InputEvent
#define ATK2_WE_OUT_0039	BlsmControl_OutputEvent
#define ATK2_TNAME_TASK_0039	"BlsmControl_Core1_100_0_Task"
#define ATK2_TNAME_IN_0039	ATK2_TNAME_TASK_0039
#define ATK2_TNAME_OUT_0039	ATK2_TNAME_TASK_0039
#define ATK2_TID_TASK_0039	"0039"
#define ATK2_TID_IN_0039	ATK2_TID_TASK_0039
#define ATK2_TID_OUT_0039	ATK2_TID_TASK_0039
#define ATK2_WE_TASK_0039	BlsmControl_WakeupEvent
#define ATK2_SE_TASK_0039	BlsmControl_WakeupEvent

/* Synchronus Memory */
#ifndef CH__SYNCM
#if defined(__v850__) && defined(__GNUC__)
#define CH__SYNCM()	__asm__ __volatile__ ("syncm" ::: "memory")
#elif defined(__nios2__) && defined(__GNUC__)
#define CH__SYNCM()	__asm__ __volatile__ ("sync" ::: "memory")
#else
#define CH__SYNCM()	do { ; } while (0)
#endif /* !__v850__ || !__nios2__ || !__GNUC__ */
#endif /* !CH__SYNCM */

/* Memory Barrier for Compiler */
#ifndef CH__MEM_BARRIER
#ifdef __GNUC__
#define CH__MEM_BARRIER()	__asm__ __volatile__ ("" ::: "memory")
#else /* !__GNUC__ */
#define CH__MEM_BARRIER()	do { ; } while (0)
#endif /* !__GNUC__ */
#endif /* !CH__MEM_BARRIER */

/* TOPPERS/ATK2 OS API Wrapper Macros */
#ifndef OS__SENDEVENT
#define OS__SENDEVENT(T,R,E)	\
  SetEvent(CH__VEC_CONCAT(ATK2_,R),CH__VEC_CONCAT(ATK2_,E))
#endif
#ifndef OS__WAITEVENT
#define OS__WAITEVENT(T,E)	\
  do { WaitEvent(CH__VEC_CONCAT(ATK2_,E));	\
       ClearEvent(CH__VEC_CONCAT(ATK2_,E)); } while (0)
#endif
#ifndef OS__SETALARM
#define OS__SETALARM(A,O,R)	\
  SetRelAlarm(CH__VEC_CONCAT(ATK2_,A),O,R)
#endif

/* TOPPERS/ATK2 Channel Macros */
#ifndef CH__TASK_SCHED
#define CH__TASK_SCHED(C,V,S)	\
  do { WaitEvent(CH__VEC_CONCAT(ATK2_WE_,C));	\
       ClearEvent(CH__VEC_CONCAT(ATK2_WE_,C)); } while (0)
#endif
#ifndef CH__CORE_THREAD_SCHED
#define CH__CORE_THREAD_SCHED(C,V,S)	CH__TASK_SCHED(C,V,S)
#endif
/* one argument wrapper macro */
#ifndef CH__SetEvent
#define CH__SetEvent(C)	SetEvent(C)
#endif
#ifndef CH__EVENT
#define CH__EVENT(T,C)	\
  do { CH__SYNCM(); \
       CH__SetEvent(CH__VEC_CONCAT(ATK2_SE_,C)); } while (0)
#endif
#ifndef CH__END_LOG
#define CH__END_LOG(C)	do { ; } while (0)
#endif

/* check channle bitvec */
static __inline__ int
ch__bitcheck (CH__VEC_ELEMENT_TYPE *v, size_t b)
{
  return (v[b/CH__VEC_ELEMENT_BITS]
          & (CH__VEC_ELEMENT_1 << (b % CH__VEC_ELEMENT_BITS)));
}

/* set channle bitvec */
static __inline__ void
ch__bitset (CH__VEC_ELEMENT_TYPE *v, size_t b)
{
  v[b/CH__VEC_ELEMENT_BITS]
    |= (CH__VEC_ELEMENT_1 << (b % CH__VEC_ELEMENT_BITS));
}

/* completed channel wait */
static __inline__ int
ch__completed (CH__VEC_ELEMENT_TYPE *v, size_t n)
{
  const size_t restbits = n % CH__VEC_ELEMENT_BITS;
  size_t i;
  size_t e = (n - 1) / CH__VEC_ELEMENT_BITS;
  CH__VEC_ELEMENT_TYPE const
    restmask =  ((restbits == 0)
		 ? (~CH__VEC_ELEMENT_0)
		 : ((CH__VEC_ELEMENT_1 << restbits) - 1));
  for (i = 0; i < e; i++, v++)
    {
      if (*v != ~(CH__VEC_ELEMENT_0))
	return 0;
    }
  if (*v != restmask)
    return 0;
  return 1;
}

/*
 * Channels
 */

/* From: BlsmControl_pos
   To: BlsmControl_System_pos_to_theta_MultiportSwitch */
typedef struct {
  int flag__0002_0088;
  real_T BlsmControl_pos_1;
} TYPE_BlsmControl_0002_0088;
TYPE_BlsmControl_0002_0088 CH_BlsmControl_0002_0088;

/* From: BlsmControl_System_duty_culc_Abs
   To: BlsmControl_System_duty_culc_Zero_OrderHold4
    [1] -> [100] */
typedef struct {
  int flag__0015_0076;
  unsigned long time__0015_0076;
  real_T BlsmControl_System_duty_culc_Abs_1;
} TYPE_BlsmControl_0015_0076;
TYPE_BlsmControl_0015_0076 CH_BlsmControl_0015_0076;

/* From: BlsmControl_System_duty_culc_Product3
   To: BlsmControl_System_duty_culc_RateTransition3
    [1] -> [100] */
typedef struct {
  int flag__0036_0038;
  unsigned long time__0036_0038;
  real_T BlsmControl_System_duty_culc_Product3_1;
} TYPE_BlsmControl_0036_0038;
TYPE_BlsmControl_0036_0038 CH_BlsmControl_0036_0038;

/* From: BlsmControl_System_duty_culc_RateTransition2
   To: BlsmControl_System_duty_culc_Switch6 */
typedef struct {
  int flag__0037_0072;
  real_T BlsmControl_System_duty_culc_RateTransition2_1;
} TYPE_BlsmControl_0037_0072;
TYPE_BlsmControl_0037_0072 CH_BlsmControl_0037_0072;

/* From: BlsmControl_System_duty_culc_RateTransition3
   To: BlsmControl_System_duty_culc_Product2 */
typedef struct {
  int flag__0038_0035;
  real_T BlsmControl_System_duty_culc_RateTransition3_1;
} TYPE_BlsmControl_0038_0035;
TYPE_BlsmControl_0038_0035 CH_BlsmControl_0038_0035;

/* From: BlsmControl_System_duty_culc_Saturation1
   To: BlsmControl_System_duty_culc_RateTransition2
    [100] -> [1] */
typedef struct {
  int flag__0039_0037;
  unsigned long time__0039_0037;
  real_T BlsmControl_System_duty_culc_Saturation1_1;
} TYPE_BlsmControl_0039_0037;
TYPE_BlsmControl_0039_0037 CH_BlsmControl_0039_0037;

/* From: BlsmControl_System_duty_culc_Subsystem1_Divide
   To: BlsmControl_System_duty_culc_Zero_OrderHold5
    [1] -> [100] */
typedef struct {
  int flag__0043_0077;
  unsigned long time__0043_0077;
  real_T BlsmControl_System_duty_culc_Subsystem1_Divide_1;
} TYPE_BlsmControl_0043_0077;
TYPE_BlsmControl_0043_0077 CH_BlsmControl_0043_0077;

CORE_TASK_CODE_PREPARE(0)
/* rate: 1 */
/* Core#0: Rate: 1 */
TASK(BlsmControl_Core0_1_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0103,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0103,4);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0127,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0127,2);

  /* input channel bit vector */
  CH__VEC(IN_0121,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0121,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0117,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0117,1); */

  /* input channel bit vector */
  CH__VEC(IN_0120,2);

  /* output channel bit vector */
  CH__VEC(OUT_0120,3);

  /* input channel bit vector */
  CH__VEC(IN_0108,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0108,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0095,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0095,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0094,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0094,1); */

  /* input channel bit vector */
  CH__VEC(IN_0096,2);

  /* output channel bit vector */
  CH__VEC(OUT_0096,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0075,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0075,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0066,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0066,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0067,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0067,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0071,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0071,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0049,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0049,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0051,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0051,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0048,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0048,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0047,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0047,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0059,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0059,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0058,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0058,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0057,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0057,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0056,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0056,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0055,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0055,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0054,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0054,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0053,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0053,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0052,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0052,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0046,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0046,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0045,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0045,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0073,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0073,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0060,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0060,2);

  /* input channel bit vector */
  CH__VEC(IN_0044,15);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0044,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0043,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0043,1); */

  /* input channel bit vector */
  CH__VEC(IN_0072,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0072,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0070,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0070,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0069,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0069,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0068,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0068,1); */

  /* input channel bit vector */
  CH__VEC(IN_0036,4);

  /* output channel bit vector */
  CH__VEC(OUT_0036,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0101,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0101,1); */

  /* input channel bit vector */
  CH__VEC(IN_0102,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0102,1); */

  /* input channel bit vector */
  CH__VEC(IN_0126,3);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0126,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0186,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0002,3);

  /* states */
  struct {
    real_T UnitDelay_DSTATE;
    real_T UnitDelay1_DSTATE;
    real_T UnitDelay_DSTATE_g;
    real_T UnitDelay12_DSTATE;
    real_T UnitDelay14_DSTATE;
    real_T UnitDelay11_DSTATE;
    real_T UnitDelay10_DSTATE;
    real_T UnitDelay9_DSTATE;
    real_T UnitDelay8_DSTATE;
    real_T UnitDelay7_DSTATE;
    real_T UnitDelay6_DSTATE;
    real_T UnitDelay5_DSTATE;
    real_T UnitDelay4_DSTATE;
    real_T UnitDelay3_DSTATE;
    real_T UnitDelay2_DSTATE;
    real_T UnitDelay1_DSTATE_g;
    real_T UnitDelay_DSTATE_f;
    real_T UnitDelay13_DSTATE;
  } BlsmControl_DW;

  /* params */
  struct {
    real_T UnitDelay_InitialCondition;
    real_T UnitDelay1_InitialCondition;
    real_T CompareToConstant_const;
    real_T UnitDelay_InitialCondition_h;
    real_T Gain_Gain;
    real_T Constant1_Value_d;
    real_T Switch_Threshold;
    real_T Constant_Value_k;
    real_T Switch16_Threshold;
    real_T Constant25_Value;
    real_T Switch17_Threshold;
    real_T Constant27_Value;
    real_T Switch5_Threshold;
    real_T Constant9_Value;
    real_T rpm_ast;
    real_T Switch8_Threshold;
    real_T Constant12_Value;
    real_T Constant_Value_b;
    real_T Switch6_Threshold;
    real_T Constant10_Value;
    real_T Switch4_Threshold;
    real_T Constant1_Value;
    real_T Constant8_Value;
    real_T Switch3_Threshold;
    real_T Constant26_Value;
    real_T Constant6_Value;
    real_T Switch2_Threshold;
    real_T Constant24_Value;
    real_T Constant7_Value;
    real_T Constant_Value_e;
    real_T Constant5_Value_k;
  } BlsmControl_P = {
  5001.0,                              /* Expression: 5001
                                        * Referenced by: '<S7>/UnitDelay'
                                        */

  5001.0,                              /* Expression: 5001
                                        * Referenced by: '<S10>/UnitDelay1'
                                        */

  5000.0,                              /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S11>/Constant'
                                        */

  5001.0,                              /* Expression: 5001
                                        * Referenced by: '<S8>/UnitDelay'
                                        */

  1.0E-5,                              /* Expression: 1e-5
                                        * Referenced by: '<S4>/Gain'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S4>/Constant1'
                                        */

  5000.0,                              /* Expression: 5000
                                        * Referenced by: '<S4>/Switch'
                                        */

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S4>/Constant'
                                        */

  100.0,                               /* Expression: 100
                                        * Referenced by: '<S2>/Switch16'
                                        */

  100000.0,                            /* Expression: 100000
                                        * Referenced by: '<S2>/Constant25'
                                        */

  500.0,                               /* Expression: 500
                                        * Referenced by: '<S2>/Switch17'
                                        */

  100000.0,                            /* Expression: 100000
                                        * Referenced by: '<S2>/Constant27'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S2>/Switch5'
                                        */

  100000.0,                            /* Expression: 100000
                                        * Referenced by: '<S2>/Constant9'
                                        */

  1000.0,                              /* Variable: rpm_ast
                                        * Referenced by:
                                        *   '<S6>/UnitDelay'
                                        *   '<S6>/UnitDelay1'
                                        *   '<S6>/UnitDelay10'
                                        *   '<S6>/UnitDelay11'
                                        *   '<S6>/UnitDelay12'
                                        *   '<S6>/UnitDelay13'
                                        *   '<S6>/UnitDelay14'
                                        *   '<S6>/UnitDelay2'
                                        *   '<S6>/UnitDelay3'
                                        *   '<S6>/UnitDelay4'
                                        *   '<S6>/UnitDelay5'
                                        *   '<S6>/UnitDelay6'
                                        *   '<S6>/UnitDelay7'
                                        *   '<S6>/UnitDelay8'
                                        *   '<S6>/UnitDelay9'
                                        */

  20.0,                                /* Expression: 20
                                        * Referenced by: '<S2>/Switch8'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S2>/Constant12'
                                        */

  16.0,                                /* Expression: 16
                                        * Referenced by: '<S6>/Constant'
                                        */

  20000.0,                             /* Expression: 20000
                                        * Referenced by: '<S2>/Switch6'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant10'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S2>/Switch4'
                                        */

  0.2,                                 /* Expression: 0.200
                                        * Referenced by: '<S2>/Constant1'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant8'
                                        */

  500.0,                               /* Expression: 500
                                        * Referenced by: '<S2>/Switch3'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant26'
                                        */

  0.17,                                /* Expression: 0.170
                                        * Referenced by: '<S2>/Constant6'
                                        */

  100.0,                               /* Expression: 100
                                        * Referenced by: '<S2>/Switch2'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant24'
                                        */

  0.15,                                /* Expression: 0.150
                                        * Referenced by: '<S2>/Constant7'
                                        */

  1.0                                  /* Expression: 1
                                        * Referenced by: '<S7>/Constant'
                                        */,

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S7>/Constant5'
                                        */

  };

  /* input/output/auto variables */
  real_T BlsmControl_System_rpm_culc_counter_Switch_1;
  real_T BlsmControl_System_rpm_culc_counter_UnitDelay_1;
  real_T BlsmControl_System_rpm_culc_update_prev_pos_Switch3_1;
  real_T BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1_1;
  real_T BlsmControl_pos_1;
  boolean_T BlsmControl_System_rpm_culc_if_RelationalOperator_1;
  boolean_T BlsmControl_System_rpm_culc_if_CompareToConstant_Compare_1;
  boolean_T BlsmControl_System_rpm_culc_if_LogicalOperator_1;
  real_T BlsmControl_System_rpm_culc_culc_div_num_UnitDelay_1;
  real_T BlsmControl_System_rpm_culc_culc_div_num_Switch1_1;
  real_T BlsmControl_System_rpm_culc_Gain_1;
  real_T BlsmControl_System_rpm_culc_Divide_1;
  real_T BlsmControl_System_rpm_culc_Switch_1;
  real_T BlsmControl_System_duty_culc_Zero_OrderHold1_1;
  real_T BlsmControl_System_duty_culc_Switch16_1;
  real_T BlsmControl_System_duty_culc_Switch17_1;
  real_T BlsmControl_System_duty_culc_Switch5_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay14_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay12_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay11_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay10_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay9_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay8_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay7_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay6_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay5_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay4_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay3_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay2_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay1_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold_1;
  real_T BlsmControl_System_duty_culc_Switch8_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_UnitDelay13_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_Subtract_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_Divide_1;
  real_T BlsmControl_System_duty_culc_RateTransition2_1;
  real_T BlsmControl_System_duty_culc_Switch6_1;
  real_T BlsmControl_System_duty_culc_Switch4_1;
  real_T BlsmControl_System_duty_culc_Switch3_1;
  real_T BlsmControl_System_duty_culc_Switch2_1;
  real_T BlsmControl_System_duty_culc_Product3_1;
  real_T BlsmControl_System_rpm_culc_counter_Sum_1;

#ifdef TASK_BlsmControl_Core0_1_0_Task_DEFINITIONS
  TASK_BlsmControl_Core0_1_0_Task_DEFINITIONS
#endif

#ifdef TASK_0103_DEFINITIONS
  TASK_0103_DEFINITIONS
#endif

#ifdef TASK_0127_DEFINITIONS
  TASK_0127_DEFINITIONS
#endif

#ifdef TASK_0121_DEFINITIONS
  TASK_0121_DEFINITIONS
#endif

#ifdef TASK_0117_DEFINITIONS
  TASK_0117_DEFINITIONS
#endif

#ifdef TASK_0120_DEFINITIONS
  TASK_0120_DEFINITIONS
#endif

#ifdef TASK_0108_DEFINITIONS
  TASK_0108_DEFINITIONS
#endif

#ifdef TASK_0095_DEFINITIONS
  TASK_0095_DEFINITIONS
#endif

#ifdef TASK_0094_DEFINITIONS
  TASK_0094_DEFINITIONS
#endif

#ifdef TASK_0096_DEFINITIONS
  TASK_0096_DEFINITIONS
#endif

#ifdef TASK_0075_DEFINITIONS
  TASK_0075_DEFINITIONS
#endif

#ifdef TASK_0066_DEFINITIONS
  TASK_0066_DEFINITIONS
#endif

#ifdef TASK_0067_DEFINITIONS
  TASK_0067_DEFINITIONS
#endif

#ifdef TASK_0071_DEFINITIONS
  TASK_0071_DEFINITIONS
#endif

#ifdef TASK_0049_DEFINITIONS
  TASK_0049_DEFINITIONS
#endif

#ifdef TASK_0051_DEFINITIONS
  TASK_0051_DEFINITIONS
#endif

#ifdef TASK_0048_DEFINITIONS
  TASK_0048_DEFINITIONS
#endif

#ifdef TASK_0047_DEFINITIONS
  TASK_0047_DEFINITIONS
#endif

#ifdef TASK_0059_DEFINITIONS
  TASK_0059_DEFINITIONS
#endif

#ifdef TASK_0058_DEFINITIONS
  TASK_0058_DEFINITIONS
#endif

#ifdef TASK_0057_DEFINITIONS
  TASK_0057_DEFINITIONS
#endif

#ifdef TASK_0056_DEFINITIONS
  TASK_0056_DEFINITIONS
#endif

#ifdef TASK_0055_DEFINITIONS
  TASK_0055_DEFINITIONS
#endif

#ifdef TASK_0054_DEFINITIONS
  TASK_0054_DEFINITIONS
#endif

#ifdef TASK_0053_DEFINITIONS
  TASK_0053_DEFINITIONS
#endif

#ifdef TASK_0052_DEFINITIONS
  TASK_0052_DEFINITIONS
#endif

#ifdef TASK_0046_DEFINITIONS
  TASK_0046_DEFINITIONS
#endif

#ifdef TASK_0045_DEFINITIONS
  TASK_0045_DEFINITIONS
#endif

#ifdef TASK_0073_DEFINITIONS
  TASK_0073_DEFINITIONS
#endif

#ifdef TASK_0060_DEFINITIONS
  TASK_0060_DEFINITIONS
#endif

#ifdef TASK_0044_DEFINITIONS
  TASK_0044_DEFINITIONS
#endif

#ifdef TASK_0043_DEFINITIONS
  TASK_0043_DEFINITIONS
#endif

#ifdef TASK_0072_DEFINITIONS
  TASK_0072_DEFINITIONS
#endif

#ifdef TASK_0070_DEFINITIONS
  TASK_0070_DEFINITIONS
#endif

#ifdef TASK_0069_DEFINITIONS
  TASK_0069_DEFINITIONS
#endif

#ifdef TASK_0068_DEFINITIONS
  TASK_0068_DEFINITIONS
#endif

#ifdef TASK_0036_DEFINITIONS
  TASK_0036_DEFINITIONS
#endif

#ifdef TASK_0101_DEFINITIONS
  TASK_0101_DEFINITIONS
#endif

#ifdef TASK_0102_DEFINITIONS
  TASK_0102_DEFINITIONS
#endif

#ifdef TASK_0126_DEFINITIONS
  TASK_0126_DEFINITIONS
#endif

#ifdef TASK_0186_DEFINITIONS
  TASK_0186_DEFINITIONS
#endif

#ifdef TASK_0002_DEFINITIONS
  TASK_0002_DEFINITIONS
#endif

  /* initialize state structure */
  memset(&BlsmControl_DW, 0, sizeof BlsmControl_DW);

  /* Block: BlsmControl_System_rpm_culc_counter_UnitDelay */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S7>/UnitDelay' */
  BlsmControl_DW.UnitDelay_DSTATE = BlsmControl_P.UnitDelay_InitialCondition;

}

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_counter_UnitDelay_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S10>/UnitDelay1' */
  BlsmControl_DW.UnitDelay1_DSTATE = BlsmControl_P.UnitDelay1_InitialCondition;

}

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_if_RelationalOperator */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_if_RelationalOperator_1 = false;
}

  /* Block: BlsmControl_System_rpm_culc_if_CompareToConstant_Compare */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_if_CompareToConstant_Compare_1 = false;
}

  /* Block: BlsmControl_System_rpm_culc_if_LogicalOperator */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_if_LogicalOperator_1 = false;

}

  /* Block: BlsmControl_System_rpm_culc_culc_div_num_Switch1 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S8>/UnitDelay' */
  BlsmControl_DW.UnitDelay_DSTATE_g = BlsmControl_P.UnitDelay_InitialCondition_h;

}

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_culc_div_num_UnitDelay_1 = 0.0;
}
{
  BlsmControl_System_rpm_culc_culc_div_num_Switch1_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_Gain */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_Gain_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_Divide */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_Divide_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_Switch */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_Switch_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Zero_OrderHold1 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Zero_OrderHold1_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch16 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch16_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch17 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch17_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch5 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch5_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay12 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay12' */
  BlsmControl_DW.UnitDelay12_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay12_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay14 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay14' */
  BlsmControl_DW.UnitDelay14_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay14_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay11 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay11' */
  BlsmControl_DW.UnitDelay11_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay11_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay10 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay10' */
  BlsmControl_DW.UnitDelay10_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay10_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay9 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay9' */
  BlsmControl_DW.UnitDelay9_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay9_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay8 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay8' */
  BlsmControl_DW.UnitDelay8_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay8_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay7 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay7' */
  BlsmControl_DW.UnitDelay7_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay7_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay6 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay6' */
  BlsmControl_DW.UnitDelay6_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay6_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay5 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay5' */
  BlsmControl_DW.UnitDelay5_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay5_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay4 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay4' */
  BlsmControl_DW.UnitDelay4_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay4_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay3 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay3' */
  BlsmControl_DW.UnitDelay3_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay3_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay2 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay2' */
  BlsmControl_DW.UnitDelay2_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay2_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay1 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay1' */
  BlsmControl_DW.UnitDelay1_DSTATE_g = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay1_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay' */
  BlsmControl_DW.UnitDelay_DSTATE_f = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch8 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch8_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_Subtract */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay13' */
  BlsmControl_DW.UnitDelay13_DSTATE = BlsmControl_P.rpm_ast;

}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay13_1 = 0.0;
}
{
  BlsmControl_System_duty_culc_Subsystem1_Subtract_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Subsystem1_Divide */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Subsystem1_Divide_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch6 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch6_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch4 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch4_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch3 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch3_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch2 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch2_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Product3 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Product3_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_counter_Sum */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_counter_Sum_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_counter_Switch */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_counter_Switch_1 = 0.0;
}

  /* Block: BlsmControl_System_rpm_culc_update_prev_pos_Switch3 */

  /* initialize task context */
{
  BlsmControl_System_rpm_culc_update_prev_pos_Switch3_1 = 0.0;
}

  /* Block: BlsmControl_duty */

  /* Block: BlsmControl_pos */

  /* initialize task context */
{
  BlsmControl_pos_1 = 0.0;
}

#ifdef TASK_INITIALIZE_0103_COMPLETED
  TASK_INITIALIZE_0103_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0127_COMPLETED
  TASK_INITIALIZE_0127_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0121_COMPLETED
  TASK_INITIALIZE_0121_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0117_COMPLETED
  TASK_INITIALIZE_0117_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0120_COMPLETED
  TASK_INITIALIZE_0120_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0108_COMPLETED
  TASK_INITIALIZE_0108_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0095_COMPLETED
  TASK_INITIALIZE_0095_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0094_COMPLETED
  TASK_INITIALIZE_0094_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0096_COMPLETED
  TASK_INITIALIZE_0096_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0075_COMPLETED
  TASK_INITIALIZE_0075_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0066_COMPLETED
  TASK_INITIALIZE_0066_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0067_COMPLETED
  TASK_INITIALIZE_0067_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0071_COMPLETED
  TASK_INITIALIZE_0071_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0049_COMPLETED
  TASK_INITIALIZE_0049_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0051_COMPLETED
  TASK_INITIALIZE_0051_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0048_COMPLETED
  TASK_INITIALIZE_0048_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0047_COMPLETED
  TASK_INITIALIZE_0047_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0059_COMPLETED
  TASK_INITIALIZE_0059_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0058_COMPLETED
  TASK_INITIALIZE_0058_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0057_COMPLETED
  TASK_INITIALIZE_0057_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0056_COMPLETED
  TASK_INITIALIZE_0056_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0055_COMPLETED
  TASK_INITIALIZE_0055_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0054_COMPLETED
  TASK_INITIALIZE_0054_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0053_COMPLETED
  TASK_INITIALIZE_0053_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0052_COMPLETED
  TASK_INITIALIZE_0052_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0046_COMPLETED
  TASK_INITIALIZE_0046_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0045_COMPLETED
  TASK_INITIALIZE_0045_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0073_COMPLETED
  TASK_INITIALIZE_0073_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0060_COMPLETED
  TASK_INITIALIZE_0060_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0044_COMPLETED
  TASK_INITIALIZE_0044_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0043_COMPLETED
  TASK_INITIALIZE_0043_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0072_COMPLETED
  TASK_INITIALIZE_0072_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0070_COMPLETED
  TASK_INITIALIZE_0070_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0069_COMPLETED
  TASK_INITIALIZE_0069_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0068_COMPLETED
  TASK_INITIALIZE_0068_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0036_COMPLETED
  TASK_INITIALIZE_0036_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0101_COMPLETED
  TASK_INITIALIZE_0101_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0102_COMPLETED
  TASK_INITIALIZE_0102_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0126_COMPLETED
  TASK_INITIALIZE_0126_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0186_COMPLETED
  TASK_INITIALIZE_0186_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0002_COMPLETED
  TASK_INITIALIZE_0002_COMPLETED();
#endif

  /*
   * Set Alarm and Send start event
   */

  /* BlsmControl_Core0_1_0_Task */
  OS__SETALARM(ALARM_BlsmControl_Core0_1_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 1,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 1);

  /* BlsmControl_Core1_1_0_Task */
  OS__SETALARM(ALARM_BlsmControl_Core1_1_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 1,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 1);
  OS__SENDEVENT(TASK_BlsmControl_Core0_1_0_Task,
		TASK_BlsmControl_Core1_1_0_Task,
		SE_BlsmControl_Core1_1_0_Task);

  /* BlsmControl_Core0_100_0_Task */
  OS__SETALARM(ALARM_BlsmControl_Core0_100_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 100,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 100);
  OS__SENDEVENT(TASK_BlsmControl_Core0_1_0_Task,
		TASK_BlsmControl_Core0_100_0_Task,
		SE_BlsmControl_Core0_100_0_Task);

  /* BlsmControl_Core1_100_0_Task */
  OS__SETALARM(ALARM_BlsmControl_Core1_100_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 100,
	       (SwCnt0_TIMER_CLOCK_HZ / 10000) * 100);
  OS__SENDEVENT(TASK_BlsmControl_Core0_1_0_Task,
		TASK_BlsmControl_Core1_100_0_Task,
		SE_BlsmControl_Core1_100_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_BlsmControl_Core0_1_0_Task_LOOP_TOP
    TASK_BlsmControl_Core0_1_0_Task_LOOP_TOP
#endif

    /*
     * Block: BlsmControl_pos
     * Task: 0002
     * rate: 1
     */

    /****************************************************
     *
     * You should insert some codes here for external input.
     *
     ****************************************************/
/* >>>>>>>>>>>>>>>>>>>>>> */
/* add code  by takematsu */
/* >>>>>>>>>>>>>>>>>>>>>> */
      /* pos_input */
      {
        unsigned short pos;
        unsigned short ppr20_reg;
        unsigned short ppr9_reg;

        ppr20_reg = sil_reh_mem((void *) PPR(20));
        ppr9_reg = sil_reh_mem((void *) PPR(9));

        pos = ((ppr20_reg & 0x02) >> 1) | ((ppr20_reg & 0x10) >> 3) | (ppr9_reg & 0x04);

        BlsmControl_pos_1 = pos;
      }
/* <<<<<<<<<<<<<<<<<<<<<< */
/* add code  by takematsu */
/* <<<<<<<<<<<<<<<<<<<<<< */

#ifdef TASK_EXECUTE_0002_COMPLETED
    TASK_EXECUTE_0002_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_pos_to_theta_MultiportSwitch */
      if (CH_BlsmControl_0002_0088.flag__0002_0088 == 0) {
  CH_BlsmControl_0002_0088.BlsmControl_pos_1 = BlsmControl_pos_1;
  CH__SYNCM();
  CH_BlsmControl_0002_0088.flag__0002_0088 = 1;
  CH__EVENT(TASK_0002,IN_0088);
  CH__END_LOG(OUT_0002);
  break;
      }

      /* skip local output CH_0002_0121: BlsmControl_System_rpm_culc_if_RelationalOperator */

      /* skip local output CH_0002_0126: BlsmControl_System_rpm_culc_update_prev_pos_Switch3 */

      CH__TASK_SCHED(OUT_0002,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0002_COMPLETED
    TASK_OUT_0002_COMPLETED();
#endif

    /*
     * Block: BlsmControl_System_rpm_culc_counter_UnitDelay
     * Task: 0103
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S7>/UnitDelay' */
  BlsmControl_System_rpm_culc_counter_UnitDelay_1 =
    BlsmControl_DW.UnitDelay_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0103_COMPLETED
    TASK_EXECUTE_0103_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1
     * Task: 0127
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S10>/UnitDelay1' */
  BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1_1 =
    BlsmControl_DW.UnitDelay1_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0127_COMPLETED
    TASK_EXECUTE_0127_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_if_RelationalOperator
     * Task: 0121
     * rate: 1
     */

    /* C code */
    {
{

  /* RelationalOperator: '<S9>/RelationalOperator' incorporates:
   *  Inport: '<Root>/pos'
   */
  BlsmControl_System_rpm_culc_if_RelationalOperator_1 = (BlsmControl_pos_1 !=
    BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1_1);


}

    }

#ifdef TASK_EXECUTE_0121_COMPLETED
    TASK_EXECUTE_0121_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_if_CompareToConstant_Compare
     * Task: 0117
     * rate: 1
     */

    /* C code */
    {
{

  /* RelationalOperator: '<S11>/Compare' incorporates:
   *  Constant: '<S11>/Constant'
   */
  BlsmControl_System_rpm_culc_if_CompareToConstant_Compare_1 =
    (BlsmControl_System_rpm_culc_counter_UnitDelay_1 >
     BlsmControl_P.CompareToConstant_const);


}

    }

#ifdef TASK_EXECUTE_0117_COMPLETED
    TASK_EXECUTE_0117_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_if_LogicalOperator
     * Task: 0120
     * rate: 1
     */

    /* C code */
    {
{

  /* Logic: '<S9>/LogicalOperator' */
  BlsmControl_System_rpm_culc_if_LogicalOperator_1 =
    (BlsmControl_System_rpm_culc_if_RelationalOperator_1 ||
     BlsmControl_System_rpm_culc_if_CompareToConstant_Compare_1);


}

    }

#ifdef TASK_EXECUTE_0120_COMPLETED
    TASK_EXECUTE_0120_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_culc_div_num_Switch1
     * Task: 0108
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S8>/UnitDelay' */
  BlsmControl_System_rpm_culc_culc_div_num_UnitDelay_1 =
    BlsmControl_DW.UnitDelay_DSTATE_g;


}

{

  /* Switch: '<S8>/Switch1' */
  if (BlsmControl_System_rpm_culc_if_LogicalOperator_1) {
    BlsmControl_System_rpm_culc_culc_div_num_Switch1_1 =
      BlsmControl_System_rpm_culc_counter_UnitDelay_1;
  } else {
    BlsmControl_System_rpm_culc_culc_div_num_Switch1_1 =
      BlsmControl_System_rpm_culc_culc_div_num_UnitDelay_1;
  }

  /* End of Switch: '<S8>/Switch1' */

}

    }

#ifdef TASK_EXECUTE_0108_COMPLETED
    TASK_EXECUTE_0108_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_Gain
     * Task: 0095
     * rate: 1
     */

    /* C code */
    {
{

  /* Gain: '<S4>/Gain' */
  BlsmControl_System_rpm_culc_Gain_1 = BlsmControl_P.Gain_Gain *
    BlsmControl_System_rpm_culc_culc_div_num_Switch1_1;


}

    }

#ifdef TASK_EXECUTE_0095_COMPLETED
    TASK_EXECUTE_0095_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_Divide
     * Task: 0094
     * rate: 1
     */

    /* C code */
    {
{

  /* Product: '<S4>/Divide' incorporates:
   *  Constant: '<S4>/Constant1'
   */
  BlsmControl_System_rpm_culc_Divide_1 = BlsmControl_P.Constant1_Value_d /
    BlsmControl_System_rpm_culc_Gain_1;


}

    }

#ifdef TASK_EXECUTE_0094_COMPLETED
    TASK_EXECUTE_0094_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_Switch
     * Task: 0096
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S4>/Constant'
   */
  if (BlsmControl_System_rpm_culc_counter_UnitDelay_1 >
      BlsmControl_P.Switch_Threshold) {
    BlsmControl_System_rpm_culc_Switch_1 = BlsmControl_P.Constant_Value_k;
  } else {
    BlsmControl_System_rpm_culc_Switch_1 = BlsmControl_System_rpm_culc_Divide_1;
  }

  /* End of Switch: '<S4>/Switch' */

}

    }

#ifdef TASK_EXECUTE_0096_COMPLETED
    TASK_EXECUTE_0096_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Zero_OrderHold1
     * Task: 0075
     * rate: 1
     */

    /* C code */
    {
{

  /* ZeroOrderHold: '<S2>/Zero_OrderHold1' */
  BlsmControl_System_duty_culc_Zero_OrderHold1_1 =
    BlsmControl_System_rpm_culc_Switch_1;


}

    }

#ifdef TASK_EXECUTE_0075_COMPLETED
    TASK_EXECUTE_0075_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch16
     * Task: 0066
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch16' incorporates:
   *  Constant: '<S2>/Constant25'
   */
  if (BlsmControl_System_duty_culc_Zero_OrderHold1_1 >
      BlsmControl_P.Switch16_Threshold) {
    BlsmControl_System_duty_culc_Switch16_1 =
      BlsmControl_System_duty_culc_Zero_OrderHold1_1;
  } else {
    BlsmControl_System_duty_culc_Switch16_1 = BlsmControl_P.Constant25_Value;
  }

  /* End of Switch: '<S2>/Switch16' */

}

    }

#ifdef TASK_EXECUTE_0066_COMPLETED
    TASK_EXECUTE_0066_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch17
     * Task: 0067
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch17' incorporates:
   *  Constant: '<S2>/Constant27'
   */
  if (BlsmControl_System_duty_culc_Switch16_1 > BlsmControl_P.Switch17_Threshold)
  {
    BlsmControl_System_duty_culc_Switch17_1 =
      BlsmControl_System_duty_culc_Switch16_1;
  } else {
    BlsmControl_System_duty_culc_Switch17_1 = BlsmControl_P.Constant27_Value;
  }

  /* End of Switch: '<S2>/Switch17' */

}

    }

#ifdef TASK_EXECUTE_0067_COMPLETED
    TASK_EXECUTE_0067_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch5
     * Task: 0071
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch5' incorporates:
   *  Constant: '<S2>/Constant9'
   */
  if (BlsmControl_System_duty_culc_Switch17_1 > BlsmControl_P.Switch5_Threshold)
  {
    BlsmControl_System_duty_culc_Switch5_1 =
      BlsmControl_System_duty_culc_Switch17_1;
  } else {
    BlsmControl_System_duty_culc_Switch5_1 = BlsmControl_P.Constant9_Value;
  }

  /* End of Switch: '<S2>/Switch5' */

}

    }

#ifdef TASK_EXECUTE_0071_COMPLETED
    TASK_EXECUTE_0071_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay12
     * Task: 0049
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay12' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay12_1 =
    BlsmControl_DW.UnitDelay12_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0049_COMPLETED
    TASK_EXECUTE_0049_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay14
     * Task: 0051
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay14' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay14_1 =
    BlsmControl_DW.UnitDelay14_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0051_COMPLETED
    TASK_EXECUTE_0051_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay11
     * Task: 0048
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay11' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay11_1 =
    BlsmControl_DW.UnitDelay11_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0048_COMPLETED
    TASK_EXECUTE_0048_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay10
     * Task: 0047
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay10' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay10_1 =
    BlsmControl_DW.UnitDelay10_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0047_COMPLETED
    TASK_EXECUTE_0047_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay9
     * Task: 0059
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay9' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay9_1 =
    BlsmControl_DW.UnitDelay9_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0059_COMPLETED
    TASK_EXECUTE_0059_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay8
     * Task: 0058
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay8' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay8_1 =
    BlsmControl_DW.UnitDelay8_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0058_COMPLETED
    TASK_EXECUTE_0058_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay7
     * Task: 0057
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay7' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay7_1 =
    BlsmControl_DW.UnitDelay7_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0057_COMPLETED
    TASK_EXECUTE_0057_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay6
     * Task: 0056
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay6' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay6_1 =
    BlsmControl_DW.UnitDelay6_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0056_COMPLETED
    TASK_EXECUTE_0056_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay5
     * Task: 0055
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay5' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay5_1 =
    BlsmControl_DW.UnitDelay5_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0055_COMPLETED
    TASK_EXECUTE_0055_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay4
     * Task: 0054
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay4' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay4_1 =
    BlsmControl_DW.UnitDelay4_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0054_COMPLETED
    TASK_EXECUTE_0054_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay3
     * Task: 0053
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay3' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay3_1 =
    BlsmControl_DW.UnitDelay3_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0053_COMPLETED
    TASK_EXECUTE_0053_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay2
     * Task: 0052
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay2' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay2_1 =
    BlsmControl_DW.UnitDelay2_DSTATE;


}

    }

#ifdef TASK_EXECUTE_0052_COMPLETED
    TASK_EXECUTE_0052_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay1
     * Task: 0046
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay1' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay1_1 =
    BlsmControl_DW.UnitDelay1_DSTATE_g;


}

    }

#ifdef TASK_EXECUTE_0046_COMPLETED
    TASK_EXECUTE_0046_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay
     * Task: 0045
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay_1 =
    BlsmControl_DW.UnitDelay_DSTATE_f;


}

    }

#ifdef TASK_EXECUTE_0045_COMPLETED
    TASK_EXECUTE_0045_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch8
     * Task: 0073
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch8' incorporates:
   *  Constant: '<S2>/Constant12'
   */
  if (BlsmControl_System_rpm_culc_Switch_1 > BlsmControl_P.Switch8_Threshold) {
    BlsmControl_System_duty_culc_Switch8_1 =
      BlsmControl_System_rpm_culc_Switch_1;
  } else {
    BlsmControl_System_duty_culc_Switch8_1 = BlsmControl_P.Constant12_Value;
  }

  /* End of Switch: '<S2>/Switch8' */

}

    }

#ifdef TASK_EXECUTE_0073_COMPLETED
    TASK_EXECUTE_0073_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold
     * Task: 0060
     * rate: 1
     */

    /* C code */
    {
{

  /* ZeroOrderHold: '<S6>/Zero_OrderHold' */
  BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold_1 =
    BlsmControl_System_duty_culc_Switch8_1;


}

    }

#ifdef TASK_EXECUTE_0060_COMPLETED
    TASK_EXECUTE_0060_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_Subtract
     * Task: 0044
     * rate: 1
     */

    /* C code */
    {
{

  /* UnitDelay: '<S6>/UnitDelay13' */
  BlsmControl_System_duty_culc_Subsystem1_UnitDelay13_1 =
    BlsmControl_DW.UnitDelay13_DSTATE;


}

{

  /* Sum: '<S6>/Subtract' */
  BlsmControl_System_duty_culc_Subsystem1_Subtract_1 =
    ((((((((((((((BlsmControl_System_duty_culc_Subsystem1_UnitDelay13_1 +
                  BlsmControl_System_duty_culc_Subsystem1_UnitDelay12_1) +
                 BlsmControl_System_duty_culc_Subsystem1_UnitDelay14_1) +
                BlsmControl_System_duty_culc_Subsystem1_UnitDelay11_1) +
               BlsmControl_System_duty_culc_Subsystem1_UnitDelay10_1) +
              BlsmControl_System_duty_culc_Subsystem1_UnitDelay9_1) +
             BlsmControl_System_duty_culc_Subsystem1_UnitDelay8_1) +
            BlsmControl_System_duty_culc_Subsystem1_UnitDelay7_1) +
           BlsmControl_System_duty_culc_Subsystem1_UnitDelay6_1) +
          BlsmControl_System_duty_culc_Subsystem1_UnitDelay5_1) +
         BlsmControl_System_duty_culc_Subsystem1_UnitDelay4_1) +
        BlsmControl_System_duty_culc_Subsystem1_UnitDelay3_1) +
       BlsmControl_System_duty_culc_Subsystem1_UnitDelay2_1) +
      BlsmControl_System_duty_culc_Subsystem1_UnitDelay1_1) +
     BlsmControl_System_duty_culc_Subsystem1_UnitDelay_1) +
    BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold_1;


}

    }

#ifdef TASK_EXECUTE_0044_COMPLETED
    TASK_EXECUTE_0044_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_Divide
     * Task: 0043
     * rate: 1
     */

    /* C code */
    {
{

  /* Product: '<S6>/Divide' incorporates:
   *  Constant: '<S6>/Constant'
   */
  BlsmControl_System_duty_culc_Subsystem1_Divide_1 =
    BlsmControl_System_duty_culc_Subsystem1_Subtract_1 /
    BlsmControl_P.Constant_Value_b;

}

    }

#ifdef TASK_EXECUTE_0043_COMPLETED
    TASK_EXECUTE_0043_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_duty_culc_Zero_OrderHold5 */
      if (CH__TIME_LE(task__time+1, CH_BlsmControl_0043_0077.time__0043_0077)) {
	/* no send this period */
	CH__END_LOG(OUT_0043);
	break;
      } else if (CH_BlsmControl_0043_0077.flag__0043_0077 == 0) {
	CH_BlsmControl_0043_0077.BlsmControl_System_duty_culc_Subsystem1_Divide_1 = BlsmControl_System_duty_culc_Subsystem1_Divide_1;
	CH__SYNCM();
	CH_BlsmControl_0043_0077.flag__0043_0077 = 1;
	CH_BlsmControl_0043_0077.time__0043_0077 += 100;
	CH__EVENT(TASK_0043,IN_0077);
	CH__END_LOG(OUT_0043);
	break;
      }

      CH__TASK_SCHED(OUT_0043,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0043_COMPLETED
    TASK_OUT_0043_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch6
     * Task: 0072
     * rate: 1
     */

    /* wait input channel */
    while (1) {
      /* input: BlsmControl_System_duty_culc_RateTransition2 */
      if (CH_BlsmControl_0037_0072.flag__0037_0072 > 0) {
	int ch__flag;
	BlsmControl_System_duty_culc_RateTransition2_1 = CH_BlsmControl_0037_0072.BlsmControl_System_duty_culc_RateTransition2_1;
	ch__flag = CH_BlsmControl_0037_0072.flag__0037_0072 - 1;
	CH_BlsmControl_0037_0072.flag__0037_0072 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0072, OUT_0037);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0072);
	break;
      }

      /* skip local input CH_0071_0072: BlsmControl_System_duty_culc_Switch5 */

      CH__TASK_SCHED(IN_0072,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0072_COMPLETED
    TASK_IN_0072_COMPLETED();
#endif

    /* C code */
    {
{

  /* Switch: '<S2>/Switch6' incorporates:
   *  Constant: '<S2>/Constant10'
   */
  if (BlsmControl_System_duty_culc_Switch5_1 > BlsmControl_P.Switch6_Threshold)
  {
    BlsmControl_System_duty_culc_Switch6_1 = BlsmControl_P.Constant10_Value;
  } else {
    BlsmControl_System_duty_culc_Switch6_1 =
      BlsmControl_System_duty_culc_RateTransition2_1;
  }

  /* End of Switch: '<S2>/Switch6' */

}

    }

#ifdef TASK_EXECUTE_0072_COMPLETED
    TASK_EXECUTE_0072_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch4
     * Task: 0070
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch4' incorporates:
   *  Constant: '<S2>/Constant1'
   *  Constant: '<S2>/Constant8'
   */
  if (BlsmControl_System_duty_culc_Switch17_1 > BlsmControl_P.Switch4_Threshold)
  {
    BlsmControl_System_duty_culc_Switch4_1 = BlsmControl_P.Constant8_Value;
  } else {
    BlsmControl_System_duty_culc_Switch4_1 = BlsmControl_P.Constant1_Value;
  }

  /* End of Switch: '<S2>/Switch4' */

}

    }

#ifdef TASK_EXECUTE_0070_COMPLETED
    TASK_EXECUTE_0070_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch3
     * Task: 0069
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch3' incorporates:
   *  Constant: '<S2>/Constant26'
   *  Constant: '<S2>/Constant6'
   */
  if (BlsmControl_System_duty_culc_Switch16_1 > BlsmControl_P.Switch3_Threshold)
  {
    BlsmControl_System_duty_culc_Switch3_1 = BlsmControl_P.Constant26_Value;
  } else {
    BlsmControl_System_duty_culc_Switch3_1 = BlsmControl_P.Constant6_Value;
  }

  /* End of Switch: '<S2>/Switch3' */

}

    }

#ifdef TASK_EXECUTE_0069_COMPLETED
    TASK_EXECUTE_0069_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch2
     * Task: 0068
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S2>/Switch2' incorporates:
   *  Constant: '<S2>/Constant24'
   *  Constant: '<S2>/Constant7'
   */
  if (BlsmControl_System_duty_culc_Zero_OrderHold1_1 >
      BlsmControl_P.Switch2_Threshold) {
    BlsmControl_System_duty_culc_Switch2_1 = BlsmControl_P.Constant24_Value;
  } else {
    BlsmControl_System_duty_culc_Switch2_1 = BlsmControl_P.Constant7_Value;
  }

  /* End of Switch: '<S2>/Switch2' */

}

    }

#ifdef TASK_EXECUTE_0068_COMPLETED
    TASK_EXECUTE_0068_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Product3
     * Task: 0036
     * rate: 1
     */

    /* C code */
    {
{

  /* Product: '<S2>/Product3' */
  BlsmControl_System_duty_culc_Product3_1 =
    BlsmControl_System_duty_culc_Switch6_1 *
    BlsmControl_System_duty_culc_Switch4_1 *
    BlsmControl_System_duty_culc_Switch3_1 *
    BlsmControl_System_duty_culc_Switch2_1;


}

    }

#ifdef TASK_EXECUTE_0036_COMPLETED
    TASK_EXECUTE_0036_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_duty_culc_RateTransition3 */
      if (CH__TIME_LE(task__time+1, CH_BlsmControl_0036_0038.time__0036_0038)) {
	/* no send this period */
	CH__END_LOG(OUT_0036);
	break;
      } else if (CH_BlsmControl_0036_0038.flag__0036_0038 == 0) {
	CH_BlsmControl_0036_0038.BlsmControl_System_duty_culc_Product3_1 = BlsmControl_System_duty_culc_Product3_1;
	CH__SYNCM();
	CH_BlsmControl_0036_0038.flag__0036_0038 = 1;
	CH_BlsmControl_0036_0038.time__0036_0038 += 100;
	CH__EVENT(TASK_0036,IN_0038);
	CH__END_LOG(OUT_0036);
	break;
      }

      /* skip local output CH_0036_0186: BlsmControl_duty */

      CH__TASK_SCHED(OUT_0036,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0036_COMPLETED
    TASK_OUT_0036_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_counter_Sum
     * Task: 0101
     * rate: 1
     */

    /* C code */
    {
{

  /* Sum: '<S7>/Sum' incorporates:
   *  Constant: '<S7>/Constant'
   */
  BlsmControl_System_rpm_culc_counter_Sum_1 = BlsmControl_P.Constant_Value_e +
    BlsmControl_System_rpm_culc_counter_UnitDelay_1;


}

    }

#ifdef TASK_EXECUTE_0101_COMPLETED
    TASK_EXECUTE_0101_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_counter_Switch
     * Task: 0102
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S7>/Switch' incorporates:
   *  Constant: '<S7>/Constant5'
   */
  if (BlsmControl_System_rpm_culc_if_LogicalOperator_1) {
    BlsmControl_System_rpm_culc_counter_Switch_1 =
      BlsmControl_P.Constant5_Value_k;
  } else {
    BlsmControl_System_rpm_culc_counter_Switch_1 =
      BlsmControl_System_rpm_culc_counter_Sum_1;
  }

  /* End of Switch: '<S7>/Switch' */

}

    }

#ifdef TASK_EXECUTE_0102_COMPLETED
    TASK_EXECUTE_0102_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_update_prev_pos_Switch3
     * Task: 0126
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S10>/Switch3' incorporates:
   *  Inport: '<Root>/pos'
   */
  if (BlsmControl_System_rpm_culc_if_LogicalOperator_1) {
    BlsmControl_System_rpm_culc_update_prev_pos_Switch3_1 = BlsmControl_pos_1;
  } else {
    BlsmControl_System_rpm_culc_update_prev_pos_Switch3_1 =
      BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1_1;
  }

  /* End of Switch: '<S10>/Switch3' */

}

    }

#ifdef TASK_EXECUTE_0126_COMPLETED
    TASK_EXECUTE_0126_COMPLETED();
#endif
    /*
     * Block: BlsmControl_duty
     * Task: 0186
     * rate: 1
     */

#ifdef TASK_EXECUTE_0186_COMPLETED
    TASK_EXECUTE_0186_COMPLETED();
#endif

    /****************************************************
     *
     * You may insert some codes here for external output.
     *
     ****************************************************/
#define TAUB_BASE(n)    ((uint32) (0xffe30000U + (n * 0x1000U)))
#define TAUB_CDR(n, m)  (TAUB_BASE(n) + 0x00U + (m * 0x04U))
#define TAUB_RDT(n)     (TAUB_BASE(n) + 0x44U)


/* >>>>>>>>>>>>>>>>>>>>>> */
/* add code  by takematsu */
/* >>>>>>>>>>>>>>>>>>>>>> */
      /* duty_output */
      {
        uint16 wk;

        //duty比を新しいduty比で変更
        wk = (uint16)((real_T) sil_reh_mem((void *) TAUB_CDR(0, 0)) * BlsmControl_System_duty_culc_Product3_1);
        sil_wrh_mem((void *) TAUB_CDR(0, 2), wk);

        //duty比の指令を更新
        sil_wrh_mem((void *) TAUB_RDT(0), 0x0015);
      }
/* <<<<<<<<<<<<<<<<<<<<<< */
/* add code  by takematsu */
/* <<<<<<<<<<<<<<<<<<<<<< */

    /*
     * Block: BlsmControl_System_rpm_culc_counter_UnitDelay
     * Update: 0103
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S7>/UnitDelay' */
  BlsmControl_DW.UnitDelay_DSTATE = BlsmControl_System_rpm_culc_counter_Switch_1;


}

    }

#ifdef TASK_UPDATE_0103_COMPLETED
    TASK_UPDATE_0103_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_update_prev_pos_UnitDelay1
     * Update: 0127
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S10>/UnitDelay1' */
  BlsmControl_DW.UnitDelay1_DSTATE =
    BlsmControl_System_rpm_culc_update_prev_pos_Switch3_1;


}

    }

#ifdef TASK_UPDATE_0127_COMPLETED
    TASK_UPDATE_0127_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_rpm_culc_culc_div_num_Switch1
     * Update: 0108
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S8>/UnitDelay' */
  BlsmControl_DW.UnitDelay_DSTATE_g =
    BlsmControl_System_rpm_culc_culc_div_num_Switch1_1;


}

    }

#ifdef TASK_UPDATE_0108_COMPLETED
    TASK_UPDATE_0108_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay12
     * Update: 0049
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay12' */
  BlsmControl_DW.UnitDelay12_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay14_1;


}

    }

#ifdef TASK_UPDATE_0049_COMPLETED
    TASK_UPDATE_0049_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay14
     * Update: 0051
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay14' */
  BlsmControl_DW.UnitDelay14_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay11_1;


}

    }

#ifdef TASK_UPDATE_0051_COMPLETED
    TASK_UPDATE_0051_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay11
     * Update: 0048
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay11' */
  BlsmControl_DW.UnitDelay11_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay10_1;


}

    }

#ifdef TASK_UPDATE_0048_COMPLETED
    TASK_UPDATE_0048_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay10
     * Update: 0047
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay10' */
  BlsmControl_DW.UnitDelay10_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay9_1;


}

    }

#ifdef TASK_UPDATE_0047_COMPLETED
    TASK_UPDATE_0047_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay9
     * Update: 0059
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay9' */
  BlsmControl_DW.UnitDelay9_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay8_1;


}

    }

#ifdef TASK_UPDATE_0059_COMPLETED
    TASK_UPDATE_0059_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay8
     * Update: 0058
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay8' */
  BlsmControl_DW.UnitDelay8_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay7_1;


}

    }

#ifdef TASK_UPDATE_0058_COMPLETED
    TASK_UPDATE_0058_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay7
     * Update: 0057
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay7' */
  BlsmControl_DW.UnitDelay7_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay6_1;


}

    }

#ifdef TASK_UPDATE_0057_COMPLETED
    TASK_UPDATE_0057_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay6
     * Update: 0056
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay6' */
  BlsmControl_DW.UnitDelay6_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay5_1;


}

    }

#ifdef TASK_UPDATE_0056_COMPLETED
    TASK_UPDATE_0056_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay5
     * Update: 0055
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay5' */
  BlsmControl_DW.UnitDelay5_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay4_1;


}

    }

#ifdef TASK_UPDATE_0055_COMPLETED
    TASK_UPDATE_0055_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay4
     * Update: 0054
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay4' */
  BlsmControl_DW.UnitDelay4_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay3_1;


}

    }

#ifdef TASK_UPDATE_0054_COMPLETED
    TASK_UPDATE_0054_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay3
     * Update: 0053
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay3' */
  BlsmControl_DW.UnitDelay3_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay2_1;


}

    }

#ifdef TASK_UPDATE_0053_COMPLETED
    TASK_UPDATE_0053_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay2
     * Update: 0052
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay2' */
  BlsmControl_DW.UnitDelay2_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay1_1;


}

    }

#ifdef TASK_UPDATE_0052_COMPLETED
    TASK_UPDATE_0052_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay1
     * Update: 0046
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay1' */
  BlsmControl_DW.UnitDelay1_DSTATE_g =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay_1;


}

    }

#ifdef TASK_UPDATE_0046_COMPLETED
    TASK_UPDATE_0046_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_UnitDelay
     * Update: 0045
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay' */
  BlsmControl_DW.UnitDelay_DSTATE_f =
    BlsmControl_System_duty_culc_Subsystem1_Zero_OrderHold_1;

}

    }

#ifdef TASK_UPDATE_0045_COMPLETED
    TASK_UPDATE_0045_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Subsystem1_Subtract
     * Update: 0044
     * rate: 1
     */

    /* Update */
    {
{

  /* Update for UnitDelay: '<S6>/UnitDelay13' */
  BlsmControl_DW.UnitDelay13_DSTATE =
    BlsmControl_System_duty_culc_Subsystem1_UnitDelay12_1;


}

    }

#ifdef TASK_UPDATE_0044_COMPLETED
    TASK_UPDATE_0044_COMPLETED();
#endif

#ifdef TASK_BlsmControl_Core0_1_0_Task_BEFORE_SLEEP
    TASK_BlsmControl_Core0_1_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_BlsmControl_Core0_1_0_Task,
		  WE_BlsmControl_Core0_1_0_Task);

    task__time += 1;

#ifdef TASK_BlsmControl_Core0_1_0_Task_AFTER_SLEEP
    TASK_BlsmControl_Core0_1_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(1)
/* rate: 1 */
/* Core#1: Rate: 1 */
TASK(BlsmControl_Core1_1_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0015,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0015,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0037,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0037,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0088,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0088,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0087,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0087,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0182,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0182,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0160,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0160,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0174,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0174,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0175,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0175,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0177,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0177,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0179,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0179,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0181,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0181,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0165,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0165,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0162,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0162,1); */

  /* input channel bit vector */
  CH__VEC(IN_0166,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0166,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0167,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0167,1); */

  /* input channel bit vector */
  CH__VEC(IN_0164,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0164,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0168,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0168,1); */

  /* input channel bit vector */
  CH__VEC(IN_0180,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0180,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0169,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0169,1); */

  /* input channel bit vector */
  CH__VEC(IN_0178,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0178,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0170,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0170,1); */

  /* input channel bit vector */
  CH__VEC(IN_0176,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0176,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0171,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0171,1); */

  /* input channel bit vector */
  CH__VEC(IN_0172,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0172,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0173,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0173,1); */

  /* input channel bit vector */
  CH__VEC(IN_0161,7);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0161,1); */

  /* input channel bit vector */
  CH__VEC(IN_0163,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0163,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0187,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0004,8);

  /* params */
  struct {
    real_T Constant_Value;
    real_T Constant1_Value_m;
    real_T Constant2_Value;
    real_T Constant3_Value;
    real_T Constant4_Value;
    real_T Constant5_Value_a;
    real_T PI;
    real_T Switch24_Threshold;
    real_T Constant36_Value;
    real_T Switch3_Threshold_m;
    real_T Constant4_Value_i;
    real_T Switch5_Threshold_b;
    real_T Constant7_Value_i;
    real_T Switch7_Threshold;
    real_T Constant8_Value_d;
    real_T Switch9_Threshold;
    real_T Constant9_Value_a;
    real_T Switch11_Threshold;
    real_T Constant10_Value_o;
    real_T Switch_Threshold_p;
    real_T Constant16_Value[3];
    real_T Constant18_Value[3];
    real_T Switch12_Threshold;
    real_T Constant15_Value[3];
    real_T Switch13_Threshold;
    real_T Constant13_Value[3];
    real_T Constant19_Value[3];
    real_T Switch10_Threshold;
    real_T Constant14_Value[3];
    real_T Switch14_Threshold;
    real_T Constant11_Value[3];
    real_T Constant20_Value[3];
    real_T Switch8_Threshold_k;
    real_T Constant12_Value_k[3];
    real_T Switch15_Threshold;
    real_T Constant21_Value[3];
    real_T Constant5_Value_kk[3];
    real_T Switch6_Threshold_o;
    real_T Constant6_Value_e[3];
    real_T Switch16_Threshold_f;
    real_T Constant1_Value_p[3];
    real_T Constant22_Value[3];
    real_T Switch4_Threshold_o;
    real_T Constant3_Value_l[3];
    real_T Switch17_Threshold_l;
    real_T Constant2_Value_d[3];
    real_T Constant23_Value[3];
    real_T Switch2_Threshold_o;
    real_T Constant_Value_g[3];
    real_T Switch23_Threshold;
    real_T Constant34_Value[3];
    real_T Constant35_Value[3];
    real_T Constant17_Value;
  } BlsmControl_P = {
  -90.0,                               /* Expression: -90
                                        * Referenced by: '<S3>/Constant'
                                        */

  30.0,                                /* Expression: 30
                                        * Referenced by: '<S3>/Constant1'
                                        */

  -30.0,                               /* Expression: -30
                                        * Referenced by: '<S3>/Constant2'
                                        */

  150.0,                               /* Expression: 150
                                        * Referenced by: '<S3>/Constant3'
                                        */

  -150.0,                              /* Expression: -150
                                        * Referenced by: '<S3>/Constant4'
                                        */

  90.0,                                /* Expression: 90
                                        * Referenced by: '<S3>/Constant5'
                                        */

  3.1415926535897931,                  /* Variable: PI
                                        * Referenced by:
                                        *   '<S3>/Gain'
                                        *   '<S5>/Gain'
                                        */

  -180.0,                              /* Expression: -180
                                        * Referenced by: '<S5>/Switch24'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S5>/Constant36'
                                        */

  -120.0,                              /* Expression: -120
                                        * Referenced by: '<S5>/Switch3'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S5>/Constant4'
                                        */

  -60.0,                               /* Expression: -60
                                        * Referenced by: '<S5>/Switch5'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S5>/Constant7'
                                        */

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch7'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S5>/Constant8'
                                        */

  60.0,                                /* Expression: 60
                                        * Referenced by: '<S5>/Switch9'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S5>/Constant9'
                                        */

  120.0,                               /* Expression: 120
                                        * Referenced by: '<S5>/Switch11'
                                        */

  1000.0,                              /* Expression: 1000
                                        * Referenced by: '<S5>/Constant10'
                                        */

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch'
                                        */

  /*  Expression: [-1 0 1]
   * Referenced by: '<S5>/Constant16'
   */
  { -1.0, 0.0, 1.0 },

  /*  Expression: [1 -1 0]
   * Referenced by: '<S5>/Constant18'
   */
  { 1.0, -1.0, 0.0 },

  180.0,                               /* Expression: 180
                                        * Referenced by: '<S5>/Switch12'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant15'
   */
  { 1.0, 1.0, 1.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch13'
                                        */

  /*  Expression: [-1 1 0]
   * Referenced by: '<S5>/Constant13'
   */
  { -1.0, 1.0, 0.0 },

  /*  Expression: [0 -1 1]
   * Referenced by: '<S5>/Constant19'
   */
  { 0.0, -1.0, 1.0 },

  120.0,                               /* Expression: 120
                                        * Referenced by: '<S5>/Switch10'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant14'
   */
  { 1.0, 1.0, 1.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch14'
                                        */

  /*  Expression: [0 1 -1]
   * Referenced by: '<S5>/Constant11'
   */
  { 0.0, 1.0, -1.0 },

  /*  Expression: [-1 0 1]
   * Referenced by: '<S5>/Constant20'
   */
  { -1.0, 0.0, 1.0 },

  60.0,                                /* Expression: 60
                                        * Referenced by: '<S5>/Switch8'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant12'
   */
  { 1.0, 1.0, 1.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch15'
                                        */

  /*  Expression: [-1 1 0]
   * Referenced by: '<S5>/Constant21'
   */
  { -1.0, 1.0, 0.0 },

  /*  Expression: [1 0 -1]
   * Referenced by: '<S5>/Constant5'
   */
  { 1.0, 0.0, -1.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch6'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant6'
   */
  { 1.0, 1.0, 1.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch16'
                                        */

  /*  Expression: [1 -1 0]
   * Referenced by: '<S5>/Constant1'
   */
  { 1.0, -1.0, 0.0 },

  /*  Expression: [0 1 -1]
   * Referenced by: '<S5>/Constant22'
   */
  { 0.0, 1.0, -1.0 },

  -60.0,                               /* Expression: -60
                                        * Referenced by: '<S5>/Switch4'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant3'
   */
  { 1.0, 1.0, 1.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Switch17'
                                        */

  /*  Expression: [0 -1 1]
   * Referenced by: '<S5>/Constant2'
   */
  { 0.0, -1.0, 1.0 },

  /*  Expression: [1 0 -1]
   * Referenced by: '<S5>/Constant23'
   */
  { 1.0, 0.0, -1.0 },

  -120.0,                              /* Expression: -120
                                        * Referenced by: '<S5>/Switch2'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant'
   */
  { 1.0, 1.0, 1.0 },

  -180.0,                              /* Expression: -180
                                        * Referenced by: '<S5>/Switch23'
                                        */

  /*  Expression: [1 1 1]
   * Referenced by: '<S5>/Constant34'
   */
  { 1.0, 1.0, 1.0 },

  /*  Expression: [0 0 0]
   * Referenced by: '<S5>/Constant35'
   */
  { 0.0, 0.0, 0.0 },

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/Constant17'
                                        */

  };

  /* input/output/auto variables */
  real_T BlsmControl_speed_command_1;
  real_T BlsmControl_System_duty_culc_Abs_1;
  real_T BlsmControl_System_duty_culc_Saturation1_1;
  real_T BlsmControl_System_duty_culc_RateTransition2_1;
  real_T BlsmControl_pos_1;
  real_T BlsmControl_System_pos_to_theta_MultiportSwitch_1;
  real_T BlsmControl_System_pos_to_theta_Gain_1;
  real_T BlsmControl_System_sector_culc_Zero_OrderHold1_1;
  real_T BlsmControl_System_sector_culc_Gain_1;
  real_T BlsmControl_System_sector_culc_Switch24_1;
  real_T BlsmControl_System_sector_culc_Switch3_1;
  real_T BlsmControl_System_sector_culc_Switch5_1;
  real_T BlsmControl_System_sector_culc_Switch7_1;
  real_T BlsmControl_System_sector_culc_Switch9_1;
  real_T BlsmControl_System_sector_culc_Switch11_1;
  real_T BlsmControl_System_sector_culc_Switch_1[3];
  real_T BlsmControl_System_sector_culc_Switch12_1[3];
  real_T BlsmControl_System_sector_culc_Switch13_1[3];
  real_T BlsmControl_System_sector_culc_Switch10_1[3];
  real_T BlsmControl_System_sector_culc_Switch14_1[3];
  real_T BlsmControl_System_sector_culc_Switch8_1[3];
  real_T BlsmControl_System_sector_culc_Switch15_1[3];
  real_T BlsmControl_System_sector_culc_Switch6_1[3];
  real_T BlsmControl_System_sector_culc_Switch16_1[3];
  real_T BlsmControl_System_sector_culc_Switch4_1[3];
  real_T BlsmControl_System_sector_culc_Switch17_1[3];
  real_T BlsmControl_System_sector_culc_Switch2_1[3];
  real_T BlsmControl_System_sector_culc_Switch23_1[3];
  real_T BlsmControl_System_sector_culc_Product_1[3];
  real_T BlsmControl_System_sector_culc_Switch1_1[3];

#ifdef TASK_BlsmControl_Core1_1_0_Task_DEFINITIONS
  TASK_BlsmControl_Core1_1_0_Task_DEFINITIONS
#endif

#ifdef TASK_0015_DEFINITIONS
  TASK_0015_DEFINITIONS
#endif

#ifdef TASK_0037_DEFINITIONS
  TASK_0037_DEFINITIONS
#endif

#ifdef TASK_0088_DEFINITIONS
  TASK_0088_DEFINITIONS
#endif

#ifdef TASK_0087_DEFINITIONS
  TASK_0087_DEFINITIONS
#endif

#ifdef TASK_0182_DEFINITIONS
  TASK_0182_DEFINITIONS
#endif

#ifdef TASK_0160_DEFINITIONS
  TASK_0160_DEFINITIONS
#endif

#ifdef TASK_0174_DEFINITIONS
  TASK_0174_DEFINITIONS
#endif

#ifdef TASK_0175_DEFINITIONS
  TASK_0175_DEFINITIONS
#endif

#ifdef TASK_0177_DEFINITIONS
  TASK_0177_DEFINITIONS
#endif

#ifdef TASK_0179_DEFINITIONS
  TASK_0179_DEFINITIONS
#endif

#ifdef TASK_0181_DEFINITIONS
  TASK_0181_DEFINITIONS
#endif

#ifdef TASK_0165_DEFINITIONS
  TASK_0165_DEFINITIONS
#endif

#ifdef TASK_0162_DEFINITIONS
  TASK_0162_DEFINITIONS
#endif

#ifdef TASK_0166_DEFINITIONS
  TASK_0166_DEFINITIONS
#endif

#ifdef TASK_0167_DEFINITIONS
  TASK_0167_DEFINITIONS
#endif

#ifdef TASK_0164_DEFINITIONS
  TASK_0164_DEFINITIONS
#endif

#ifdef TASK_0168_DEFINITIONS
  TASK_0168_DEFINITIONS
#endif

#ifdef TASK_0180_DEFINITIONS
  TASK_0180_DEFINITIONS
#endif

#ifdef TASK_0169_DEFINITIONS
  TASK_0169_DEFINITIONS
#endif

#ifdef TASK_0178_DEFINITIONS
  TASK_0178_DEFINITIONS
#endif

#ifdef TASK_0170_DEFINITIONS
  TASK_0170_DEFINITIONS
#endif

#ifdef TASK_0176_DEFINITIONS
  TASK_0176_DEFINITIONS
#endif

#ifdef TASK_0171_DEFINITIONS
  TASK_0171_DEFINITIONS
#endif

#ifdef TASK_0172_DEFINITIONS
  TASK_0172_DEFINITIONS
#endif

#ifdef TASK_0173_DEFINITIONS
  TASK_0173_DEFINITIONS
#endif

#ifdef TASK_0161_DEFINITIONS
  TASK_0161_DEFINITIONS
#endif

#ifdef TASK_0163_DEFINITIONS
  TASK_0163_DEFINITIONS
#endif

#ifdef TASK_0187_DEFINITIONS
  TASK_0187_DEFINITIONS
#endif

#ifdef TASK_0004_DEFINITIONS
  TASK_0004_DEFINITIONS
#endif

  /* Block: BlsmControl_System_duty_culc_Abs */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Abs_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_RateTransition2 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_RateTransition2_1 = 0.0;
}

  /* Block: BlsmControl_System_pos_to_theta_MultiportSwitch */

  /* initialize task context */
{
  BlsmControl_System_pos_to_theta_MultiportSwitch_1 = 0.0;
}

  /* Block: BlsmControl_System_pos_to_theta_Gain */

  /* initialize task context */
{
  BlsmControl_System_pos_to_theta_Gain_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Zero_OrderHold1 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Zero_OrderHold1_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Gain */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Gain_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch24 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch24_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch3 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch3_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch5 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch5_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch7 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch7_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch9 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch9_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch11 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch11_1 = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch12 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch12_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch12_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch12_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch13 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch13_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch13_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch13_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch10 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch10_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch10_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch10_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch14 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch14_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch14_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch14_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch8 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch8_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch8_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch8_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch15 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch15_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch15_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch15_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch6 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch6_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch6_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch6_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch16 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch16_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch16_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch16_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch4 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch4_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch4_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch4_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch17 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch17_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch17_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch17_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch2 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch2_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch2_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch2_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch23 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch23_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch23_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch23_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Product */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Product_1[0] = 0.0;
  BlsmControl_System_sector_culc_Product_1[1] = 0.0;
  BlsmControl_System_sector_culc_Product_1[2] = 0.0;
}

  /* Block: BlsmControl_System_sector_culc_Switch1 */

  /* initialize task context */
{
  BlsmControl_System_sector_culc_Switch1_1[0] = 0.0;
  BlsmControl_System_sector_culc_Switch1_1[1] = 0.0;
  BlsmControl_System_sector_culc_Switch1_1[2] = 0.0;
}

  /* Block: BlsmControl_sector */

  /* Block: BlsmControl_speed_command */

  /* initialize task context */
{
  BlsmControl_speed_command_1 = 0.0;

}

#ifdef TASK_INITIALIZE_0015_COMPLETED
  TASK_INITIALIZE_0015_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0037_COMPLETED
  TASK_INITIALIZE_0037_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0088_COMPLETED
  TASK_INITIALIZE_0088_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0087_COMPLETED
  TASK_INITIALIZE_0087_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0182_COMPLETED
  TASK_INITIALIZE_0182_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0160_COMPLETED
  TASK_INITIALIZE_0160_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0174_COMPLETED
  TASK_INITIALIZE_0174_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0175_COMPLETED
  TASK_INITIALIZE_0175_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0177_COMPLETED
  TASK_INITIALIZE_0177_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0179_COMPLETED
  TASK_INITIALIZE_0179_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0181_COMPLETED
  TASK_INITIALIZE_0181_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0165_COMPLETED
  TASK_INITIALIZE_0165_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0162_COMPLETED
  TASK_INITIALIZE_0162_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0166_COMPLETED
  TASK_INITIALIZE_0166_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0167_COMPLETED
  TASK_INITIALIZE_0167_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0164_COMPLETED
  TASK_INITIALIZE_0164_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0168_COMPLETED
  TASK_INITIALIZE_0168_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0180_COMPLETED
  TASK_INITIALIZE_0180_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0169_COMPLETED
  TASK_INITIALIZE_0169_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0178_COMPLETED
  TASK_INITIALIZE_0178_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0170_COMPLETED
  TASK_INITIALIZE_0170_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0176_COMPLETED
  TASK_INITIALIZE_0176_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0171_COMPLETED
  TASK_INITIALIZE_0171_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0172_COMPLETED
  TASK_INITIALIZE_0172_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0173_COMPLETED
  TASK_INITIALIZE_0173_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0161_COMPLETED
  TASK_INITIALIZE_0161_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0163_COMPLETED
  TASK_INITIALIZE_0163_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0187_COMPLETED
  TASK_INITIALIZE_0187_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0004_COMPLETED
  TASK_INITIALIZE_0004_COMPLETED();
#endif

  /* wait start event */
  OS__WAITEVENT(TASK_BlsmControl_Core1_1_0_Task,
		WE_BlsmControl_Core1_1_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_BlsmControl_Core1_1_0_Task_LOOP_TOP
    TASK_BlsmControl_Core1_1_0_Task_LOOP_TOP
#endif

    /*
     * Block: BlsmControl_speed_command
     * Task: 0004
     * rate: 1
     */

    /****************************************************
     *
     * You should insert some codes here for external input.
     *
     ****************************************************/
/* >>>>>>>>>>>>>>>>>>>>>> */
/* add code  by takematsu */
/* >>>>>>>>>>>>>>>>>>>>>> */
      /* speed_command_input */
      {
        BlsmControl_speed_command_1 = (real_T)blsm_speed;
      }
/* <<<<<<<<<<<<<<<<<<<<<< */
/* add code  by takematsu */
/* <<<<<<<<<<<<<<<<<<<<<< */

#ifdef TASK_EXECUTE_0004_COMPLETED
    TASK_EXECUTE_0004_COMPLETED();
#endif

    /*
     * Block: BlsmControl_System_duty_culc_Abs
     * Task: 0015
     * rate: 1
     */

    /* C code */
    {
{

  /* Abs: '<S2>/Abs' incorporates:
   *  Inport: '<Root>/speed_command'
   */
  BlsmControl_System_duty_culc_Abs_1 = fabs(BlsmControl_speed_command_1);

}

    }

#ifdef TASK_EXECUTE_0015_COMPLETED
    TASK_EXECUTE_0015_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_duty_culc_Zero_OrderHold4 */
      if (CH__TIME_LE(task__time+1, CH_BlsmControl_0015_0076.time__0015_0076)) {
	/* no send this period */
	CH__END_LOG(OUT_0015);
	break;
      } else if (CH_BlsmControl_0015_0076.flag__0015_0076 == 0) {
	CH_BlsmControl_0015_0076.BlsmControl_System_duty_culc_Abs_1 = BlsmControl_System_duty_culc_Abs_1;
	CH__SYNCM();
	CH_BlsmControl_0015_0076.flag__0015_0076 = 1;
	CH_BlsmControl_0015_0076.time__0015_0076 += 100;
	CH__EVENT(TASK_0015,IN_0076);
	CH__END_LOG(OUT_0015);
	break;
      }

      CH__TASK_SCHED(OUT_0015,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0015_COMPLETED
    TASK_OUT_0015_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_RateTransition2
     * Task: 0037
     * rate: 1
     */

    /* wait input channel */
    while (1) {
      /* input: BlsmControl_System_duty_culc_Saturation1 */
      if (CH_BlsmControl_0039_0037.flag__0039_0037 > 0) {
	int ch__flag;
	BlsmControl_System_duty_culc_Saturation1_1 = CH_BlsmControl_0039_0037.BlsmControl_System_duty_culc_Saturation1_1;
	ch__flag = CH_BlsmControl_0039_0037.flag__0039_0037 - 1;
	CH_BlsmControl_0039_0037.flag__0039_0037 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0037, OUT_0039);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0037);
	break;
      }

      CH__TASK_SCHED(IN_0037,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0037_COMPLETED
    TASK_IN_0037_COMPLETED();
#endif

    /* C code */
    {
{

  /* RateTransition: '<S2>/RateTransition2' */
  BlsmControl_System_duty_culc_RateTransition2_1 =
    BlsmControl_System_duty_culc_Saturation1_1;


}

    }

#ifdef TASK_EXECUTE_0037_COMPLETED
    TASK_EXECUTE_0037_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_duty_culc_Switch6 */
      if (CH_BlsmControl_0037_0072.flag__0037_0072 == 0) {
	CH_BlsmControl_0037_0072.BlsmControl_System_duty_culc_RateTransition2_1 = BlsmControl_System_duty_culc_RateTransition2_1;
	CH__SYNCM();
	CH_BlsmControl_0037_0072.flag__0037_0072 = 1;
	CH__EVENT(TASK_0037,IN_0072);
	CH__END_LOG(OUT_0037);
	break;
      }

      CH__TASK_SCHED(OUT_0037,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0037_COMPLETED
    TASK_OUT_0037_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_pos_to_theta_MultiportSwitch
     * Task: 0088
     * rate: 1
     */

    /* wait input channel */
    while (1) {
      /* input: BlsmControl_pos */
      if (CH_BlsmControl_0002_0088.flag__0002_0088 > 0) {
	int ch__flag;
	BlsmControl_pos_1 = CH_BlsmControl_0002_0088.BlsmControl_pos_1;
	ch__flag = CH_BlsmControl_0002_0088.flag__0002_0088 - 1;
	CH_BlsmControl_0002_0088.flag__0002_0088 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0088, OUT_0002);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0088);
	break;
      }

      CH__TASK_SCHED(IN_0088,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0088_COMPLETED
    TASK_IN_0088_COMPLETED();
#endif

    /* C code */
    {
{

  /* MultiPortSwitch: '<S3>/MultiportSwitch' incorporates:
   *  Constant: '<S3>/Constant'
   *  Constant: '<S3>/Constant1'
   *  Constant: '<S3>/Constant2'
   *  Constant: '<S3>/Constant3'
   *  Constant: '<S3>/Constant4'
   *  Constant: '<S3>/Constant5'
   *  Inport: '<Root>/pos'
   */
  switch ((int32_T)BlsmControl_pos_1) {
   case 1:
    BlsmControl_System_pos_to_theta_MultiportSwitch_1 =
      BlsmControl_P.Constant_Value;
    break;

   case 2:
    BlsmControl_System_pos_to_theta_MultiportSwitch_1 =
      BlsmControl_P.Constant1_Value_m;
    break;

   case 3:
    BlsmControl_System_pos_to_theta_MultiportSwitch_1 =
      BlsmControl_P.Constant2_Value;
    break;

   case 4:
    BlsmControl_System_pos_to_theta_MultiportSwitch_1 =
      BlsmControl_P.Constant3_Value;
    break;

   case 5:
    BlsmControl_System_pos_to_theta_MultiportSwitch_1 =
      BlsmControl_P.Constant4_Value;
    break;

   default:
    BlsmControl_System_pos_to_theta_MultiportSwitch_1 =
      BlsmControl_P.Constant5_Value_a;
    break;
  }

  /* End of MultiPortSwitch: '<S3>/MultiportSwitch' */

}

    }

#ifdef TASK_EXECUTE_0088_COMPLETED
    TASK_EXECUTE_0088_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_pos_to_theta_Gain
     * Task: 0087
     * rate: 1
     */

    /* C code */
    {
{

  /* Gain: '<S3>/Gain' */
  u0 = BlsmControl_P.PI;
  u0 /= 180.0;
  BlsmControl_System_pos_to_theta_Gain_1 = u0 *
    BlsmControl_System_pos_to_theta_MultiportSwitch_1;


}

    }

#ifdef TASK_EXECUTE_0087_COMPLETED
    TASK_EXECUTE_0087_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Zero_OrderHold1
     * Task: 0182
     * rate: 1
     */

    /* C code */
    {
{

  /* ZeroOrderHold: '<S5>/Zero_OrderHold1' */
  BlsmControl_System_sector_culc_Zero_OrderHold1_1 =
    BlsmControl_System_pos_to_theta_Gain_1;


}

    }

#ifdef TASK_EXECUTE_0182_COMPLETED
    TASK_EXECUTE_0182_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Gain
     * Task: 0160
     * rate: 1
     */

    /* C code */
    {
{

  /* Gain: '<S5>/Gain' */
  u0 = BlsmControl_P.PI;
  u0 = 180.0 / u0;
  BlsmControl_System_sector_culc_Gain_1 = u0 *
    BlsmControl_System_sector_culc_Zero_OrderHold1_1;


}

    }

#ifdef TASK_EXECUTE_0160_COMPLETED
    TASK_EXECUTE_0160_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch24
     * Task: 0174
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch24' incorporates:
   *  Constant: '<S5>/Constant36'
   */
  if (BlsmControl_System_sector_culc_Gain_1 >= BlsmControl_P.Switch24_Threshold)
  {
    BlsmControl_System_sector_culc_Switch24_1 =
      BlsmControl_System_sector_culc_Gain_1;
  } else {
    BlsmControl_System_sector_culc_Switch24_1 = BlsmControl_P.Constant36_Value;
  }

  /* End of Switch: '<S5>/Switch24' */

}

    }

#ifdef TASK_EXECUTE_0174_COMPLETED
    TASK_EXECUTE_0174_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch3
     * Task: 0175
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch3' incorporates:
   *  Constant: '<S5>/Constant4'
   */
  if (BlsmControl_System_sector_culc_Switch24_1 >=
      BlsmControl_P.Switch3_Threshold_m) {
    BlsmControl_System_sector_culc_Switch3_1 =
      BlsmControl_System_sector_culc_Switch24_1;
  } else {
    BlsmControl_System_sector_culc_Switch3_1 = BlsmControl_P.Constant4_Value_i;
  }

  /* End of Switch: '<S5>/Switch3' */

}

    }

#ifdef TASK_EXECUTE_0175_COMPLETED
    TASK_EXECUTE_0175_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch5
     * Task: 0177
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch5' incorporates:
   *  Constant: '<S5>/Constant7'
   */
  if (BlsmControl_System_sector_culc_Switch3_1 >=
      BlsmControl_P.Switch5_Threshold_b) {
    BlsmControl_System_sector_culc_Switch5_1 =
      BlsmControl_System_sector_culc_Switch3_1;
  } else {
    BlsmControl_System_sector_culc_Switch5_1 = BlsmControl_P.Constant7_Value_i;
  }

  /* End of Switch: '<S5>/Switch5' */

}

    }

#ifdef TASK_EXECUTE_0177_COMPLETED
    TASK_EXECUTE_0177_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch7
     * Task: 0179
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch7' incorporates:
   *  Constant: '<S5>/Constant8'
   */
  if (BlsmControl_System_sector_culc_Switch5_1 >=
      BlsmControl_P.Switch7_Threshold) {
    BlsmControl_System_sector_culc_Switch7_1 =
      BlsmControl_System_sector_culc_Switch5_1;
  } else {
    BlsmControl_System_sector_culc_Switch7_1 = BlsmControl_P.Constant8_Value_d;
  }

  /* End of Switch: '<S5>/Switch7' */

}

    }

#ifdef TASK_EXECUTE_0179_COMPLETED
    TASK_EXECUTE_0179_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch9
     * Task: 0181
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch9' incorporates:
   *  Constant: '<S5>/Constant9'
   */
  if (BlsmControl_System_sector_culc_Switch7_1 >=
      BlsmControl_P.Switch9_Threshold) {
    BlsmControl_System_sector_culc_Switch9_1 =
      BlsmControl_System_sector_culc_Switch7_1;
  } else {
    BlsmControl_System_sector_culc_Switch9_1 = BlsmControl_P.Constant9_Value_a;
  }

  /* End of Switch: '<S5>/Switch9' */

}

    }

#ifdef TASK_EXECUTE_0181_COMPLETED
    TASK_EXECUTE_0181_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch11
     * Task: 0165
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch11' incorporates:
   *  Constant: '<S5>/Constant10'
   */
  if (BlsmControl_System_sector_culc_Switch9_1 >=
      BlsmControl_P.Switch11_Threshold) {
    BlsmControl_System_sector_culc_Switch11_1 =
      BlsmControl_System_sector_culc_Switch9_1;
  } else {
    BlsmControl_System_sector_culc_Switch11_1 = BlsmControl_P.Constant10_Value_o;
  }

  /* End of Switch: '<S5>/Switch11' */

}

    }

#ifdef TASK_EXECUTE_0165_COMPLETED
    TASK_EXECUTE_0165_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch
     * Task: 0162
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch' incorporates:
   *  Constant: '<S5>/Constant16'
   *  Constant: '<S5>/Constant18'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 >= BlsmControl_P.Switch_Threshold_p) {
    BlsmControl_System_sector_culc_Switch_1[0] = BlsmControl_P.Constant16_Value
      [0];
    BlsmControl_System_sector_culc_Switch_1[1] = BlsmControl_P.Constant16_Value
      [1];
    BlsmControl_System_sector_culc_Switch_1[2] = BlsmControl_P.Constant16_Value
      [2];
  } else {
    BlsmControl_System_sector_culc_Switch_1[0] = BlsmControl_P.Constant18_Value
      [0];
    BlsmControl_System_sector_culc_Switch_1[1] = BlsmControl_P.Constant18_Value
      [1];
    BlsmControl_System_sector_culc_Switch_1[2] = BlsmControl_P.Constant18_Value
      [2];
  }

  /* End of Switch: '<S5>/Switch' */

}

    }

#ifdef TASK_EXECUTE_0162_COMPLETED
    TASK_EXECUTE_0162_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch12
     * Task: 0166
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch12' incorporates:
   *  Constant: '<S5>/Constant15'
   */
  if (BlsmControl_System_sector_culc_Switch11_1 >=
      BlsmControl_P.Switch12_Threshold) {
    BlsmControl_System_sector_culc_Switch12_1[0] =
      BlsmControl_P.Constant15_Value[0];
    BlsmControl_System_sector_culc_Switch12_1[1] =
      BlsmControl_P.Constant15_Value[1];
    BlsmControl_System_sector_culc_Switch12_1[2] =
      BlsmControl_P.Constant15_Value[2];
  } else {
    BlsmControl_System_sector_culc_Switch12_1[0] =
      BlsmControl_System_sector_culc_Switch_1[0];
    BlsmControl_System_sector_culc_Switch12_1[1] =
      BlsmControl_System_sector_culc_Switch_1[1];
    BlsmControl_System_sector_culc_Switch12_1[2] =
      BlsmControl_System_sector_culc_Switch_1[2];
  }

  /* End of Switch: '<S5>/Switch12' */

}

    }

#ifdef TASK_EXECUTE_0166_COMPLETED
    TASK_EXECUTE_0166_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch13
     * Task: 0167
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch13' incorporates:
   *  Constant: '<S5>/Constant13'
   *  Constant: '<S5>/Constant19'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 >= BlsmControl_P.Switch13_Threshold) {
    BlsmControl_System_sector_culc_Switch13_1[0] =
      BlsmControl_P.Constant13_Value[0];
    BlsmControl_System_sector_culc_Switch13_1[1] =
      BlsmControl_P.Constant13_Value[1];
    BlsmControl_System_sector_culc_Switch13_1[2] =
      BlsmControl_P.Constant13_Value[2];
  } else {
    BlsmControl_System_sector_culc_Switch13_1[0] =
      BlsmControl_P.Constant19_Value[0];
    BlsmControl_System_sector_culc_Switch13_1[1] =
      BlsmControl_P.Constant19_Value[1];
    BlsmControl_System_sector_culc_Switch13_1[2] =
      BlsmControl_P.Constant19_Value[2];
  }

  /* End of Switch: '<S5>/Switch13' */

}

    }

#ifdef TASK_EXECUTE_0167_COMPLETED
    TASK_EXECUTE_0167_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch10
     * Task: 0164
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch10' incorporates:
   *  Constant: '<S5>/Constant14'
   */
  if (BlsmControl_System_sector_culc_Switch9_1 >=
      BlsmControl_P.Switch10_Threshold) {
    BlsmControl_System_sector_culc_Switch10_1[0] =
      BlsmControl_P.Constant14_Value[0];
    BlsmControl_System_sector_culc_Switch10_1[1] =
      BlsmControl_P.Constant14_Value[1];
    BlsmControl_System_sector_culc_Switch10_1[2] =
      BlsmControl_P.Constant14_Value[2];
  } else {
    BlsmControl_System_sector_culc_Switch10_1[0] =
      BlsmControl_System_sector_culc_Switch13_1[0];
    BlsmControl_System_sector_culc_Switch10_1[1] =
      BlsmControl_System_sector_culc_Switch13_1[1];
    BlsmControl_System_sector_culc_Switch10_1[2] =
      BlsmControl_System_sector_culc_Switch13_1[2];
  }

  /* End of Switch: '<S5>/Switch10' */

}

    }

#ifdef TASK_EXECUTE_0164_COMPLETED
    TASK_EXECUTE_0164_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch14
     * Task: 0168
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch14' incorporates:
   *  Constant: '<S5>/Constant11'
   *  Constant: '<S5>/Constant20'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 >= BlsmControl_P.Switch14_Threshold) {
    BlsmControl_System_sector_culc_Switch14_1[0] =
      BlsmControl_P.Constant11_Value[0];
    BlsmControl_System_sector_culc_Switch14_1[1] =
      BlsmControl_P.Constant11_Value[1];
    BlsmControl_System_sector_culc_Switch14_1[2] =
      BlsmControl_P.Constant11_Value[2];
  } else {
    BlsmControl_System_sector_culc_Switch14_1[0] =
      BlsmControl_P.Constant20_Value[0];
    BlsmControl_System_sector_culc_Switch14_1[1] =
      BlsmControl_P.Constant20_Value[1];
    BlsmControl_System_sector_culc_Switch14_1[2] =
      BlsmControl_P.Constant20_Value[2];
  }

  /* End of Switch: '<S5>/Switch14' */

}

    }

#ifdef TASK_EXECUTE_0168_COMPLETED
    TASK_EXECUTE_0168_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch8
     * Task: 0180
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch8' incorporates:
   *  Constant: '<S5>/Constant12'
   */
  if (BlsmControl_System_sector_culc_Switch7_1 >=
      BlsmControl_P.Switch8_Threshold_k) {
    BlsmControl_System_sector_culc_Switch8_1[0] =
      BlsmControl_P.Constant12_Value_k[0];
    BlsmControl_System_sector_culc_Switch8_1[1] =
      BlsmControl_P.Constant12_Value_k[1];
    BlsmControl_System_sector_culc_Switch8_1[2] =
      BlsmControl_P.Constant12_Value_k[2];
  } else {
    BlsmControl_System_sector_culc_Switch8_1[0] =
      BlsmControl_System_sector_culc_Switch14_1[0];
    BlsmControl_System_sector_culc_Switch8_1[1] =
      BlsmControl_System_sector_culc_Switch14_1[1];
    BlsmControl_System_sector_culc_Switch8_1[2] =
      BlsmControl_System_sector_culc_Switch14_1[2];
  }

  /* End of Switch: '<S5>/Switch8' */

}

    }

#ifdef TASK_EXECUTE_0180_COMPLETED
    TASK_EXECUTE_0180_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch15
     * Task: 0169
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch15' incorporates:
   *  Constant: '<S5>/Constant21'
   *  Constant: '<S5>/Constant5'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 >= BlsmControl_P.Switch15_Threshold) {
    BlsmControl_System_sector_culc_Switch15_1[0] =
      BlsmControl_P.Constant5_Value_kk[0];
    BlsmControl_System_sector_culc_Switch15_1[1] =
      BlsmControl_P.Constant5_Value_kk[1];
    BlsmControl_System_sector_culc_Switch15_1[2] =
      BlsmControl_P.Constant5_Value_kk[2];
  } else {
    BlsmControl_System_sector_culc_Switch15_1[0] =
      BlsmControl_P.Constant21_Value[0];
    BlsmControl_System_sector_culc_Switch15_1[1] =
      BlsmControl_P.Constant21_Value[1];
    BlsmControl_System_sector_culc_Switch15_1[2] =
      BlsmControl_P.Constant21_Value[2];
  }

  /* End of Switch: '<S5>/Switch15' */

}

    }

#ifdef TASK_EXECUTE_0169_COMPLETED
    TASK_EXECUTE_0169_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch6
     * Task: 0178
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch6' incorporates:
   *  Constant: '<S5>/Constant6'
   */
  if (BlsmControl_System_sector_culc_Switch5_1 >=
      BlsmControl_P.Switch6_Threshold_o) {
    BlsmControl_System_sector_culc_Switch6_1[0] =
      BlsmControl_P.Constant6_Value_e[0];
    BlsmControl_System_sector_culc_Switch6_1[1] =
      BlsmControl_P.Constant6_Value_e[1];
    BlsmControl_System_sector_culc_Switch6_1[2] =
      BlsmControl_P.Constant6_Value_e[2];
  } else {
    BlsmControl_System_sector_culc_Switch6_1[0] =
      BlsmControl_System_sector_culc_Switch15_1[0];
    BlsmControl_System_sector_culc_Switch6_1[1] =
      BlsmControl_System_sector_culc_Switch15_1[1];
    BlsmControl_System_sector_culc_Switch6_1[2] =
      BlsmControl_System_sector_culc_Switch15_1[2];
  }

  /* End of Switch: '<S5>/Switch6' */

}

    }

#ifdef TASK_EXECUTE_0178_COMPLETED
    TASK_EXECUTE_0178_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch16
     * Task: 0170
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch16' incorporates:
   *  Constant: '<S5>/Constant1'
   *  Constant: '<S5>/Constant22'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 >= BlsmControl_P.Switch16_Threshold_f) {
    BlsmControl_System_sector_culc_Switch16_1[0] =
      BlsmControl_P.Constant1_Value_p[0];
    BlsmControl_System_sector_culc_Switch16_1[1] =
      BlsmControl_P.Constant1_Value_p[1];
    BlsmControl_System_sector_culc_Switch16_1[2] =
      BlsmControl_P.Constant1_Value_p[2];
  } else {
    BlsmControl_System_sector_culc_Switch16_1[0] =
      BlsmControl_P.Constant22_Value[0];
    BlsmControl_System_sector_culc_Switch16_1[1] =
      BlsmControl_P.Constant22_Value[1];
    BlsmControl_System_sector_culc_Switch16_1[2] =
      BlsmControl_P.Constant22_Value[2];
  }

  /* End of Switch: '<S5>/Switch16' */

}

    }

#ifdef TASK_EXECUTE_0170_COMPLETED
    TASK_EXECUTE_0170_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch4
     * Task: 0176
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch4' incorporates:
   *  Constant: '<S5>/Constant3'
   */
  if (BlsmControl_System_sector_culc_Switch3_1 >=
      BlsmControl_P.Switch4_Threshold_o) {
    BlsmControl_System_sector_culc_Switch4_1[0] =
      BlsmControl_P.Constant3_Value_l[0];
    BlsmControl_System_sector_culc_Switch4_1[1] =
      BlsmControl_P.Constant3_Value_l[1];
    BlsmControl_System_sector_culc_Switch4_1[2] =
      BlsmControl_P.Constant3_Value_l[2];
  } else {
    BlsmControl_System_sector_culc_Switch4_1[0] =
      BlsmControl_System_sector_culc_Switch16_1[0];
    BlsmControl_System_sector_culc_Switch4_1[1] =
      BlsmControl_System_sector_culc_Switch16_1[1];
    BlsmControl_System_sector_culc_Switch4_1[2] =
      BlsmControl_System_sector_culc_Switch16_1[2];
  }

  /* End of Switch: '<S5>/Switch4' */

}

    }

#ifdef TASK_EXECUTE_0176_COMPLETED
    TASK_EXECUTE_0176_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch17
     * Task: 0171
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch17' incorporates:
   *  Constant: '<S5>/Constant2'
   *  Constant: '<S5>/Constant23'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 >= BlsmControl_P.Switch17_Threshold_l) {
    BlsmControl_System_sector_culc_Switch17_1[0] =
      BlsmControl_P.Constant2_Value_d[0];
    BlsmControl_System_sector_culc_Switch17_1[1] =
      BlsmControl_P.Constant2_Value_d[1];
    BlsmControl_System_sector_culc_Switch17_1[2] =
      BlsmControl_P.Constant2_Value_d[2];
  } else {
    BlsmControl_System_sector_culc_Switch17_1[0] =
      BlsmControl_P.Constant23_Value[0];
    BlsmControl_System_sector_culc_Switch17_1[1] =
      BlsmControl_P.Constant23_Value[1];
    BlsmControl_System_sector_culc_Switch17_1[2] =
      BlsmControl_P.Constant23_Value[2];
  }

  /* End of Switch: '<S5>/Switch17' */

}

    }

#ifdef TASK_EXECUTE_0171_COMPLETED
    TASK_EXECUTE_0171_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch2
     * Task: 0172
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch2' incorporates:
   *  Constant: '<S5>/Constant'
   */
  if (BlsmControl_System_sector_culc_Switch24_1 >=
      BlsmControl_P.Switch2_Threshold_o) {
    BlsmControl_System_sector_culc_Switch2_1[0] =
      BlsmControl_P.Constant_Value_g[0];
    BlsmControl_System_sector_culc_Switch2_1[1] =
      BlsmControl_P.Constant_Value_g[1];
    BlsmControl_System_sector_culc_Switch2_1[2] =
      BlsmControl_P.Constant_Value_g[2];
  } else {
    BlsmControl_System_sector_culc_Switch2_1[0] =
      BlsmControl_System_sector_culc_Switch17_1[0];
    BlsmControl_System_sector_culc_Switch2_1[1] =
      BlsmControl_System_sector_culc_Switch17_1[1];
    BlsmControl_System_sector_culc_Switch2_1[2] =
      BlsmControl_System_sector_culc_Switch17_1[2];
  }

  /* End of Switch: '<S5>/Switch2' */

}

    }

#ifdef TASK_EXECUTE_0172_COMPLETED
    TASK_EXECUTE_0172_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch23
     * Task: 0173
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch23' incorporates:
   *  Constant: '<S5>/Constant34'
   *  Constant: '<S5>/Constant35'
   */
  if (BlsmControl_System_sector_culc_Gain_1 >= BlsmControl_P.Switch23_Threshold)
  {
    BlsmControl_System_sector_culc_Switch23_1[0] =
      BlsmControl_P.Constant34_Value[0];
    BlsmControl_System_sector_culc_Switch23_1[1] =
      BlsmControl_P.Constant34_Value[1];
    BlsmControl_System_sector_culc_Switch23_1[2] =
      BlsmControl_P.Constant34_Value[2];
  } else {
    BlsmControl_System_sector_culc_Switch23_1[0] =
      BlsmControl_P.Constant35_Value[0];
    BlsmControl_System_sector_culc_Switch23_1[1] =
      BlsmControl_P.Constant35_Value[1];
    BlsmControl_System_sector_culc_Switch23_1[2] =
      BlsmControl_P.Constant35_Value[2];
  }

  /* End of Switch: '<S5>/Switch23' */

}

    }

#ifdef TASK_EXECUTE_0173_COMPLETED
    TASK_EXECUTE_0173_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Product
     * Task: 0161
     * rate: 1
     */

    /* C code */
    {
{

  /* Product: '<S5>/Product' */
  BlsmControl_System_sector_culc_Product_1[0] =
    BlsmControl_System_sector_culc_Switch12_1[0] *
    BlsmControl_System_sector_culc_Switch10_1[0] *
    BlsmControl_System_sector_culc_Switch8_1[0] *
    BlsmControl_System_sector_culc_Switch6_1[0] *
    BlsmControl_System_sector_culc_Switch4_1[0] *
    BlsmControl_System_sector_culc_Switch2_1[0] *
    BlsmControl_System_sector_culc_Switch23_1[0];
  BlsmControl_System_sector_culc_Product_1[1] =
    BlsmControl_System_sector_culc_Switch12_1[1] *
    BlsmControl_System_sector_culc_Switch10_1[1] *
    BlsmControl_System_sector_culc_Switch8_1[1] *
    BlsmControl_System_sector_culc_Switch6_1[1] *
    BlsmControl_System_sector_culc_Switch4_1[1] *
    BlsmControl_System_sector_culc_Switch2_1[1] *
    BlsmControl_System_sector_culc_Switch23_1[1];
  BlsmControl_System_sector_culc_Product_1[2] =
    BlsmControl_System_sector_culc_Switch12_1[2] *
    BlsmControl_System_sector_culc_Switch10_1[2] *
    BlsmControl_System_sector_culc_Switch8_1[2] *
    BlsmControl_System_sector_culc_Switch6_1[2] *
    BlsmControl_System_sector_culc_Switch4_1[2] *
    BlsmControl_System_sector_culc_Switch2_1[2] *
    BlsmControl_System_sector_culc_Switch23_1[2];


}

    }

#ifdef TASK_EXECUTE_0161_COMPLETED
    TASK_EXECUTE_0161_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_sector_culc_Switch1
     * Task: 0163
     * rate: 1
     */

    /* C code */
    {
{

  /* Switch: '<S5>/Switch1' incorporates:
   *  Constant: '<S5>/Constant17'
   *  Inport: '<Root>/speed_command'
   */
  if (BlsmControl_speed_command_1 != 0.0) {
    BlsmControl_System_sector_culc_Switch1_1[0] =
      BlsmControl_System_sector_culc_Product_1[0];
    BlsmControl_System_sector_culc_Switch1_1[1] =
      BlsmControl_System_sector_culc_Product_1[1];
    BlsmControl_System_sector_culc_Switch1_1[2] =
      BlsmControl_System_sector_culc_Product_1[2];
  } else {
    BlsmControl_System_sector_culc_Switch1_1[0] = BlsmControl_P.Constant17_Value;
    BlsmControl_System_sector_culc_Switch1_1[1] = BlsmControl_P.Constant17_Value;
    BlsmControl_System_sector_culc_Switch1_1[2] = BlsmControl_P.Constant17_Value;
  }

  /* End of Switch: '<S5>/Switch1' */

}

    }

#ifdef TASK_EXECUTE_0163_COMPLETED
    TASK_EXECUTE_0163_COMPLETED();
#endif
    /*
     * Block: BlsmControl_sector
     * Task: 0187
     * rate: 1
     */

#ifdef TASK_EXECUTE_0187_COMPLETED
    TASK_EXECUTE_0187_COMPLETED();
#endif

    /****************************************************
     *
     * You may insert some codes here for external output.
     *
     ****************************************************/
/* >>>>>>>>>>>>>>>>>>>>>> */
/* add code  by takematsu */
/* >>>>>>>>>>>>>>>>>>>>>> */
      /* sector_output */
      {
        size_t i;
        uint16 p_port;
        uint16 n_port;
        uint16 p1;
        uint16 p2;

        p1 = sil_reh_mem((void *) P(1)) & ~0x4060;
        p2 = sil_reh_mem((void *) P(2)) & ~0x002c;

          for(i=0;i<3;i++){
            switch(i){
              case(0):  /* U相 */
                  p_port = 0x0020;
                  n_port = 0x0004;
                  break;
              case(1):  /* V相 */
                  p_port = 0x0040;
                  n_port = 0x0008;
                  break;
              case(2):  /* W相 */
                  p_port = 0x4000;
                  n_port = 0x0020;
                  break;
            }

            switch((int)BlsmControl_System_sector_culc_Switch1_1[i]){
              case 1:
                  p1 |= p_port;
                  p2 &= ~n_port;
                  break;
              case -1:
                  p1 &= ~p_port;
                  p2 |= n_port;
                  break;
              case 0:
              default:
                  p1 &= ~p_port;
                  p2 &= ~n_port;
                  break;
            }
          }

          sil_wrh_mem((void *) P(1), p1);
          sil_wrh_mem((void *) P(2), p2);
      }
/* <<<<<<<<<<<<<<<<<<<<<< */
/* add code  by takematsu */
/* <<<<<<<<<<<<<<<<<<<<<< */

#ifdef TASK_BlsmControl_Core1_1_0_Task_BEFORE_SLEEP
    TASK_BlsmControl_Core1_1_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_BlsmControl_Core1_1_0_Task,
		  WE_BlsmControl_Core1_1_0_Task);

    task__time += 1;

#ifdef TASK_BlsmControl_Core1_1_0_Task_AFTER_SLEEP
    TASK_BlsmControl_Core1_1_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(0)
/* rate: 100 */
/* Core#0: Rate: 100 */
TASK(BlsmControl_Core0_100_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0038,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0038,1); */

  /* input/output/auto variables */
  real_T BlsmControl_System_duty_culc_Product3_1;
  real_T BlsmControl_System_duty_culc_RateTransition3_1;

#ifdef TASK_BlsmControl_Core0_100_0_Task_DEFINITIONS
  TASK_BlsmControl_Core0_100_0_Task_DEFINITIONS
#endif

#ifdef TASK_0038_DEFINITIONS
  TASK_0038_DEFINITIONS
#endif

  /* Block: BlsmControl_System_duty_culc_RateTransition3 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_RateTransition3_1 = 0.0;
}

#ifdef TASK_INITIALIZE_0038_COMPLETED
  TASK_INITIALIZE_0038_COMPLETED();
#endif

  /* wait start event */
  OS__WAITEVENT(TASK_BlsmControl_Core0_100_0_Task,
		WE_BlsmControl_Core0_100_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_BlsmControl_Core0_100_0_Task_LOOP_TOP
    TASK_BlsmControl_Core0_100_0_Task_LOOP_TOP
#endif
    /*
     * Block: BlsmControl_System_duty_culc_RateTransition3
     * Task: 0038
     * rate: 100
     */

    /* wait input channel */
    while (1) {
      /* input: BlsmControl_System_duty_culc_Product3 */
      if (CH_BlsmControl_0036_0038.flag__0036_0038 > 0) {
	int ch__flag;
	BlsmControl_System_duty_culc_Product3_1 = CH_BlsmControl_0036_0038.BlsmControl_System_duty_culc_Product3_1;
	ch__flag = CH_BlsmControl_0036_0038.flag__0036_0038 - 1;
	CH_BlsmControl_0036_0038.flag__0036_0038 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0038, OUT_0036);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0038);
	break;
      }

      CH__TASK_SCHED(IN_0038,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0038_COMPLETED
    TASK_IN_0038_COMPLETED();
#endif

    /* C code */
    {
{

  /* RateTransition: '<S2>/RateTransition3' */

    BlsmControl_System_duty_culc_RateTransition3_1 =
      BlsmControl_System_duty_culc_Product3_1;

  /* End of RateTransition: '<S2>/RateTransition3' */

}

    }

#ifdef TASK_EXECUTE_0038_COMPLETED
    TASK_EXECUTE_0038_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_duty_culc_Product2 */
      if (CH_BlsmControl_0038_0035.flag__0038_0035 == 0) {
	CH_BlsmControl_0038_0035.BlsmControl_System_duty_culc_RateTransition3_1 = BlsmControl_System_duty_culc_RateTransition3_1;
	CH__SYNCM();
	CH_BlsmControl_0038_0035.flag__0038_0035 = 1;
	CH__EVENT(TASK_0038,IN_0035);
	CH__END_LOG(OUT_0038);
	break;
      }

      CH__TASK_SCHED(OUT_0038,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0038_COMPLETED
    TASK_OUT_0038_COMPLETED();
#endif

#ifdef TASK_BlsmControl_Core0_100_0_Task_BEFORE_SLEEP
    TASK_BlsmControl_Core0_100_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_BlsmControl_Core0_100_0_Task,
		  WE_BlsmControl_Core0_100_0_Task);

    task__time += 100;

#ifdef TASK_BlsmControl_Core0_100_0_Task_AFTER_SLEEP
    TASK_BlsmControl_Core0_100_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(1)
/* rate: 100 */
/* Core#1: Rate: 100 */
TASK(BlsmControl_Core1_100_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0076,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0076,2);

  /* omit input channel bit vector */
  /* CH__VEC(IN_0077,1); */

  /* output channel bit vector */
  CH__VEC(OUT_0077,3);

  /* input channel bit vector */
  CH__VEC(IN_0032,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0032,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0063,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0063,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0034,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0034,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0064,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0064,1); */

  /* input channel bit vector */
  CH__VEC(IN_0062,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0062,1); */

  /* input channel bit vector */
  CH__VEC(IN_0033,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0033,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0016,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0016,1); */

  /* input channel bit vector */
  CH__VEC(IN_0065,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0065,1); */

  /* input channel bit vector */
  CH__VEC(IN_0035,2);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0035,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0039,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0039,1); */

  /* states */
  struct {
    real_T UnitDelay1_DSTATE_n;
  } BlsmControl_DW;

  /* params */
  struct {
    real_T Constant2_Value_k;
    real_T Constant3_Value_n;
    real_T Constant4_Value_b;
    real_T Switch1_Threshold;
    real_T Constant5_Value;
    real_T UnitDelay1_InitialCondition_k;
    real_T Saturation1_UpperSat;
    real_T Saturation1_LowerSat;
  } BlsmControl_P = {
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant2'
                                        */

  0.1,                                 /* Expression: 0.1
                                        * Referenced by: '<S2>/Constant3'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant4'
                                        */

  0.01,                                /* Expression: 0.01
                                        * Referenced by: '<S2>/Switch1'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Constant5'
                                        */

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S2>/UnitDelay1'
                                        */

  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S2>/Saturation1'
                                        */

  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S2>/Saturation1'
                                        */

  };

  /* input/output/auto variables */
  real_T BlsmControl_System_duty_culc_Abs_1;
  real_T BlsmControl_System_duty_culc_Zero_OrderHold4_1;
  real_T BlsmControl_System_duty_culc_Subsystem1_Divide_1;
  real_T BlsmControl_System_duty_culc_Zero_OrderHold5_1;
  real_T BlsmControl_System_duty_culc_Divide1_1;
  real_T BlsmControl_System_duty_culc_Sum4_1;
  real_T BlsmControl_System_duty_culc_Product1_1;
  real_T BlsmControl_System_duty_culc_Sum5_1;
  real_T BlsmControl_System_duty_culc_Sum3_1;
  real_T BlsmControl_System_duty_culc_Divide2_1;
  real_T BlsmControl_System_duty_culc_Abs1_1;
  real_T BlsmControl_System_duty_culc_Switch1_1;
  real_T BlsmControl_System_duty_culc_UnitDelay1_1;
  real_T BlsmControl_System_duty_culc_RateTransition3_1;
  real_T BlsmControl_System_duty_culc_Product2_1;
  real_T BlsmControl_System_duty_culc_Saturation1_1;

#ifdef TASK_BlsmControl_Core1_100_0_Task_DEFINITIONS
  TASK_BlsmControl_Core1_100_0_Task_DEFINITIONS
#endif

#ifdef TASK_0076_DEFINITIONS
  TASK_0076_DEFINITIONS
#endif

#ifdef TASK_0077_DEFINITIONS
  TASK_0077_DEFINITIONS
#endif

#ifdef TASK_0032_DEFINITIONS
  TASK_0032_DEFINITIONS
#endif

#ifdef TASK_0063_DEFINITIONS
  TASK_0063_DEFINITIONS
#endif

#ifdef TASK_0034_DEFINITIONS
  TASK_0034_DEFINITIONS
#endif

#ifdef TASK_0064_DEFINITIONS
  TASK_0064_DEFINITIONS
#endif

#ifdef TASK_0062_DEFINITIONS
  TASK_0062_DEFINITIONS
#endif

#ifdef TASK_0033_DEFINITIONS
  TASK_0033_DEFINITIONS
#endif

#ifdef TASK_0016_DEFINITIONS
  TASK_0016_DEFINITIONS
#endif

#ifdef TASK_0065_DEFINITIONS
  TASK_0065_DEFINITIONS
#endif

#ifdef TASK_0035_DEFINITIONS
  TASK_0035_DEFINITIONS
#endif

#ifdef TASK_0039_DEFINITIONS
  TASK_0039_DEFINITIONS
#endif

  /* initialize state structure */
  memset(&BlsmControl_DW, 0, sizeof BlsmControl_DW);

  /* Block: BlsmControl_System_duty_culc_Zero_OrderHold4 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Zero_OrderHold4_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Zero_OrderHold5 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Zero_OrderHold5_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Divide1 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Divide1_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Sum4 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Sum4_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Product1 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Product1_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Sum5 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Sum5_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Sum3 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Sum3_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Divide2 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Divide2_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Abs1 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Abs1_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Switch1 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Switch1_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Product2 */

  /* initialize states */
{
  /* InitializeConditions for UnitDelay: '<S2>/UnitDelay1' */
  BlsmControl_DW.UnitDelay1_DSTATE_n =
    BlsmControl_P.UnitDelay1_InitialCondition_k;
}

  /* initialize task context */
{
  BlsmControl_System_duty_culc_UnitDelay1_1 = 0.0;
}
{
  BlsmControl_System_duty_culc_Product2_1 = 0.0;
}

  /* Block: BlsmControl_System_duty_culc_Saturation1 */

  /* initialize task context */
{
  BlsmControl_System_duty_culc_Saturation1_1 = 0.0;
}

#ifdef TASK_INITIALIZE_0076_COMPLETED
  TASK_INITIALIZE_0076_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0077_COMPLETED
  TASK_INITIALIZE_0077_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0032_COMPLETED
  TASK_INITIALIZE_0032_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0063_COMPLETED
  TASK_INITIALIZE_0063_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0034_COMPLETED
  TASK_INITIALIZE_0034_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0064_COMPLETED
  TASK_INITIALIZE_0064_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0062_COMPLETED
  TASK_INITIALIZE_0062_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0033_COMPLETED
  TASK_INITIALIZE_0033_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0016_COMPLETED
  TASK_INITIALIZE_0016_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0065_COMPLETED
  TASK_INITIALIZE_0065_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0035_COMPLETED
  TASK_INITIALIZE_0035_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0039_COMPLETED
  TASK_INITIALIZE_0039_COMPLETED();
#endif

  /* wait start event */
  OS__WAITEVENT(TASK_BlsmControl_Core1_100_0_Task,
		WE_BlsmControl_Core1_100_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_BlsmControl_Core1_100_0_Task_LOOP_TOP
    TASK_BlsmControl_Core1_100_0_Task_LOOP_TOP
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Zero_OrderHold4
     * Task: 0076
     * rate: 100
     */

    /* wait input channel */
    while (1) {
      /* input: BlsmControl_System_duty_culc_Abs */
      if (CH_BlsmControl_0015_0076.flag__0015_0076 > 0) {
	int ch__flag;
	BlsmControl_System_duty_culc_Abs_1 = CH_BlsmControl_0015_0076.BlsmControl_System_duty_culc_Abs_1;
	ch__flag = CH_BlsmControl_0015_0076.flag__0015_0076 - 1;
	CH_BlsmControl_0015_0076.flag__0015_0076 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0076, OUT_0015);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0076);
	break;
      }

      CH__TASK_SCHED(IN_0076,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0076_COMPLETED
    TASK_IN_0076_COMPLETED();
#endif

    /* C code */
    {
{

    /* ZeroOrderHold: '<S2>/Zero_OrderHold4' */
    BlsmControl_System_duty_culc_Zero_OrderHold4_1 =
      BlsmControl_System_duty_culc_Abs_1;

}

    }

#ifdef TASK_EXECUTE_0076_COMPLETED
    TASK_EXECUTE_0076_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Zero_OrderHold5
     * Task: 0077
     * rate: 100
     */

    /* wait input channel */
    while (1) {
      /* input: BlsmControl_System_duty_culc_Subsystem1_Divide */
      if (CH_BlsmControl_0043_0077.flag__0043_0077 > 0) {
	int ch__flag;
	BlsmControl_System_duty_culc_Subsystem1_Divide_1 = CH_BlsmControl_0043_0077.BlsmControl_System_duty_culc_Subsystem1_Divide_1;
	ch__flag = CH_BlsmControl_0043_0077.flag__0043_0077 - 1;
	CH_BlsmControl_0043_0077.flag__0043_0077 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0077, OUT_0043);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0077);
	break;
      }

      CH__TASK_SCHED(IN_0077,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0077_COMPLETED
    TASK_IN_0077_COMPLETED();
#endif

    /* C code */
    {
{

    /* ZeroOrderHold: '<S2>/Zero_OrderHold5' */
    BlsmControl_System_duty_culc_Zero_OrderHold5_1 =
      BlsmControl_System_duty_culc_Subsystem1_Divide_1;


}

    }

#ifdef TASK_EXECUTE_0077_COMPLETED
    TASK_EXECUTE_0077_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Divide1
     * Task: 0032
     * rate: 100
     */

    /* C code */
    {
{

    /* Product: '<S2>/Divide1' */
    BlsmControl_System_duty_culc_Divide1_1 =
      BlsmControl_System_duty_culc_Zero_OrderHold4_1 /
      BlsmControl_System_duty_culc_Zero_OrderHold5_1;


}

    }

#ifdef TASK_EXECUTE_0032_COMPLETED
    TASK_EXECUTE_0032_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Sum4
     * Task: 0063
     * rate: 100
     */

    /* C code */
    {
{

    /* Sum: '<S2>/Sum4' incorporates:
     *  Constant: '<S2>/Constant2'
     */
    BlsmControl_System_duty_culc_Sum4_1 = BlsmControl_System_duty_culc_Divide1_1
      - BlsmControl_P.Constant2_Value_k;


}

    }

#ifdef TASK_EXECUTE_0063_COMPLETED
    TASK_EXECUTE_0063_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Product1
     * Task: 0034
     * rate: 100
     */

    /* C code */
    {
{

    /* Product: '<S2>/Product1' incorporates:
     *  Constant: '<S2>/Constant3'
     */
    BlsmControl_System_duty_culc_Product1_1 =
      BlsmControl_System_duty_culc_Sum4_1 * BlsmControl_P.Constant3_Value_n;


}

    }

#ifdef TASK_EXECUTE_0034_COMPLETED
    TASK_EXECUTE_0034_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Sum5
     * Task: 0064
     * rate: 100
     */

    /* C code */
    {
{

    /* Sum: '<S2>/Sum5' incorporates:
     *  Constant: '<S2>/Constant4'
     */
    BlsmControl_System_duty_culc_Sum5_1 =
      BlsmControl_System_duty_culc_Product1_1 + BlsmControl_P.Constant4_Value_b;


}

    }

#ifdef TASK_EXECUTE_0064_COMPLETED
    TASK_EXECUTE_0064_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Sum3
     * Task: 0062
     * rate: 100
     */

    /* C code */
    {
{

    /* Sum: '<S2>/Sum3' */
    BlsmControl_System_duty_culc_Sum3_1 =
      BlsmControl_System_duty_culc_Zero_OrderHold4_1 -
      BlsmControl_System_duty_culc_Zero_OrderHold5_1;


}

    }

#ifdef TASK_EXECUTE_0062_COMPLETED
    TASK_EXECUTE_0062_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Divide2
     * Task: 0033
     * rate: 100
     */

    /* C code */
    {
{

    /* Product: '<S2>/Divide2' */
    BlsmControl_System_duty_culc_Divide2_1 = BlsmControl_System_duty_culc_Sum3_1
      / BlsmControl_System_duty_culc_Zero_OrderHold5_1;


}

    }

#ifdef TASK_EXECUTE_0033_COMPLETED
    TASK_EXECUTE_0033_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Abs1
     * Task: 0016
     * rate: 100
     */

    /* C code */
    {
{

    /* Abs: '<S2>/Abs1' */
    BlsmControl_System_duty_culc_Abs1_1 = fabs
      (BlsmControl_System_duty_culc_Divide2_1);


}

    }

#ifdef TASK_EXECUTE_0016_COMPLETED
    TASK_EXECUTE_0016_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Switch1
     * Task: 0065
     * rate: 100
     */

    /* C code */
    {
{

    /* Switch: '<S2>/Switch1' incorporates:
     *  Constant: '<S2>/Constant5'
     */
    if (BlsmControl_System_duty_culc_Abs1_1 > BlsmControl_P.Switch1_Threshold) {
      BlsmControl_System_duty_culc_Switch1_1 =
        BlsmControl_System_duty_culc_Sum5_1;
    } else {
      BlsmControl_System_duty_culc_Switch1_1 = BlsmControl_P.Constant5_Value;
    }

    /* End of Switch: '<S2>/Switch1' */

}

    }

#ifdef TASK_EXECUTE_0065_COMPLETED
    TASK_EXECUTE_0065_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Product2
     * Task: 0035
     * rate: 100
     */

    /* C code */
    {
{

    /* UnitDelay: '<S2>/UnitDelay1' */
    BlsmControl_System_duty_culc_UnitDelay1_1 =
      BlsmControl_DW.UnitDelay1_DSTATE_n;


}

{

    /* Product: '<S2>/Product2' */
    BlsmControl_System_duty_culc_Product2_1 =
      BlsmControl_System_duty_culc_Switch1_1 *
      BlsmControl_System_duty_culc_UnitDelay1_1;


}

    }

#ifdef TASK_EXECUTE_0035_COMPLETED
    TASK_EXECUTE_0035_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Saturation1
     * Task: 0039
     * rate: 100
     */

    /* C code */
    {
{

    /* Saturate: '<S2>/Saturation1' */
    u0 = BlsmControl_System_duty_culc_Product2_1;
    u1 = BlsmControl_P.Saturation1_LowerSat;
    u2 = BlsmControl_P.Saturation1_UpperSat;
    if (u0 > u2) {
      BlsmControl_System_duty_culc_Saturation1_1 = u2;
    } else if (u0 < u1) {
      BlsmControl_System_duty_culc_Saturation1_1 = u1;
    } else {
      BlsmControl_System_duty_culc_Saturation1_1 = u0;
    }

    /* End of Saturate: '<S2>/Saturation1' */

}

    }

#ifdef TASK_EXECUTE_0039_COMPLETED
    TASK_EXECUTE_0039_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: BlsmControl_System_duty_culc_RateTransition2 */
      if (CH_BlsmControl_0039_0037.flag__0039_0037 == 0) {
	unsigned long ch__sendcount;
	CH_BlsmControl_0039_0037.BlsmControl_System_duty_culc_Saturation1_1 = BlsmControl_System_duty_culc_Saturation1_1;
	CH__SYNCM();
	ch__sendcount = CH__SEND_COUNT(task__time+100, CH_BlsmControl_0039_0037.time__0039_0037, 1);
	CH_BlsmControl_0039_0037.flag__0039_0037 = ch__sendcount;
	CH_BlsmControl_0039_0037.time__0039_0037 += ch__sendcount;
	CH__EVENT(TASK_0039,IN_0037);
	CH__END_LOG(OUT_0039);
	break;
      }

      CH__TASK_SCHED(OUT_0039,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0039_COMPLETED
    TASK_OUT_0039_COMPLETED();
#endif
    /*
     * Block: BlsmControl_System_duty_culc_Product2
     * Update: 0035
     * rate: 100
     */

    /* wait input channel (update) */
    while (1) {
      /* update: BlsmControl_System_duty_culc_RateTransition3 */
      if (CH_BlsmControl_0038_0035.flag__0038_0035 > 0) {
	int ch__flag;
	BlsmControl_System_duty_culc_RateTransition3_1 = CH_BlsmControl_0038_0035.BlsmControl_System_duty_culc_RateTransition3_1;
	ch__flag = CH_BlsmControl_0038_0035.flag__0038_0035 - 1;
	CH_BlsmControl_0038_0035.flag__0038_0035 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0035, OUT_0038);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0035);
	break;
      }

      CH__CORE_THREAD_SCHED(IN_0035,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_UPDATE_IN_0035_COMPLETED
    TASK_UPDATE_IN_0035_COMPLETED();
#endif

    /* Update */
    {
{

    /* Update for UnitDelay: '<S2>/UnitDelay1' */
    BlsmControl_DW.UnitDelay1_DSTATE_n =
      BlsmControl_System_duty_culc_RateTransition3_1;

}

    }

#ifdef TASK_UPDATE_0035_COMPLETED
    TASK_UPDATE_0035_COMPLETED();
#endif

#ifdef TASK_BlsmControl_Core1_100_0_Task_BEFORE_SLEEP
    TASK_BlsmControl_Core1_100_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_BlsmControl_Core1_100_0_Task,
		  WE_BlsmControl_Core1_100_0_Task);

    task__time += 100;

#ifdef TASK_BlsmControl_Core1_100_0_Task_AFTER_SLEEP
    TASK_BlsmControl_Core1_100_0_Task_AFTER_SLEEP
#endif
  }
}
