/*******************************************************************************
 * sal_que.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : hsy <heshengyuan@hikvision.com>
 * Version: V1.0.0  2020��4��14�� Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include "sal.h"
#include "sal_que.h"
/* ========================================================================== */
/*                          ���ݽṹ������                                    */
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
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_queCreate
* ��  ��  : ����һ������
            ���б�����һ��ָ�����飬�ڲ��ɴ��maxLen������ָ������
* ��  ��  : - phQue  : ���нṹ��
*         : - maxLen : �����г���
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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

    /* ���maxElems��ֵ�Ƿ���2��N�η�������ǣ������put/get�в�ʹ��%������������*/
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
* ������  : SAL_queDelete
* ��  ��  : ����һ������
* ��  ��  : - hQue: ���нṹ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
* ������  : SAL_QuePut
* ��  ��  : �������з�����
* ��  ��  : - hQue   : ���о����
*         : - pElem  : ������е�����ָ�롣���г�Ա��
*         : - timeout: ��ʱʱ�䡣��λ�Ǻ��롣SAL_TIMEOUT_NONE ��ʾ���ȴ���SAL_TIMEOUT_FOREVER ��ʾ���޵ȴ���
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
* ������  : SAL_queGet
* ��  ��  : �Ӷ����л�ȡһ����Ա����
* ��  ��  : - hQue   : ���о����
*         : - ppElem : �Ӷ����л�ȡ��ַ�Ķ���ָ�롣��ȡ�����ĳ�Ա��
*         : - timeout: ��ʱʱ�䡣��λ�Ǻ��롣SAL_TIMEOUT_NONE ��ʾ���ȴ���SAL_TIMEOUT_FOREVER ��ʾ���޵ȴ���
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
                /* pthread_cond_timedwait��Ҫʹ�õ�����һ��δ���ض���ʱ�䣬���Գ�ʱ�ǻ��ڵ�ǰʱ�� */
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
* ������  : SAL_QueGetQueuedCount
* ��  ��  : ��ȡ���е�ǰ�Ѿ�����ĳ�Ա������          
* ��  ��  : - hQue: ���о����
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK  : �ɹ���
*           SAL_FAIL: ʧ�ܡ�
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
* ������  : SAL_queIsEmpty
* ��  ��  : �ж϶����Ƿ�Ϊ�ա�          
* ��  ��  : - hQue: ���о����
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK  : �ɹ���
*           SAL_FAIL: ʧ�ܡ�
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


