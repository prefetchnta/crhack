/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-01-21  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 线程操作函数库 for POSIX <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"
#include "mtplib.h"

#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#if defined(_CR_SYS32_) && \
    defined(_CR_NO_FAST_ATOM_)
    #undef  AO_REQUIRE_CAS
    #include "../ex3rd/atom/atomic_ops.h"
    #define _CR_USE_ATOM_OPS_
#endif

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
    return ((sint_t)0);
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
    return ((sint_t)0);
}

/* 线程内部结构 */
typedef struct
{
        void_t*             parm;
        sint_t              copy;
        sint_t              suspend;
        sint_t              counter;
        mt_main_t           mt_main;
        pthread_t           thrd_id;
        pthread_cond_t      thrd_cd;
        pthread_mutex_t     thrd_mt;

} sTHREAD_INT;

/*
---------------------------------------
    线程函数包装
---------------------------------------
*/
static void_t*
thread_main (
  __CR_IO__ void_t* arg
    )
{
    sTHREAD_INT     self;
    sTHREAD_INT*    real;

    /* 参数复制到本地栈 */
    real = (sTHREAD_INT*)arg;
    struct_cpy(&self, real, sTHREAD_INT);
    atom_inc(&real->copy);

    /* 是否为启动便挂起 */
    if (self.suspend) {
        thread_stop((thrd_t)real);
        atom_dec(&real->suspend);
    }

    /* 调用用户线程入口点 */
    return ((void_t*)((leng_t)self.mt_main(self.parm)));
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
    sTHREAD_INT*    thrd;
    pthread_attr_t  attr;

    /* 创建结构 */
    thrd = struct_new(sTHREAD_INT);
    if (thrd == NULL)
        return (NULL);
    CR_NOUSE(priority);
    thrd->parm = param;
    thrd->copy = FALSE;
    thrd->suspend = suspended ? TRUE : FALSE;
    thrd->counter = 0;
    thrd->mt_main = main;

    /* 设置初始属性 (自释放) */
    if (pthread_attr_init(&attr) != 0)
        goto _failure1;
    if (stack_size != 0) {
        if (pthread_attr_setstacksize(&attr, stack_size) != 0)
            goto _failure2;
    }
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
        goto _failure2;

    /* 创建挂起的信号量 */
    if (pthread_cond_init(&thrd->thrd_cd, NULL) != 0)
        goto _failure2;
    if (pthread_mutex_init(&thrd->thrd_mt, NULL) != 0)
        goto _failure3;

    /* 创建线程 */
    if (pthread_create(&thrd->thrd_id, &attr, thread_main, thrd) != 0)
        goto _failure4;

    /* 返回线程号 */
    if (addition != NULL)
        *addition = (size_t)thrd->thrd_id;
    pthread_attr_destroy(&attr);

    /* 等待参数复制到本地 */
    while (!thrd->copy)
        sched_yield();
    return ((thrd_t)thrd);

_failure4:
    pthread_mutex_destroy(&thrd->thrd_mt);
_failure3:
    pthread_cond_destroy(&thrd->thrd_cd);
_failure2:
    pthread_attr_destroy(&attr);
_failure1:
    mem_free(thrd);
    return (NULL);
}

