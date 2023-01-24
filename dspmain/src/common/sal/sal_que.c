/*******************************************************************************
 * sal_que.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : hsy <heshengyuan@hikvision.com>
 * Version: V1.0.0  2020年4月14日 Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "sal.h"
#include "sal_que.h"
/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct
{
    UINT32              len;
    UINT32              count;
    UINT32              curRd;
    UINT32              curWr;

    UINT32              isAlign;
    void              **pQueue;

    pthread_mutex_t     lock;
    pthread_cond_t      condRd;
    pthread_cond_t      condWr;
} SAL_QueObj;

/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_queCreate
* 描  述  : 创建一个队列
            队列本体是一个指针数组，内部可存放maxLen个数的指针数据
* 输  入  : - phQue  : 队列结构体
*         : - maxLen : 最大队列长度
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QueCreate(Handle *phQue, unsigned int maxElems)
{
    SAL_QueObj  *pObj       = NULL;
    UINT32       queSize    = 0;
    
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    if (SAL_isNull(phQue) || (maxElems == 0))
    {
        SAL_ERROR("Input parms NULL !\n");
        return SAL_FAIL;
    }

    queSize = sizeof(SAL_QueObj) + (sizeof(void *) * maxElems);
    pObj    = (SAL_QueObj *)SAL_memAlloc(queSize, "Sal");
    if(SAL_isNull(pObj))
    {
        SAL_ERROR("Alloc obj/queue failed\n");
        return SAL_FAIL;
    }

    SAL_clearSize(pObj, queSize);
    
    pObj->pQueue    = (void **)((char*)pObj + sizeof(SAL_QueObj));
       
    pObj->len       = maxElems;

    /* 检测maxElems的值是否是2的N次方，如果是，则后续put/get中不使用%符号求余数。*/
    if (SAL_checkIs2n(maxElems))
    {
        SAL_DEBUG("maxElems %u is 2^n !\n", maxElems);
        pObj->isAlign = SAL_TRUE;
    }

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);
    
    pthread_mutex_init(&pObj->lock, &mutex_attr);
    pthread_cond_init(&pObj->condRd, &cond_attr);
    pthread_cond_init(&pObj->condWr, &cond_attr);
    pthread_condattr_destroy(&cond_attr);
    pthread_mutexattr_destroy(&mutex_attr);

    *phQue = (Handle)pObj;
  
    return SAL_SOK;
}


