/* Os_Lcfg.h */
#ifndef TOPPERS_OS_LCFG_H
#define TOPPERS_OS_LCFG_H

#define TNUM_ALARM				UINT_C(8)
#define TNUM_COUNTER			UINT_C(2)
#define TNUM_HARDCOUNTER		UINT_C(2)
#define TNUM_ISR2				UINT_C(4)
#define TNUM_ICI				UINT_C(0)
#define TNUM_STD_RESOURCE		UINT_C(0)
#define TNUM_TASK				UINT_C(7)
#define TNUM_EXTTASK			UINT_C(4)
#define TNUM_APP_MODE			UINT_C(1)
#define TNUM_SCHEDULETABLE		UINT_C(0)
#define TNUM_IMPLSCHEDULETABLE	UINT_C(0)
#define TNUM_OSAP				UINT_C(5)
#define TNUM_SPINLOCK			UINT_C(0)

/*
 *  Default Definitions of Trace Log Macros
 */

#ifndef TOPPERS_ENABLE_TRACE
#ifndef LOG_USER_MARK
#define LOG_USER_MARK(str)
#endif /* LOG_USER_MARK */
#endif /* TOPPERS_ENABLE_TRACE */

/****** Object TASK ******/

#define BlsmControl_Core0_1_0_Task	UINT_C(0)
#define BlsmControl_Core1_1_0_Task	UINT_C(1)
#define BlsmControl_Core0_100_0_Task	UINT_C(2)
#define BlsmControl_Core1_100_0_Task	UINT_C(3)
#define OperationManagerTask	UINT_C(4)
#define DriveManagerTask	UINT_C(5)
#define BodyControlTask	UINT_C(6)

/****** Object COUNTER ******/

#define MAIN_HW_COUNTER_CORE0	UINT_C(0)
#define MAIN_HW_COUNTER_CORE1	UINT_C(1)

#define OSMAXALLOWEDVALUE_MAIN_HW_COUNTER_CORE0	((TickType) 536870911)
#define OSTICKSPERBASE_MAIN_HW_COUNTER_CORE0	((TickType) 10)
#define OSMINCYCLE_MAIN_HW_COUNTER_CORE0	((TickType) 400)
#define OSMAXALLOWEDVALUE_MAIN_HW_COUNTER_CORE1	((TickType) 536870911)
#define OSTICKSPERBASE_MAIN_HW_COUNTER_CORE1	((TickType) 10)
#define OSMINCYCLE_MAIN_HW_COUNTER_CORE1	((TickType) 400)

#define OS_TICKS2SEC_MAIN_HW_COUNTER_CORE0(tick)	(((PhysicalTimeType)125U) * (tick) / 1000000000U)	/* (0.000000125 * 1000000000) * (tick) / 1000000000 */
#define OS_TICKS2MS_MAIN_HW_COUNTER_CORE0(tick)	(((PhysicalTimeType)125U) * (tick) / 1000000U)		/* (0.000000125 * 1000000000) * (tick) / 1000000 */
#define OS_TICKS2US_MAIN_HW_COUNTER_CORE0(tick)	(((PhysicalTimeType)125U) * (tick) / 1000U)			/* (0.000000125 * 1000000000) * (tick) / 1000 */
#define OS_TICKS2NS_MAIN_HW_COUNTER_CORE0(tick)	(((PhysicalTimeType)125U) * (tick))					/* (0.000000125 * 1000000000) * (tick) */

#define OS_TICKS2SEC_MAIN_HW_COUNTER_CORE1(tick)	(((PhysicalTimeType)125U) * (tick) / 1000000000U)	/* (0.000000125 * 1000000000) * (tick) / 1000000000 */
#define OS_TICKS2MS_MAIN_HW_COUNTER_CORE1(tick)	(((PhysicalTimeType)125U) * (tick) / 1000000U)		/* (0.000000125 * 1000000000) * (tick) / 1000000 */
#define OS_TICKS2US_MAIN_HW_COUNTER_CORE1(tick)	(((PhysicalTimeType)125U) * (tick) / 1000U)			/* (0.000000125 * 1000000000) * (tick) / 1000 */
#define OS_TICKS2NS_MAIN_HW_COUNTER_CORE1(tick)	(((PhysicalTimeType)125U) * (tick))					/* (0.000000125 * 1000000000) * (tick) */

/****** Object ALARM ******/

