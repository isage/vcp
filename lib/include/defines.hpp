#ifndef VITA_COREDUMP_DEFINES_H
#define VITA_COREDUMP_DEFINES_H

namespace vita
{
namespace coredump
{

enum class processAttr {
    PROCESS_ATTR_LOADED      = 0x00000001,
    PROCESS_ATTR_DEBUG_STATE = 0x00000002,
    PROCESS_ATTR_SYSTEM      = 0x00000004,
    PROCESS_ATTR_SUSPENDED   = 0x00000008,
    PROCESS_ATTR_DEBUGGABLE  = 0x00010000,
    PROCESS_ATTR_ATTACHED    = 0x00020000,
    PROCESS_ATTR_FIBER_STATE = 0x01000000
};

enum class stopReasonAttr {
    STOP_NOTIF_REASON_ATTR_READ = 0x00,
    STOP_NOTIF_READON_ATTR_WRITE = 0x01
};

enum class stopReason {
    NOTHING = 0x00000,
    SUSPEND_PROCESS = 0x10001,
    SUSPEND_THREAD = 0x10002,
    APPLICATION_IS_SUSPENDED = 0x10003,
    APPMGR_DETECTED_HUNGUP = 0x10004,
    SPONTANEOUS_EXIT = 0x10005,
    STACK_OVERFLOW = 0x10006,
    SYSCALL_ERROR_ILLEGAL_CONTEXT = 0x10007,
    SYSCALL_ERROR_CRITICAL_USAGE = 0x10008,
    SYSCALL_ERROR_ILLEGAL_NUMBER =  0x10009,
    HARDWARE_WATCHPOINT = 0x20001,
    SOFTWARE_WATCHPOINT  =  0x20002,
    HARDWARE_BRKPT = 0x20003,
    SOFTWARE_BRKPT = 0x20004,
    STARTUP_FAILED = 0x20005,
    PRX_STOP_INIT = 0x20006,
    DTRACE_BRKPT = 0x20007,
    EXCEPTION_UNDEF = 0x30002,
    EXCEPTION_PREFETCH_ABORT = 0x30003,
    EXCEPTION_DATA_ABORT = 0x30004,
    FPU_VFP = 0x40001,
    FPU_NEON = 0x40002,
    INT_DIV0 = 0x60080,
    GPU_EXCEPTION =  0x70000,
    UNRECOVERABLE_ERROR_LOW = 0x80000,
    UNRECOVERABLE_ERROR_HIGH = 0x800FF
};

enum class moduleFlags {
    LOADED_COMMON_DIALOG_BUDGET = 0x0001,
    LOADED_DEVTOOL_BUDGET = 0x0002,
    SHARED_TEXT_MODULE = 0x0100,
    SHAREABLE_TEXT_MODULE = 0x0400,
    SYSTEM_MODULE = 0x1000,
    PROCESS_MAIN_MODULE = 0x4000,
    RELOCATABLE_MODULE = 0x8000
};

enum class moduleStatus {
    LOADED = 0x00,
    STARTING = 0x01,
    LIVE = 0x02,
    STOPPING = 0x03,
    STOPPED = 0x04
};

enum class moduleType {
    FIXED_ELF = 0x10,
    PRX = 0x20
};

enum class PRXSegmentAttr {
    READABLE = 0x4,
    WRITABLE = 0x2,
    EXECUTABLE = 0x1
};

enum class memblockType : uint32_t {
    SCE_KERNEL_MEMBLOCK_TYPE_FREE                           = 0x00000000U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_MAIN_RW            = 0x0320d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_MAIN_R             = 0x0320d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_MAIN_RX            = 0x0320d005U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_SHARED_R           = 0x0390d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_SHARED_RX          = 0x0390d005U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_SHARED_RW          = 0x0390d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_CDIALOG_R          = 0x03a0d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_CDIALOG_RX         = 0x03a0d005U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_CDIALOG_RW         = 0x03a0d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_TOOL_R             = 0x03f0d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_TOOL_RX            = 0x03f0d005U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_SHARED_TOOL_RW            = 0x03f0d006U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM2_RW                 = 0x05408060U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_KTMP_RW                   = 0x0620d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_KTMP_R                    = 0x0620d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_KTMP_NC_R                 = 0x06208004U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_KTMP_NC_RW                = 0x06208006U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_NC_RW               = 0x09408060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_NC_R                = 0x09408040U,
    SCE_KERNEL_MEMBLOCK_CODE_USER_CDRAM_L1WBWA_RW           = 0x09404060U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_IO_DEVICE_RW              = 0x0b100860U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_IO_SO_RW                  = 0x0b100260U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_DEVICE_RW            = 0x0c200860U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_NC_RW                = 0x0c208060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_R                    = 0x0c20d040U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_RX                   = 0x0c20d050U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_RW                   = 0x0c20d060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_R            = 0x0c80d040U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_RW           = 0x0c80d060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_GAME_RW              = 0x0c50d060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_CDIALOG_NC_RW        = 0x0ca08060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_CDIALOG_RW           = 0x0ca0d060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_TOOL_RW              = 0x0cf0d060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_TOOL_RX              = 0x0cf0d050U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_TOOL_NC_RW           = 0x0cf08060U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW        = 0x0d808060U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_R         = 0x0d808040U,

    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDIALOG_R                 = 0x0e20d040U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDIALOG_RX                = 0x0e20d050U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDIALOG_RW                = 0x0e20d060U,
    SCE_KERNEL_MEMBLOCK_TYPE_CDIALOG_NC_R                   = 0x0e208040U,
    SCE_KERNEL_MEMBLOCK_TYPE_USER_CDIALOG_NC_RW             = 0x0e208050U,


    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_SO_RW              = 0x10200206U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_DEVICE_RW          = 0x10200806U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_NC_R               = 0x10208004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_NC_RW              = 0x10208006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_R                  = 0x1020d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_RX                 = 0x1020d005U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_RW                 = 0x1020d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_BOOT_IMAGE              = 0x1020d007U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_GAME_RW            = 0x1050d006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_PHYCONT_R          = 0x1080d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_PHYCONT_RW         = 0x1080d006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_UMAIN_RW           = 0x10c0d006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_CDIALOG_RW         = 0x10a0d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_CDIALOG_R          = 0x10a0d004U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TOOL_R                  = 0x10f0d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TOOL_RX                 = 0x10f0d005U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TOOL_RW                 = 0x10f0d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TOOL_NC_R               = 0x10f08004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TOOL_NC_RW              = 0x10f08006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_IO_DEVICE_RW            = 0x20100806U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_IO_DEVICE_R             = 0x20100804U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_IO_SO_R                 = 0x20100204U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_IO_SO_RW                = 0x20100206U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_IO_NC_R                 = 0x20108004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_IO_NC_RW                = 0x20108006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_PHYCONT_NC_R       = 0x30808004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_ROOT_PHYCONT_NC_RW      = 0x30808006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_CDRAM_RW                = 0x40408006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_CDRAM_L1WBWA_RW         = 0x40404006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_CDRAM2_RW               = 0x50408006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TMP_R                   = 0x6020d004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TMP_RW                  = 0x6020d006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TMP_NC_R                = 0x60208004U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TMP_NC_RW               = 0x60208006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_TMP_GAME_RW             = 0x6050d006U,

    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_GPU_CDRAM_RW            = 0xa0408006U,
    SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_KMP_TOOL_RW             = 0xc0f0d006U,
};

enum class fiberState {
    FIBER_STATE_INITALIZED = 0x000,
    FIBER_STATE_RUNNING = 0x001,
    FIBER_STATE_SUSPENDED = 0x002
};

enum class waitType {
    WAITTYPE_NONE = 0x00000000,
    WAITTYPE_DELAY = 0x00000001,
    WAITTYPE_WAITTHEND = 0x00000002, // unknown
    WAITTYPE_SIGNAL = 0x00000004,
    WAITTYPE_WAITTHSUSPEND = 0x00000008, // unknown
    WAITTYPE_EVENTFLAG = 0x00000010,
    WAITTYPE_SEMAPHORE = 0x00000020,
    WAITTYPE_MUTEX = 0x00000040,
    WAITTYPE_RW_LOCK = 0x00000080,
    WAITTYPE_COND_SIGNAL = 0x00000100, // unknown
    WAITTYPE_COND_MUTEX = 0x00000200, // condition
    WAITTYPE_FAST_MUTEX = 0x00001000, // unknown
    WAITTYPE_FAST_MUTEX_SPIN = 0x00002000, // unknown
    WAITTYPE_EVENT = 0x00010000,
    WAITTYPE_MP_EVENTS = 0x00020000,
    WAITTYPE_MSG_PIPE  = 0x00040000,
    WAITTYPE_LW_MUTEX = 0x00100000,
    WAITTYPE_LW_COND_SIGNAL = 0x00200000,  // unknown
    WAITTYPE_LW_COND_LW_MUTEX = 0x00400000,
    WAITTYPE_EXCEPTION_VM = 0x01000000
};

enum class ulthreadState {
    ULTHREAD_STATE_INITIALIZED = 0x000,
    ULTHREAD_STATE_EXECUTION = 0x001,
    ULTHREAD_STATE_EXECUTABLE = 0x002,
    ULTHREAD_STATE_WAIT = 0x003,
    ULTHREAD_STATE_TERMINATED = 0x004,
    ULTHREAD_STATE_DELETED = 0x005
};

enum class threadStatus : uint32_t {
    THREAD_STATUS_RUNNING = 0x001,
    THREAD_STATUS_READY = 0x002,
    THREAD_STATUS_STANDBY = 0x004,
    THREAD_STATUS_WAITING = 0x008,
    THREAD_STATUS_DORMANT = 0x010,
    THREAD_STATUS_DELETED = 0x020,
    THREAD_STATUS_DEAD = 0x040,
    THREAD_STATUS_STAGNANT = 0x080,
    THREAD_STATUS_VM_SUSPENDED = 0x100,
    THREAD_STATUS_INSIDE_SYSCALL = 0x80000000
};

enum class eventLogFacility {
    SYSTEM  = 10001,
    NETWORK = 20000,
    WLANBT  = 20001
};

}; // coredump
}; // vita

#endif