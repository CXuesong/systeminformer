/*
 * Copyright (c) 2022 Winsider Seminars & Solutions, Inc.  All rights reserved.
 *
 * This file is part of System Informer.
 *
 * Authors:
 *
 *     jxy-s   2022
 *
 */

#pragma once
#ifdef _KERNEL_MODE
#define PHNT_MODE PHNT_MODE_KERNEL
#endif
#pragma warning(push)
#pragma warning(disable : 4201)
#include <phnt.h>

#define KPH_PROTECTION_SUPPRESSED 0

// Process

typedef ULONG KPH_PROCESS_STATE;
typedef KPH_PROCESS_STATE* PKPH_PROCESS_STATE;

#define KPH_PROCESS_SECURELY_CREATED                     0x00000001ul
#define KPH_PROCESS_VERIFIED_PROCESS                     0x00000002ul
#define KPH_PROCESS_PROTECTED_PROCESS                    0x00000004ul
#define KPH_PROCESS_NO_UNTRUSTED_IMAGES                  0x00000008ul
#define KPH_PROCESS_HAS_FILE_OBJECT                      0x00000010ul
#define KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS          0x00000020ul
#define KPH_PROCESS_NO_USER_WRITABLE_REFERENCES          0x00000040ul
#define KPH_PROCESS_NO_FILE_TRANSACTION                  0x00000080ul
#define KPH_PROCESS_NOT_BEING_DEBUGGED                   0x00000100ul

#if KPH_PROTECTION_SUPPRESSED

#define KPH_PROCESS_STATE_MAXIMUM (KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_NO_UNTRUSTED_IMAGES |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION)

#define KPH_PROCESS_STATE_HIGH    (KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_NO_UNTRUSTED_IMAGES |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION)

#define KPH_PROCESS_STATE_MEDIUM  (KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION)

#else

#define KPH_PROCESS_STATE_MAXIMUM (KPH_PROCESS_SECURELY_CREATED |\
                                   KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_PROTECTED_PROCESS |\
                                   KPH_PROCESS_NO_UNTRUSTED_IMAGES |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION |\
                                   KPH_PROCESS_NOT_BEING_DEBUGGED)

#define KPH_PROCESS_STATE_HIGH    (KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_PROTECTED_PROCESS |\
                                   KPH_PROCESS_NO_UNTRUSTED_IMAGES |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION |\
                                   KPH_PROCESS_NOT_BEING_DEBUGGED)

#define KPH_PROCESS_STATE_MEDIUM  (KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_PROTECTED_PROCESS |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION)

#endif

#define KPH_PROCESS_STATE_LOW     (KPH_PROCESS_VERIFIED_PROCESS |\
                                   KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION)

#define KPH_PROCESS_STATE_MINIMUM (KPH_PROCESS_HAS_FILE_OBJECT |\
                                   KPH_PROCESS_HAS_SECTION_OBJECT_POINTERS |\
                                   KPH_PROCESS_NO_USER_WRITABLE_REFERENCES |\
                                   KPH_PROCESS_NO_FILE_TRANSACTION)

typedef enum _KPH_PROCESS_INFORMATION_CLASS
{
    KphProcessBasicInformation,      // q: KPH_PROCESS_BASIC_INFORMATION
    KphProcessStateInformation,      // q: KPH_PROCESS_STATE
    KphProcessQuotaLimits,           // s: QUOTA_LIMITS
    KphProcessBasePriority,          // s: KPRIORITY
    KphProcessRaisePriority,         // s: ULONG
    KphProcessPriorityClass,         // s: PROCESS_PRIORITY_CLASS
    KphProcessAffinityMask,          // s: KAFFINITY/GROUP_AFFINITY
    KphProcessPriorityBoost,         // s: ULONG
    KphProcessIoPriority,            // s: IO_PRIORITY_HINT
    KphProcessPagePriority,          // s: PAGE_PRIORITY_INFORMATION
    KphProcessPowerThrottlingState,  // s: POWER_THROTTLING_PROCESS_STATE
    KphProcessPriorityClassEx,       // s: PROCESS_PRIORITY_CLASS_EX

} KPH_PROCESS_INFORMATION_CLASS;