#define OperationManagerCycAlarm	UINT_C(0)
#define DriveManagerCycAlarm	UINT_C(1)
#define BodyControlCycAlarm	UINT_C(2)
#define BuzzerControlCycAlarm	UINT_C(3)
#define BlsmControl_Core0_1_0_Alarm	UINT_C(4)
#define BlsmControl_Core1_1_0_Alarm	UINT_C(5)
#define BlsmControl_Core0_100_0_Alarm	UINT_C(6)
#define BlsmControl_Core1_100_0_Alarm	UINT_C(7)

/****** Object SCHEDULETABLE ******/


/****** Object RESOURCE ******/


/****** Object ISR ******/

#define RLIN3x_TX_ISR	UINT_C(0)
#define RLIN3x_RX_ISR	UINT_C(1)
#define C2ISR_for_MAIN_HW_COUNTER_CORE0	UINT_C(2)
#define C2ISR_for_MAIN_HW_COUNTER_CORE1	UINT_C(3)


/****** Object APPMODE ******/

#define BlsmControl_AppMode	UINT_C(0)

/****** Object EVENT ******/
#define BlsmControl_WakeupEvent	UINT_C(0x00000001)
#define BlsmControl_InputEvent	UINT_C(0x00000002)
#define BlsmControl_OutputEvent	UINT_C(0x00000004)
#define BlsmControl_CompleteEvent	UINT_C(0x00000008)

/****** Object OSApplication ******/
#define blsm_OSAP_CORE0	UINT_C(0)
#define BlsmControl_OSAP_CORE0	UINT_C(1)
#define BlsmControl_OSAP_CORE1	UINT_C(2)
#define OSAP_for_MAIN_HW_COUNTER_CORE0	UINT_C(3)
#define OSAP_for_MAIN_HW_COUNTER_CORE1	UINT_C(4)

/****** Object SPINLOCK ******/


/****** Object IOC ******/
#define TNUM_IOC			UINT_C(0)
#define TNUM_QUEUEIOC		UINT_C(0)
#define TNUM_IOC_WRAPPER	UINT_C(0)

#ifndef TOPPERS_MACRO_ONLY
extern TASK(BlsmControl_Core0_1_0_Task);
extern TASK(BlsmControl_Core1_1_0_Task);
extern TASK(BlsmControl_Core0_100_0_Task);
extern TASK(BlsmControl_Core1_100_0_Task);
extern TASK(OperationManagerTask);
extern TASK(DriveManagerTask);
extern TASK(BodyControlTask);

extern void init_hwcounter_MAIN_HW_COUNTER_CORE0(TickType maxval, TimeType nspertick);
extern void start_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void stop_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void set_hwcounter_MAIN_HW_COUNTER_CORE0(TickType exprtick);
extern TickType get_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void cancel_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void trigger_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void int_clear_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void int_cancel_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void increment_hwcounter_MAIN_HW_COUNTER_CORE0(void);
extern void init_hwcounter_MAIN_HW_COUNTER_CORE1(TickType maxval, TimeType nspertick);
extern void start_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void stop_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void set_hwcounter_MAIN_HW_COUNTER_CORE1(TickType exprtick);
extern TickType get_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void cancel_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void trigger_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void int_clear_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void int_cancel_hwcounter_MAIN_HW_COUNTER_CORE1(void);
extern void increment_hwcounter_MAIN_HW_COUNTER_CORE1(void);

extern ALARMCALLBACK(BuzzerControlCycAlarmCb);


extern void _kernel_inthdr_0xffff00a5(void);
extern void _kernel_inthdr_0xffff00a6(void);
extern void _kernel_inthdr_0xffff0050(void);
extern void _kernel_inthdr_0xffff00a8(void);

extern ISR(RLIN3x_TX_ISR);
extern ISR(RLIN3x_RX_ISR);
extern ISR(C2ISR_for_MAIN_HW_COUNTER_CORE0);
extern ISR(C2ISR_for_MAIN_HW_COUNTER_CORE1);

extern ISR(target_ici_handler0);
extern ISR(target_ici_handler1);

#ifdef TOPPERS_ENABLE_TRACE
extern const char8 *atk2_appid_str(AppModeType id);
extern const char8 *atk2_tskid_str(TaskType id);
extern const char8 *atk2_isrid_str(ISRType id);
extern const char8 *atk2_cntid_str(CounterType id);
extern const char8 *atk2_almid_str(AlarmType id);
extern const char8 *atk2_resid_str(ResourceType id);
extern const char8 *atk2_schtblid_str(ScheduleTableType id);
extern const char8 *atk2_evtid_str(TaskType task, EventMaskType event);
extern const char8 *atk2_osapid_str(ApplicationType id);
extern const char8 *atk2_iocid_str(IocType id);
#endif /* TOPPERS_ENABLE_TRACE */
#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_OS_LCFG_H */