/*
---------------------------------------
    是否是当前线程
---------------------------------------
*/
static bool_t
thread_is_self (
  __CR_IN__ thrd_t  thread
    )
{
    pthread_t       thrd;
    sTHREAD_INT*    real;

    thrd = pthread_self();
    real = (sTHREAD_INT*)thread;
    if (pthread_equal(thrd, real->thrd_id) == 0)
        return (FALSE);
    return (TRUE);
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
    sTHREAD_INT*    real;

    /* 必须别人释放自己 */
    if (thread_is_self(thread))
        return;
    real = (sTHREAD_INT*)thread;
    while (real->suspend)
        sched_yield();
    pthread_mutex_destroy(&real->thrd_mt);
    pthread_cond_destroy(&real->thrd_cd);
    mem_free(thread);
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
    sTHREAD_INT*    real;

    /* 必须自己挂起自己 */
    if (!thread_is_self(thread))
        return (FALSE);
    real = (sTHREAD_INT*)thread;
    pthread_mutex_lock(&real->thrd_mt);
    real->counter += 1;
    while (real->counter > 0)
        pthread_cond_wait(&real->thrd_cd, &real->thrd_mt);
    pthread_mutex_unlock(&real->thrd_mt);
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
    sTHREAD_INT*    real;

    /* 必须别人恢复自己 */
    if (thread_is_self(thread))
        return (FALSE);
    real = (sTHREAD_INT*)thread;
    pthread_mutex_lock(&real->thrd_mt);
    real->counter -= 1;
    if (real->counter == 0)
        pthread_cond_signal(&real->thrd_cd);
    pthread_mutex_unlock(&real->thrd_mt);
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
    /* 确保其阻塞不能 */
    thread_goon(thread);
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
    uint_t          dltt;
    xtime_t         xtim;
    sTHREAD_INT*    real;

    /* 必须别人等待自己 */
    if (thread_is_self(thread))
        return (FALSE);
    real = (sTHREAD_INT*)thread;
    if (time_ms == CR_INFINITE) {
        for (;;) {
            if (pthread_kill(real->thrd_id, 0) == ESRCH)
                break;
            thread_sleep(1);
        }
    }
    else {
        if (pthread_kill(real->thrd_id, 0) == ESRCH)
            return (TRUE);
        xtim = timer_new();
        if (xtim == NULL)
            return (FALSE);
        timer_set_base(xtim);
        do {
            if (pthread_kill(real->thrd_id, 0) == ESRCH)
                break;
            thread_sleep(1);
            dltt = (sint_t)timer_get_delta(xtim);
        } while (dltt < time_ms);

        timer_del(xtim);
    }
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
    struct timespec req, rem;

    if (time_ms == 0) {
        sched_yield();
    }
    else
    if (time_ms != CR_INFINITE) {
        req.tv_sec  = (time_ms / 1000);
        req.tv_nsec = (time_ms % 1000) * 1000000;
        while (nanosleep(&req, &rem) && errno == EINTR)
            req = rem;
    }
    else {
        for (;;)
            sched_yield();
    }
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
#if defined(_CR_USE_ATOM_OPS_)
    #if defined(AO_HAVE_fetch_and_add1)
    AO_fetch_and_add1((volatile AO_t*)dst);
    #else
    *dst += 1;
    #endif
#else
    __sync_fetch_and_add(dst, 1);
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
#if defined(_CR_USE_ATOM_OPS_)
    #if defined(AO_HAVE_fetch_and_sub1)
    AO_fetch_and_sub1((volatile AO_t*)dst);
    #else
    *dst -= 1;
    #endif
#else
    __sync_fetch_and_sub(dst, 1);
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
#if defined(_CR_USE_ATOM_OPS_)
    #if defined(AO_HAVE_fetch_and_add)
    AO_fetch_and_add((volatile AO_t*)dst, src);
    #else
    *dst += src;
    #endif
#else
    __sync_fetch_and_add(dst, src);
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
#if defined(_CR_USE_ATOM_OPS_)
    #if defined(AO_HAVE_fetch_and_add)
    AO_fetch_and_add((volatile AO_t*)dst, -src);
    #else
    *dst -= src;
    #endif
#else
    __sync_fetch_and_sub(dst, src);
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
#if defined(_CR_USE_ATOM_OPS_)
    #if defined(AO_HAVE_store)
    AO_store((volatile AO_t*)dst, val);
    #else
    *dst = val;
    #endif
#else
    __sync_lock_test_and_set(dst, val);
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
    sint_t              rett;
    pthread_mutex_t*    mutex;
    pthread_mutexattr_t mattr;

    mutex = struct_new(pthread_mutex_t);
    if (mutex == NULL)
        return (NULL);
    if (pthread_mutexattr_init(&mattr) != 0)
        goto _failure;
#if defined(_CR_OS_MACOSX_)
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
#else
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP);
#endif
    rett = pthread_mutex_init(mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);
    if (rett != 0)
        goto _failure;
    return ((crsc_t)mutex);