typedef enum _KPH_THREAD_INFORMATION_CLASS
{
    KphThreadPriority,               // s: KPRIORITY
    KphThreadBasePriority,           // s: KPRIORITY
    KphThreadAffinityMask,           // s: KAFFINITY
    KphThreadIdealProcessor,         // s: ULONG
    KphThreadPriorityBoost,          // s: ULONG
    KphThreadIoPriority,             // s: IO_PRIORITY_HINT
    KphThreadPagePriority,           // s: PAGE_PRIORITY_INFORMATION
    KphThreadActualBasePriority,     // s: LONG
    KphThreadGroupInformation,       // s: GROUP_AFFINITY
    KphThreadIdealProcessorEx,       // s: PROCESSOR_NUMBER
    KphThreadActualGroupAffinity,    // s: GROUP_AFFINITY
    KphThreadPowerThrottlingState,   // s: POWER_THROTTLING_THREAD_STATE

} KPH_THREAD_INFORMATION_CLASS;

typedef struct _KPH_PROCESS_BASIC_INFORMATION
{
    KPH_PROCESS_STATE ProcessState;

    CLIENT_ID CreatorClientId;

    ULONG UserWritableReferences;

    SIZE_T NumberOfImageLoads;

    union
    {
        ULONG Flags;
        struct
        {
            ULONG CreateNotification : 1;
            ULONG ExitNotification : 1;
            ULONG VerifiedProcess : 1;
            ULONG SecurelyCreated : 1;
            ULONG Protected : 1;
            ULONG Reserved : 27;
        };
    };

    SIZE_T NumberOfThreads;

    //
    // Only valid if Protected flag is set.
    //
    ACCESS_MASK ProcessAllowedMask;
    ACCESS_MASK ThreadAllowedMask;

    //
    // These are only tracked for verified processes.
    //
    SIZE_T NumberOfMicrosoftImageLoads;
    SIZE_T NumberOfAntimalwareImageLoads;
    SIZE_T NumberOfVerifiedImageLoads;
    SIZE_T NumberOfUntrustedImageLoads;

} KPH_PROCESS_BASIC_INFORMATION, *PKPH_PROCESS_BASIC_INFORMATION;


// Process handle information

typedef struct _KPH_PROCESS_HANDLE
{
    HANDLE Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
    USHORT ObjectTypeIndex;
    USHORT Reserved1;
    ULONG HandleAttributes;
    ULONG Reserved2;
} KPH_PROCESS_HANDLE, *PKPH_PROCESS_HANDLE;

typedef struct _KPH_PROCESS_HANDLE_INFORMATION
{
    ULONG HandleCount;
    KPH_PROCESS_HANDLE Handles[1];
} KPH_PROCESS_HANDLE_INFORMATION, *PKPH_PROCESS_HANDLE_INFORMATION;

// Object information

typedef enum _KPH_OBJECT_INFORMATION_CLASS
{
    KphObjectBasicInformation, // q: OBJECT_BASIC_INFORMATION
    KphObjectNameInformation, // q: OBJECT_NAME_INFORMATION
    KphObjectTypeInformation, // q: OBJECT_TYPE_INFORMATION
    KphObjectHandleFlagInformation, // qs: OBJECT_HANDLE_FLAG_INFORMATION
    KphObjectProcessBasicInformation, // q: PROCESS_BASIC_INFORMATION
    KphObjectThreadBasicInformation, // q: THREAD_BASIC_INFORMATION
    KphObjectEtwRegBasicInformation, // q: ETWREG_BASIC_INFORMATION
    KphObjectFileObjectInformation, // q: KPH_FILE_OBJECT_INFORMATION
    KphObjectFileObjectDriver, // q: KPH_FILE_OBJECT_DRIVER
    MaxKphObjectInfoClass
} KPH_OBJECT_INFORMATION_CLASS;

typedef struct _KPH_FILE_OBJECT_INFORMATION
{
    BOOLEAN LockOperation;
    BOOLEAN DeletePending;
    BOOLEAN ReadAccess;
    BOOLEAN WriteAccess;
    BOOLEAN DeleteAccess;
    BOOLEAN SharedRead;
    BOOLEAN SharedWrite;
    BOOLEAN SharedDelete;
    LARGE_INTEGER CurrentByteOffset;
    ULONG Flags;
    ULONG UserWritableReferences;
    BOOLEAN HasActiveTransaction;
    BOOLEAN IsIgnoringSharing;
} KPH_FILE_OBJECT_INFORMATION, *PKPH_FILE_OBJECT_INFORMATION;

typedef struct _KPH_FILE_OBJECT_DRIVER
{
    HANDLE DriverHandle;
} KPH_FILE_OBJECT_DRIVER, *PKPH_FILE_OBJECT_DRIVER;

// Driver information

typedef enum _DRIVER_INFORMATION_CLASS
{
    DriverBasicInformation,
    DriverNameInformation,
    DriverServiceKeyNameInformation,
    MaxDriverInfoClass
} DRIVER_INFORMATION_CLASS;

