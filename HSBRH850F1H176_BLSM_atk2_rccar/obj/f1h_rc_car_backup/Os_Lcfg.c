/* Os_Lcfg.c */
#include "kernel/kernel_int.h"
#include "Os_Lcfg.h"

#ifndef TOPPERS_EMPTY_LABEL
#define TOPPERS_EMPTY_LABEL(x, y) x y[0]
#endif

/*
 *  Default Definitions of Trace Log Macros
 */

#ifndef LOG_ISR_ENTER
#define LOG_ISR_ENTER(isrid)
#endif /* LOG_ISR_ENTER */

#ifndef LOG_ISR_LEAVE
#define LOG_ISR_LEAVE(isrid)
#endif /* LOG_ISR_LEAVE */

/*
 *  Include Directives (#include)
 */

#include "a_controller-app.h"
#include "rlin3x.h"
#include "target_hw_counter.h"

const AlarmType					tnum_alarm				= TNUM_ALARM;
const CounterType				tnum_counter			= TNUM_COUNTER;
const CounterType				tnum_hardcounter		= TNUM_HARDCOUNTER;
const ISRType					tnum_isr2				= TNUM_ISR2;
const ISRType					tnum_ici				= TNUM_ICI;
const ResourceType				tnum_stdresource		= TNUM_STD_RESOURCE;
const TaskType					tnum_task				= TNUM_TASK;
const TaskType					tnum_exttask			= TNUM_EXTTASK;
const AppModeType				tnum_appmode			= TNUM_APP_MODE;
const ScheduleTableType			tnum_scheduletable		= TNUM_SCHEDULETABLE;
const ScheduleTableType			tnum_implscheduletable	= TNUM_IMPLSCHEDULETABLE;
const ApplicationType			tnum_osap				= TNUM_OSAP;
const SpinlockIdType			tnum_spinlock			= TNUM_SPINLOCK;


/****** CCB ******/

CCB _kernel_core0_ccb = {0, FALSE, 0, 0, 0, 0, 0};
CCB _kernel_core1_ccb = {0, FALSE, 0, 0, 0, 0, 0};


CCB * const p_ccb_table[TotalNumberOfCores] = {
	&_kernel_core0_ccb,
	&_kernel_core1_ccb
};


/****** Object TASK ******/

static StackType _kernel_stack_a_controller_Core0_1_0_Task[COUNT_STK_T(5000U)] __attribute__((section(".pe1_lram")));
static StackType _kernel_stack_a_controller_Core1_1_0_Task[COUNT_STK_T(5000U)] __attribute__((section(".pe2_lram")));
static StackType _kernel_stack_a_controller_Core0_100_0_Task[COUNT_STK_T(5000U)] __attribute__((section(".pe1_lram")));
static StackType _kernel_stack_a_controller_Core1_100_0_Task[COUNT_STK_T(5000U)] __attribute__((section(".pe2_lram")));
static StackType _kernel_shared_stack_2[COUNT_STK_T(2048U)] __attribute__((section(".pe1_lram")));
static StackType _kernel_shared_stack_3[COUNT_STK_T(2048U)] __attribute__((section(".pe1_lram")));

const TINIB tinib_table[TNUM_TASK] = {
	{
		&TASKNAME(a_controller_Core0_1_0_Task),
		ROUND_STK_T(5000U),
		_kernel_stack_a_controller_Core0_1_0_Task,
		&(osapinib_table[a_controller_OSAP_CORE0]),
		0,
		0,
		(1U) - 1U,
		0x00000003U,
		&_kernel_core0_ccb
	},
	{
		&TASKNAME(a_controller_Core1_1_0_Task),
		ROUND_STK_T(5000U),
		_kernel_stack_a_controller_Core1_1_0_Task,
		&(osapinib_table[a_controller_OSAP_CORE1]),
		0,
		0,
		(1U) - 1U,
		0x00000003U,
		&_kernel_core1_ccb
	},
	{
		&TASKNAME(a_controller_Core0_100_0_Task),
		ROUND_STK_T(5000U),
		_kernel_stack_a_controller_Core0_100_0_Task,
		&(osapinib_table[a_controller_OSAP_CORE0]),
		1,
		1,
		(1U) - 1U,
		0x00000003U,
		&_kernel_core0_ccb
	},
	{
		&TASKNAME(a_controller_Core1_100_0_Task),
		ROUND_STK_T(5000U),
		_kernel_stack_a_controller_Core1_100_0_Task,
		&(osapinib_table[a_controller_OSAP_CORE1]),
		1,
		1,
		(1U) - 1U,
		0x00000003U,
		&_kernel_core1_ccb
	},
	{
		&TASKNAME(OperationManagerTask),
		ROUND_STK_T(2048U),
		_kernel_shared_stack_2,
		&(osapinib_table[blsm_OSAP_CORE0]),
		13,
		13,
		(1U) - 1U,
		0x00000000U,
		&_kernel_core0_ccb
	},
	{
		&TASKNAME(DriveManagerTask),
		ROUND_STK_T(2048U),
		_kernel_shared_stack_3,
		&(osapinib_table[blsm_OSAP_CORE0]),
		12,
		12,
		(1U) - 1U,
		0x00000000U,
		&_kernel_core0_ccb
	},
	{
		&TASKNAME(BodyControlTask),
		ROUND_STK_T(2048U),
		_kernel_shared_stack_2,
		&(osapinib_table[blsm_OSAP_CORE0]),
		13,
		13,
		(1U) - 1U,
		0x00000000U,
		&_kernel_core0_ccb
	}
};

TCB _kernel_tcb_a_controller_Core0_1_0_Task;
TCB _kernel_tcb_a_controller_Core1_1_0_Task;
TCB _kernel_tcb_a_controller_Core0_100_0_Task;
TCB _kernel_tcb_a_controller_Core1_100_0_Task;
TCB _kernel_tcb_OperationManagerTask;
TCB _kernel_tcb_DriveManagerTask;
TCB _kernel_tcb_BodyControlTask;

TCB * const p_tcb_table[TNUM_TASK] ={
	&_kernel_tcb_a_controller_Core0_1_0_Task,
	&_kernel_tcb_a_controller_Core1_1_0_Task,
	&_kernel_tcb_a_controller_Core0_100_0_Task,
	&_kernel_tcb_a_controller_Core1_100_0_Task,
	&_kernel_tcb_OperationManagerTask,
	&_kernel_tcb_DriveManagerTask,
	&_kernel_tcb_BodyControlTask
};

/****** Object COUNTER ******/

const CNTINIB cntinib_table[TNUM_COUNTER] = {
	{ 536870911U, (536870911U * 2U) + 1U, 10U, 400U, 0, &(osapinib_table[OSAP_for_MAIN_HW_COUNTER_CORE0]) },
	{ 536870911U, (536870911U * 2U) + 1U, 10U, 400U, 1, &(osapinib_table[OSAP_for_MAIN_HW_COUNTER_CORE1]) }
};

CNTCB _kernel_cntcb_MAIN_HW_COUNTER_CORE0;
CNTCB _kernel_cntcb_MAIN_HW_COUNTER_CORE1;

CNTCB * const p_cntcb_table[TNUM_COUNTER] = {
	&_kernel_cntcb_MAIN_HW_COUNTER_CORE0,
	&_kernel_cntcb_MAIN_HW_COUNTER_CORE1
};
const HWCNTINIB hwcntinib_table[TNUM_HARDCOUNTER] = 
{
	{
		&init_hwcounter_MAIN_HW_COUNTER_CORE0,
		&start_hwcounter_MAIN_HW_COUNTER_CORE0,
		&stop_hwcounter_MAIN_HW_COUNTER_CORE0,
		&set_hwcounter_MAIN_HW_COUNTER_CORE0,
		&get_hwcounter_MAIN_HW_COUNTER_CORE0,
		&cancel_hwcounter_MAIN_HW_COUNTER_CORE0,
		&trigger_hwcounter_MAIN_HW_COUNTER_CORE0,
		&int_clear_hwcounter_MAIN_HW_COUNTER_CORE0,
		&int_cancel_hwcounter_MAIN_HW_COUNTER_CORE0,
		&increment_hwcounter_MAIN_HW_COUNTER_CORE0,
		125U,		/* 0.000000125 * 1000000000 */ 
		0
	},
	{
		&init_hwcounter_MAIN_HW_COUNTER_CORE1,
		&start_hwcounter_MAIN_HW_COUNTER_CORE1,
		&stop_hwcounter_MAIN_HW_COUNTER_CORE1,
		&set_hwcounter_MAIN_HW_COUNTER_CORE1,
		&get_hwcounter_MAIN_HW_COUNTER_CORE1,
		&cancel_hwcounter_MAIN_HW_COUNTER_CORE1,
		&trigger_hwcounter_MAIN_HW_COUNTER_CORE1,
		&int_clear_hwcounter_MAIN_HW_COUNTER_CORE1,
		&int_cancel_hwcounter_MAIN_HW_COUNTER_CORE1,
		&increment_hwcounter_MAIN_HW_COUNTER_CORE1,
		125U,		/* 0.000000125 * 1000000000 */ 
		1
	}
};


/****** Object ALARM ******/

static void
_activate_alarm_1(void);
static void
_activate_alarm_1(void)
{
	(void) activate_task_action(OperationManagerTask);
}

static void
_activate_alarm_2(void);
static void
_activate_alarm_2(void)
{
	(void) activate_task_action(DriveManagerTask);
}

static void
_activate_alarm_3(void);
static void
_activate_alarm_3(void)
{
	(void) activate_task_action(BodyControlTask);
}


static void
_setevent_alarm_5(void);
static void
_setevent_alarm_5(void)
{
	(void) set_event_action(a_controller_Core0_1_0_Task, a_controller_WakeupEvent);
}

static void
_setevent_alarm_6(void);
static void
_setevent_alarm_6(void)
{
	(void) set_event_action(a_controller_Core1_1_0_Task, a_controller_WakeupEvent);
}

static void
_setevent_alarm_7(void);
static void
_setevent_alarm_7(void)
{
	(void) set_event_action(a_controller_Core0_100_0_Task, a_controller_WakeupEvent);
}

static void
_setevent_alarm_8(void);
static void
_setevent_alarm_8(void)
{
	(void) set_event_action(a_controller_Core1_100_0_Task, a_controller_WakeupEvent);
}