_failure:
    mem_free(mutex);
    return (NULL);
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
    pthread_mutex_destroy((pthread_mutex_t*)crisec);
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
    pthread_mutex_lock((pthread_mutex_t*)crisec);
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
    pthread_mutex_unlock((pthread_mutex_t*)crisec);
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
#if defined(_CR_USE_ATOM_OPS_)
    while (AO_test_and_set((volatile AO_TS_t*)lock))
#else
    while (__sync_lock_test_and_set(lock, TRUE))
#endif
        sched_yield();
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
#if defined(_CR_USE_ATOM_OPS_) && \
    !defined(_CR_USE_SYNC_LOCK_RELEASE_)
    #if defined(AO_HAVE_store)
    AO_store((volatile AO_t*)lock, FALSE);
    #else
    *lock = FALSE;
    #endif
#else
    __sync_lock_release(lock);
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
#if defined(_CR_USE_ATOM_OPS_)
    while (AO_test_and_set((volatile AO_TS_t*)lock));
#else
    while (__sync_lock_test_and_set(lock, TRUE));
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
#if defined(_CR_USE_ATOM_OPS_) && \
    !defined(_CR_USE_SYNC_LOCK_RELEASE_)
    #if defined(AO_HAVE_store)
    AO_store((volatile AO_t*)lock, FALSE);
    #else
    *lock = FALSE;
    #endif
#else
    __sync_lock_release(lock);
#endif
}

/*****************************************************************************/
/*                                 事件信号                                  */
/*****************************************************************************/

/* 事件内部结构 */
typedef struct
{
        bool_t              evts[2];
        pthread_cond_t      thrd_cd;
        pthread_mutex_t     thrd_mt;

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
    if (pthread_cond_init(&rett->thrd_cd, NULL) != 0)
        goto _failure;
    if (pthread_mutex_init(&rett->thrd_mt, NULL) != 0) {
        pthread_cond_destroy(&rett->thrd_cd);
        goto _failure;
    }
    rett->evts[0] = FALSE;
    rett->evts[1] = FALSE;
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
    pthread_cond_destroy(&real->thrd_cd);
    pthread_mutex_destroy(&real->thrd_mt);
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
    pthread_mutex_lock(&real->thrd_mt);
    while (!real->evts[0] && !real->evts[1])
        pthread_cond_wait(&real->thrd_cd, &real->thrd_mt);
    if (real->evts[0])
        real->evts[0] = FALSE;
    pthread_mutex_unlock(&real->thrd_mt);
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
    sEVENT2*    real;

    real = (sEVENT2*)event;
    if (broadcast) {
        pthread_mutex_lock(&real->thrd_mt);
        real->evts[1] = TRUE;
        pthread_cond_broadcast(&real->thrd_cd);
        pthread_mutex_unlock(&real->thrd_mt);
    }
    else {
        pthread_mutex_lock(&real->thrd_mt);
        real->evts[0] = TRUE;
        pthread_cond_signal(&real->thrd_cd);
        pthread_mutex_unlock(&real->thrd_mt);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                                 系统杂项                                  */
/*****************************************************************************/

#if !defined(_CR_OS_MACOSX_)
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
    pid_t   status;

    if (cmd == NULL)
        return (FALSE);
    status = system(cmd);
    if (status == -1)
        return (FALSE);
    if (WIFEXITED(status)) {
        if (retc != NULL)
            *retc = WEXITSTATUS(status);
        return (TRUE);
    }
    return (FALSE);
}

#endif  /* !_CR_OS_MACOSX_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