typedef struct _DRIVER_BASIC_INFORMATION
{
    ULONG Flags;
    PVOID DriverStart;
    ULONG DriverSize;
} DRIVER_BASIC_INFORMATION, *PDRIVER_BASIC_INFORMATION;

typedef struct _DRIVER_NAME_INFORMATION
{
    UNICODE_STRING DriverName;
} DRIVER_NAME_INFORMATION, *PDRIVER_NAME_INFORMATION;

typedef struct _DRIVER_SERVICE_KEY_NAME_INFORMATION
{
    UNICODE_STRING ServiceKeyName;
} DRIVER_SERVICE_KEY_NAME_INFORMATION, *PDRIVER_SERVICE_KEY_NAME_INFORMATION;

// ETW registration object information

typedef struct _ETWREG_BASIC_INFORMATION
{
    GUID Guid;
    ULONG_PTR SessionId;
} ETWREG_BASIC_INFORMATION, *PETWREG_BASIC_INFORMATION;

// Dynamic Configuration

#define KPH_DYN_CONFIGURATION_VERSION 4

#define KPH_DYN_CI_INVALID ((USHORT)0xffff)
#define KPH_DYN_CI_V1      ((USHORT)0x1)
#define KPH_DYN_CI_V2      ((USHORT)0x2)

typedef struct _KPH_DYN_CONFIGURATION
{
    ULONG Version;
    USHORT MajorVersion;
    USHORT MinorVersion;
    USHORT ServicePackMajor;        // -1 to ignore
    USHORT BuildNumber;             // -1 to ignore
    ULONG ResultingNtVersion;       // PHNT_*

    SHORT EgeGuid;                  // dt nt!_ETW_GUID_ENTRY Guid
    SHORT EpObjectTable;            // dt nt!_EPROCESS ObjectTable
    SHORT EreGuidEntry;             // dt nt!_ETW_REG_ENTRY GuidEntry
    SHORT HtHandleContentionEvent;  // dt nt!_HANDLE_TABLE HandleContentionEvent
    SHORT OtName;                   // dt nt!_OBJECT_ATTRIBUTES ObjectName
    SHORT OtIndex;                  // dt nt!_OBJECT_TYPE Index
    SHORT ObDecodeShift;            // dt nt!_HANDLE_TABLE_ENTRY ObjectPointerBits
    SHORT ObAttributesShift;        // dt nt!_HANDLE_TABLE_ENTRY Attributes
    USHORT CiVersion;               // ci.dll exports version

} KPH_DYN_CONFIGURATION, *PKPH_DYN_CONFIGURATION;

// Verification

#define KPH_PROCESS_READ_ACCESS (STANDARD_RIGHTS_READ | SYNCHRONIZE |  PROCESS_QUERY_INFORMATION | \
    PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ)
#define KPH_THREAD_READ_ACCESS (STANDARD_RIGHTS_READ | SYNCHRONIZE | THREAD_QUERY_INFORMATION | \
    THREAD_QUERY_LIMITED_INFORMATION | THREAD_GET_CONTEXT)
#define KPH_TOKEN_READ_ACCESS TOKEN_READ
#define KPH_JOB_READ_ACCESS (STANDARD_RIGHTS_READ | SYNCHRONIZE | JOB_OBJECT_QUERY)

// Informer

typedef struct _KPH_INFORMER_SETTINGS
{
    union
    {
        volatile ULONGLONG Flags;
        struct
        {
            ULONGLONG ProcessCreate : 1;
            ULONGLONG ProcessExit : 1;
            ULONGLONG ThreadCreate : 1;
            ULONGLONG ThreadExecute : 1;
            ULONGLONG ThreadExit : 1;
            ULONGLONG ImageLoad : 1;
            ULONGLONG DebugPrint : 1;
            ULONGLONG ProcessHandlePreCreate : 1;
            ULONGLONG ProcessHandlePostCreate : 1;
            ULONGLONG ProcessHandlePreDuplicate : 1;
            ULONGLONG ProcessHandlePostDuplicate : 1;
            ULONGLONG ThreadHandlePreCreate : 1;
            ULONGLONG ThreadHandlePostCreate : 1;
            ULONGLONG ThreadHandlePreDuplicate : 1;
            ULONGLONG ThreadHandlePostDuplicate : 1;
            ULONGLONG DesktopHandlePreCreate : 1;
            ULONGLONG DesktopHandlePostCreate : 1;
            ULONGLONG DesktopHandlePreDuplicate : 1;
            ULONGLONG DesktopHandlePostDuplicate : 1;
            ULONGLONG Reserved : 45;
        };
    };

} KPH_INFORMER_SETTINGS, *PKPH_INFORMER_SETTINGS;

#pragma warning(pop)