/*******************************************************************************
* 函数名  : SAL_queDelete
* 描  述  : 销毁一个队列
* 输  入  : - hQue: 队列结构体
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QueDelete(Handle hQue)
{
    SAL_QueObj *pObj = (SAL_QueObj *)hQue;

    if(SAL_isNull(hQue))
    {
        return SAL_FAIL;
    }

    pthread_cond_destroy(&pObj->condRd);
    pthread_cond_destroy(&pObj->condWr);
    pthread_mutex_destroy(&pObj->lock);

    SAL_clear(pObj);
    SAL_memFree(pObj);
    return SAL_SOK;

}

/*******************************************************************************
* 函数名  : SAL_QuePut
* 描  述  : 往对列中放数据
* 输  入  : - hQue   : 队列句柄。
*         : - pElem  : 放入队列的数据指针。队列成员。
*         : - timeout: 超时时间。单位是毫秒。SAL_TIMEOUT_NONE 表示不等待，SAL_TIMEOUT_FOREVER 表示无限等待。
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QuePut(Handle hQue, void *pElem, unsigned int timeout)
{
    INT32          status   = SAL_SOK;
    SAL_QueObj      *pObj   = (SAL_QueObj *)hQue;
    struct timespec waitTime;

    if (SAL_isNull(hQue))
    {
        return SAL_FAIL;
    }

    pthread_mutex_lock(&pObj->lock);

    //SAL_DEBUG("len=%d count=%d curRd=%d curWr=%d\n", pObj->len, pObj->count, pObj->curRd, pObj->curWr);
    do
    { 
        if( pObj->count < pObj->len )
        {     
            pObj->pQueue[pObj->curWr] = pElem;
           
            if (pObj->isAlign)
            {
                pObj->curWr = (pObj->curWr + 1) & (pObj->len - 1);     
            }
            else
            { 
                pObj->curWr = (pObj->curWr + 1) % pObj->len;
            }
            if (0 == pObj->count)
            {
                pthread_cond_signal(&pObj->condRd);
            }

            pObj->count++;

            break;
        }
        else
        {
            if(timeout==SAL_TIMEOUT_NONE)
            {
                status = SAL_FAIL;
                break;
            }

            if(timeout == (UINT32)SAL_TIMEOUT_FOREVER)
            {
                status = pthread_cond_wait(&pObj->condWr, &pObj->lock);
                if(SAL_SOK != status)
                {
                    SAL_ERROR("pthread_cond_wait failed, ret:%d\n", status);
                    status = SAL_FAIL;
                    break;
                }
            }
            else
            {
                waitTime.tv_sec  = timeout/1000;
                waitTime.tv_nsec = (timeout%1000)*1000000;

                status = pthread_cond_timedwait(&pObj->condWr, &pObj->lock, &waitTime);
                if(ETIMEDOUT == status)
                {
                    status = SAL_ETIMEOUT;
                    break;
                }
                else if(SAL_SOK != status)
                {
                    SAL_ERROR("pthread_cond_timedwait failed, ret:%d\n", status);
                    status = SAL_FAIL;
                    break;
                }
            }
        }
    } while(0);

    pthread_mutex_unlock(&pObj->lock);

    return status;

}

/*******************************************************************************
* 函数名  : SAL_queGet
* 描  述  : 从队列中获取一个成员数据
* 输  入  : - hQue   : 队列句柄。
*         : - ppElem : 从队列中获取地址的二级指针。获取上来的成员。
*         : - timeout: 超时时间。单位是毫秒。SAL_TIMEOUT_NONE 表示不等待，SAL_TIMEOUT_FOREVER 表示无限等待。
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QueGet(Handle hQue, void **ppElem, unsigned int timeout)
{
    INT32               status  = SAL_SOK;
    SAL_QueObj         *pObj    = (SAL_QueObj *)hQue;
    struct timespec     waitTime;

    if (SAL_isNull(hQue))
    {
        return SAL_FAIL;
    }

    pthread_mutex_lock(&pObj->lock);
    do
    {
        if(pObj->count > 0)
        {
            if(SAL_isNotNull(ppElem))
            {
                *ppElem = pObj->pQueue[pObj->curRd];
            }
            else
            {
                status = SAL_FAIL;
                break;
            }

            if (pObj->isAlign)
            {
                pObj->curRd = (pObj->curRd + 1) & (pObj->len - 1);
            }
            else
            {
                pObj->curRd = (pObj->curRd + 1) % pObj->len;
            }
            
            if (pObj->count == pObj->len)
            {
                pthread_cond_signal(&pObj->condWr);
            }

            pObj->count--;

            break;
        }
        else
        {
            if(timeout==SAL_TIMEOUT_NONE)
            {
                status = SAL_FAIL;
                break;
            }

            if(timeout == (UINT32)SAL_TIMEOUT_FOREVER)
            {
                status = pthread_cond_wait(&pObj->condRd, &pObj->lock);
                if(SAL_SOK != status)
                {
                    SAL_ERROR("pthread_cond_wait failed, ret:%d\n", status);
                    status = SAL_FAIL;
                    break;
                }
            }
            else
            {
                struct timeval stTimeVal;

                waitTime.tv_sec   = timeout/1000;
                waitTime.tv_nsec  = (timeout%1000)*1000000;

                SAL_getTimeOfDay(&stTimeVal);
                waitTime.tv_sec  += stTimeVal.tv_sec;
                waitTime.tv_nsec += stTimeVal.tv_usec * 1000;
                if(waitTime.tv_nsec >= 1000000000)
                {
                    waitTime.tv_sec  += 1;
                    waitTime.tv_nsec %= 1000000000;
                }
                /* pthread_cond_timedwait需要使用到的是一个未来特定的时间，所以超时是基于当前时间 */
                status = pthread_cond_timedwait(&pObj->condRd, &pObj->lock, &waitTime);
                if(ETIMEDOUT == status)
                {
                    status = SAL_ETIMEOUT;
                    break;
                }
                else if(SAL_SOK != status)
                {
                    SAL_ERROR("pthread_cond_timedwait failed, ret:%d\n", status);
                    status = SAL_FAIL;
                    break;
                }
            }
        }
    } while(1);
    pthread_mutex_unlock(&pObj->lock);
    return status;
}

/*******************************************************************************
* 函数名  : SAL_QueGetQueuedCount
* 描  述  : 获取队列当前已经缓存的成员数量。          
* 输  入  : - hQue: 队列句柄。
* 输  出  : 无。
* 返回值  : SAL_SOK  : 成功。
*           SAL_FAIL: 失败。
*******************************************************************************/
unsigned int SAL_QueGetQueuedCount(Handle hQue)
{
    unsigned int queuedCount = 0;
    SAL_QueObj *pQueObj = (SAL_QueObj *)hQue;

    if (SAL_isNull(hQue))
    {
        return 0;
    }

    pthread_mutex_lock(&pQueObj->lock);
    queuedCount = pQueObj->count;
    pthread_mutex_unlock(&pQueObj->lock);

    return queuedCount;

}

/*******************************************************************************
* 函数名  : SAL_queIsEmpty
* 描  述  : 判断队列是否为空。          
* 输  入  : - hQue: 队列句柄。
* 输  出  : 无。
* 返回值  : SAL_SOK  : 成功。
*           SAL_FAIL: 失败。
*******************************************************************************/
int SAL_QueIsEmpty(Handle hQue)
{
    int isEmpty;
    SAL_QueObj *pQueObj = (SAL_QueObj *)hQue;

    if (SAL_isNull(hQue))
    {
        return SAL_TRUE;
    }

    pthread_mutex_lock(&pQueObj->lock);

    if (0 == pQueObj->count)
    {
        isEmpty = SAL_TRUE;
    }
    else
    {
        isEmpty = SAL_FALSE;
    }

    pthread_mutex_unlock(&pQueObj->lock);

    return isEmpty;
}


