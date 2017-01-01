/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>> CrHack 线程操作函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "mtplib.h"

/* WinCE 无此部分 LIBC */
#if defined(_CR_OS_WINCE_)
    #define _CR_NO_STDC_
#endif

#if !defined(_CR_NO_STDC_)
    #include <stdlib.h>
    #include <process.h>
#endif

#include <windows.h>

/* [port] 使用指令级函数来优化代码 */
#if defined(_CR_CC_MSC_) && !defined(_CR_NO_INTRIN_) && \
   (defined(_CR_AR_X86_) || defined(_CR_AR_X64_) || defined(_CR_AR_IA64_))
    #include <intrin.h>

    #define InterlockedExchange         _InterlockedExchange
    #define InterlockedIncrement        _InterlockedIncrement
    #define InterlockedDecrement        _InterlockedDecrement
    #define InterlockedExchangeAdd      _InterlockedExchangeAdd
    #define InterlockedExchangeAdd64    _InterlockedExchangeAdd64
#endif

/* 优先级常数的查表 */
static const int s_priority[] =
{
#ifndef _CR_OS_WINCE_
    THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST,
    THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST,
    THREAD_PRIORITY_TIME_CRITICAL,
#else
    THREAD_PRIORITY_IDLE, THREAD_PRIORITY_ABOVE_IDLE,
    THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL,
    THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL,
    THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL,
#endif
};

/*****************************************************************************/
/*                                 线程控制                                  */
/*****************************************************************************/

/*
=======================================
    返回线程普通优先级
=======================================
*/
CR_API sint_t
thread_prrt_nrm (void_t)
{
#ifndef _CR_OS_WINCE_
    return ((sint_t)3);
#else
    return ((sint_t)4);
#endif
}

/*
=======================================
    返回线程最小优先级
=======================================
*/
CR_API sint_t
thread_prrt_min (void_t)
{
    return ((sint_t)0);
}

/*
=======================================
    返回线程最大优先级
=======================================
*/
CR_API sint_t
thread_prrt_max (void_t)
{
    return ((sint_t)cntsof(s_priority) - 1);
}

/*
=======================================
    创建一个新线程
=======================================
*/
CR_API thrd_t
thread_new (
  __CR_IN__ uint_t      stack_size,
  __CR_IN__ mt_main_t   main,
  __CR_IN__ void_t*     param,
  __CR_IN__ bool_t      suspended,
  __CR_IN__ sint_t      priority,
  __CR_OT__ size_t*     addition
    )
{
    thrd_t  thread;
    uint_t  thrd_id;
    uint_t  initflag;

    initflag = (!suspended) ? 0 : CREATE_SUSPENDED;
    if (priority < CR_PRRT_NRM ||
        priority > thread_prrt_max())
        return (NULL);
#ifndef _CR_NO_STDC_    /* Win9x 下必须要返回线程 ID */
    thread = (thrd_t)_beginthreadex(NULL, stack_size, main,
                                    param, initflag, &thrd_id);
    if (thread == (thrd_t)CR_BTEX_FAIL)
        return (NULL);
#else
    thread = (thrd_t)CreateThread(NULL, stack_size,
          (LPTHREAD_START_ROUTINE)main, param, initflag,
                         (LPDWORD)(&thrd_id));
    if (thread == NULL)
        return (NULL);
#endif
    /* 优先级设置 (容错执行) */
    if (priority != CR_PRRT_NRM) {
        priority = s_priority[priority];
        SetThreadPriority((HANDLE)thread, priority);
    }
    if (addition != NULL)
        *addition = thrd_id;
    return (thread);
}

/*
=======================================
    释放线程的句柄 (不是结束线程)
=======================================
*/
CR_API void_t
thread_del (
  __CR_IN__ thrd_t  thread
    )
{
    CloseHandle((HANDLE)thread);
}

