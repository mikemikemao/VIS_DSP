/*******************************************************************************
 * sal_time.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2016��10��8�� Create
 *
 * Description :
 * Modification:
 *******************************************************************************/
/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>
#include <sys/times.h>
#include <sys/time.h>


/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_getTimeOfJiffies
* ��  ��  : ��ȡϵͳ����������ʱ�䡣
* ��  ��  : �ޡ�
* ��  ��  : �ޡ�
* ����ֵ  : ʱ��ֵ����λ�Ǻ��롣
*******************************************************************************/
UINT64 SAL_getTimeOfJiffies(void)
{
    struct tms tBuf;

    /* ��ǰtick��������ʼ��Ϊһ���м�ֵ�������һ����� */
    static UINT64 curtick = 0x7fffffff;

    /* �ȱ���һ�£�������̷߳��ʳ��� */
    UINT64 tmptick = curtick;

    /* times����ʹ�÷ǿղ������Լ��ݸ����ƽ̨ */
    UINT64 tick = (UINT32)times(&tBuf);

    if(tick == (UINT32)-1)             /* �����ȡerrno��ֵ */
    {
        /* glibc�ڴ������ʱ�����ص�errno���ܲ�׼����������� */
        /* tick = (UINT32)-errno; */
        tick = tmptick;                /* ʹ����һ�κ����ֵ�����ո��� */
    }

    if((UINT32)tmptick != (UINT32)tick)/* ��32λ�仯˵��tick�仯 */
    {
        while(tick < tmptick)          /* ������� */
        {
            tick += 0xffffffff;
            tick += 1;
        }

        /* ��ǰ������߳������ʱ�䵹�������⣬���Ч�� */
        if(curtick < tick)
        {
            curtick = tick;
        }
    }

    return curtick * (1000 / sysconf(_SC_CLK_TCK));
}


/*******************************************************************************
* Function      : SAL_msleep
* Description   : ����ľ�ȷ��ʱ���̰߳�ȫ����ռ��CPUʱ��
* Input         : - usecTimeOut: ��ʱʱ�䣬΢��ֵ
* Output        : NONE
* Return        : HIK_SOK  : Success
*                 HIK_FAIL : Fail
*******************************************************************************/
INT32 SAL_msleep(INT32 msecTimeOut)
{
    INT32            s32Ret;
    struct timeval   tv;
    INT32            tv_sec  = msecTimeOut / 1000;
    INT32            tv_usec = msecTimeOut % 1000;

    tv.tv_sec  = tv_sec;
    tv.tv_usec = tv_usec * 1000;

    s32Ret = select(0, NULL, NULL, NULL, &tv);
    if(s32Ret < 0)
    {
        SAL_ERROR("select TimeOut err %d\n", s32Ret);
        return SAL_FAIL;
    }

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_getTimeOfDay
* ��  ��  : ��ȡ��ǰʱ�䡣
* ��  ��  : - tv: ʱ������ṹ�������΢����ɡ�
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_getTimeOfDay(struct timeval* tv)
{
    INT32            status;

    status = gettimeofday(tv, NULL);
    if (status < 0)
    {
        SAL_ERROR("select TimeOut err %d\n", status);
        return SAL_FAIL;
    }

    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_mselect
* ��  ��  : select ��ʱ����ȷ��ʱ����ʱʱʹ�ã��̰߳�ȫ
* ��  ��  : - fd         : ������Ҫ��ʱ�������ļ����
*         : - msecTimeOut: ��ʱʱ�䣬����ֵ
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_mselect(INT32 fd, INT32 msecTimeOut)
{
    INT32  s32Ret;
    fd_set fdSet;
    struct timeval tv;
    INT32   tv_sec  = msecTimeOut / 1000;
    INT32   tv_usec = msecTimeOut % 1000;

    SAL_memSetSafe(&fdSet, 0, sizeof(fdSet));
    FD_ZERO (&fdSet);
    FD_SET (fd, &fdSet);

    tv.tv_sec  = tv_sec;
    tv.tv_usec = tv_usec * 1000;
    s32Ret = select(fd + 1, &fdSet, NULL, NULL, &tv);

    if (s32Ret <= 0)
    {
        return SAL_FAIL;
    }
    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_getCurrentTime
* ��  ��  : ��ȡ��ǰʱ�䣬�������շ����ʽ���
* ��  ��  :��
* ��  ��  :- current         : ������Ҫ��ʱ�������ļ����
* ����ֵ  : SAL_SOK:�ɹ�
                       SAL_FAIL:ʧ��
*           
*******************************************************************************/
INT32 SAL_getCurrentTime(INT8 *current)
{
    struct timeval tv;
    struct timespec time;
    struct tm nowTime;

    if(NULL == current)
    {
        SAL_ERROR("current is NULL!!\n");
        return SAL_FAIL;
    }

    gettimeofday(&tv,NULL);

    clock_gettime(CLOCK_REALTIME, &time);  //��ȡ�����1970�����ڵ�����
    localtime_r(&time.tv_sec, &nowTime);

    snprintf(current, 64, "%02d%02d%02d%02d%02d%02d%03ld", 
            nowTime.tm_year + 1900-2000,
            nowTime.tm_mon+1,
            nowTime.tm_mday,
            nowTime.tm_hour,
            nowTime.tm_min,
            nowTime.tm_sec,
            tv.tv_usec / 1000);

    return SAL_SOK;
}

/*******************************************************************************
* ������  : getDateTime
* ��  ��  : ��ȡ��ǰϵͳʱ��
* ��  ��  : - pDT:
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_getDateTime(DATE_TIME *pDT)
{
    struct tm   *p = NULL;
    time_t      tp;

    if(NULL == pDT)
    {
        SAL_ERROR("pDT is NULL!!\n");
        return;
    }

    /* �õ�����ʱ�䣬�ӱ�׼ʱ��㵽���ڵ����� */
    tp = time(NULL);

    /* ��ʽ������ʱ�� */
    p = localtime(&tp);

    if(NULL == p)
    {
        SAL_ERROR("p is NULL!!\n");
        return;
    }

    pDT->year         = p->tm_year + 1900;
    pDT->month        = p->tm_mon  + 1;
    pDT->day          = p->tm_mday;
    pDT->hour         = p->tm_hour;
    pDT->minute       = p->tm_min;
    pDT->second       = p->tm_sec;
    pDT->dayOfWeek    = p->tm_wday;
    pDT->milliSecond  = 0;

    return;
}

/*******************************************************************************
* ������  : SAL_getCurrentTimeUs
* ��  ��  : ��ȡ��ǰʱ�䣬�� us ��λ���
* ��  ��  : - pCurTime:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
UINT64 SAL_getCurrentTimeUs(void)
{
    struct timeval timeval;

    gettimeofday(&timeval, NULL);

    return (UINT64)timeval.tv_sec*1000000 + timeval.tv_usec;
}

/*******************************************************************************
* ������  : SAL_getCurrentTimeMs
* ��  ��  : ��ȡ��ǰʱ�䣬�� ms ��λ���
* ��  ��  : - pCurTime:  �� ms ��λ���
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
UINT64 SAL_getCurrentTimeMs(void)
{
    struct timeval timeval;

    gettimeofday(&timeval, NULL);

    return (UINT64)timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
}
