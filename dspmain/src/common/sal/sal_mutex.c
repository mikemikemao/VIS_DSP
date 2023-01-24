/*******************************************************************************
 * sal_mutex.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com> liuyun10 <liuyun10@hikvision.com>
 * Version: V1.0.0  2018��11��21�� Create
 *
 * Description : ��������ģ������ṩ�Ļ������ӿڣ�ʹ����������:
           ==========================
                   |                            
           SAL_mutexCreate
                   |
           SAL_mutexLock/SAL_mutexUnlock/SAL_mutexWait/SAL_mutexSignal
                   |
           SAL_mutexDelete
           ===========================
           
           ע��:
                SAL_MUTEX_NORMAL Ϊ�����Ļ�������ͬһ�̲߳��ܼ������
                SAL_MUTEX_RECURSIVE Ϊ֧��Ƕ�����Ļ�������ͬһ�߳̿���ѭ��������Σ�С��ʹ�ã���������
 * Modification: 
 *******************************************************************************/

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>

/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */
/*���廥��������*/
typedef struct
{
    pthread_mutex_t         lock;        /*�û�̬����������*/
    pthread_cond_t          cond;
    pthread_condattr_t      cattr;
}SAL_MutexObject;


/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_mutexCreate
* ��  ��  : �ú������𴴽�һ��������
* ��  ��  : - type   : ����������,�μ� SAL_MutexType����
*         : - phMutex: ���������ָ��,�������ɹ�ʱ������������
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
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

    /*�����ڴ�*/
    pMutexObj = (SAL_MutexObject *)SAL_memAlloc(sizeof(SAL_MutexObject),  "Sal");
    if(NULL == pMutexObj)
    {
        SAL_ERROR("alloc mutex failed\n");
        return SAL_FAIL;
    }

    if(SAL_MUTEX_RECURSIVE == type)
    {
        /* ������֧��Ƕ�ף���Ҫ��makfile�����_D_GNU_SOURCE��*/
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
* ������  : SAL_mutexLock
* ��  ��  : ����������
* ��  ��  : - hMutex: ���������,
* ��  ��  : ��
* ����ֵ  : HIK_SOK:   �ɹ�
*           HIK_FAIL: ʧ��
*******************************************************************************/
INT32 SAL_mutexLock(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*���handle��Ч��*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_mutex_lock(&pMutexObj->lock);

    return (0 == ret) ? SAL_SOK : SAL_FAIL;
}

/*******************************************************************************
* ������  : SAL_mutexUnlock
* ��  ��  : ����������
*           �ýӿڲ������ں�̬�ж������ĵ���
* ��  ��  : - hMutex: ���������,
* ��  ��  : ��
* ����ֵ  : HIK_SOK:   �ɹ�
*           HIK_FAIL: ʧ��
*******************************************************************************/
INT32 SAL_mutexUnlock(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*���handle��Ч��*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_mutex_unlock(&pMutexObj->lock);

    return (0 == ret) ? SAL_SOK : SAL_FAIL;
}

/*******************************************************************************
* ������  : SAL_mutexTimeOutWait
* ��  ��  : �ȴ��źŽ����͵ȴ���ʱ����Ҫ������ֵ
* ��  ��  : - hMutex     : ���������
*         : - msecTimeOut: ����ĳ�ʱʱ��
* ��  ��  : ��
* ����ֵ  : SAL_ETIMEOUT : ��ʱ����
*           SAL_SOK      : ���յ��ź���������
*           SAL_FAIL     : �쳣����
*******************************************************************************/
INT32 SAL_mutexTimeOutWait(Handle hMutex, INT32 msecTimeOut)
{
    SAL_MutexObject    *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32               ret;
    struct timespec     timeToWait,timespec1;

    /*���handle��Ч��*/    
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
    /* ��ʱ���� */
    if(ret == ETIMEDOUT)
    {
        return SAL_ETIMEOUT;
    }

    /* ���յ��ź��������� */
    if(ret == 0)
    {
        return SAL_SOK;
    }

    /* �쳣���� */
    return SAL_FAIL;
}

/*******************************************************************************
* ������  : SAL_mutexWait
* ��  ��  : �����ȴ������źŲ����˳����ⲿ��Ҫ�����SAL_mutexLock/SAL_mutexUnlock��ʹ��
* ��  ��  : - hMutex: ���������
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_mutexWait(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*���handle��Ч��*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_cond_wait(&pMutexObj->cond, &pMutexObj->lock);
    return ret;
}

/*******************************************************************************
* ������  : SAL_mutexSignal
* ��  ��  : �� SAL_mutexWait �����˳��ź�
* ��  ��  : - hMutex: ���������
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_mutexSignal(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*���handle��Ч��*/    
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_cond_signal(&pMutexObj->cond);
    return ret;
}

/*******************************************************************************
* ������  : SAL_mutexBroadcast
* ��  ��  : ������ SAL_mutexWait �����˳��ź�
* ��  ��  : - hMutex: ���������
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_mutexBroadcast(Handle hMutex)
{
    SAL_MutexObject *pMutexObj = (SAL_MutexObject *)hMutex;
    INT32            ret;

    /*���handle��Ч��*/
    if(NULL == (pMutexObj))
    {
        SAL_ERROR("NULL handle\n");
        return SAL_FAIL;
    }

    ret = pthread_cond_broadcast(&pMutexObj->cond);
    return ret;
}

/*******************************************************************************
* ������  : SAL_mutexDelete
* ��  ��  : �ú�����������һ��������
*           �ýӿڲ������ں�̬�ж������ĵ���
* ��  ��  : - hMutex: ���������
* ��  ��  : �ޡ�
* ����ֵ  : HIK_SOK:   �ɹ�
*           HIK_FAIL: ʧ��
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




