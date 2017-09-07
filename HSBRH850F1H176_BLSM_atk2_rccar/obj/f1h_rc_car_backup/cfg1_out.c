/* cfg1_out.c */
#define TOPPERS_CFG1_OUT  1
#include "kernel/kernel_int.h"
#include "a_controller-app.h"
#include "rlin3x.h"
#include "target_hw_counter.h"


#ifdef INT64_MAX
  typedef sint64 signed_t;
  typedef uint64 unsigned_t;
#else
  typedef sint32 signed_t;
  typedef uint32 unsigned_t;
#endif

#include "target_cfg1_out.h"

const uint32 TOPPERS_cfg_magic_number = 0x12345678;
const uint32 TOPPERS_cfg_sizeof_signed_t = sizeof(signed_t);
const uint32 TOPPERS_cfg_sizeof_pointer = sizeof(const volatile void*);

const unsigned_t TOPPERS_cfg_SIL_ENDIAN_BIG = 
#if defined(SIL_ENDIAN_BIG)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_SIL_ENDIAN_LITTLE = 
#if defined(SIL_ENDIAN_LITTLE)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TARGET_MIN_STKSZ = 
#if defined(TARGET_MIN_STKSZ)
(TARGET_MIN_STKSZ);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_STKSZ_ALIGN = 
#if defined(CHECK_STKSZ_ALIGN)
(CHECK_STKSZ_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_FUNC_ALIGN = 
#if defined(CHECK_FUNC_ALIGN)
(CHECK_FUNC_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_FUNC_NONNULL = 
#if defined(CHECK_FUNC_NONNULL)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_STACK_ALIGN = 
#if defined(CHECK_STACK_ALIGN)
(CHECK_STACK_ALIGN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_CHECK_STACK_NONNULL = 
#if defined(CHECK_STACK_NONNULL)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_sizeof_void_ptr = ( unsigned_t )sizeof(void*);
const unsigned_t TOPPERS_cfg_sizeof_FunctionRefType = ( unsigned_t )sizeof(FunctionRefType);
const unsigned_t TOPPERS_cfg_sizeof_TINIB = ( unsigned_t )sizeof(TINIB);
const unsigned_t TOPPERS_cfg_sizeof_StackType = ( unsigned_t )sizeof(StackType);
const unsigned_t TOPPERS_cfg_sizeof_HWCNTINIB = ( unsigned_t )sizeof(HWCNTINIB);
const unsigned_t TOPPERS_cfg_sizeof_ALMINIB = ( unsigned_t )sizeof(ALMINIB);
const unsigned_t TOPPERS_cfg_offsetof_ALMINIB_action = ( unsigned_t )offsetof(ALMINIB,action);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_task = ( unsigned_t )offsetof(TINIB,task);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_stk = 
#if !defined(USE_TSKINICTXB)
(offsetof(TINIB,stk));
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_offsetof_HWCNTINIB_init = ( unsigned_t )offsetof(HWCNTINIB,init);
const unsigned_t TOPPERS_cfg_ENABLE = ( unsigned_t )ENABLE;
const unsigned_t TOPPERS_cfg_DISABLE = ( unsigned_t )DISABLE;
const unsigned_t TOPPERS_cfg_AUTO = ( unsigned_t )AUTO;
const unsigned_t TOPPERS_cfg_TMIN_TPRI = ( unsigned_t )TMIN_TPRI;
const unsigned_t TOPPERS_cfg_TMAX_TPRI = ( unsigned_t )TMAX_TPRI;
const unsigned_t TOPPERS_cfg_TNUM_TPRI = ( unsigned_t )TNUM_TPRI;
const unsigned_t TOPPERS_cfg_TPRI_MAXTASK = ( unsigned_t )TPRI_MAXTASK;
const unsigned_t TOPPERS_cfg_OMIT_INITIALIZE_INTERRUPT = 
#if defined(OMIT_INITIALIZE_INTERRUPT)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_USE_TSKINICTXB = 
#if defined(USE_TSKINICTXB)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_OMIT_CHECK_CYCLIC_CHAIN = 
#if defined(OMIT_CHECK_CYCLIC_CHAIN)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_MINIMUM_OSTKSZ = ( unsigned_t )MINIMUM_OSTKSZ;
const unsigned_t TOPPERS_cfg_DEFAULT_TASKSTKSZ = ( unsigned_t )DEFAULT_TASKSTKSZ;
const unsigned_t TOPPERS_cfg_DEFAULT_ISRSTKSZ = ( unsigned_t )DEFAULT_ISRSTKSZ;
const unsigned_t TOPPERS_cfg_DEFAULT_HOOKSTKSZ = ( unsigned_t )DEFAULT_HOOKSTKSZ;
const unsigned_t TOPPERS_cfg_DEFAULT_OSSTKSZ = ( unsigned_t )DEFAULT_OSSTKSZ;
const unsigned_t TOPPERS_cfg_OMIT_CHECK_DEADLOCK_SPINLOCK = 
#if defined(OMIT_CHECK_DEADLOCK_SPINLOCK)
(1);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_TNUM_HWCORE = ( unsigned_t )TNUM_HWCORE;
const unsigned_t TOPPERS_cfg_TTYPE_KLOCK = ( unsigned_t )TTYPE_KLOCK;
const unsigned_t TOPPERS_cfg_G_KLOCK = ( unsigned_t )G_KLOCK;
const unsigned_t TOPPERS_cfg_C_KLOCK = ( unsigned_t )C_KLOCK;
const unsigned_t TOPPERS_cfg_TTYPE_SPN = ( unsigned_t )TTYPE_SPN;
const unsigned_t TOPPERS_cfg_NATIVE_SPN = ( unsigned_t )NATIVE_SPN;
const unsigned_t TOPPERS_cfg_EMULATE_SPN = ( unsigned_t )EMULATE_SPN;
const signed_t TOPPERS_cfg_TMAX_NATIVE_SPN = 
#if (TTYPE_SPN == NATIVE_SPN)
(TMAX_NATIVE_SPN);
#else
(0);
#endif
const unsigned_t TOPPERS_cfg_INTPRI_ICI0 = ( unsigned_t )INTPRI_ICI0;
const unsigned_t TOPPERS_cfg_INTPRI_ICI1 = ( unsigned_t )INTPRI_ICI1;
const unsigned_t TOPPERS_cfg_OS_CORE_ID_MASTER = ( unsigned_t )OS_CORE_ID_MASTER;
const unsigned_t TOPPERS_cfg_OS_CORE_ID_0 = ( unsigned_t )OS_CORE_ID_0;
const unsigned_t TOPPERS_cfg_OS_CORE_ID_1 = ( unsigned_t )OS_CORE_ID_1;
const unsigned_t TOPPERS_cfg_TNUM_INTPRI = ( unsigned_t )TNUM_INTPRI;
const unsigned_t TOPPERS_cfg_TMAX_INTNO = ( unsigned_t )TMAX_INTNO;
const unsigned_t TOPPERS_cfg_offsetof_TCB_p_tinib = ( unsigned_t )offsetof(TCB,p_tinib);
const unsigned_t TOPPERS_cfg_offsetof_TCB_curpri = ( unsigned_t )offsetof(TCB,curpri);
const unsigned_t TOPPERS_cfg_offsetof_TCB_sp = ( unsigned_t )offsetof(TCB,tskctxb.sp);
const unsigned_t TOPPERS_cfg_offsetof_TCB_pc = ( unsigned_t )offsetof(TCB,tskctxb.pc);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_exepri = ( unsigned_t )offsetof(TINIB,exepri);
const unsigned_t TOPPERS_cfg_offsetof_TINIB_p_osapinib = ( unsigned_t )offsetof(TINIB,p_osapinib);
const unsigned_t TOPPERS_cfg_offsetof_ISRINIB_p_osapinib = ( unsigned_t )offsetof(ISRINIB,p_osapinib);
const unsigned_t TOPPERS_cfg_offsetof_ISRCB_p_isrinib = ( unsigned_t )offsetof(ISRCB,p_isrinib);
const unsigned_t TOPPERS_cfg_offsetof_CCB_p_runtsk = ( unsigned_t )offsetof(CCB,p_runtsk);
const unsigned_t TOPPERS_cfg_offsetof_CCB_p_schedtsk = ( unsigned_t )offsetof(CCB,p_schedtsk);
const unsigned_t TOPPERS_cfg_offsetof_CCB_p_runisr = ( unsigned_t )offsetof(CCB,p_runisr);
const unsigned_t TOPPERS_cfg_offsetof_CCB_p_runosap = ( unsigned_t )offsetof(CCB,p_runosap);
const unsigned_t TOPPERS_cfg_offsetof_CCB_kerflg = ( unsigned_t )offsetof(CCB,kerflg);
const unsigned_t TOPPERS_cfg_offsetof_CCB_ici_request_map = ( unsigned_t )offsetof(CCB,ici_request_map);
const unsigned_t TOPPERS_cfg_offsetof_CCB_ici_disreqflg = ( unsigned_t )offsetof(CCB,ici_disreqflg);
const unsigned_t TOPPERS_cfg_offsetof_CCB_callevel_stat = ( unsigned_t )offsetof(CCB,callevel_stat);
const unsigned_t TOPPERS_cfg_offsetof_ISRINIB_p_intinib = ( unsigned_t )offsetof(ISRINIB,p_intinib);
const unsigned_t TOPPERS_cfg_offsetof_INTINIB_remain_stksz = ( unsigned_t )offsetof(INTINIB,remain_stksz);
const unsigned_t TOPPERS_cfg_offsetof_CCB_except_nest_cnt = ( unsigned_t )offsetof(CCB,target_ccb.except_nest_cnt);
const unsigned_t TOPPERS_cfg_offsetof_CCB_current_iintpri = ( unsigned_t )offsetof(CCB,target_ccb.current_iintpri);
const unsigned_t TOPPERS_cfg_offsetof_CCB_nested_lock_os_int_cnt = ( unsigned_t )offsetof(CCB,target_ccb.nested_lock_os_int_cnt);
const unsigned_t TOPPERS_cfg_offsetof_CCB_current_intpri = ( unsigned_t )offsetof(CCB,target_ccb.current_intpri);
const unsigned_t TOPPERS_cfg_offsetof_CCB_trusted_hook_savedsp = ( unsigned_t )offsetof(CCB,target_ccb.trusted_hook_savedsp);
/* #include "a_controller-app.h" */
/* #include "rlin3x.h" */
/* #include "target_hw_counter.h" */
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_ALARMTIME_OsAlarmAutostart_OperationManagerCycAlarm = ( TICK_FOR_10MS ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_CYCLETIME_OsAlarmAutostart_OperationManagerCycAlarm = ( TICK_FOR_10MS ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_ALARMTIME_OsAlarmAutostart_DriveManagerCycAlarm = ( TICK_FOR_10MS*2 ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_CYCLETIME_OsAlarmAutostart_DriveManagerCycAlarm = ( TICK_FOR_10MS*2 ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_ALARMTIME_OsAlarmAutostart_BodyControlCycAlarm = ( TICK_FOR_10MS*10 ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_CYCLETIME_OsAlarmAutostart_BodyControlCycAlarm = ( TICK_FOR_10MS*10 ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_ALARMTIME_OsAlarmAutostart_BuzzerControlCycAlarm = ( TICK_FOR_10MS/80 ); 
const unsigned_t TOPPERS_cfg_valueof_ALM_AUTOSTART_CYCLETIME_OsAlarmAutostart_BuzzerControlCycAlarm = ( TICK_FOR_10MS/80 ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_PRIORITY_a_controller_Core0_1_0_Task_Os = ( a_controller_Core0_1_0_Task_Priority ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_STKSZ_a_controller_Core0_1_0_Task_Os = ( a_controller_Core0_1_0_Task_Stack ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_PRIORITY_a_controller_Core1_1_0_Task_Os = ( a_controller_Core1_1_0_Task_Priority ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_STKSZ_a_controller_Core1_1_0_Task_Os = ( a_controller_Core1_1_0_Task_Stack ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_PRIORITY_a_controller_Core0_100_0_Task_Os = ( a_controller_Core0_100_0_Task_Priority ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_STKSZ_a_controller_Core0_100_0_Task_Os = ( a_controller_Core0_100_0_Task_Stack ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_PRIORITY_a_controller_Core1_100_0_Task_Os = ( a_controller_Core1_100_0_Task_Priority ); 
const unsigned_t TOPPERS_cfg_valueof_TSK_STKSZ_a_controller_Core1_100_0_Task_Os = ( a_controller_Core1_100_0_Task_Stack ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTNO_RLIN3x_TX_ISR_Os = ( INTNO_RLIN3x_TX ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTPRI_RLIN3x_TX_ISR_Os = ( INTPRI_RLIN3x_TX ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTNO_RLIN3x_RX_ISR_Os = ( INTNO_RLIN3x_RX ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTPRI_RLIN3x_RX_ISR_Os = ( INTPRI_RLIN3x_RX ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTNO_C2ISR_for_MAIN_HW_COUNTER_CORE0_Os = ( HWC_DTIM_CORE0_INTNO ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTPRI_C2ISR_for_MAIN_HW_COUNTER_CORE0_Os = ( HWC_DTIM_CORE0_INTPRI ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTNO_C2ISR_for_MAIN_HW_COUNTER_CORE1_Os = ( HWC_DTIM_CORE1_INTNO ); 
const unsigned_t TOPPERS_cfg_valueof_ISR_INTPRI_C2ISR_for_MAIN_HW_COUNTER_CORE1_Os = ( HWC_DTIM_CORE1_INTPRI ); 