/*
=======================================
    线程暂停
=======================================
*/
CR_API bool_t
thread_stop (
  __CR_IN__ thrd_t  thread
    )
{
    if (SuspendThread((HANDLE)thread) == (DWORD)-1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    线程继续
=======================================
*/
CR_API bool_t
thread_goon (
  __CR_IN__ thrd_t  thread
    )
{
    if (ResumeThread((HANDLE)thread) == (DWORD)-1)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    线程醒来
=======================================
*/
CR_API void_t
thread_wake (
  __CR_IN__ thrd_t  thread
    )
{
    DWORD   retc;

    /* 确保其从阻塞中醒来 */
    for (;;) {
        retc = ResumeThread((HANDLE)thread);
        if (retc == 1)
            break;
        if (retc == 0)
            Sleep(0);
    }
}

/*
=======================================
    等待线程结束
=======================================
*/
CR_API bool_t
thread_wait (
  __CR_IN__ thrd_t  thread,
  __CR_IN__ uint_t  time_ms
    )
{
    if (WaitForSingleObject((HANDLE)thread, time_ms) == WAIT_FAILED)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    线程休眠一段时间
=======================================
*/
CR_API void_t
thread_sleep (
  __CR_IN__ uint_t  time_ms
    )
{
    Sleep(time_ms);
}

/*****************************************************************************/
/*                                 原子运算                                  */
/*****************************************************************************/

/*
=======================================
    原子加一
=======================================
*/
CR_API void_t
atom_inc (
  __CR_IO__ sint_t volatile*    dst
    )
{
#if defined(_CR_CC_GCC_)
    __sync_fetch_and_add(dst, 1);
#else
    InterlockedIncrement((LONG*)dst);
#endif
}

/*
=======================================
    原子减一
=======================================
*/
CR_API void_t
atom_dec (
  __CR_IO__ sint_t volatile*    dst
    )
{
#if defined(_CR_CC_GCC_)
    __sync_fetch_and_sub(dst, 1);
#else
    InterlockedDecrement((LONG*)dst);
#endif
}

/*
=======================================
    原子加法
=======================================
*/
CR_API void_t
atom_add (
  __CR_IO__ dist_t volatile*    dst,
  __CR_IN__ dist_t              src
    )
{
#if defined(_CR_CC_GCC_)
    __sync_fetch_and_add(dst, src);
#else
    #if defined(_CR_SYS32_)
    InterlockedExchangeAdd((LONG*)dst, src);
    #else
    InterlockedExchangeAdd64((LONGLONG*)dst, src);
    #endif
#endif
}

/*
=======================================
    原子减法
=======================================
*/
CR_API void_t
atom_sub (
  __CR_IO__ dist_t volatile*    dst,
  __CR_IN__ dist_t              src
    )
{
#if defined(_CR_CC_GCC_)
    __sync_fetch_and_sub(dst, src);
#else
    #if defined(_CR_SYS32_)
    InterlockedExchangeAdd((LONG*)dst, -src);
    #else
    InterlockedExchangeAdd64((LONGLONG*)dst, -src);
    #endif
#endif
}

/*
=======================================
    原子设值
=======================================
*/
CR_API void_t
atom_set (
  __CR_IO__ sint_t volatile*    dst,
  __CR_IN__ sint_t              val
    )
{
#if defined(_CR_CC_GCC_)
    __sync_lock_test_and_set(dst, val);
#else
    InterlockedExchange((LONG*)dst, val);
#endif
}

/*****************************************************************************/
/*                                  临界区                                   */
/*****************************************************************************/

/*
=======================================
    创建临界区对象
=======================================
*/
CR_API crsc_t
crisec_new (void_t)
{
    LPCRITICAL_SECTION  crisec;

    crisec = struct_new(CRITICAL_SECTION);
    if (crisec == NULL)
        return (NULL);
    InitializeCriticalSection(crisec);
    return ((crsc_t)crisec);
}

/*
=======================================
    释放临界区对象
=======================================
*/
CR_API void_t
crisec_del (
  __CR_IN__ crsc_t  crisec
    )
{
    DeleteCriticalSection((LPCRITICAL_SECTION)crisec);
    mem_free(crisec);
}

/*
=======================================
    进入临界区
=======================================
*/
CR_API void_t
crisec_enter (
  __CR_IN__ crsc_t  crisec
    )
{
    EnterCriticalSection((LPCRITICAL_SECTION)crisec);
}

/*
=======================================
    退出临界区
=======================================
*/
CR_API void_t
crisec_leave (
  __CR_IN__ crsc_t  crisec
    )
{
    LeaveCriticalSection((LPCRITICAL_SECTION)crisec);
}

/*****************************************************************************/
/*                                 多线程锁                                  */
/*****************************************************************************/

/*
=======================================
    生成多线程锁
=======================================
*/
CR_API void_t
mtlock_init (
  __CR_OT__ lock_t volatile*    lock
    )
{
    *lock = (lock_t)(FALSE);
}

/*
=======================================
    释放多线程锁
=======================================
*/
CR_API void_t
mtlock_free (
  __CR_IN__ lock_t volatile*    lock
    )
{
    *lock = (lock_t)(FALSE);
}

/*
=======================================
    多线程加锁
=======================================
*/
CR_API void_t
mtlock_acquire (
  __CR_IO__ lock_t volatile*    lock
    )
{
#if defined(_CR_CC_GCC_)
    while (__sync_lock_test_and_set(lock, TRUE))
#else
    while (InterlockedExchange((LONG*)lock, TRUE))
#endif
        Sleep(0);
}

/*
=======================================
    多线程解锁
=======================================
*/
CR_API void_t
mtlock_release (
  __CR_IO__ lock_t volatile*    lock
    )
{
#if defined(_CR_CC_GCC_)
    __sync_lock_release(lock);
#else
    InterlockedExchange((LONG*)lock, FALSE);
#endif
}

/*
=======================================
    生成自旋转锁
=======================================
*/
CR_API void_t
splock_init (
  __CR_OT__ lock_t volatile*    lock
    )
{
    *lock = (lock_t)(FALSE);
}

/*
=======================================
    释放自旋转锁
=======================================
*/
CR_API void_t
splock_free (
  __CR_IN__ lock_t volatile*    lock
    )
{
    *lock = (lock_t)(FALSE);
}

/*
=======================================
    自旋转加锁
=======================================
*/
CR_API void_t
splock_acquire (
  __CR_IO__ lock_t volatile*    lock
    )
{
#if defined(_CR_CC_GCC_)
    while (__sync_lock_test_and_set(lock, TRUE));
#else
    while (InterlockedExchange((LONG*)lock, TRUE));
#endif
}

/*
=======================================
    自旋转解锁
=======================================
*/
CR_API void_t
splock_release (
  __CR_IO__ lock_t volatile*    lock
    )
{
#if defined(_CR_CC_GCC_)
    __sync_lock_release(lock);
#else
    InterlockedExchange((LONG*)lock, FALSE);
#endif
}

/*****************************************************************************/
/*                                 事件信号                                  */
/*****************************************************************************/

/* 事件内部结构 */
typedef struct
{
        HANDLE  evts[2];    /* 第二个需要手动复位 */

} sEVENT2;

/*
=======================================
    创建事件信号对象
=======================================
*/
CR_API evts_t
event_new (void_t)
{
    sEVENT2*    rett;

    rett = struct_new(sEVENT2);
    if (rett == NULL)
        return (NULL);
    rett->evts[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (rett->evts[0] == NULL)
        goto _failure;
    rett->evts[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (rett->evts[1] == NULL) {
        CloseHandle(rett->evts[0]);
        goto _failure;
    }
    return ((evts_t)rett);

_failure:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    释放事件信号对象
=======================================
*/
CR_API void_t
event_del (
  __CR_IN__ evts_t  event
    )
{
    sEVENT2*    real;

    real = (sEVENT2*)event;
    CloseHandle(real->evts[1]);
    CloseHandle(real->evts[0]);
    mem_free(event);
}

/*
=======================================
    等待事件信号发生
=======================================
*/
CR_API bool_t
event_wait (
  __CR_IN__ evts_t  event
    )
{
    sEVENT2*    real;

    real = (sEVENT2*)event;
    if (WaitForMultipleObjects(2, real->evts,
            FALSE, INFINITE) == WAIT_FAILED)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    发送事件信号
=======================================
*/
CR_API bool_t
event_fire (
  __CR_IN__ evts_t  event,
  __CR_IN__ bool_t  broadcast
    )
{
    uint_t      idx;
    sEVENT2*    real;

    real = (sEVENT2*)event;
    idx = broadcast ? 1 : 0;
    if (!SetEvent(real->evts[idx]))
        return (FALSE);
    return (TRUE);
}

/*****************************************************************************/
/*                                 系统杂项                                  */
/*****************************************************************************/

#if !defined(_CR_NO_STDC_)
/*
=======================================
    调用 Shell 命令
=======================================
*/
CR_API bool_t
shell_cmd (
  __CR_IN__ const ansi_t*   cmd,
  __CR_OT__ sint_t*         retc
    )
{
    sint_t  status;

    if (cmd == NULL)
        return (FALSE);
    status = system(cmd);
    if (retc != NULL)
        *retc = status;
    return (TRUE);
}

#endif  /* !_CR_NO_STDC_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
