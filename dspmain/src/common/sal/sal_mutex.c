/*******************************************************************************
 * sal_mutex.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com> liuyun10 <liuyun10@hikvision.com>
 * Version: V1.0.0  2018年11月21日 Create
 *
 * Description : 软件抽象层模块对外提供的互斥锁接口，使用流程如下:
           ==========================
                   |                            
           SAL_mutexCreate
                   |
           SAL_mutexLock/SAL_mutexUnlock/SAL_mutexWait/SAL_mutexSignal
                   |
           SAL_mutexDelete
           ===========================
           
           注意:
                SAL_MUTEX_NORMAL 为常见的互斥锁，同一线程不能加锁多次
                SAL_MUTEX_RECURSIVE 为支持嵌套锁的互斥锁，同一线程可以循环加锁多次，小心使用，避免死锁
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
/*定义互斥锁对象*/
typedef struct
{
    pthread_mutex_t         lock;        /*用户态互斥锁变量*/
    pthread_cond_t          cond;
    pthread_condattr_t      cattr;
}SAL_MutexObject;


/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_mutexCreate
* 描  述  : 该函数负责创建一个互斥锁
* 输  入  : - type   : 互斥锁类型,参见 SAL_MutexType定义
*         : - phMutex: 互斥锁句柄指针,当创建成功时输出互斥锁句柄
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
INT32 SAL_mutexCreate(UINT32 type, Handle *phMutex)
{
    SAL_MutexObject *pMutexObj = NULL;
    pthread_mutexattr_t mutex_attr;
    
    if(NULL == phMutex)
    {
        SAL_ERROR("Null phMutex\n");
        return SAL_FAIL;
    }

    /*分配内存*/
    pMutexObj = (SAL_MutexObject *)SAL_memAlloc(sizeof(SAL_MutexObject),  "Sal");
    if(NULL == pMutexObj)
    {
        SAL_ERROR("alloc mutex failed\n");
        return SAL_FAIL;
    }

    if(SAL_MUTEX_RECURSIVE == type)
    {
        /* 设置锁支持嵌套，需要在makfile中添加_D_GNU_SOURCE。*/
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&pMutexObj->lock, &mutex_attr);

        pthread_condattr_init(&pMutexObj->cattr);
        pthread_condattr_setclock(&pMutexObj->cattr, CLOCK_MONOTONIC);
        pthread_cond_init(&pMutexObj->cond, &pMutexObj->cattr);
    
        pthread_mutexattr_destroy(&mutex_attr);
    }
    else
    {
        pthread_condattr_init(&pMutexObj->cattr);
        pthread_condattr_setclock(&pMutexObj->cattr, CLOCK_MONOTONIC);

        pthread_mutex_init(&pMutexObj->lock, NULL);
        pthread_cond_init(&pMutexObj->cond, &pMutexObj->cattr);
    }

    *phMutex = (Handle)pMutexObj;
    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_mutexLock
* 描  述  : 互斥锁加锁
* 输  入  : - hMutex: 互斥锁句柄,
* 输  出  : 无
* 返回值  : HIK_SOK:   成功
*           HIK_FAIL: 失败
*******************************************************************************/
INT32 SAL_mutexLock(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*检查handle有效性*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_mutex_lock(&pMutexObj->lock);

    return (0 == ret) ? SAL_SOK : SAL_FAIL;
}

/*******************************************************************************
* 函数名  : SAL_mutexUnlock
* 描  述  : 互斥锁解锁
*           该接口不能在内核态中断上下文调用
* 输  入  : - hMutex: 互斥锁句柄,
* 输  出  : 无
* 返回值  : HIK_SOK:   成功
*           HIK_FAIL: 失败
*******************************************************************************/
INT32 SAL_mutexUnlock(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*检查handle有效性*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_mutex_unlock(&pMutexObj->lock);

    return (0 == ret) ? SAL_SOK : SAL_FAIL;
}

/*******************************************************************************
* 函数名  : SAL_mutexTimeOutWait
* 描  述  : 等待信号解锁和等待超时，需要处理返回值
* 输  入  : - hMutex     : 互斥锁句柄
*         : - msecTimeOut: 毫秒的超时时间
* 输  出  : 无
* 返回值  : SAL_ETIMEOUT : 超时返回
*           SAL_SOK      : 接收到信号正常返回
*           SAL_FAIL     : 异常返回
*******************************************************************************/
INT32 SAL_mutexTimeOutWait(Handle hMutex, INT32 msecTimeOut)
{
    SAL_MutexObject    *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32               ret;
    struct timespec     timeToWait,timespec1;

    /*检查handle有效性*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    clock_gettime(CLOCK_MONOTONIC, &timespec1);
    timeToWait.tv_nsec = timespec1.tv_nsec + (msecTimeOut % 1000) * 1000L * 1000L;
    timeToWait.tv_sec  = timespec1.tv_sec + msecTimeOut / 1000 + timeToWait.tv_nsec / 1000000000L;
    timeToWait.tv_nsec = timeToWait.tv_nsec % 1000000000L;
    
    ret = pthread_cond_timedwait(&pMutexObj->cond, &pMutexObj->lock, &timeToWait);
    /* 超时返回 */
    if(ret == ETIMEDOUT)
    {
        return SAL_ETIMEOUT;
    }

    /* 接收到信号正常返回 */
    if(ret == 0)
    {
        return SAL_SOK;
    }

    /* 异常返回 */
    return SAL_FAIL;
}

/*******************************************************************************
* 函数名  : SAL_mutexWait
* 描  述  : 阻塞等待接收信号才能退出，外部需要管理好SAL_mutexLock/SAL_mutexUnlock的使用
* 输  入  : - hMutex: 互斥锁句柄
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
INT32 SAL_mutexWait(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*检查handle有效性*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_cond_wait(&pMutexObj->cond, &pMutexObj->lock);
    return ret;
}

/*******************************************************************************
* 函数名  : SAL_mutexSignal
* 描  述  : 给 SAL_mutexWait 发送退出信号
* 输  入  : - hMutex: 互斥锁句柄
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
INT32 SAL_mutexSignal(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*检查handle有效性*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_cond_signal(&pMutexObj->cond);
    return ret;
}

/*******************************************************************************
* 函数名  : SAL_mutexBroadcast
* 描  述  : 给所有 SAL_mutexWait 发送退出信号
* 输  入  : - hMutex: 互斥锁句柄
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
INT32 SAL_mutexBroadcast(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*检查handle有效性*/
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_cond_broadcast(&pMutexObj->cond);
    return ret;
}

/*******************************************************************************
* 函数名  : SAL_mutexDelete
* 描  述  : 该函数负责销毁一个互斥锁
*           该接口不能在内核态中断上下文调用
* 输  入  : - hMutex: 互斥锁句柄
* 输  出  : 无。
* 返回值  : HIK_SOK:   成功
*           HIK_FAIL: 失败
*******************************************************************************/
INT32 SAL_mutexDelete(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;

    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    pthread_cond_destroy(&pMutexObj->cond);
    pthread_mutex_destroy(&pMutexObj->lock);  

    SAL_memFree(pMutexObj);
    return SAL_SOK;
}