const ALMINIB alminib_table[TNUM_ALARM] = {
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_activate_alarm_1, 0x00000003U, 80000U, 80000U, (ACTIVATETASK & CALLBACK) | ABSOLUTE, &(osapinib_table[blsm_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_activate_alarm_2, 0x00000003U, 160000U, 160000U, (ACTIVATETASK & CALLBACK) | ABSOLUTE, &(osapinib_table[blsm_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_activate_alarm_3, 0x00000001U, 800000U, 800000U, (ACTIVATETASK & CALLBACK) | ABSOLUTE, &(osapinib_table[blsm_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, ALARMCALLBACKNAME(BuzzerControlCycAlarmCb), 0x00000001U, 1000U, 1000U, (CALLBACK & CALLBACK) | ABSOLUTE, &(osapinib_table[blsm_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_setevent_alarm_5, 0x00000000U, 0U, 0U, SETEVENT & CALLBACK, &(osapinib_table[a_controller_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_setevent_alarm_6, 0x00000000U, 0U, 0U, SETEVENT & CALLBACK, &(osapinib_table[a_controller_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_setevent_alarm_7, 0x00000000U, 0U, 0U, SETEVENT & CALLBACK, &(osapinib_table[a_controller_OSAP_CORE0]) },
	{ &_kernel_cntcb_MAIN_HW_COUNTER_CORE0, &_setevent_alarm_8, 0x00000000U, 0U, 0U, SETEVENT & CALLBACK, &(osapinib_table[a_controller_OSAP_CORE0]) }
};

ALMCB _kernel_almcb_OperationManagerCycAlarm;
ALMCB _kernel_almcb_DriveManagerCycAlarm;
ALMCB _kernel_almcb_BodyControlCycAlarm;
ALMCB _kernel_almcb_BuzzerControlCycAlarm;
ALMCB _kernel_almcb_a_controller_Core0_1_0_Alarm;
ALMCB _kernel_almcb_a_controller_Core1_1_0_Alarm;
ALMCB _kernel_almcb_a_controller_Core0_100_0_Alarm;
ALMCB _kernel_almcb_a_controller_Core1_100_0_Alarm;

ALMCB * const p_almcb_table[TNUM_ALARM] = {
	&_kernel_almcb_OperationManagerCycAlarm,
	&_kernel_almcb_DriveManagerCycAlarm,
	&_kernel_almcb_BodyControlCycAlarm,
	&_kernel_almcb_BuzzerControlCycAlarm,
	&_kernel_almcb_a_controller_Core0_1_0_Alarm,
	&_kernel_almcb_a_controller_Core1_1_0_Alarm,
	&_kernel_almcb_a_controller_Core0_100_0_Alarm,
	&_kernel_almcb_a_controller_Core1_100_0_Alarm
};

/****** Object SCHEDULETABLE ******/


TOPPERS_EMPTY_LABEL(const SCHTBLINIB, schtblinib_table);
TOPPERS_EMPTY_LABEL(SCHTBLCB * const, p_schtblcb_table);


/****** Object RESOURCE ******/

TOPPERS_EMPTY_LABEL(const RESINIB, resinib_table);
TOPPERS_EMPTY_LABEL(RESCB * const, p_rescb_table);


/****** Object ISR(include InterCoreInterrupts) ******/


const ISRINIB isrinib_table[TNUM_ISR2] = {
	{
		&(intinib_table[RLIN3x_TX_ISR]), &(osapinib_table[blsm_OSAP_CORE0])
	},
	{
		&(intinib_table[RLIN3x_RX_ISR]), &(osapinib_table[blsm_OSAP_CORE0])
	},
	{
		&(intinib_table[C2ISR_for_MAIN_HW_COUNTER_CORE0]), &(osapinib_table[OSAP_for_MAIN_HW_COUNTER_CORE0])
	},
	{
		&(intinib_table[C2ISR_for_MAIN_HW_COUNTER_CORE1]), &(osapinib_table[OSAP_for_MAIN_HW_COUNTER_CORE1])
	}
};

ISRCB _kernel_isrcb_RLIN3x_TX_ISR;
ISRCB _kernel_isrcb_RLIN3x_RX_ISR;
ISRCB _kernel_isrcb_C2ISR_for_MAIN_HW_COUNTER_CORE0;
ISRCB _kernel_isrcb_C2ISR_for_MAIN_HW_COUNTER_CORE1;


ISRCB * const p_isrcb_table[TNUM_ISR2] = {
	&_kernel_isrcb_RLIN3x_TX_ISR,
	&_kernel_isrcb_RLIN3x_RX_ISR,
	&_kernel_isrcb_C2ISR_for_MAIN_HW_COUNTER_CORE0,
	&_kernel_isrcb_C2ISR_for_MAIN_HW_COUNTER_CORE1
};


/****** Object InterCoreInterrupts ******/

TOPPERS_EMPTY_LABEL(const ICIINIB, iciinib_table);
const ICIINIB * p_iciinb_table[TotalNumberOfCores] = {0};
const uint8 tnum_ici_of_core[TotalNumberOfCores] = {0};


const OSAPINIB osapinib_table[TNUM_OSAP] = {
	{
		0U
	},
	{
		0U
	},
	{
		1U
	},
	{
		0U
	},
	{
		1U
	}
};

/****** Object SPINLOCK ******/
TOPPERS_EMPTY_LABEL(const SPNINIB, spninib_table);
TOPPERS_EMPTY_LABEL(SPNCB, spncb_table);

void
object_initialize(void)
{
	interrupt_initialize();
	task_initialize();
	counter_initialize();
	alarm_initialize();
}


void
object_terminate(void)
{
	counter_terminate();
}


/*
 *  Interrupt Management Functions
 */

void
_kernel_inthdr_0xffff00a5(void)
{
	i_begin_int(4294901925U);
	LOG_ISR_ENTER(RLIN3x_TX_ISR);
	ISRNAME(RLIN3x_TX_ISR)();
	LOG_ISR_LEAVE(RLIN3x_TX_ISR);
	i_end_int(4294901925U);
}
void
_kernel_inthdr_0xffff00a6(void)
{
	i_begin_int(4294901926U);
	LOG_ISR_ENTER(RLIN3x_RX_ISR);
	ISRNAME(RLIN3x_RX_ISR)();
	LOG_ISR_LEAVE(RLIN3x_RX_ISR);
	i_end_int(4294901926U);
}
void
_kernel_inthdr_0xffff0050(void)
{
	i_begin_int(4294901840U);
	LOG_ISR_ENTER(C2ISR_for_MAIN_HW_COUNTER_CORE0);
	ISRNAME(C2ISR_for_MAIN_HW_COUNTER_CORE0)();
	LOG_ISR_LEAVE(C2ISR_for_MAIN_HW_COUNTER_CORE0);
	i_end_int(4294901840U);
}
void
_kernel_inthdr_0xffff00a8(void)
{
	i_begin_int(4294901928U);
	LOG_ISR_ENTER(C2ISR_for_MAIN_HW_COUNTER_CORE1);
	ISRNAME(C2ISR_for_MAIN_HW_COUNTER_CORE1)();
	LOG_ISR_LEAVE(C2ISR_for_MAIN_HW_COUNTER_CORE1);
	i_end_int(4294901928U);
}

/* HardWare Counter Interrupt Handler(C2ISR) */
ISR(C2ISR_for_MAIN_HW_COUNTER_CORE0)
{
	notify_hardware_counter(MAIN_HW_COUNTER_CORE0);
}
ISR(C2ISR_for_MAIN_HW_COUNTER_CORE1)
{
	notify_hardware_counter(MAIN_HW_COUNTER_CORE1);
}

/*
 *  Stack Area for Non-task Context
 */

/* calculate stack size for MC */
#define TNUM_INTNO	UINT_C(4)
const InterruptNumberType tnum_intno = TNUM_INTNO;

const INTINIB intinib_table[TNUM_INTNO] = {
	{ (4294901925U), ENABLE, (-4), 0U, 0x450U},
	{ (4294901926U), ENABLE, (-4), 0U, 0x450U},
	{ (4294901840U), ENABLE, (-1), 0U, 0x850U},
	{ (4294901928U), ENABLE, (-1), 1U, 0x650U}
};

const MemorySizeType ici_remain_stksz[TotalNumberOfCores] = {
	 0x450U,
	 0x400U
};
static StackType _kernel_core0_ostack[COUNT_STK_T(2384)] __attribute__((section(".pe1_lram")));
#define TOPPERS_CORE0_OSTKSZ		ROUND_STK_T(2384)
#define TOPPERS_CORE0_OSTK		_kernel_core0_ostack

static StackType _kernel_core1_ostack[COUNT_STK_T(0x750U)] __attribute__((section(".pe2_lram")));
#define TOPPERS_CORE1_OSTKSZ		ROUND_STK_T(0x750U)
#define TOPPERS_CORE1_OSTK		_kernel_core1_ostack


const MemorySizeType	_ostksz_table[TotalNumberOfCores] = {
	TOPPERS_CORE0_OSTKSZ,
	TOPPERS_CORE1_OSTKSZ
};

StackType * const		_ostk_table[TotalNumberOfCores] = {
	(StackType *) TOPPERS_CORE0_OSTK,
	(StackType *) TOPPERS_CORE1_OSTK
};

#ifdef TOPPERS_OSTKPT
StackType * const	_ostkpt_table[TotalNumberOfCores] = {
	TOPPERS_OSTKPT(TOPPERS_CORE0_OSTK, TOPPERS_CORE0_OSTKSZ),
	TOPPERS_OSTKPT(TOPPERS_CORE1_OSTK, TOPPERS_CORE1_OSTKSZ)
};
#endif /* TOPPERS_OSTKPT */


/****** Object IOC ******/
const IocType	tnum_ioc = TNUM_IOC;
const IocType	tnum_queueioc = TNUM_QUEUEIOC;

TOPPERS_EMPTY_LABEL(void *, ioc_inival_table);
TOPPERS_EMPTY_LABEL(IOCCB, ioccb_table);
TOPPERS_EMPTY_LABEL(const IOCINIB, iocinib_table);
TOPPERS_EMPTY_LABEL(const IOCWRPINIB, iocwrpinib_table);

#ifdef TOPPERS_ENABLE_TRACE
const char8 *
atk2_appid_str(AppModeType id)
{
	const char8	*appid_str;
	switch (id) {
	case a_controller_AppMode:
		appid_str = "a_controller_AppMode";
		break;
	case debug_AppMode:
		appid_str = "debug_AppMode";
		break;
	default:
		appid_str = "";
		break;
	}
	return(appid_str);
}
const char8 *
atk2_tskid_str(TaskType id)
{
	const char8	*tskid_str;
	switch (id) {
	case a_controller_Core0_1_0_Task:
		tskid_str = "a_controller_Core0_1_0_Task";
		break;
	case a_controller_Core1_1_0_Task:
		tskid_str = "a_controller_Core1_1_0_Task";
		break;
	case a_controller_Core0_100_0_Task:
		tskid_str = "a_controller_Core0_100_0_Task";
		break;
	case a_controller_Core1_100_0_Task:
		tskid_str = "a_controller_Core1_100_0_Task";
		break;
	case OperationManagerTask:
		tskid_str = "OperationManagerTask";
		break;
	case DriveManagerTask:
		tskid_str = "DriveManagerTask";
		break;
	case BodyControlTask:
		tskid_str = "BodyControlTask";
		break;
	case INVALID_TASK:
		tskid_str = "INVALID_TASK";
		break;
	default:
		tskid_str = "";
		break;
	}
	return(tskid_str);
}

const char8 *
atk2_isrid_str(ISRType id)
{
	const char8	*isrid_str;
	switch (id) {
	case RLIN3x_TX_ISR:
		isrid_str = "RLIN3x_TX_ISR";
		break;
	case RLIN3x_RX_ISR:
		isrid_str = "RLIN3x_RX_ISR";
		break;
	case C2ISR_for_MAIN_HW_COUNTER_CORE0:
		isrid_str = "C2ISR_for_MAIN_HW_COUNTER_CORE0";
		break;
	case C2ISR_for_MAIN_HW_COUNTER_CORE1:
		isrid_str = "C2ISR_for_MAIN_HW_COUNTER_CORE1";
		break;
	case INVALID_ISR:
		isrid_str = "INVALID_ISR";
		break;
	default:
		isrid_str = "";
		break;
	}
	return(isrid_str);
}

const char8 *
atk2_cntid_str(CounterType id)
{
	const char8	*cntid_str;
	switch (id) {
	case MAIN_HW_COUNTER_CORE0:
		cntid_str = "MAIN_HW_COUNTER_CORE0";
		break;
	case MAIN_HW_COUNTER_CORE1:
		cntid_str = "MAIN_HW_COUNTER_CORE1";
		break;
	default:
		cntid_str = "";
		break;
	}
	return(cntid_str);
}

const char8 *
atk2_almid_str(AlarmType id)
{
	const char8	*almid_str;
	switch (id) {
	case OperationManagerCycAlarm:
		almid_str = "OperationManagerCycAlarm";
		break;
	case DriveManagerCycAlarm:
		almid_str = "DriveManagerCycAlarm";
		break;
	case BodyControlCycAlarm:
		almid_str = "BodyControlCycAlarm";
		break;
	case BuzzerControlCycAlarm:
		almid_str = "BuzzerControlCycAlarm";
		break;
	case a_controller_Core0_1_0_Alarm:
		almid_str = "a_controller_Core0_1_0_Alarm";
		break;
	case a_controller_Core1_1_0_Alarm:
		almid_str = "a_controller_Core1_1_0_Alarm";
		break;
	case a_controller_Core0_100_0_Alarm:
		almid_str = "a_controller_Core0_100_0_Alarm";
		break;
	case a_controller_Core1_100_0_Alarm:
		almid_str = "a_controller_Core1_100_0_Alarm";
		break;
	default:
		almid_str = "";
		break;
	}
	return(almid_str);
}

const char8 *
atk2_resid_str(ResourceType id)
{
	return("");
}

const char8 *
atk2_schtblid_str(ScheduleTableType id)
{
	return("");
}

const char8 *
atk2_evtid_str(TaskType task, EventMaskType event)
{
	const char8	*evtid_str;
	switch (task) {
	case a_controller_Core0_1_0_Task:
		switch (event) {
		case a_controller_WakeupEvent:
			evtid_str = "a_controller_WakeupEvent";
			break;
		case a_controller_InputEvent:
			evtid_str = "a_controller_InputEvent";
			break;
		case a_controller_OutputEvent:
			evtid_str = "a_controller_OutputEvent";
			break;
		case a_controller_CompleteEvent:
			evtid_str = "a_controller_CompleteEvent";
			break;
		default:
			evtid_str = NULL;
			break;
		}
		break;
	case a_controller_Core1_1_0_Task:
		switch (event) {
		case a_controller_WakeupEvent:
			evtid_str = "a_controller_WakeupEvent";
			break;
		case a_controller_InputEvent:
			evtid_str = "a_controller_InputEvent";
			break;
		case a_controller_OutputEvent:
			evtid_str = "a_controller_OutputEvent";
			break;
		case a_controller_CompleteEvent:
			evtid_str = "a_controller_CompleteEvent";
			break;
		default:
			evtid_str = NULL;
			break;
		}
		break;
	case a_controller_Core0_100_0_Task:
		switch (event) {
		case a_controller_WakeupEvent:
			evtid_str = "a_controller_WakeupEvent";
			break;
		case a_controller_InputEvent:
			evtid_str = "a_controller_InputEvent";
			break;
		case a_controller_OutputEvent:
			evtid_str = "a_controller_OutputEvent";
			break;
		case a_controller_CompleteEvent:
			evtid_str = "a_controller_CompleteEvent";
			break;
		default:
			evtid_str = NULL;
			break;
		}
		break;
	case a_controller_Core1_100_0_Task:
		switch (event) {
		case a_controller_WakeupEvent:
			evtid_str = "a_controller_WakeupEvent";
			break;
		case a_controller_InputEvent:
			evtid_str = "a_controller_InputEvent";
			break;
		case a_controller_OutputEvent:
			evtid_str = "a_controller_OutputEvent";
			break;
		case a_controller_CompleteEvent:
			evtid_str = "a_controller_CompleteEvent";
			break;
		default:
			evtid_str = NULL;
			break;
		}
		break;
	case OperationManagerTask:
		evtid_str = NULL;
		break;
	case DriveManagerTask:
		evtid_str = NULL;
		break;
	case BodyControlTask:
		evtid_str = NULL;
		break;
	default:
		evtid_str = NULL;
		break;
	}
	if (evtid_str == NULL) {
		if (event == a_controller_WakeupEvent) {
			evtid_str = "a_controller_WakeupEvent";
		}
		if (event == a_controller_InputEvent) {
			evtid_str = "a_controller_InputEvent";
		}
		if (event == a_controller_OutputEvent) {
			evtid_str = "a_controller_OutputEvent";
		}
		if (event == a_controller_CompleteEvent) {
			evtid_str = "a_controller_CompleteEvent";
		}
	}
	return(evtid_str);
}

const char8 *
atk2_osapid_str(ApplicationType id)
{
	const char8	*osapid_str;
	switch (id) {
	case blsm_OSAP_CORE0:
		osapid_str = "blsm_OSAP_CORE0";
		break;
	case a_controller_OSAP_CORE0:
		osapid_str = "a_controller_OSAP_CORE0";
		break;
	case a_controller_OSAP_CORE1:
		osapid_str = "a_controller_OSAP_CORE1";
		break;
	case OSAP_for_MAIN_HW_COUNTER_CORE0:
		osapid_str = "OSAP_for_MAIN_HW_COUNTER_CORE0";
		break;
	case OSAP_for_MAIN_HW_COUNTER_CORE1:
		osapid_str = "OSAP_for_MAIN_HW_COUNTER_CORE1";
		break;
	case INVALID_OSAPPLICATION:
		osapid_str = "INVALID_OSAPPLICATION";
		break;
	default:
		osapid_str = "";
		break;
	}
	return(osapid_str);
}

const char8 *
atk2_iocid_str(IocType id)
{
	const char8	*iocid_str;
	switch (id) {
	default:
		iocid_str = "";
		break;
	}
	return(iocid_str);
}
#endif /* TOPPERS_ENABLE_TRACE */

void
_kernel_inthdr_0x10000(void)
{
	i_begin_int(65536U);
	ISRNAME(target_ici_handler0)();
	i_end_int(65536U);
}
void
_kernel_inthdr_0x20000(void)
{
	i_begin_int(131072U);
	ISRNAME(target_ici_handler1)();
	i_end_int(131072U);
}
const uint16 pmr_isr2_mask = 0xf000;
const uint16 pmr_isr1_mask = 0xfff;

const FunctionRefType core0_isr_tbl[TNUM_INT] = {
	&_kernel_inthdr_0x10000,	/* 0x10000 */
	&default_int_handler,	/* 0x10001 */
	&default_int_handler,	/* 0x10002 */
	&default_int_handler,	/* 0x10003 */
	&default_int_handler,	/* 0x10004 */
	&default_int_handler,	/* 0x10005 */
	&default_int_handler,	/* 0x10006 */
	&default_int_handler,	/* 0x10007 */
	&default_int_handler,	/* 0x10008 */
	&default_int_handler,	/* 0x10009 */
	&default_int_handler,	/* 0x1000a */
	&default_int_handler,	/* 0x1000b */
	&default_int_handler,	/* 0x1000c */
	&default_int_handler,	/* 0x1000d */
	&default_int_handler,	/* 0x1000e */
	&default_int_handler,	/* 0x1000f */
	&default_int_handler,	/* 0x10010 */
	&default_int_handler,	/* 0x10011 */
	&default_int_handler,	/* 0x10012 */
	&default_int_handler,	/* 0x10013 */
	&default_int_handler,	/* 0x10014 */
	&default_int_handler,	/* 0x10015 */
	&default_int_handler,	/* 0x10016 */
	&default_int_handler,	/* 0x10017 */
	&default_int_handler,	/* 0x10018 */
	&default_int_handler,	/* 0x10019 */
	&default_int_handler,	/* 0x1001a */
	&default_int_handler,	/* 0x1001b */
	&default_int_handler,	/* 0x1001c */
	&default_int_handler,	/* 0x1001d */
	&default_int_handler,	/* 0x1001e */
	&default_int_handler,	/* 0x1001f */
	&default_int_handler,	/* 0xffff0020 */
	&default_int_handler,	/* 0xffff0021 */
	&default_int_handler,	/* 0xffff0022 */
	&default_int_handler,	/* 0xffff0023 */
	&default_int_handler,	/* 0xffff0024 */
	&default_int_handler,	/* 0xffff0025 */
	&default_int_handler,	/* 0xffff0026 */
	&default_int_handler,	/* 0xffff0027 */
	&default_int_handler,	/* 0xffff0028 */
	&default_int_handler,	/* 0xffff0029 */
	&default_int_handler,	/* 0xffff002a */
	&default_int_handler,	/* 0xffff002b */
	&default_int_handler,	/* 0xffff002c */
	&default_int_handler,	/* 0xffff002d */
	&default_int_handler,	/* 0xffff002e */
	&default_int_handler,	/* 0xffff002f */
	&default_int_handler,	/* 0xffff0030 */
	&default_int_handler,	/* 0xffff0031 */
	&default_int_handler,	/* 0xffff0032 */
	&default_int_handler,	/* 0xffff0033 */
	&default_int_handler,	/* 0xffff0034 */
	&default_int_handler,	/* 0xffff0035 */
	&default_int_handler,	/* 0xffff0036 */
	&default_int_handler,	/* 0xffff0037 */
	&default_int_handler,	/* 0xffff0038 */
	&default_int_handler,	/* 0xffff0039 */
	&default_int_handler,	/* 0xffff003a */
	&default_int_handler,	/* 0xffff003b */
	&default_int_handler,	/* 0xffff003c */
	&default_int_handler,	/* 0xffff003d */
	&default_int_handler,	/* 0xffff003e */
	&default_int_handler,	/* 0xffff003f */
	&default_int_handler,	/* 0xffff0040 */
	&default_int_handler,	/* 0xffff0041 */
	&default_int_handler,	/* 0xffff0042 */
	&default_int_handler,	/* 0xffff0043 */
	&default_int_handler,	/* 0xffff0044 */
	&default_int_handler,	/* 0xffff0045 */
	&default_int_handler,	/* 0xffff0046 */
	&default_int_handler,	/* 0xffff0047 */
	&default_int_handler,	/* 0xffff0048 */
	&default_int_handler,	/* 0xffff0049 */
	&default_int_handler,	/* 0xffff004a */
	&default_int_handler,	/* 0xffff004b */
	&default_int_handler,	/* 0xffff004c */
	&default_int_handler,	/* 0xffff004d */
	&default_int_handler,	/* 0xffff004e */
	&default_int_handler,	/* 0xffff004f */
	&_kernel_inthdr_0xffff0050,	/* 0xffff0050 */
	&default_int_handler,	/* 0xffff0051 */
	&default_int_handler,	/* 0xffff0052 */
	&default_int_handler,	/* 0xffff0053 */
	&default_int_handler,	/* 0xffff0054 */
	&default_int_handler,	/* 0xffff0055 */
	&default_int_handler,	/* 0xffff0056 */
	&default_int_handler,	/* 0xffff0057 */
	&default_int_handler,	/* 0xffff0058 */
	&default_int_handler,	/* 0xffff0059 */
	&default_int_handler,	/* 0xffff005a */
	&default_int_handler,	/* 0xffff005b */
	&default_int_handler,	/* 0xffff005c */
	&default_int_handler,	/* 0xffff005d */
	&default_int_handler,	/* 0xffff005e */
	&default_int_handler,	/* 0xffff005f */
	&default_int_handler,	/* 0xffff0060 */
	&default_int_handler,	/* 0xffff0061 */
	&default_int_handler,	/* 0xffff0062 */
	&default_int_handler,	/* 0xffff0063 */
	&default_int_handler,	/* 0xffff0064 */
	&default_int_handler,	/* 0xffff0065 */
	&default_int_handler,	/* 0xffff0066 */
	&default_int_handler,	/* 0xffff0067 */
	&default_int_handler,	/* 0xffff0068 */
	&default_int_handler,	/* 0xffff0069 */
	&default_int_handler,	/* 0xffff006a */
	&default_int_handler,	/* 0xffff006b */
	&default_int_handler,	/* 0xffff006c */
	&default_int_handler,	/* 0xffff006d */
	&default_int_handler,	/* 0xffff006e */
	&default_int_handler,	/* 0xffff006f */
	&default_int_handler,	/* 0xffff0070 */
	&default_int_handler,	/* 0xffff0071 */
	&default_int_handler,	/* 0xffff0072 */
	&default_int_handler,	/* 0xffff0073 */
	&default_int_handler,	/* 0xffff0074 */
	&default_int_handler,	/* 0xffff0075 */
	&default_int_handler,	/* 0xffff0076 */
	&default_int_handler,	/* 0xffff0077 */
	&default_int_handler,	/* 0xffff0078 */
	&default_int_handler,	/* 0xffff0079 */
	&default_int_handler,	/* 0xffff007a */
	&default_int_handler,	/* 0xffff007b */
	&default_int_handler,	/* 0xffff007c */
	&default_int_handler,	/* 0xffff007d */
	&default_int_handler,	/* 0xffff007e */
	&default_int_handler,	/* 0xffff007f */
	&default_int_handler,	/* 0xffff0080 */
	&default_int_handler,	/* 0xffff0081 */
	&default_int_handler,	/* 0xffff0082 */
	&default_int_handler,	/* 0xffff0083 */
	&default_int_handler,	/* 0xffff0084 */
	&default_int_handler,	/* 0xffff0085 */
	&default_int_handler,	/* 0xffff0086 */
	&default_int_handler,	/* 0xffff0087 */
	&default_int_handler,	/* 0xffff0088 */
	&default_int_handler,	/* 0xffff0089 */
	&default_int_handler,	/* 0xffff008a */
	&default_int_handler,	/* 0xffff008b */
	&default_int_handler,	/* 0xffff008c */
	&default_int_handler,	/* 0xffff008d */
	&default_int_handler,	/* 0xffff008e */
	&default_int_handler,	/* 0xffff008f */
	&default_int_handler,	/* 0xffff0090 */
	&default_int_handler,	/* 0xffff0091 */
	&default_int_handler,	/* 0xffff0092 */
	&default_int_handler,	/* 0xffff0093 */
	&default_int_handler,	/* 0xffff0094 */
	&default_int_handler,	/* 0xffff0095 */
	&default_int_handler,	/* 0xffff0096 */
	&default_int_handler,	/* 0xffff0097 */
	&default_int_handler,	/* 0xffff0098 */
	&default_int_handler,	/* 0xffff0099 */
	&default_int_handler,	/* 0xffff009a */
	&default_int_handler,	/* 0xffff009b */
	&default_int_handler,	/* 0xffff009c */
	&default_int_handler,	/* 0xffff009d */
	&default_int_handler,	/* 0xffff009e */
	&default_int_handler,	/* 0xffff009f */
	&default_int_handler,	/* 0xffff00a0 */
	&default_int_handler,	/* 0xffff00a1 */
	&default_int_handler,	/* 0xffff00a2 */
	&default_int_handler,	/* 0xffff00a3 */
	&default_int_handler,	/* 0xffff00a4 */
	&_kernel_inthdr_0xffff00a5,	/* 0xffff00a5 */
	&_kernel_inthdr_0xffff00a6,	/* 0xffff00a6 */
	&default_int_handler,	/* 0xffff00a7 */
	&default_int_handler,	/* 0xffff00a8 */
	&default_int_handler,	/* 0xffff00a9 */
	&default_int_handler,	/* 0xffff00aa */
	&default_int_handler,	/* 0xffff00ab */
	&default_int_handler,	/* 0xffff00ac */
	&default_int_handler,	/* 0xffff00ad */
	&default_int_handler,	/* 0xffff00ae */
	&default_int_handler,	/* 0xffff00af */
	&default_int_handler,	/* 0xffff00b0 */
	&default_int_handler,	/* 0xffff00b1 */
	&default_int_handler,	/* 0xffff00b2 */
	&default_int_handler,	/* 0xffff00b3 */
	&default_int_handler,	/* 0xffff00b4 */
	&default_int_handler,	/* 0xffff00b5 */
	&default_int_handler,	/* 0xffff00b6 */
	&default_int_handler,	/* 0xffff00b7 */
	&default_int_handler,	/* 0xffff00b8 */
	&default_int_handler,	/* 0xffff00b9 */
	&default_int_handler,	/* 0xffff00ba */
	&default_int_handler,	/* 0xffff00bb */
	&default_int_handler,	/* 0xffff00bc */
	&default_int_handler,	/* 0xffff00bd */
	&default_int_handler,	/* 0xffff00be */
	&default_int_handler,	/* 0xffff00bf */
	&default_int_handler,	/* 0xffff00c0 */
	&default_int_handler,	/* 0xffff00c1 */
	&default_int_handler,	/* 0xffff00c2 */
	&default_int_handler,	/* 0xffff00c3 */
	&default_int_handler,	/* 0xffff00c4 */
	&default_int_handler,	/* 0xffff00c5 */
	&default_int_handler,	/* 0xffff00c6 */
	&default_int_handler,	/* 0xffff00c7 */
	&default_int_handler,	/* 0xffff00c8 */
	&default_int_handler,	/* 0xffff00c9 */
	&default_int_handler,	/* 0xffff00ca */
	&default_int_handler,	/* 0xffff00cb */
	&default_int_handler,	/* 0xffff00cc */
	&default_int_handler,	/* 0xffff00cd */
	&default_int_handler,	/* 0xffff00ce */
	&default_int_handler,	/* 0xffff00cf */
	&default_int_handler,	/* 0xffff00d0 */
	&default_int_handler,	/* 0xffff00d1 */
	&default_int_handler,	/* 0xffff00d2 */
	&default_int_handler,	/* 0xffff00d3 */
	&default_int_handler,	/* 0xffff00d4 */
	&default_int_handler,	/* 0xffff00d5 */
	&default_int_handler,	/* 0xffff00d6 */
	&default_int_handler,	/* 0xffff00d7 */
	&default_int_handler,	/* 0xffff00d8 */
	&default_int_handler,	/* 0xffff00d9 */
	&default_int_handler,	/* 0xffff00da */
	&default_int_handler,	/* 0xffff00db */
	&default_int_handler,	/* 0xffff00dc */
	&default_int_handler,	/* 0xffff00dd */
	&default_int_handler,	/* 0xffff00de */
	&default_int_handler,	/* 0xffff00df */
	&default_int_handler,	/* 0xffff00e0 */
	&default_int_handler,	/* 0xffff00e1 */
	&default_int_handler,	/* 0xffff00e2 */
	&default_int_handler,	/* 0xffff00e3 */
	&default_int_handler,	/* 0xffff00e4 */
	&default_int_handler,	/* 0xffff00e5 */
	&default_int_handler,	/* 0xffff00e6 */
	&default_int_handler,	/* 0xffff00e7 */
	&default_int_handler,	/* 0xffff00e8 */
	&default_int_handler,	/* 0xffff00e9 */
	&default_int_handler,	/* 0xffff00ea */
	&default_int_handler,	/* 0xffff00eb */
	&default_int_handler,	/* 0xffff00ec */
	&default_int_handler,	/* 0xffff00ed */
	&default_int_handler,	/* 0xffff00ee */
	&default_int_handler,	/* 0xffff00ef */
	&default_int_handler,	/* 0xffff00f0 */
	&default_int_handler,	/* 0xffff00f1 */
	&default_int_handler,	/* 0xffff00f2 */
	&default_int_handler,	/* 0xffff00f3 */
	&default_int_handler,	/* 0xffff00f4 */
	&default_int_handler,	/* 0xffff00f5 */
	&default_int_handler,	/* 0xffff00f6 */
	&default_int_handler,	/* 0xffff00f7 */
	&default_int_handler,	/* 0xffff00f8 */
	&default_int_handler,	/* 0xffff00f9 */
	&default_int_handler,	/* 0xffff00fa */
	&default_int_handler,	/* 0xffff00fb */
	&default_int_handler,	/* 0xffff00fc */
	&default_int_handler,	/* 0xffff00fd */
	&default_int_handler,	/* 0xffff00fe */
	&default_int_handler,	/* 0xffff00ff */
	&default_int_handler,	/* 0xffff0100 */
	&default_int_handler,	/* 0xffff0101 */
	&default_int_handler,	/* 0xffff0102 */
	&default_int_handler,	/* 0xffff0103 */
	&default_int_handler,	/* 0xffff0104 */
	&default_int_handler,	/* 0xffff0105 */
	&default_int_handler,	/* 0xffff0106 */
	&default_int_handler,	/* 0xffff0107 */
	&default_int_handler,	/* 0xffff0108 */
	&default_int_handler,	/* 0xffff0109 */
	&default_int_handler,	/* 0xffff010a */
	&default_int_handler,	/* 0xffff010b */
	&default_int_handler,	/* 0xffff010c */
	&default_int_handler,	/* 0xffff010d */
	&default_int_handler,	/* 0xffff010e */
	&default_int_handler,	/* 0xffff010f */
	&default_int_handler,	/* 0xffff0110 */
	&default_int_handler,	/* 0xffff0111 */
	&default_int_handler,	/* 0xffff0112 */
	&default_int_handler,	/* 0xffff0113 */
	&default_int_handler,	/* 0xffff0114 */
	&default_int_handler,	/* 0xffff0115 */
	&default_int_handler,	/* 0xffff0116 */
	&default_int_handler,	/* 0xffff0117 */
	&default_int_handler,	/* 0xffff0118 */
	&default_int_handler,	/* 0xffff0119 */
	&default_int_handler,	/* 0xffff011a */
	&default_int_handler,	/* 0xffff011b */
	&default_int_handler,	/* 0xffff011c */
	&default_int_handler,	/* 0xffff011d */
	&default_int_handler,	/* 0xffff011e */
	&default_int_handler,	/* 0xffff011f */
	&default_int_handler,	/* 0xffff0120 */
	&default_int_handler,	/* 0xffff0121 */
	&default_int_handler,	/* 0xffff0122 */
	&default_int_handler,	/* 0xffff0123 */
	&default_int_handler,	/* 0xffff0124 */
	&default_int_handler,	/* 0xffff0125 */
	&default_int_handler,	/* 0xffff0126 */
	&default_int_handler,	/* 0xffff0127 */
	&default_int_handler,	/* 0xffff0128 */
	&default_int_handler,	/* 0xffff0129 */
	&default_int_handler,	/* 0xffff012a */
	&default_int_handler,	/* 0xffff012b */
	&default_int_handler,	/* 0xffff012c */
	&default_int_handler,	/* 0xffff012d */
	&default_int_handler,	/* 0xffff012e */
	&default_int_handler,	/* 0xffff012f */
	&default_int_handler,	/* 0xffff0130 */
	&default_int_handler,	/* 0xffff0131 */
	&default_int_handler,	/* 0xffff0132 */
	&default_int_handler,	/* 0xffff0133 */
	&default_int_handler,	/* 0xffff0134 */
	&default_int_handler,	/* 0xffff0135 */
	&default_int_handler,	/* 0xffff0136 */
	&default_int_handler,	/* 0xffff0137 */
	&default_int_handler,	/* 0xffff0138 */
	&default_int_handler,	/* 0xffff0139 */
	&default_int_handler,	/* 0xffff013a */
	&default_int_handler,	/* 0xffff013b */
	&default_int_handler,	/* 0xffff013c */
	&default_int_handler,	/* 0xffff013d */
	&default_int_handler,	/* 0xffff013e */
	&default_int_handler,	/* 0xffff013f */
	&default_int_handler,	/* 0xffff0140 */
	&default_int_handler,	/* 0xffff0141 */
	&default_int_handler,	/* 0xffff0142 */
	&default_int_handler,	/* 0xffff0143 */
	&default_int_handler,	/* 0xffff0144 */
	&default_int_handler,	/* 0xffff0145 */
	&default_int_handler,	/* 0xffff0146 */
	&default_int_handler,	/* 0xffff0147 */
	&default_int_handler,	/* 0xffff0148 */
	&default_int_handler,	/* 0xffff0149 */
	&default_int_handler,	/* 0xffff014a */
	&default_int_handler,	/* 0xffff014b */
	&default_int_handler,	/* 0xffff014c */
	&default_int_handler,	/* 0xffff014d */
	&default_int_handler,	/* 0xffff014e */
	&default_int_handler,	/* 0xffff014f */
	&default_int_handler,	/* 0xffff0150 */
	&default_int_handler,	/* 0xffff0151 */
	&default_int_handler,	/* 0xffff0152 */
	&default_int_handler,	/* 0xffff0153 */
	&default_int_handler,	/* 0xffff0154 */
	&default_int_handler,	/* 0xffff0155 */
	&default_int_handler,	/* 0xffff0156 */
	&default_int_handler,	/* 0xffff0157 */
	&default_int_handler,	/* 0xffff0158 */
	&default_int_handler,	/* 0xffff0159 */
	&default_int_handler,	/* 0xffff015a */
	&default_int_handler,	/* 0xffff015b */
	&default_int_handler,	/* 0xffff015c */
	&default_int_handler,	/* 0xffff015d */
	&default_int_handler	/* 0xffff015e */
};

const FunctionRefType core1_isr_tbl[TNUM_INT] = {
	&_kernel_inthdr_0x20000,	/* 0x20000 */
	&default_int_handler,	/* 0x20001 */
	&default_int_handler,	/* 0x20002 */
	&default_int_handler,	/* 0x20003 */
	&default_int_handler,	/* 0x20004 */
	&default_int_handler,	/* 0x20005 */
	&default_int_handler,	/* 0x20006 */
	&default_int_handler,	/* 0x20007 */
	&default_int_handler,	/* 0x20008 */
	&default_int_handler,	/* 0x20009 */
	&default_int_handler,	/* 0x2000a */
	&default_int_handler,	/* 0x2000b */
	&default_int_handler,	/* 0x2000c */
	&default_int_handler,	/* 0x2000d */
	&default_int_handler,	/* 0x2000e */
	&default_int_handler,	/* 0x2000f */
	&default_int_handler,	/* 0x20010 */
	&default_int_handler,	/* 0x20011 */
	&default_int_handler,	/* 0x20012 */
	&default_int_handler,	/* 0x20013 */
	&default_int_handler,	/* 0x20014 */
	&default_int_handler,	/* 0x20015 */
	&default_int_handler,	/* 0x20016 */
	&default_int_handler,	/* 0x20017 */
	&default_int_handler,	/* 0x20018 */
	&default_int_handler,	/* 0x20019 */
	&default_int_handler,	/* 0x2001a */
	&default_int_handler,	/* 0x2001b */
	&default_int_handler,	/* 0x2001c */
	&default_int_handler,	/* 0x2001d */
	&default_int_handler,	/* 0x2001e */
	&default_int_handler,	/* 0x2001f */
	&default_int_handler,	/* 0xffff0020 */
	&default_int_handler,	/* 0xffff0021 */
	&default_int_handler,	/* 0xffff0022 */
	&default_int_handler,	/* 0xffff0023 */
	&default_int_handler,	/* 0xffff0024 */
	&default_int_handler,	/* 0xffff0025 */
	&default_int_handler,	/* 0xffff0026 */
	&default_int_handler,	/* 0xffff0027 */
	&default_int_handler,	/* 0xffff0028 */
	&default_int_handler,	/* 0xffff0029 */
	&default_int_handler,	/* 0xffff002a */
	&default_int_handler,	/* 0xffff002b */
	&default_int_handler,	/* 0xffff002c */
	&default_int_handler,	/* 0xffff002d */
	&default_int_handler,	/* 0xffff002e */
	&default_int_handler,	/* 0xffff002f */
	&default_int_handler,	/* 0xffff0030 */
	&default_int_handler,	/* 0xffff0031 */
	&default_int_handler,	/* 0xffff0032 */
	&default_int_handler,	/* 0xffff0033 */
	&default_int_handler,	/* 0xffff0034 */
	&default_int_handler,	/* 0xffff0035 */
	&default_int_handler,	/* 0xffff0036 */
	&default_int_handler,	/* 0xffff0037 */
	&default_int_handler,	/* 0xffff0038 */
	&default_int_handler,	/* 0xffff0039 */
	&default_int_handler,	/* 0xffff003a */
	&default_int_handler,	/* 0xffff003b */
	&default_int_handler,	/* 0xffff003c */
	&default_int_handler,	/* 0xffff003d */
	&default_int_handler,	/* 0xffff003e */
	&default_int_handler,	/* 0xffff003f */
	&default_int_handler,	/* 0xffff0040 */
	&default_int_handler,	/* 0xffff0041 */
	&default_int_handler,	/* 0xffff0042 */
	&default_int_handler,	/* 0xffff0043 */
	&default_int_handler,	/* 0xffff0044 */
	&default_int_handler,	/* 0xffff0045 */
	&default_int_handler,	/* 0xffff0046 */
	&default_int_handler,	/* 0xffff0047 */
	&default_int_handler,	/* 0xffff0048 */
	&default_int_handler,	/* 0xffff0049 */
	&default_int_handler,	/* 0xffff004a */
	&default_int_handler,	/* 0xffff004b */
	&default_int_handler,	/* 0xffff004c */
	&default_int_handler,	/* 0xffff004d */
	&default_int_handler,	/* 0xffff004e */
	&default_int_handler,	/* 0xffff004f */
	&default_int_handler,	/* 0xffff0050 */
	&default_int_handler,	/* 0xffff0051 */
	&default_int_handler,	/* 0xffff0052 */
	&default_int_handler,	/* 0xffff0053 */
	&default_int_handler,	/* 0xffff0054 */
	&default_int_handler,	/* 0xffff0055 */
	&default_int_handler,	/* 0xffff0056 */
	&default_int_handler,	/* 0xffff0057 */
	&default_int_handler,	/* 0xffff0058 */
	&default_int_handler,	/* 0xffff0059 */
	&default_int_handler,	/* 0xffff005a */
	&default_int_handler,	/* 0xffff005b */
	&default_int_handler,	/* 0xffff005c */
	&default_int_handler,	/* 0xffff005d */
	&default_int_handler,	/* 0xffff005e */
	&default_int_handler,	/* 0xffff005f */
	&default_int_handler,	/* 0xffff0060 */
	&default_int_handler,	/* 0xffff0061 */
	&default_int_handler,	/* 0xffff0062 */
	&default_int_handler,	/* 0xffff0063 */
	&default_int_handler,	/* 0xffff0064 */
	&default_int_handler,	/* 0xffff0065 */
	&default_int_handler,	/* 0xffff0066 */
	&default_int_handler,	/* 0xffff0067 */
	&default_int_handler,	/* 0xffff0068 */
	&default_int_handler,	/* 0xffff0069 */
	&default_int_handler,	/* 0xffff006a */
	&default_int_handler,	/* 0xffff006b */
	&default_int_handler,	/* 0xffff006c */
	&default_int_handler,	/* 0xffff006d */
	&default_int_handler,	/* 0xffff006e */
	&default_int_handler,	/* 0xffff006f */
	&default_int_handler,	/* 0xffff0070 */
	&default_int_handler,	/* 0xffff0071 */
	&default_int_handler,	/* 0xffff0072 */
	&default_int_handler,	/* 0xffff0073 */
	&default_int_handler,	/* 0xffff0074 */
	&default_int_handler,	/* 0xffff0075 */
	&default_int_handler,	/* 0xffff0076 */
	&default_int_handler,	/* 0xffff0077 */
	&default_int_handler,	/* 0xffff0078 */
	&default_int_handler,	/* 0xffff0079 */
	&default_int_handler,	/* 0xffff007a */
	&default_int_handler,	/* 0xffff007b */
	&default_int_handler,	/* 0xffff007c */
	&default_int_handler,	/* 0xffff007d */
	&default_int_handler,	/* 0xffff007e */
	&default_int_handler,	/* 0xffff007f */
	&default_int_handler,	/* 0xffff0080 */
	&default_int_handler,	/* 0xffff0081 */
	&default_int_handler,	/* 0xffff0082 */
	&default_int_handler,	/* 0xffff0083 */
	&default_int_handler,	/* 0xffff0084 */
	&default_int_handler,	/* 0xffff0085 */
	&default_int_handler,	/* 0xffff0086 */
	&default_int_handler,	/* 0xffff0087 */
	&default_int_handler,	/* 0xffff0088 */
	&default_int_handler,	/* 0xffff0089 */
	&default_int_handler,	/* 0xffff008a */
	&default_int_handler,	/* 0xffff008b */
	&default_int_handler,	/* 0xffff008c */
	&default_int_handler,	/* 0xffff008d */
	&default_int_handler,	/* 0xffff008e */
	&default_int_handler,	/* 0xffff008f */
	&default_int_handler,	/* 0xffff0090 */
	&default_int_handler,	/* 0xffff0091 */
	&default_int_handler,	/* 0xffff0092 */
	&default_int_handler,	/* 0xffff0093 */
	&default_int_handler,	/* 0xffff0094 */
	&default_int_handler,	/* 0xffff0095 */
	&default_int_handler,	/* 0xffff0096 */
	&default_int_handler,	/* 0xffff0097 */
	&default_int_handler,	/* 0xffff0098 */
	&default_int_handler,	/* 0xffff0099 */
	&default_int_handler,	/* 0xffff009a */
	&default_int_handler,	/* 0xffff009b */
	&default_int_handler,	/* 0xffff009c */
	&default_int_handler,	/* 0xffff009d */
	&default_int_handler,	/* 0xffff009e */
	&default_int_handler,	/* 0xffff009f */
	&default_int_handler,	/* 0xffff00a0 */
	&default_int_handler,	/* 0xffff00a1 */
	&default_int_handler,	/* 0xffff00a2 */
	&default_int_handler,	/* 0xffff00a3 */
	&default_int_handler,	/* 0xffff00a4 */
	&default_int_handler,	/* 0xffff00a5 */
	&default_int_handler,	/* 0xffff00a6 */
	&default_int_handler,	/* 0xffff00a7 */
	&_kernel_inthdr_0xffff00a8,	/* 0xffff00a8 */
	&default_int_handler,	/* 0xffff00a9 */
	&default_int_handler,	/* 0xffff00aa */
	&default_int_handler,	/* 0xffff00ab */
	&default_int_handler,	/* 0xffff00ac */
	&default_int_handler,	/* 0xffff00ad */
	&default_int_handler,	/* 0xffff00ae */
	&default_int_handler,	/* 0xffff00af */
	&default_int_handler,	/* 0xffff00b0 */
	&default_int_handler,	/* 0xffff00b1 */
	&default_int_handler,	/* 0xffff00b2 */
	&default_int_handler,	/* 0xffff00b3 */
	&default_int_handler,	/* 0xffff00b4 */
	&default_int_handler,	/* 0xffff00b5 */
	&default_int_handler,	/* 0xffff00b6 */
	&default_int_handler,	/* 0xffff00b7 */
	&default_int_handler,	/* 0xffff00b8 */
	&default_int_handler,	/* 0xffff00b9 */
	&default_int_handler,	/* 0xffff00ba */
	&default_int_handler,	/* 0xffff00bb */
	&default_int_handler,	/* 0xffff00bc */
	&default_int_handler,	/* 0xffff00bd */
	&default_int_handler,	/* 0xffff00be */
	&default_int_handler,	/* 0xffff00bf */
	&default_int_handler,	/* 0xffff00c0 */
	&default_int_handler,	/* 0xffff00c1 */
	&default_int_handler,	/* 0xffff00c2 */
	&default_int_handler,	/* 0xffff00c3 */
	&default_int_handler,	/* 0xffff00c4 */
	&default_int_handler,	/* 0xffff00c5 */
	&default_int_handler,	/* 0xffff00c6 */
	&default_int_handler,	/* 0xffff00c7 */
	&default_int_handler,	/* 0xffff00c8 */
	&default_int_handler,	/* 0xffff00c9 */
	&default_int_handler,	/* 0xffff00ca */
	&default_int_handler,	/* 0xffff00cb */
	&default_int_handler,	/* 0xffff00cc */
	&default_int_handler,	/* 0xffff00cd */
	&default_int_handler,	/* 0xffff00ce */
	&default_int_handler,	/* 0xffff00cf */
	&default_int_handler,	/* 0xffff00d0 */
	&default_int_handler,	/* 0xffff00d1 */
	&default_int_handler,	/* 0xffff00d2 */
	&default_int_handler,	/* 0xffff00d3 */
	&default_int_handler,	/* 0xffff00d4 */
	&default_int_handler,	/* 0xffff00d5 */
	&default_int_handler,	/* 0xffff00d6 */
	&default_int_handler,	/* 0xffff00d7 */
	&default_int_handler,	/* 0xffff00d8 */
	&default_int_handler,	/* 0xffff00d9 */
	&default_int_handler,	/* 0xffff00da */
	&default_int_handler,	/* 0xffff00db */
	&default_int_handler,	/* 0xffff00dc */
	&default_int_handler,	/* 0xffff00dd */
	&default_int_handler,	/* 0xffff00de */
	&default_int_handler,	/* 0xffff00df */
	&default_int_handler,	/* 0xffff00e0 */
	&default_int_handler,	/* 0xffff00e1 */
	&default_int_handler,	/* 0xffff00e2 */
	&default_int_handler,	/* 0xffff00e3 */
	&default_int_handler,	/* 0xffff00e4 */
	&default_int_handler,	/* 0xffff00e5 */
	&default_int_handler,	/* 0xffff00e6 */
	&default_int_handler,	/* 0xffff00e7 */
	&default_int_handler,	/* 0xffff00e8 */
	&default_int_handler,	/* 0xffff00e9 */
	&default_int_handler,	/* 0xffff00ea */
	&default_int_handler,	/* 0xffff00eb */
	&default_int_handler,	/* 0xffff00ec */
	&default_int_handler,	/* 0xffff00ed */
	&default_int_handler,	/* 0xffff00ee */
	&default_int_handler,	/* 0xffff00ef */
	&default_int_handler,	/* 0xffff00f0 */
	&default_int_handler,	/* 0xffff00f1 */
	&default_int_handler,	/* 0xffff00f2 */
	&default_int_handler,	/* 0xffff00f3 */
	&default_int_handler,	/* 0xffff00f4 */
	&default_int_handler,	/* 0xffff00f5 */
	&default_int_handler,	/* 0xffff00f6 */
	&default_int_handler,	/* 0xffff00f7 */
	&default_int_handler,	/* 0xffff00f8 */
	&default_int_handler,	/* 0xffff00f9 */
	&default_int_handler,	/* 0xffff00fa */
	&default_int_handler,	/* 0xffff00fb */
	&default_int_handler,	/* 0xffff00fc */
	&default_int_handler,	/* 0xffff00fd */
	&default_int_handler,	/* 0xffff00fe */
	&default_int_handler,	/* 0xffff00ff */
	&default_int_handler,	/* 0xffff0100 */
	&default_int_handler,	/* 0xffff0101 */
	&default_int_handler,	/* 0xffff0102 */
	&default_int_handler,	/* 0xffff0103 */
	&default_int_handler,	/* 0xffff0104 */
	&default_int_handler,	/* 0xffff0105 */
	&default_int_handler,	/* 0xffff0106 */
	&default_int_handler,	/* 0xffff0107 */
	&default_int_handler,	/* 0xffff0108 */
	&default_int_handler,	/* 0xffff0109 */
	&default_int_handler,	/* 0xffff010a */
	&default_int_handler,	/* 0xffff010b */
	&default_int_handler,	/* 0xffff010c */
	&default_int_handler,	/* 0xffff010d */
	&default_int_handler,	/* 0xffff010e */
	&default_int_handler,	/* 0xffff010f */
	&default_int_handler,	/* 0xffff0110 */
	&default_int_handler,	/* 0xffff0111 */
	&default_int_handler,	/* 0xffff0112 */
	&default_int_handler,	/* 0xffff0113 */
	&default_int_handler,	/* 0xffff0114 */
	&default_int_handler,	/* 0xffff0115 */
	&default_int_handler,	/* 0xffff0116 */
	&default_int_handler,	/* 0xffff0117 */
	&default_int_handler,	/* 0xffff0118 */
	&default_int_handler,	/* 0xffff0119 */
	&default_int_handler,	/* 0xffff011a */
	&default_int_handler,	/* 0xffff011b */
	&default_int_handler,	/* 0xffff011c */
	&default_int_handler,	/* 0xffff011d */
	&default_int_handler,	/* 0xffff011e */
	&default_int_handler,	/* 0xffff011f */
	&default_int_handler,	/* 0xffff0120 */
	&default_int_handler,	/* 0xffff0121 */
	&default_int_handler,	/* 0xffff0122 */
	&default_int_handler,	/* 0xffff0123 */
	&default_int_handler,	/* 0xffff0124 */
	&default_int_handler,	/* 0xffff0125 */
	&default_int_handler,	/* 0xffff0126 */
	&default_int_handler,	/* 0xffff0127 */
	&default_int_handler,	/* 0xffff0128 */
	&default_int_handler,	/* 0xffff0129 */
	&default_int_handler,	/* 0xffff012a */
	&default_int_handler,	/* 0xffff012b */
	&default_int_handler,	/* 0xffff012c */
	&default_int_handler,	/* 0xffff012d */
	&default_int_handler,	/* 0xffff012e */
	&default_int_handler,	/* 0xffff012f */
	&default_int_handler,	/* 0xffff0130 */
	&default_int_handler,	/* 0xffff0131 */
	&default_int_handler,	/* 0xffff0132 */
	&default_int_handler,	/* 0xffff0133 */
	&default_int_handler,	/* 0xffff0134 */
	&default_int_handler,	/* 0xffff0135 */
	&default_int_handler,	/* 0xffff0136 */
	&default_int_handler,	/* 0xffff0137 */
	&default_int_handler,	/* 0xffff0138 */
	&default_int_handler,	/* 0xffff0139 */
	&default_int_handler,	/* 0xffff013a */
	&default_int_handler,	/* 0xffff013b */
	&default_int_handler,	/* 0xffff013c */
	&default_int_handler,	/* 0xffff013d */
	&default_int_handler,	/* 0xffff013e */
	&default_int_handler,	/* 0xffff013f */
	&default_int_handler,	/* 0xffff0140 */
	&default_int_handler,	/* 0xffff0141 */
	&default_int_handler,	/* 0xffff0142 */
	&default_int_handler,	/* 0xffff0143 */
	&default_int_handler,	/* 0xffff0144 */
	&default_int_handler,	/* 0xffff0145 */
	&default_int_handler,	/* 0xffff0146 */
	&default_int_handler,	/* 0xffff0147 */
	&default_int_handler,	/* 0xffff0148 */
	&default_int_handler,	/* 0xffff0149 */
	&default_int_handler,	/* 0xffff014a */
	&default_int_handler,	/* 0xffff014b */
	&default_int_handler,	/* 0xffff014c */
	&default_int_handler,	/* 0xffff014d */
	&default_int_handler,	/* 0xffff014e */
	&default_int_handler,	/* 0xffff014f */
	&default_int_handler,	/* 0xffff0150 */
	&default_int_handler,	/* 0xffff0151 */
	&default_int_handler,	/* 0xffff0152 */
	&default_int_handler,	/* 0xffff0153 */
	&default_int_handler,	/* 0xffff0154 */
	&default_int_handler,	/* 0xffff0155 */
	&default_int_handler,	/* 0xffff0156 */
	&default_int_handler,	/* 0xffff0157 */
	&default_int_handler,	/* 0xffff0158 */
	&default_int_handler,	/* 0xffff0159 */
	&default_int_handler,	/* 0xffff015a */
	&default_int_handler,	/* 0xffff015b */
	&default_int_handler,	/* 0xffff015c */
	&default_int_handler,	/* 0xffff015d */
	&default_int_handler	/* 0xffff015e */
};

const uint32 isr_table[TotalNumberOfCores] = {
	(const uint32) core0_isr_tbl,
	(const uint32) core1_isr_tbl
};

ISRCB * const core0_isr_p_isrcb_tbl[TNUM_INT] = {
	NULL,	/* 0x10000 */
	NULL,	/* 0x10001 */
	NULL,	/* 0x10002 */
	NULL,	/* 0x10003 */
	NULL,	/* 0x10004 */
	NULL,	/* 0x10005 */
	NULL,	/* 0x10006 */
	NULL,	/* 0x10007 */
	NULL,	/* 0x10008 */
	NULL,	/* 0x10009 */
	NULL,	/* 0x1000a */
	NULL,	/* 0x1000b */
	NULL,	/* 0x1000c */
	NULL,	/* 0x1000d */
	NULL,	/* 0x1000e */
	NULL,	/* 0x1000f */
	NULL,	/* 0x10010 */
	NULL,	/* 0x10011 */
	NULL,	/* 0x10012 */
	NULL,	/* 0x10013 */
	NULL,	/* 0x10014 */
	NULL,	/* 0x10015 */
	NULL,	/* 0x10016 */
	NULL,	/* 0x10017 */
	NULL,	/* 0x10018 */
	NULL,	/* 0x10019 */
	NULL,	/* 0x1001a */
	NULL,	/* 0x1001b */
	NULL,	/* 0x1001c */
	NULL,	/* 0x1001d */
	NULL,	/* 0x1001e */
	NULL,	/* 0x1001f */
	NULL,	/* 0xffff0020 */
	NULL,	/* 0xffff0021 */
	NULL,	/* 0xffff0022 */
	NULL,	/* 0xffff0023 */
	NULL,	/* 0xffff0024 */
	NULL,	/* 0xffff0025 */
	NULL,	/* 0xffff0026 */
	NULL,	/* 0xffff0027 */
	NULL,	/* 0xffff0028 */
	NULL,	/* 0xffff0029 */
	NULL,	/* 0xffff002a */
	NULL,	/* 0xffff002b */
	NULL,	/* 0xffff002c */
	NULL,	/* 0xffff002d */
	NULL,	/* 0xffff002e */
	NULL,	/* 0xffff002f */
	NULL,	/* 0xffff0030 */
	NULL,	/* 0xffff0031 */
	NULL,	/* 0xffff0032 */
	NULL,	/* 0xffff0033 */
	NULL,	/* 0xffff0034 */
	NULL,	/* 0xffff0035 */
	NULL,	/* 0xffff0036 */
	NULL,	/* 0xffff0037 */
	NULL,	/* 0xffff0038 */
	NULL,	/* 0xffff0039 */
	NULL,	/* 0xffff003a */
	NULL,	/* 0xffff003b */
	NULL,	/* 0xffff003c */
	NULL,	/* 0xffff003d */
	NULL,	/* 0xffff003e */
	NULL,	/* 0xffff003f */
	NULL,	/* 0xffff0040 */
	NULL,	/* 0xffff0041 */
	NULL,	/* 0xffff0042 */
	NULL,	/* 0xffff0043 */
	NULL,	/* 0xffff0044 */
	NULL,	/* 0xffff0045 */
	NULL,	/* 0xffff0046 */
	NULL,	/* 0xffff0047 */
	NULL,	/* 0xffff0048 */
	NULL,	/* 0xffff0049 */
	NULL,	/* 0xffff004a */
	NULL,	/* 0xffff004b */
	NULL,	/* 0xffff004c */
	NULL,	/* 0xffff004d */
	NULL,	/* 0xffff004e */
	NULL,	/* 0xffff004f */
	&_kernel_isrcb_C2ISR_for_MAIN_HW_COUNTER_CORE0,	/* 0xffff0050 */
	NULL,	/* 0xffff0051 */
	NULL,	/* 0xffff0052 */
	NULL,	/* 0xffff0053 */
	NULL,	/* 0xffff0054 */
	NULL,	/* 0xffff0055 */
	NULL,	/* 0xffff0056 */
	NULL,	/* 0xffff0057 */
	NULL,	/* 0xffff0058 */
	NULL,	/* 0xffff0059 */
	NULL,	/* 0xffff005a */
	NULL,	/* 0xffff005b */
	NULL,	/* 0xffff005c */
	NULL,	/* 0xffff005d */
	NULL,	/* 0xffff005e */
	NULL,	/* 0xffff005f */
	NULL,	/* 0xffff0060 */
	NULL,	/* 0xffff0061 */
	NULL,	/* 0xffff0062 */
	NULL,	/* 0xffff0063 */
	NULL,	/* 0xffff0064 */
	NULL,	/* 0xffff0065 */
	NULL,	/* 0xffff0066 */
	NULL,	/* 0xffff0067 */
	NULL,	/* 0xffff0068 */
	NULL,	/* 0xffff0069 */
	NULL,	/* 0xffff006a */
	NULL,	/* 0xffff006b */
	NULL,	/* 0xffff006c */
	NULL,	/* 0xffff006d */
	NULL,	/* 0xffff006e */
	NULL,	/* 0xffff006f */
	NULL,	/* 0xffff0070 */
	NULL,	/* 0xffff0071 */
	NULL,	/* 0xffff0072 */
	NULL,	/* 0xffff0073 */
	NULL,	/* 0xffff0074 */
	NULL,	/* 0xffff0075 */
	NULL,	/* 0xffff0076 */
	NULL,	/* 0xffff0077 */
	NULL,	/* 0xffff0078 */
	NULL,	/* 0xffff0079 */
	NULL,	/* 0xffff007a */
	NULL,	/* 0xffff007b */
	NULL,	/* 0xffff007c */
	NULL,	/* 0xffff007d */
	NULL,	/* 0xffff007e */
	NULL,	/* 0xffff007f */
	NULL,	/* 0xffff0080 */
	NULL,	/* 0xffff0081 */
	NULL,	/* 0xffff0082 */
	NULL,	/* 0xffff0083 */
	NULL,	/* 0xffff0084 */
	NULL,	/* 0xffff0085 */
	NULL,	/* 0xffff0086 */
	NULL,	/* 0xffff0087 */
	NULL,	/* 0xffff0088 */
	NULL,	/* 0xffff0089 */
	NULL,	/* 0xffff008a */
	NULL,	/* 0xffff008b */
	NULL,	/* 0xffff008c */
	NULL,	/* 0xffff008d */
	NULL,	/* 0xffff008e */
	NULL,	/* 0xffff008f */
	NULL,	/* 0xffff0090 */
	NULL,	/* 0xffff0091 */
	NULL,	/* 0xffff0092 */
	NULL,	/* 0xffff0093 */
	NULL,	/* 0xffff0094 */
	NULL,	/* 0xffff0095 */
	NULL,	/* 0xffff0096 */
	NULL,	/* 0xffff0097 */
	NULL,	/* 0xffff0098 */
	NULL,	/* 0xffff0099 */
	NULL,	/* 0xffff009a */
	NULL,	/* 0xffff009b */
	NULL,	/* 0xffff009c */
	NULL,	/* 0xffff009d */
	NULL,	/* 0xffff009e */
	NULL,	/* 0xffff009f */
	NULL,	/* 0xffff00a0 */
	NULL,	/* 0xffff00a1 */
	NULL,	/* 0xffff00a2 */
	NULL,	/* 0xffff00a3 */
	NULL,	/* 0xffff00a4 */
	&_kernel_isrcb_RLIN3x_TX_ISR,	/* 0xffff00a5 */
	&_kernel_isrcb_RLIN3x_RX_ISR,	/* 0xffff00a6 */
	NULL,	/* 0xffff00a7 */
	NULL,	/* 0xffff00a8 */
	NULL,	/* 0xffff00a9 */
	NULL,	/* 0xffff00aa */
	NULL,	/* 0xffff00ab */
	NULL,	/* 0xffff00ac */
	NULL,	/* 0xffff00ad */
	NULL,	/* 0xffff00ae */
	NULL,	/* 0xffff00af */
	NULL,	/* 0xffff00b0 */
	NULL,	/* 0xffff00b1 */
	NULL,	/* 0xffff00b2 */
	NULL,	/* 0xffff00b3 */
	NULL,	/* 0xffff00b4 */
	NULL,	/* 0xffff00b5 */
	NULL,	/* 0xffff00b6 */
	NULL,	/* 0xffff00b7 */
	NULL,	/* 0xffff00b8 */
	NULL,	/* 0xffff00b9 */
	NULL,	/* 0xffff00ba */
	NULL,	/* 0xffff00bb */
	NULL,	/* 0xffff00bc */
	NULL,	/* 0xffff00bd */
	NULL,	/* 0xffff00be */
	NULL,	/* 0xffff00bf */
	NULL,	/* 0xffff00c0 */
	NULL,	/* 0xffff00c1 */
	NULL,	/* 0xffff00c2 */
	NULL,	/* 0xffff00c3 */
	NULL,	/* 0xffff00c4 */
	NULL,	/* 0xffff00c5 */
	NULL,	/* 0xffff00c6 */
	NULL,	/* 0xffff00c7 */
	NULL,	/* 0xffff00c8 */
	NULL,	/* 0xffff00c9 */
	NULL,	/* 0xffff00ca */
	NULL,	/* 0xffff00cb */
	NULL,	/* 0xffff00cc */
	NULL,	/* 0xffff00cd */
	NULL,	/* 0xffff00ce */
	NULL,	/* 0xffff00cf */
	NULL,	/* 0xffff00d0 */
	NULL,	/* 0xffff00d1 */
	NULL,	/* 0xffff00d2 */
	NULL,	/* 0xffff00d3 */
	NULL,	/* 0xffff00d4 */
	NULL,	/* 0xffff00d5 */
	NULL,	/* 0xffff00d6 */
	NULL,	/* 0xffff00d7 */
	NULL,	/* 0xffff00d8 */
	NULL,	/* 0xffff00d9 */
	NULL,	/* 0xffff00da */
	NULL,	/* 0xffff00db */
	NULL,	/* 0xffff00dc */
	NULL,	/* 0xffff00dd */
	NULL,	/* 0xffff00de */
	NULL,	/* 0xffff00df */
	NULL,	/* 0xffff00e0 */
	NULL,	/* 0xffff00e1 */
	NULL,	/* 0xffff00e2 */
	NULL,	/* 0xffff00e3 */
	NULL,	/* 0xffff00e4 */
	NULL,	/* 0xffff00e5 */
	NULL,	/* 0xffff00e6 */
	NULL,	/* 0xffff00e7 */
	NULL,	/* 0xffff00e8 */
	NULL,	/* 0xffff00e9 */
	NULL,	/* 0xffff00ea */
	NULL,	/* 0xffff00eb */
	NULL,	/* 0xffff00ec */
	NULL,	/* 0xffff00ed */
	NULL,	/* 0xffff00ee */
	NULL,	/* 0xffff00ef */
	NULL,	/* 0xffff00f0 */
	NULL,	/* 0xffff00f1 */
	NULL,	/* 0xffff00f2 */
	NULL,	/* 0xffff00f3 */
	NULL,	/* 0xffff00f4 */
	NULL,	/* 0xffff00f5 */
	NULL,	/* 0xffff00f6 */
	NULL,	/* 0xffff00f7 */
	NULL,	/* 0xffff00f8 */
	NULL,	/* 0xffff00f9 */
	NULL,	/* 0xffff00fa */
	NULL,	/* 0xffff00fb */
	NULL,	/* 0xffff00fc */
	NULL,	/* 0xffff00fd */
	NULL,	/* 0xffff00fe */
	NULL,	/* 0xffff00ff */
	NULL,	/* 0xffff0100 */
	NULL,	/* 0xffff0101 */
	NULL,	/* 0xffff0102 */
	NULL,	/* 0xffff0103 */
	NULL,	/* 0xffff0104 */
	NULL,	/* 0xffff0105 */
	NULL,	/* 0xffff0106 */
	NULL,	/* 0xffff0107 */
	NULL,	/* 0xffff0108 */
	NULL,	/* 0xffff0109 */
	NULL,	/* 0xffff010a */
	NULL,	/* 0xffff010b */
	NULL,	/* 0xffff010c */
	NULL,	/* 0xffff010d */
	NULL,	/* 0xffff010e */
	NULL,	/* 0xffff010f */
	NULL,	/* 0xffff0110 */
	NULL,	/* 0xffff0111 */
	NULL,	/* 0xffff0112 */
	NULL,	/* 0xffff0113 */
	NULL,	/* 0xffff0114 */
	NULL,	/* 0xffff0115 */
	NULL,	/* 0xffff0116 */
	NULL,	/* 0xffff0117 */
	NULL,	/* 0xffff0118 */
	NULL,	/* 0xffff0119 */
	NULL,	/* 0xffff011a */
	NULL,	/* 0xffff011b */
	NULL,	/* 0xffff011c */
	NULL,	/* 0xffff011d */
	NULL,	/* 0xffff011e */
	NULL,	/* 0xffff011f */
	NULL,	/* 0xffff0120 */
	NULL,	/* 0xffff0121 */
	NULL,	/* 0xffff0122 */
	NULL,	/* 0xffff0123 */
	NULL,	/* 0xffff0124 */
	NULL,	/* 0xffff0125 */
	NULL,	/* 0xffff0126 */
	NULL,	/* 0xffff0127 */
	NULL,	/* 0xffff0128 */
	NULL,	/* 0xffff0129 */
	NULL,	/* 0xffff012a */
	NULL,	/* 0xffff012b */
	NULL,	/* 0xffff012c */
	NULL,	/* 0xffff012d */
	NULL,	/* 0xffff012e */
	NULL,	/* 0xffff012f */
	NULL,	/* 0xffff0130 */
	NULL,	/* 0xffff0131 */
	NULL,	/* 0xffff0132 */
	NULL,	/* 0xffff0133 */
	NULL,	/* 0xffff0134 */
	NULL,	/* 0xffff0135 */
	NULL,	/* 0xffff0136 */
	NULL,	/* 0xffff0137 */
	NULL,	/* 0xffff0138 */
	NULL,	/* 0xffff0139 */
	NULL,	/* 0xffff013a */
	NULL,	/* 0xffff013b */
	NULL,	/* 0xffff013c */
	NULL,	/* 0xffff013d */
	NULL,	/* 0xffff013e */
	NULL,	/* 0xffff013f */
	NULL,	/* 0xffff0140 */
	NULL,	/* 0xffff0141 */
	NULL,	/* 0xffff0142 */
	NULL,	/* 0xffff0143 */
	NULL,	/* 0xffff0144 */
	NULL,	/* 0xffff0145 */
	NULL,	/* 0xffff0146 */
	NULL,	/* 0xffff0147 */
	NULL,	/* 0xffff0148 */
	NULL,	/* 0xffff0149 */
	NULL,	/* 0xffff014a */
	NULL,	/* 0xffff014b */
	NULL,	/* 0xffff014c */
	NULL,	/* 0xffff014d */
	NULL,	/* 0xffff014e */
	NULL,	/* 0xffff014f */
	NULL,	/* 0xffff0150 */
	NULL,	/* 0xffff0151 */
	NULL,	/* 0xffff0152 */
	NULL,	/* 0xffff0153 */
	NULL,	/* 0xffff0154 */
	NULL,	/* 0xffff0155 */
	NULL,	/* 0xffff0156 */
	NULL,	/* 0xffff0157 */
	NULL,	/* 0xffff0158 */
	NULL,	/* 0xffff0159 */
	NULL,	/* 0xffff015a */
	NULL,	/* 0xffff015b */
	NULL,	/* 0xffff015c */
	NULL,	/* 0xffff015d */
	NULL	/* 0xffff015e */
};

ISRCB * const core1_isr_p_isrcb_tbl[TNUM_INT] = {
	NULL,	/* 0x20000 */
	NULL,	/* 0x20001 */
	NULL,	/* 0x20002 */
	NULL,	/* 0x20003 */
	NULL,	/* 0x20004 */
	NULL,	/* 0x20005 */
	NULL,	/* 0x20006 */
	NULL,	/* 0x20007 */
	NULL,	/* 0x20008 */
	NULL,	/* 0x20009 */
	NULL,	/* 0x2000a */
	NULL,	/* 0x2000b */
	NULL,	/* 0x2000c */
	NULL,	/* 0x2000d */
	NULL,	/* 0x2000e */
	NULL,	/* 0x2000f */
	NULL,	/* 0x20010 */
	NULL,	/* 0x20011 */
	NULL,	/* 0x20012 */
	NULL,	/* 0x20013 */
	NULL,	/* 0x20014 */
	NULL,	/* 0x20015 */
	NULL,	/* 0x20016 */
	NULL,	/* 0x20017 */
	NULL,	/* 0x20018 */
	NULL,	/* 0x20019 */
	NULL,	/* 0x2001a */
	NULL,	/* 0x2001b */
	NULL,	/* 0x2001c */
	NULL,	/* 0x2001d */
	NULL,	/* 0x2001e */
	NULL,	/* 0x2001f */
	NULL,	/* 0xffff0020 */
	NULL,	/* 0xffff0021 */
	NULL,	/* 0xffff0022 */
	NULL,	/* 0xffff0023 */
	NULL,	/* 0xffff0024 */
	NULL,	/* 0xffff0025 */
	NULL,	/* 0xffff0026 */
	NULL,	/* 0xffff0027 */
	NULL,	/* 0xffff0028 */
	NULL,	/* 0xffff0029 */
	NULL,	/* 0xffff002a */
	NULL,	/* 0xffff002b */
	NULL,	/* 0xffff002c */
	NULL,	/* 0xffff002d */
	NULL,	/* 0xffff002e */
	NULL,	/* 0xffff002f */
	NULL,	/* 0xffff0030 */
	NULL,	/* 0xffff0031 */
	NULL,	/* 0xffff0032 */
	NULL,	/* 0xffff0033 */
	NULL,	/* 0xffff0034 */
	NULL,	/* 0xffff0035 */
	NULL,	/* 0xffff0036 */
	NULL,	/* 0xffff0037 */
	NULL,	/* 0xffff0038 */
	NULL,	/* 0xffff0039 */
	NULL,	/* 0xffff003a */
	NULL,	/* 0xffff003b */
	NULL,	/* 0xffff003c */
	NULL,	/* 0xffff003d */
	NULL,	/* 0xffff003e */
	NULL,	/* 0xffff003f */
	NULL,	/* 0xffff0040 */
	NULL,	/* 0xffff0041 */
	NULL,	/* 0xffff0042 */
	NULL,	/* 0xffff0043 */
	NULL,	/* 0xffff0044 */
	NULL,	/* 0xffff0045 */
	NULL,	/* 0xffff0046 */
	NULL,	/* 0xffff0047 */
	NULL,	/* 0xffff0048 */
	NULL,	/* 0xffff0049 */
	NULL,	/* 0xffff004a */
	NULL,	/* 0xffff004b */
	NULL,	/* 0xffff004c */
	NULL,	/* 0xffff004d */
	NULL,	/* 0xffff004e */
	NULL,	/* 0xffff004f */
	NULL,	/* 0xffff0050 */
	NULL,	/* 0xffff0051 */
	NULL,	/* 0xffff0052 */
	NULL,	/* 0xffff0053 */
	NULL,	/* 0xffff0054 */
	NULL,	/* 0xffff0055 */
	NULL,	/* 0xffff0056 */
	NULL,	/* 0xffff0057 */
	NULL,	/* 0xffff0058 */
	NULL,	/* 0xffff0059 */
	NULL,	/* 0xffff005a */
	NULL,	/* 0xffff005b */
	NULL,	/* 0xffff005c */
	NULL,	/* 0xffff005d */
	NULL,	/* 0xffff005e */
	NULL,	/* 0xffff005f */
	NULL,	/* 0xffff0060 */
	NULL,	/* 0xffff0061 */
	NULL,	/* 0xffff0062 */
	NULL,	/* 0xffff0063 */
	NULL,	/* 0xffff0064 */
	NULL,	/* 0xffff0065 */
	NULL,	/* 0xffff0066 */
	NULL,	/* 0xffff0067 */
	NULL,	/* 0xffff0068 */
	NULL,	/* 0xffff0069 */
	NULL,	/* 0xffff006a */
	NULL,	/* 0xffff006b */
	NULL,	/* 0xffff006c */
	NULL,	/* 0xffff006d */
	NULL,	/* 0xffff006e */
	NULL,	/* 0xffff006f */
	NULL,	/* 0xffff0070 */
	NULL,	/* 0xffff0071 */
	NULL,	/* 0xffff0072 */
	NULL,	/* 0xffff0073 */
	NULL,	/* 0xffff0074 */
	NULL,	/* 0xffff0075 */
	NULL,	/* 0xffff0076 */
	NULL,	/* 0xffff0077 */
	NULL,	/* 0xffff0078 */
	NULL,	/* 0xffff0079 */
	NULL,	/* 0xffff007a */
	NULL,	/* 0xffff007b */
	NULL,	/* 0xffff007c */
	NULL,	/* 0xffff007d */
	NULL,	/* 0xffff007e */
	NULL,	/* 0xffff007f */
	NULL,	/* 0xffff0080 */
	NULL,	/* 0xffff0081 */
	NULL,	/* 0xffff0082 */
	NULL,	/* 0xffff0083 */
	NULL,	/* 0xffff0084 */
	NULL,	/* 0xffff0085 */
	NULL,	/* 0xffff0086 */
	NULL,	/* 0xffff0087 */
	NULL,	/* 0xffff0088 */
	NULL,	/* 0xffff0089 */
	NULL,	/* 0xffff008a */
	NULL,	/* 0xffff008b */
	NULL,	/* 0xffff008c */
	NULL,	/* 0xffff008d */
	NULL,	/* 0xffff008e */
	NULL,	/* 0xffff008f */
	NULL,	/* 0xffff0090 */
	NULL,	/* 0xffff0091 */
	NULL,	/* 0xffff0092 */
	NULL,	/* 0xffff0093 */
	NULL,	/* 0xffff0094 */
	NULL,	/* 0xffff0095 */
	NULL,	/* 0xffff0096 */
	NULL,	/* 0xffff0097 */
	NULL,	/* 0xffff0098 */
	NULL,	/* 0xffff0099 */
	NULL,	/* 0xffff009a */
	NULL,	/* 0xffff009b */
	NULL,	/* 0xffff009c */
	NULL,	/* 0xffff009d */
	NULL,	/* 0xffff009e */
	NULL,	/* 0xffff009f */
	NULL,	/* 0xffff00a0 */
	NULL,	/* 0xffff00a1 */
	NULL,	/* 0xffff00a2 */
	NULL,	/* 0xffff00a3 */
	NULL,	/* 0xffff00a4 */
	NULL,	/* 0xffff00a5 */
	NULL,	/* 0xffff00a6 */
	NULL,	/* 0xffff00a7 */
	&_kernel_isrcb_C2ISR_for_MAIN_HW_COUNTER_CORE1,	/* 0xffff00a8 */
	NULL,	/* 0xffff00a9 */
	NULL,	/* 0xffff00aa */
	NULL,	/* 0xffff00ab */
	NULL,	/* 0xffff00ac */
	NULL,	/* 0xffff00ad */
	NULL,	/* 0xffff00ae */
	NULL,	/* 0xffff00af */
	NULL,	/* 0xffff00b0 */
	NULL,	/* 0xffff00b1 */
	NULL,	/* 0xffff00b2 */
	NULL,	/* 0xffff00b3 */
	NULL,	/* 0xffff00b4 */
	NULL,	/* 0xffff00b5 */
	NULL,	/* 0xffff00b6 */
	NULL,	/* 0xffff00b7 */
	NULL,	/* 0xffff00b8 */
	NULL,	/* 0xffff00b9 */
	NULL,	/* 0xffff00ba */
	NULL,	/* 0xffff00bb */
	NULL,	/* 0xffff00bc */
	NULL,	/* 0xffff00bd */
	NULL,	/* 0xffff00be */
	NULL,	/* 0xffff00bf */
	NULL,	/* 0xffff00c0 */
	NULL,	/* 0xffff00c1 */
	NULL,	/* 0xffff00c2 */
	NULL,	/* 0xffff00c3 */
	NULL,	/* 0xffff00c4 */
	NULL,	/* 0xffff00c5 */
	NULL,	/* 0xffff00c6 */
	NULL,	/* 0xffff00c7 */
	NULL,	/* 0xffff00c8 */
	NULL,	/* 0xffff00c9 */
	NULL,	/* 0xffff00ca */
	NULL,	/* 0xffff00cb */
	NULL,	/* 0xffff00cc */
	NULL,	/* 0xffff00cd */
	NULL,	/* 0xffff00ce */
	NULL,	/* 0xffff00cf */
	NULL,	/* 0xffff00d0 */
	NULL,	/* 0xffff00d1 */
	NULL,	/* 0xffff00d2 */
	NULL,	/* 0xffff00d3 */
	NULL,	/* 0xffff00d4 */
	NULL,	/* 0xffff00d5 */
	NULL,	/* 0xffff00d6 */
	NULL,	/* 0xffff00d7 */
	NULL,	/* 0xffff00d8 */
	NULL,	/* 0xffff00d9 */
	NULL,	/* 0xffff00da */
	NULL,	/* 0xffff00db */
	NULL,	/* 0xffff00dc */
	NULL,	/* 0xffff00dd */
	NULL,	/* 0xffff00de */
	NULL,	/* 0xffff00df */
	NULL,	/* 0xffff00e0 */
	NULL,	/* 0xffff00e1 */
	NULL,	/* 0xffff00e2 */
	NULL,	/* 0xffff00e3 */
	NULL,	/* 0xffff00e4 */
	NULL,	/* 0xffff00e5 */
	NULL,	/* 0xffff00e6 */
	NULL,	/* 0xffff00e7 */
	NULL,	/* 0xffff00e8 */
	NULL,	/* 0xffff00e9 */
	NULL,	/* 0xffff00ea */
	NULL,	/* 0xffff00eb */
	NULL,	/* 0xffff00ec */
	NULL,	/* 0xffff00ed */
	NULL,	/* 0xffff00ee */
	NULL,	/* 0xffff00ef */
	NULL,	/* 0xffff00f0 */
	NULL,	/* 0xffff00f1 */
	NULL,	/* 0xffff00f2 */
	NULL,	/* 0xffff00f3 */
	NULL,	/* 0xffff00f4 */
	NULL,	/* 0xffff00f5 */
	NULL,	/* 0xffff00f6 */
	NULL,	/* 0xffff00f7 */
	NULL,	/* 0xffff00f8 */
	NULL,	/* 0xffff00f9 */
	NULL,	/* 0xffff00fa */
	NULL,	/* 0xffff00fb */
	NULL,	/* 0xffff00fc */
	NULL,	/* 0xffff00fd */
	NULL,	/* 0xffff00fe */
	NULL,	/* 0xffff00ff */
	NULL,	/* 0xffff0100 */
	NULL,	/* 0xffff0101 */
	NULL,	/* 0xffff0102 */
	NULL,	/* 0xffff0103 */
	NULL,	/* 0xffff0104 */
	NULL,	/* 0xffff0105 */
	NULL,	/* 0xffff0106 */
	NULL,	/* 0xffff0107 */
	NULL,	/* 0xffff0108 */
	NULL,	/* 0xffff0109 */
	NULL,	/* 0xffff010a */
	NULL,	/* 0xffff010b */
	NULL,	/* 0xffff010c */
	NULL,	/* 0xffff010d */
	NULL,	/* 0xffff010e */
	NULL,	/* 0xffff010f */
	NULL,	/* 0xffff0110 */
	NULL,	/* 0xffff0111 */
	NULL,	/* 0xffff0112 */
	NULL,	/* 0xffff0113 */
	NULL,	/* 0xffff0114 */
	NULL,	/* 0xffff0115 */
	NULL,	/* 0xffff0116 */
	NULL,	/* 0xffff0117 */
	NULL,	/* 0xffff0118 */
	NULL,	/* 0xffff0119 */
	NULL,	/* 0xffff011a */
	NULL,	/* 0xffff011b */
	NULL,	/* 0xffff011c */
	NULL,	/* 0xffff011d */
	NULL,	/* 0xffff011e */
	NULL,	/* 0xffff011f */
	NULL,	/* 0xffff0120 */
	NULL,	/* 0xffff0121 */
	NULL,	/* 0xffff0122 */
	NULL,	/* 0xffff0123 */
	NULL,	/* 0xffff0124 */
	NULL,	/* 0xffff0125 */
	NULL,	/* 0xffff0126 */
	NULL,	/* 0xffff0127 */
	NULL,	/* 0xffff0128 */
	NULL,	/* 0xffff0129 */
	NULL,	/* 0xffff012a */
	NULL,	/* 0xffff012b */
	NULL,	/* 0xffff012c */
	NULL,	/* 0xffff012d */
	NULL,	/* 0xffff012e */
	NULL,	/* 0xffff012f */
	NULL,	/* 0xffff0130 */
	NULL,	/* 0xffff0131 */
	NULL,	/* 0xffff0132 */
	NULL,	/* 0xffff0133 */
	NULL,	/* 0xffff0134 */
	NULL,	/* 0xffff0135 */
	NULL,	/* 0xffff0136 */
	NULL,	/* 0xffff0137 */
	NULL,	/* 0xffff0138 */
	NULL,	/* 0xffff0139 */
	NULL,	/* 0xffff013a */
	NULL,	/* 0xffff013b */
	NULL,	/* 0xffff013c */
	NULL,	/* 0xffff013d */
	NULL,	/* 0xffff013e */
	NULL,	/* 0xffff013f */
	NULL,	/* 0xffff0140 */
	NULL,	/* 0xffff0141 */
	NULL,	/* 0xffff0142 */
	NULL,	/* 0xffff0143 */
	NULL,	/* 0xffff0144 */
	NULL,	/* 0xffff0145 */
	NULL,	/* 0xffff0146 */
	NULL,	/* 0xffff0147 */
	NULL,	/* 0xffff0148 */
	NULL,	/* 0xffff0149 */
	NULL,	/* 0xffff014a */
	NULL,	/* 0xffff014b */
	NULL,	/* 0xffff014c */
	NULL,	/* 0xffff014d */
	NULL,	/* 0xffff014e */
	NULL,	/* 0xffff014f */
	NULL,	/* 0xffff0150 */
	NULL,	/* 0xffff0151 */
	NULL,	/* 0xffff0152 */
	NULL,	/* 0xffff0153 */
	NULL,	/* 0xffff0154 */
	NULL,	/* 0xffff0155 */
	NULL,	/* 0xffff0156 */
	NULL,	/* 0xffff0157 */
	NULL,	/* 0xffff0158 */
	NULL,	/* 0xffff0159 */
	NULL,	/* 0xffff015a */
	NULL,	/* 0xffff015b */
	NULL,	/* 0xffff015c */
	NULL,	/* 0xffff015d */
	NULL	/* 0xffff015e */
};

const uint32 isr_p_isrcb_table[TotalNumberOfCores] = {
	(const uint32) core0_isr_p_isrcb_tbl,
	(const uint32) core1_isr_p_isrcb_tbl
};

extern void interrupt(void);
const uint32 __attribute__((aligned(512))) core0_intbp_tbl[TNUM_INT] = {
	(uint32)&interrupt,	/* 0x10000 */
	(uint32)&interrupt,	/* 0x10001 */
	(uint32)&interrupt,	/* 0x10002 */
	(uint32)&interrupt,	/* 0x10003 */
	(uint32)&interrupt,	/* 0x10004 */
	(uint32)&interrupt,	/* 0x10005 */
	(uint32)&interrupt,	/* 0x10006 */
	(uint32)&interrupt,	/* 0x10007 */
	(uint32)&interrupt,	/* 0x10008 */
	(uint32)&interrupt,	/* 0x10009 */
	(uint32)&interrupt,	/* 0x1000a */
	(uint32)&interrupt,	/* 0x1000b */
	(uint32)&interrupt,	/* 0x1000c */
	(uint32)&interrupt,	/* 0x1000d */
	(uint32)&interrupt,	/* 0x1000e */
	(uint32)&interrupt,	/* 0x1000f */
	(uint32)&interrupt,	/* 0x10010 */
	(uint32)&interrupt,	/* 0x10011 */
	(uint32)&interrupt,	/* 0x10012 */
	(uint32)&interrupt,	/* 0x10013 */
	(uint32)&interrupt,	/* 0x10014 */
	(uint32)&interrupt,	/* 0x10015 */
	(uint32)&interrupt,	/* 0x10016 */
	(uint32)&interrupt,	/* 0x10017 */
	(uint32)&interrupt,	/* 0x10018 */
	(uint32)&interrupt,	/* 0x10019 */
	(uint32)&interrupt,	/* 0x1001a */
	(uint32)&interrupt,	/* 0x1001b */
	(uint32)&interrupt,	/* 0x1001c */
	(uint32)&interrupt,	/* 0x1001d */
	(uint32)&interrupt,	/* 0x1001e */
	(uint32)&interrupt,	/* 0x1001f */
	(uint32)&interrupt,	/* 0xffff0020 */
	(uint32)&interrupt,	/* 0xffff0021 */
	(uint32)&interrupt,	/* 0xffff0022 */
	(uint32)&interrupt,	/* 0xffff0023 */
	(uint32)&interrupt,	/* 0xffff0024 */
	(uint32)&interrupt,	/* 0xffff0025 */
	(uint32)&interrupt,	/* 0xffff0026 */
	(uint32)&interrupt,	/* 0xffff0027 */
	(uint32)&interrupt,	/* 0xffff0028 */
	(uint32)&interrupt,	/* 0xffff0029 */
	(uint32)&interrupt,	/* 0xffff002a */
	(uint32)&interrupt,	/* 0xffff002b */
	(uint32)&interrupt,	/* 0xffff002c */
	(uint32)&interrupt,	/* 0xffff002d */
	(uint32)&interrupt,	/* 0xffff002e */
	(uint32)&interrupt,	/* 0xffff002f */
	(uint32)&interrupt,	/* 0xffff0030 */
	(uint32)&interrupt,	/* 0xffff0031 */
	(uint32)&interrupt,	/* 0xffff0032 */
	(uint32)&interrupt,	/* 0xffff0033 */
	(uint32)&interrupt,	/* 0xffff0034 */
	(uint32)&interrupt,	/* 0xffff0035 */
	(uint32)&interrupt,	/* 0xffff0036 */
	(uint32)&interrupt,	/* 0xffff0037 */
	(uint32)&interrupt,	/* 0xffff0038 */
	(uint32)&interrupt,	/* 0xffff0039 */
	(uint32)&interrupt,	/* 0xffff003a */
	(uint32)&interrupt,	/* 0xffff003b */
	(uint32)&interrupt,	/* 0xffff003c */
	(uint32)&interrupt,	/* 0xffff003d */
	(uint32)&interrupt,	/* 0xffff003e */
	(uint32)&interrupt,	/* 0xffff003f */
	(uint32)&interrupt,	/* 0xffff0040 */
	(uint32)&interrupt,	/* 0xffff0041 */
	(uint32)&interrupt,	/* 0xffff0042 */
	(uint32)&interrupt,	/* 0xffff0043 */
	(uint32)&interrupt,	/* 0xffff0044 */
	(uint32)&interrupt,	/* 0xffff0045 */
	(uint32)&interrupt,	/* 0xffff0046 */
	(uint32)&interrupt,	/* 0xffff0047 */
	(uint32)&interrupt,	/* 0xffff0048 */
	(uint32)&interrupt,	/* 0xffff0049 */
	(uint32)&interrupt,	/* 0xffff004a */
	(uint32)&interrupt,	/* 0xffff004b */
	(uint32)&interrupt,	/* 0xffff004c */
	(uint32)&interrupt,	/* 0xffff004d */
	(uint32)&interrupt,	/* 0xffff004e */
	(uint32)&interrupt,	/* 0xffff004f */
	(uint32)&interrupt,	/* 0xffff0050 */
	(uint32)&interrupt,	/* 0xffff0051 */
	(uint32)&interrupt,	/* 0xffff0052 */
	(uint32)&interrupt,	/* 0xffff0053 */
	(uint32)&interrupt,	/* 0xffff0054 */
	(uint32)&interrupt,	/* 0xffff0055 */
	(uint32)&interrupt,	/* 0xffff0056 */
	(uint32)&interrupt,	/* 0xffff0057 */
	(uint32)&interrupt,	/* 0xffff0058 */
	(uint32)&interrupt,	/* 0xffff0059 */
	(uint32)&interrupt,	/* 0xffff005a */
	(uint32)&interrupt,	/* 0xffff005b */
	(uint32)&interrupt,	/* 0xffff005c */
	(uint32)&interrupt,	/* 0xffff005d */
	(uint32)&interrupt,	/* 0xffff005e */
	(uint32)&interrupt,	/* 0xffff005f */
	(uint32)&interrupt,	/* 0xffff0060 */
	(uint32)&interrupt,	/* 0xffff0061 */
	(uint32)&interrupt,	/* 0xffff0062 */
	(uint32)&interrupt,	/* 0xffff0063 */
	(uint32)&interrupt,	/* 0xffff0064 */
	(uint32)&interrupt,	/* 0xffff0065 */
	(uint32)&interrupt,	/* 0xffff0066 */
	(uint32)&interrupt,	/* 0xffff0067 */
	(uint32)&interrupt,	/* 0xffff0068 */
	(uint32)&interrupt,	/* 0xffff0069 */
	(uint32)&interrupt,	/* 0xffff006a */
	(uint32)&interrupt,	/* 0xffff006b */
	(uint32)&interrupt,	/* 0xffff006c */
	(uint32)&interrupt,	/* 0xffff006d */
	(uint32)&interrupt,	/* 0xffff006e */
	(uint32)&interrupt,	/* 0xffff006f */
	(uint32)&interrupt,	/* 0xffff0070 */
	(uint32)&interrupt,	/* 0xffff0071 */
	(uint32)&interrupt,	/* 0xffff0072 */
	(uint32)&interrupt,	/* 0xffff0073 */
	(uint32)&interrupt,	/* 0xffff0074 */
	(uint32)&interrupt,	/* 0xffff0075 */
	(uint32)&interrupt,	/* 0xffff0076 */
	(uint32)&interrupt,	/* 0xffff0077 */
	(uint32)&interrupt,	/* 0xffff0078 */
	(uint32)&interrupt,	/* 0xffff0079 */
	(uint32)&interrupt,	/* 0xffff007a */
	(uint32)&interrupt,	/* 0xffff007b */
	(uint32)&interrupt,	/* 0xffff007c */
	(uint32)&interrupt,	/* 0xffff007d */
	(uint32)&interrupt,	/* 0xffff007e */
	(uint32)&interrupt,	/* 0xffff007f */
	(uint32)&interrupt,	/* 0xffff0080 */
	(uint32)&interrupt,	/* 0xffff0081 */
	(uint32)&interrupt,	/* 0xffff0082 */
	(uint32)&interrupt,	/* 0xffff0083 */
	(uint32)&interrupt,	/* 0xffff0084 */
	(uint32)&interrupt,	/* 0xffff0085 */
	(uint32)&interrupt,	/* 0xffff0086 */
	(uint32)&interrupt,	/* 0xffff0087 */
	(uint32)&interrupt,	/* 0xffff0088 */
	(uint32)&interrupt,	/* 0xffff0089 */
	(uint32)&interrupt,	/* 0xffff008a */
	(uint32)&interrupt,	/* 0xffff008b */
	(uint32)&interrupt,	/* 0xffff008c */
	(uint32)&interrupt,	/* 0xffff008d */
	(uint32)&interrupt,	/* 0xffff008e */
	(uint32)&interrupt,	/* 0xffff008f */
	(uint32)&interrupt,	/* 0xffff0090 */
	(uint32)&interrupt,	/* 0xffff0091 */
	(uint32)&interrupt,	/* 0xffff0092 */
	(uint32)&interrupt,	/* 0xffff0093 */
	(uint32)&interrupt,	/* 0xffff0094 */
	(uint32)&interrupt,	/* 0xffff0095 */
	(uint32)&interrupt,	/* 0xffff0096 */
	(uint32)&interrupt,	/* 0xffff0097 */
	(uint32)&interrupt,	/* 0xffff0098 */
	(uint32)&interrupt,	/* 0xffff0099 */
	(uint32)&interrupt,	/* 0xffff009a */
	(uint32)&interrupt,	/* 0xffff009b */
	(uint32)&interrupt,	/* 0xffff009c */
	(uint32)&interrupt,	/* 0xffff009d */
	(uint32)&interrupt,	/* 0xffff009e */
	(uint32)&interrupt,	/* 0xffff009f */
	(uint32)&interrupt,	/* 0xffff00a0 */
	(uint32)&interrupt,	/* 0xffff00a1 */
	(uint32)&interrupt,	/* 0xffff00a2 */
	(uint32)&interrupt,	/* 0xffff00a3 */
	(uint32)&interrupt,	/* 0xffff00a4 */
	(uint32)&interrupt,	/* 0xffff00a5 */
	(uint32)&interrupt,	/* 0xffff00a6 */
	(uint32)&interrupt,	/* 0xffff00a7 */
	(uint32)&interrupt,	/* 0xffff00a8 */
	(uint32)&interrupt,	/* 0xffff00a9 */
	(uint32)&interrupt,	/* 0xffff00aa */
	(uint32)&interrupt,	/* 0xffff00ab */
	(uint32)&interrupt,	/* 0xffff00ac */
	(uint32)&interrupt,	/* 0xffff00ad */
	(uint32)&interrupt,	/* 0xffff00ae */
	(uint32)&interrupt,	/* 0xffff00af */
	(uint32)&interrupt,	/* 0xffff00b0 */
	(uint32)&interrupt,	/* 0xffff00b1 */
	(uint32)&interrupt,	/* 0xffff00b2 */
	(uint32)&interrupt,	/* 0xffff00b3 */
	(uint32)&interrupt,	/* 0xffff00b4 */
	(uint32)&interrupt,	/* 0xffff00b5 */
	(uint32)&interrupt,	/* 0xffff00b6 */
	(uint32)&interrupt,	/* 0xffff00b7 */
	(uint32)&interrupt,	/* 0xffff00b8 */
	(uint32)&interrupt,	/* 0xffff00b9 */
	(uint32)&interrupt,	/* 0xffff00ba */
	(uint32)&interrupt,	/* 0xffff00bb */
	(uint32)&interrupt,	/* 0xffff00bc */
	(uint32)&interrupt,	/* 0xffff00bd */
	(uint32)&interrupt,	/* 0xffff00be */
	(uint32)&interrupt,	/* 0xffff00bf */
	(uint32)&interrupt,	/* 0xffff00c0 */
	(uint32)&interrupt,	/* 0xffff00c1 */
	(uint32)&interrupt,	/* 0xffff00c2 */
	(uint32)&interrupt,	/* 0xffff00c3 */
	(uint32)&interrupt,	/* 0xffff00c4 */
	(uint32)&interrupt,	/* 0xffff00c5 */
	(uint32)&interrupt,	/* 0xffff00c6 */
	(uint32)&interrupt,	/* 0xffff00c7 */
	(uint32)&interrupt,	/* 0xffff00c8 */
	(uint32)&interrupt,	/* 0xffff00c9 */
	(uint32)&interrupt,	/* 0xffff00ca */
	(uint32)&interrupt,	/* 0xffff00cb */
	(uint32)&interrupt,	/* 0xffff00cc */
	(uint32)&interrupt,	/* 0xffff00cd */
	(uint32)&interrupt,	/* 0xffff00ce */
	(uint32)&interrupt,	/* 0xffff00cf */
	(uint32)&interrupt,	/* 0xffff00d0 */
	(uint32)&interrupt,	/* 0xffff00d1 */
	(uint32)&interrupt,	/* 0xffff00d2 */
	(uint32)&interrupt,	/* 0xffff00d3 */
	(uint32)&interrupt,	/* 0xffff00d4 */
	(uint32)&interrupt,	/* 0xffff00d5 */
	(uint32)&interrupt,	/* 0xffff00d6 */
	(uint32)&interrupt,	/* 0xffff00d7 */
	(uint32)&interrupt,	/* 0xffff00d8 */
	(uint32)&interrupt,	/* 0xffff00d9 */
	(uint32)&interrupt,	/* 0xffff00da */
	(uint32)&interrupt,	/* 0xffff00db */
	(uint32)&interrupt,	/* 0xffff00dc */
	(uint32)&interrupt,	/* 0xffff00dd */
	(uint32)&interrupt,	/* 0xffff00de */
	(uint32)&interrupt,	/* 0xffff00df */
	(uint32)&interrupt,	/* 0xffff00e0 */
	(uint32)&interrupt,	/* 0xffff00e1 */
	(uint32)&interrupt,	/* 0xffff00e2 */
	(uint32)&interrupt,	/* 0xffff00e3 */
	(uint32)&interrupt,	/* 0xffff00e4 */
	(uint32)&interrupt,	/* 0xffff00e5 */
	(uint32)&interrupt,	/* 0xffff00e6 */
	(uint32)&interrupt,	/* 0xffff00e7 */
	(uint32)&interrupt,	/* 0xffff00e8 */
	(uint32)&interrupt,	/* 0xffff00e9 */
	(uint32)&interrupt,	/* 0xffff00ea */
	(uint32)&interrupt,	/* 0xffff00eb */
	(uint32)&interrupt,	/* 0xffff00ec */
	(uint32)&interrupt,	/* 0xffff00ed */
	(uint32)&interrupt,	/* 0xffff00ee */
	(uint32)&interrupt,	/* 0xffff00ef */
	(uint32)&interrupt,	/* 0xffff00f0 */
	(uint32)&interrupt,	/* 0xffff00f1 */
	(uint32)&interrupt,	/* 0xffff00f2 */
	(uint32)&interrupt,	/* 0xffff00f3 */
	(uint32)&interrupt,	/* 0xffff00f4 */
	(uint32)&interrupt,	/* 0xffff00f5 */
	(uint32)&interrupt,	/* 0xffff00f6 */
	(uint32)&interrupt,	/* 0xffff00f7 */
	(uint32)&interrupt,	/* 0xffff00f8 */
	(uint32)&interrupt,	/* 0xffff00f9 */
	(uint32)&interrupt,	/* 0xffff00fa */
	(uint32)&interrupt,	/* 0xffff00fb */
	(uint32)&interrupt,	/* 0xffff00fc */
	(uint32)&interrupt,	/* 0xffff00fd */
	(uint32)&interrupt,	/* 0xffff00fe */
	(uint32)&interrupt,	/* 0xffff00ff */
	(uint32)&interrupt,	/* 0xffff0100 */
	(uint32)&interrupt,	/* 0xffff0101 */
	(uint32)&interrupt,	/* 0xffff0102 */
	(uint32)&interrupt,	/* 0xffff0103 */
	(uint32)&interrupt,	/* 0xffff0104 */
	(uint32)&interrupt,	/* 0xffff0105 */
	(uint32)&interrupt,	/* 0xffff0106 */
	(uint32)&interrupt,	/* 0xffff0107 */
	(uint32)&interrupt,	/* 0xffff0108 */
	(uint32)&interrupt,	/* 0xffff0109 */
	(uint32)&interrupt,	/* 0xffff010a */
	(uint32)&interrupt,	/* 0xffff010b */
	(uint32)&interrupt,	/* 0xffff010c */
	(uint32)&interrupt,	/* 0xffff010d */
	(uint32)&interrupt,	/* 0xffff010e */
	(uint32)&interrupt,	/* 0xffff010f */
	(uint32)&interrupt,	/* 0xffff0110 */
	(uint32)&interrupt,	/* 0xffff0111 */
	(uint32)&interrupt,	/* 0xffff0112 */
	(uint32)&interrupt,	/* 0xffff0113 */
	(uint32)&interrupt,	/* 0xffff0114 */
	(uint32)&interrupt,	/* 0xffff0115 */
	(uint32)&interrupt,	/* 0xffff0116 */
	(uint32)&interrupt,	/* 0xffff0117 */
	(uint32)&interrupt,	/* 0xffff0118 */
	(uint32)&interrupt,	/* 0xffff0119 */
	(uint32)&interrupt,	/* 0xffff011a */
	(uint32)&interrupt,	/* 0xffff011b */
	(uint32)&interrupt,	/* 0xffff011c */
	(uint32)&interrupt,	/* 0xffff011d */
	(uint32)&interrupt,	/* 0xffff011e */
	(uint32)&interrupt,	/* 0xffff011f */
	(uint32)&interrupt,	/* 0xffff0120 */
	(uint32)&interrupt,	/* 0xffff0121 */
	(uint32)&interrupt,	/* 0xffff0122 */
	(uint32)&interrupt,	/* 0xffff0123 */
	(uint32)&interrupt,	/* 0xffff0124 */
	(uint32)&interrupt,	/* 0xffff0125 */
	(uint32)&interrupt,	/* 0xffff0126 */
	(uint32)&interrupt,	/* 0xffff0127 */
	(uint32)&interrupt,	/* 0xffff0128 */
	(uint32)&interrupt,	/* 0xffff0129 */
	(uint32)&interrupt,	/* 0xffff012a */
	(uint32)&interrupt,	/* 0xffff012b */
	(uint32)&interrupt,	/* 0xffff012c */
	(uint32)&interrupt,	/* 0xffff012d */
	(uint32)&interrupt,	/* 0xffff012e */
	(uint32)&interrupt,	/* 0xffff012f */
	(uint32)&interrupt,	/* 0xffff0130 */
	(uint32)&interrupt,	/* 0xffff0131 */
	(uint32)&interrupt,	/* 0xffff0132 */
	(uint32)&interrupt,	/* 0xffff0133 */
	(uint32)&interrupt,	/* 0xffff0134 */
	(uint32)&interrupt,	/* 0xffff0135 */
	(uint32)&interrupt,	/* 0xffff0136 */
	(uint32)&interrupt,	/* 0xffff0137 */
	(uint32)&interrupt,	/* 0xffff0138 */
	(uint32)&interrupt,	/* 0xffff0139 */
	(uint32)&interrupt,	/* 0xffff013a */
	(uint32)&interrupt,	/* 0xffff013b */
	(uint32)&interrupt,	/* 0xffff013c */
	(uint32)&interrupt,	/* 0xffff013d */
	(uint32)&interrupt,	/* 0xffff013e */
	(uint32)&interrupt,	/* 0xffff013f */
	(uint32)&interrupt,	/* 0xffff0140 */
	(uint32)&interrupt,	/* 0xffff0141 */
	(uint32)&interrupt,	/* 0xffff0142 */
	(uint32)&interrupt,	/* 0xffff0143 */
	(uint32)&interrupt,	/* 0xffff0144 */
	(uint32)&interrupt,	/* 0xffff0145 */
	(uint32)&interrupt,	/* 0xffff0146 */
	(uint32)&interrupt,	/* 0xffff0147 */
	(uint32)&interrupt,	/* 0xffff0148 */
	(uint32)&interrupt,	/* 0xffff0149 */
	(uint32)&interrupt,	/* 0xffff014a */
	(uint32)&interrupt,	/* 0xffff014b */
	(uint32)&interrupt,	/* 0xffff014c */
	(uint32)&interrupt,	/* 0xffff014d */
	(uint32)&interrupt,	/* 0xffff014e */
	(uint32)&interrupt,	/* 0xffff014f */
	(uint32)&interrupt,	/* 0xffff0150 */
	(uint32)&interrupt,	/* 0xffff0151 */
	(uint32)&interrupt,	/* 0xffff0152 */
	(uint32)&interrupt,	/* 0xffff0153 */
	(uint32)&interrupt,	/* 0xffff0154 */
	(uint32)&interrupt,	/* 0xffff0155 */
	(uint32)&interrupt,	/* 0xffff0156 */
	(uint32)&interrupt,	/* 0xffff0157 */
	(uint32)&interrupt,	/* 0xffff0158 */
	(uint32)&interrupt,	/* 0xffff0159 */
	(uint32)&interrupt,	/* 0xffff015a */
	(uint32)&interrupt,	/* 0xffff015b */
	(uint32)&interrupt,	/* 0xffff015c */
	(uint32)&interrupt,	/* 0xffff015d */
	(uint32)&interrupt	/* 0xffff015e */
};
extern void interrupt(void);
const uint32 __attribute__((aligned(512))) core1_intbp_tbl[TNUM_INT] = {
	(uint32)&interrupt,	/* 0x20000 */
	(uint32)&interrupt,	/* 0x20001 */
	(uint32)&interrupt,	/* 0x20002 */
	(uint32)&interrupt,	/* 0x20003 */
	(uint32)&interrupt,	/* 0x20004 */
	(uint32)&interrupt,	/* 0x20005 */
	(uint32)&interrupt,	/* 0x20006 */
	(uint32)&interrupt,	/* 0x20007 */
	(uint32)&interrupt,	/* 0x20008 */
	(uint32)&interrupt,	/* 0x20009 */
	(uint32)&interrupt,	/* 0x2000a */
	(uint32)&interrupt,	/* 0x2000b */
	(uint32)&interrupt,	/* 0x2000c */
	(uint32)&interrupt,	/* 0x2000d */
	(uint32)&interrupt,	/* 0x2000e */
	(uint32)&interrupt,	/* 0x2000f */
	(uint32)&interrupt,	/* 0x20010 */
	(uint32)&interrupt,	/* 0x20011 */
	(uint32)&interrupt,	/* 0x20012 */
	(uint32)&interrupt,	/* 0x20013 */
	(uint32)&interrupt,	/* 0x20014 */
	(uint32)&interrupt,	/* 0x20015 */
	(uint32)&interrupt,	/* 0x20016 */
	(uint32)&interrupt,	/* 0x20017 */
	(uint32)&interrupt,	/* 0x20018 */
	(uint32)&interrupt,	/* 0x20019 */
	(uint32)&interrupt,	/* 0x2001a */
	(uint32)&interrupt,	/* 0x2001b */
	(uint32)&interrupt,	/* 0x2001c */
	(uint32)&interrupt,	/* 0x2001d */
	(uint32)&interrupt,	/* 0x2001e */
	(uint32)&interrupt,	/* 0x2001f */
	(uint32)&interrupt,	/* 0xffff0020 */
	(uint32)&interrupt,	/* 0xffff0021 */
	(uint32)&interrupt,	/* 0xffff0022 */
	(uint32)&interrupt,	/* 0xffff0023 */
	(uint32)&interrupt,	/* 0xffff0024 */
	(uint32)&interrupt,	/* 0xffff0025 */
	(uint32)&interrupt,	/* 0xffff0026 */
	(uint32)&interrupt,	/* 0xffff0027 */
	(uint32)&interrupt,	/* 0xffff0028 */
	(uint32)&interrupt,	/* 0xffff0029 */
	(uint32)&interrupt,	/* 0xffff002a */
	(uint32)&interrupt,	/* 0xffff002b */
	(uint32)&interrupt,	/* 0xffff002c */
	(uint32)&interrupt,	/* 0xffff002d */
	(uint32)&interrupt,	/* 0xffff002e */
	(uint32)&interrupt,	/* 0xffff002f */
	(uint32)&interrupt,	/* 0xffff0030 */
	(uint32)&interrupt,	/* 0xffff0031 */
	(uint32)&interrupt,	/* 0xffff0032 */
	(uint32)&interrupt,	/* 0xffff0033 */
	(uint32)&interrupt,	/* 0xffff0034 */
	(uint32)&interrupt,	/* 0xffff0035 */
	(uint32)&interrupt,	/* 0xffff0036 */
	(uint32)&interrupt,	/* 0xffff0037 */
	(uint32)&interrupt,	/* 0xffff0038 */
	(uint32)&interrupt,	/* 0xffff0039 */
	(uint32)&interrupt,	/* 0xffff003a */
	(uint32)&interrupt,	/* 0xffff003b */
	(uint32)&interrupt,	/* 0xffff003c */
	(uint32)&interrupt,	/* 0xffff003d */
	(uint32)&interrupt,	/* 0xffff003e */
	(uint32)&interrupt,	/* 0xffff003f */
	(uint32)&interrupt,	/* 0xffff0040 */
	(uint32)&interrupt,	/* 0xffff0041 */
	(uint32)&interrupt,	/* 0xffff0042 */
	(uint32)&interrupt,	/* 0xffff0043 */
	(uint32)&interrupt,	/* 0xffff0044 */
	(uint32)&interrupt,	/* 0xffff0045 */
	(uint32)&interrupt,	/* 0xffff0046 */
	(uint32)&interrupt,	/* 0xffff0047 */
	(uint32)&interrupt,	/* 0xffff0048 */
	(uint32)&interrupt,	/* 0xffff0049 */
	(uint32)&interrupt,	/* 0xffff004a */
	(uint32)&interrupt,	/* 0xffff004b */
	(uint32)&interrupt,	/* 0xffff004c */
	(uint32)&interrupt,	/* 0xffff004d */
	(uint32)&interrupt,	/* 0xffff004e */
	(uint32)&interrupt,	/* 0xffff004f */
	(uint32)&interrupt,	/* 0xffff0050 */
	(uint32)&interrupt,	/* 0xffff0051 */
	(uint32)&interrupt,	/* 0xffff0052 */
	(uint32)&interrupt,	/* 0xffff0053 */
	(uint32)&interrupt,	/* 0xffff0054 */
	(uint32)&interrupt,	/* 0xffff0055 */
	(uint32)&interrupt,	/* 0xffff0056 */
	(uint32)&interrupt,	/* 0xffff0057 */
	(uint32)&interrupt,	/* 0xffff0058 */
	(uint32)&interrupt,	/* 0xffff0059 */
	(uint32)&interrupt,	/* 0xffff005a */
	(uint32)&interrupt,	/* 0xffff005b */
	(uint32)&interrupt,	/* 0xffff005c */
	(uint32)&interrupt,	/* 0xffff005d */
	(uint32)&interrupt,	/* 0xffff005e */
	(uint32)&interrupt,	/* 0xffff005f */
	(uint32)&interrupt,	/* 0xffff0060 */
	(uint32)&interrupt,	/* 0xffff0061 */
	(uint32)&interrupt,	/* 0xffff0062 */
	(uint32)&interrupt,	/* 0xffff0063 */
	(uint32)&interrupt,	/* 0xffff0064 */
	(uint32)&interrupt,	/* 0xffff0065 */
	(uint32)&interrupt,	/* 0xffff0066 */
	(uint32)&interrupt,	/* 0xffff0067 */
	(uint32)&interrupt,	/* 0xffff0068 */
	(uint32)&interrupt,	/* 0xffff0069 */
	(uint32)&interrupt,	/* 0xffff006a */
	(uint32)&interrupt,	/* 0xffff006b */
	(uint32)&interrupt,	/* 0xffff006c */
	(uint32)&interrupt,	/* 0xffff006d */
	(uint32)&interrupt,	/* 0xffff006e */
	(uint32)&interrupt,	/* 0xffff006f */
	(uint32)&interrupt,	/* 0xffff0070 */
	(uint32)&interrupt,	/* 0xffff0071 */
	(uint32)&interrupt,	/* 0xffff0072 */
	(uint32)&interrupt,	/* 0xffff0073 */
	(uint32)&interrupt,	/* 0xffff0074 */
	(uint32)&interrupt,	/* 0xffff0075 */
	(uint32)&interrupt,	/* 0xffff0076 */
	(uint32)&interrupt,	/* 0xffff0077 */
	(uint32)&interrupt,	/* 0xffff0078 */
	(uint32)&interrupt,	/* 0xffff0079 */
	(uint32)&interrupt,	/* 0xffff007a */
	(uint32)&interrupt,	/* 0xffff007b */
	(uint32)&interrupt,	/* 0xffff007c */
	(uint32)&interrupt,	/* 0xffff007d */
	(uint32)&interrupt,	/* 0xffff007e */
	(uint32)&interrupt,	/* 0xffff007f */
	(uint32)&interrupt,	/* 0xffff0080 */
	(uint32)&interrupt,	/* 0xffff0081 */
	(uint32)&interrupt,	/* 0xffff0082 */
	(uint32)&interrupt,	/* 0xffff0083 */
	(uint32)&interrupt,	/* 0xffff0084 */
	(uint32)&interrupt,	/* 0xffff0085 */
	(uint32)&interrupt,	/* 0xffff0086 */
	(uint32)&interrupt,	/* 0xffff0087 */
	(uint32)&interrupt,	/* 0xffff0088 */
	(uint32)&interrupt,	/* 0xffff0089 */
	(uint32)&interrupt,	/* 0xffff008a */
	(uint32)&interrupt,	/* 0xffff008b */
	(uint32)&interrupt,	/* 0xffff008c */
	(uint32)&interrupt,	/* 0xffff008d */
	(uint32)&interrupt,	/* 0xffff008e */
	(uint32)&interrupt,	/* 0xffff008f */
	(uint32)&interrupt,	/* 0xffff0090 */
	(uint32)&interrupt,	/* 0xffff0091 */
	(uint32)&interrupt,	/* 0xffff0092 */
	(uint32)&interrupt,	/* 0xffff0093 */
	(uint32)&interrupt,	/* 0xffff0094 */
	(uint32)&interrupt,	/* 0xffff0095 */
	(uint32)&interrupt,	/* 0xffff0096 */
	(uint32)&interrupt,	/* 0xffff0097 */
	(uint32)&interrupt,	/* 0xffff0098 */
	(uint32)&interrupt,	/* 0xffff0099 */
	(uint32)&interrupt,	/* 0xffff009a */
	(uint32)&interrupt,	/* 0xffff009b */
	(uint32)&interrupt,	/* 0xffff009c */
	(uint32)&interrupt,	/* 0xffff009d */
	(uint32)&interrupt,	/* 0xffff009e */
	(uint32)&interrupt,	/* 0xffff009f */
	(uint32)&interrupt,	/* 0xffff00a0 */
	(uint32)&interrupt,	/* 0xffff00a1 */
	(uint32)&interrupt,	/* 0xffff00a2 */
	(uint32)&interrupt,	/* 0xffff00a3 */
	(uint32)&interrupt,	/* 0xffff00a4 */
	(uint32)&interrupt,	/* 0xffff00a5 */
	(uint32)&interrupt,	/* 0xffff00a6 */
	(uint32)&interrupt,	/* 0xffff00a7 */
	(uint32)&interrupt,	/* 0xffff00a8 */
	(uint32)&interrupt,	/* 0xffff00a9 */
	(uint32)&interrupt,	/* 0xffff00aa */
	(uint32)&interrupt,	/* 0xffff00ab */
	(uint32)&interrupt,	/* 0xffff00ac */
	(uint32)&interrupt,	/* 0xffff00ad */
	(uint32)&interrupt,	/* 0xffff00ae */
	(uint32)&interrupt,	/* 0xffff00af */
	(uint32)&interrupt,	/* 0xffff00b0 */
	(uint32)&interrupt,	/* 0xffff00b1 */
	(uint32)&interrupt,	/* 0xffff00b2 */
	(uint32)&interrupt,	/* 0xffff00b3 */
	(uint32)&interrupt,	/* 0xffff00b4 */
	(uint32)&interrupt,	/* 0xffff00b5 */
	(uint32)&interrupt,	/* 0xffff00b6 */
	(uint32)&interrupt,	/* 0xffff00b7 */
	(uint32)&interrupt,	/* 0xffff00b8 */
	(uint32)&interrupt,	/* 0xffff00b9 */
	(uint32)&interrupt,	/* 0xffff00ba */
	(uint32)&interrupt,	/* 0xffff00bb */
	(uint32)&interrupt,	/* 0xffff00bc */
	(uint32)&interrupt,	/* 0xffff00bd */
	(uint32)&interrupt,	/* 0xffff00be */
	(uint32)&interrupt,	/* 0xffff00bf */
	(uint32)&interrupt,	/* 0xffff00c0 */
	(uint32)&interrupt,	/* 0xffff00c1 */
	(uint32)&interrupt,	/* 0xffff00c2 */
	(uint32)&interrupt,	/* 0xffff00c3 */
	(uint32)&interrupt,	/* 0xffff00c4 */
	(uint32)&interrupt,	/* 0xffff00c5 */
	(uint32)&interrupt,	/* 0xffff00c6 */
	(uint32)&interrupt,	/* 0xffff00c7 */
	(uint32)&interrupt,	/* 0xffff00c8 */
	(uint32)&interrupt,	/* 0xffff00c9 */
	(uint32)&interrupt,	/* 0xffff00ca */
	(uint32)&interrupt,	/* 0xffff00cb */
	(uint32)&interrupt,	/* 0xffff00cc */
	(uint32)&interrupt,	/* 0xffff00cd */
	(uint32)&interrupt,	/* 0xffff00ce */
	(uint32)&interrupt,	/* 0xffff00cf */
	(uint32)&interrupt,	/* 0xffff00d0 */
	(uint32)&interrupt,	/* 0xffff00d1 */
	(uint32)&interrupt,	/* 0xffff00d2 */
	(uint32)&interrupt,	/* 0xffff00d3 */
	(uint32)&interrupt,	/* 0xffff00d4 */
	(uint32)&interrupt,	/* 0xffff00d5 */
	(uint32)&interrupt,	/* 0xffff00d6 */
	(uint32)&interrupt,	/* 0xffff00d7 */
	(uint32)&interrupt,	/* 0xffff00d8 */
	(uint32)&interrupt,	/* 0xffff00d9 */
	(uint32)&interrupt,	/* 0xffff00da */
	(uint32)&interrupt,	/* 0xffff00db */
	(uint32)&interrupt,	/* 0xffff00dc */
	(uint32)&interrupt,	/* 0xffff00dd */
	(uint32)&interrupt,	/* 0xffff00de */
	(uint32)&interrupt,	/* 0xffff00df */
	(uint32)&interrupt,	/* 0xffff00e0 */
	(uint32)&interrupt,	/* 0xffff00e1 */
	(uint32)&interrupt,	/* 0xffff00e2 */
	(uint32)&interrupt,	/* 0xffff00e3 */
	(uint32)&interrupt,	/* 0xffff00e4 */
	(uint32)&interrupt,	/* 0xffff00e5 */
	(uint32)&interrupt,	/* 0xffff00e6 */
	(uint32)&interrupt,	/* 0xffff00e7 */
	(uint32)&interrupt,	/* 0xffff00e8 */
	(uint32)&interrupt,	/* 0xffff00e9 */
	(uint32)&interrupt,	/* 0xffff00ea */
	(uint32)&interrupt,	/* 0xffff00eb */
	(uint32)&interrupt,	/* 0xffff00ec */
	(uint32)&interrupt,	/* 0xffff00ed */
	(uint32)&interrupt,	/* 0xffff00ee */
	(uint32)&interrupt,	/* 0xffff00ef */
	(uint32)&interrupt,	/* 0xffff00f0 */
	(uint32)&interrupt,	/* 0xffff00f1 */
	(uint32)&interrupt,	/* 0xffff00f2 */
	(uint32)&interrupt,	/* 0xffff00f3 */
	(uint32)&interrupt,	/* 0xffff00f4 */
	(uint32)&interrupt,	/* 0xffff00f5 */
	(uint32)&interrupt,	/* 0xffff00f6 */
	(uint32)&interrupt,	/* 0xffff00f7 */
	(uint32)&interrupt,	/* 0xffff00f8 */
	(uint32)&interrupt,	/* 0xffff00f9 */
	(uint32)&interrupt,	/* 0xffff00fa */
	(uint32)&interrupt,	/* 0xffff00fb */
	(uint32)&interrupt,	/* 0xffff00fc */
	(uint32)&interrupt,	/* 0xffff00fd */
	(uint32)&interrupt,	/* 0xffff00fe */
	(uint32)&interrupt,	/* 0xffff00ff */
	(uint32)&interrupt,	/* 0xffff0100 */
	(uint32)&interrupt,	/* 0xffff0101 */
	(uint32)&interrupt,	/* 0xffff0102 */
	(uint32)&interrupt,	/* 0xffff0103 */
	(uint32)&interrupt,	/* 0xffff0104 */
	(uint32)&interrupt,	/* 0xffff0105 */
	(uint32)&interrupt,	/* 0xffff0106 */
	(uint32)&interrupt,	/* 0xffff0107 */
	(uint32)&interrupt,	/* 0xffff0108 */
	(uint32)&interrupt,	/* 0xffff0109 */
	(uint32)&interrupt,	/* 0xffff010a */
	(uint32)&interrupt,	/* 0xffff010b */
	(uint32)&interrupt,	/* 0xffff010c */
	(uint32)&interrupt,	/* 0xffff010d */
	(uint32)&interrupt,	/* 0xffff010e */
	(uint32)&interrupt,	/* 0xffff010f */
	(uint32)&interrupt,	/* 0xffff0110 */
	(uint32)&interrupt,	/* 0xffff0111 */
	(uint32)&interrupt,	/* 0xffff0112 */
	(uint32)&interrupt,	/* 0xffff0113 */
	(uint32)&interrupt,	/* 0xffff0114 */
	(uint32)&interrupt,	/* 0xffff0115 */
	(uint32)&interrupt,	/* 0xffff0116 */
	(uint32)&interrupt,	/* 0xffff0117 */
	(uint32)&interrupt,	/* 0xffff0118 */
	(uint32)&interrupt,	/* 0xffff0119 */
	(uint32)&interrupt,	/* 0xffff011a */
	(uint32)&interrupt,	/* 0xffff011b */
	(uint32)&interrupt,	/* 0xffff011c */
	(uint32)&interrupt,	/* 0xffff011d */
	(uint32)&interrupt,	/* 0xffff011e */
	(uint32)&interrupt,	/* 0xffff011f */
	(uint32)&interrupt,	/* 0xffff0120 */
	(uint32)&interrupt,	/* 0xffff0121 */
	(uint32)&interrupt,	/* 0xffff0122 */
	(uint32)&interrupt,	/* 0xffff0123 */
	(uint32)&interrupt,	/* 0xffff0124 */
	(uint32)&interrupt,	/* 0xffff0125 */
	(uint32)&interrupt,	/* 0xffff0126 */
	(uint32)&interrupt,	/* 0xffff0127 */
	(uint32)&interrupt,	/* 0xffff0128 */
	(uint32)&interrupt,	/* 0xffff0129 */
	(uint32)&interrupt,	/* 0xffff012a */
	(uint32)&interrupt,	/* 0xffff012b */
	(uint32)&interrupt,	/* 0xffff012c */
	(uint32)&interrupt,	/* 0xffff012d */
	(uint32)&interrupt,	/* 0xffff012e */
	(uint32)&interrupt,	/* 0xffff012f */
	(uint32)&interrupt,	/* 0xffff0130 */
	(uint32)&interrupt,	/* 0xffff0131 */
	(uint32)&interrupt,	/* 0xffff0132 */
	(uint32)&interrupt,	/* 0xffff0133 */
	(uint32)&interrupt,	/* 0xffff0134 */
	(uint32)&interrupt,	/* 0xffff0135 */
	(uint32)&interrupt,	/* 0xffff0136 */
	(uint32)&interrupt,	/* 0xffff0137 */
	(uint32)&interrupt,	/* 0xffff0138 */
	(uint32)&interrupt,	/* 0xffff0139 */
	(uint32)&interrupt,	/* 0xffff013a */
	(uint32)&interrupt,	/* 0xffff013b */
	(uint32)&interrupt,	/* 0xffff013c */
	(uint32)&interrupt,	/* 0xffff013d */
	(uint32)&interrupt,	/* 0xffff013e */
	(uint32)&interrupt,	/* 0xffff013f */
	(uint32)&interrupt,	/* 0xffff0140 */
	(uint32)&interrupt,	/* 0xffff0141 */
	(uint32)&interrupt,	/* 0xffff0142 */
	(uint32)&interrupt,	/* 0xffff0143 */
	(uint32)&interrupt,	/* 0xffff0144 */
	(uint32)&interrupt,	/* 0xffff0145 */
	(uint32)&interrupt,	/* 0xffff0146 */
	(uint32)&interrupt,	/* 0xffff0147 */
	(uint32)&interrupt,	/* 0xffff0148 */
	(uint32)&interrupt,	/* 0xffff0149 */
	(uint32)&interrupt,	/* 0xffff014a */
	(uint32)&interrupt,	/* 0xffff014b */
	(uint32)&interrupt,	/* 0xffff014c */
	(uint32)&interrupt,	/* 0xffff014d */
	(uint32)&interrupt,	/* 0xffff014e */
	(uint32)&interrupt,	/* 0xffff014f */
	(uint32)&interrupt,	/* 0xffff0150 */
	(uint32)&interrupt,	/* 0xffff0151 */
	(uint32)&interrupt,	/* 0xffff0152 */
	(uint32)&interrupt,	/* 0xffff0153 */
	(uint32)&interrupt,	/* 0xffff0154 */
	(uint32)&interrupt,	/* 0xffff0155 */
	(uint32)&interrupt,	/* 0xffff0156 */
	(uint32)&interrupt,	/* 0xffff0157 */
	(uint32)&interrupt,	/* 0xffff0158 */
	(uint32)&interrupt,	/* 0xffff0159 */
	(uint32)&interrupt,	/* 0xffff015a */
	(uint32)&interrupt,	/* 0xffff015b */
	(uint32)&interrupt,	/* 0xffff015c */
	(uint32)&interrupt,	/* 0xffff015d */
	(uint32)&interrupt	/* 0xffff015e */
};

const uint32 intbp_table[TotalNumberOfCores] = {
	(const uint32) core0_intbp_tbl,
	(const uint32) core1_intbp_tbl
};
