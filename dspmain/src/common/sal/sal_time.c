/*******************************************************************************
 * sal_time.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2016年10月8日 Create
 *
 * Description :
 * Modification:
 *******************************************************************************/
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>
#include <sys/times.h>
#include <sys/time.h>


/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_getTimeOfJiffies
* 描  述  : 获取系统单调递增的时间。
* 输  入  : 无。
* 输  出  : 无。
* 返回值  : 时间值，单位是毫秒。
*******************************************************************************/
UINT64 SAL_getTimeOfJiffies(void)
{
    struct tms tBuf;

    /* 当前tick计数，初始化为一个中间值，处理第一次溢出 */
    static UINT64 curtick = 0x7fffffff;

    /* 先备份一下，避免多线程访问出错 */
    UINT64 tmptick = curtick;

    /* times必须使用非空参数，以兼容更多的平台 */
    UINT64 tick = (UINT32)times(&tBuf);

    if(tick == (UINT32)-1)             /* 溢出，取errno的值 */
    {
        /* glibc在处理溢出时，返回的errno可能不准，引起误溢出 */
        /* tick = (UINT32)-errno; */
        tick = tmptick;                /* 使用上一次合理的值，风险更低 */
    }

    if((UINT32)tmptick != (UINT32)tick)/* 低32位变化说明tick变化 */
    {
        while(tick < tmptick)          /* 溢出处理 */
        {
            tick += 0xffffffff;
            tick += 1;
        }

        /* 提前处理多线程引起的时间倒流的问题，提高效率 */
        if(curtick < tick)
        {
            curtick = tick;
        }
    }

    return curtick * (1000 / sysconf(_SC_CLK_TCK));
}


/*******************************************************************************
* Function      : SAL_msleep
* Description   : 毫秒的精确延时，线程安全，不占用CPU时间
* Input         : - usecTimeOut: 超时时间，微秒值
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
* 函数名  : SAL_getTimeOfDay
* 描  述  : 获取当前时间。
* 输  入  : - tv: 时间表述结构，由秒和微妙组成。
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
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
* 函数名  : SAL_mselect
* 描  述  : select 超时，精确延时，超时时使用，线程安全
* 输  入  : - fd         : 设置需要超时操作的文件句柄
*         : - msecTimeOut: 超时时间，毫秒值
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
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
* 函数名  : SAL_getCurrentTime
* 描  述  : 获取当前时间，以年月日分秒格式输出
* 输  入  :无
* 输  出  :- current         : 设置需要超时操作的文件句柄
* 返回值  : SAL_SOK:成功
                       SAL_FAIL:失败
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

    clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
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
* 函数名  : getDateTime
* 描  述  : 获取当前系统时间
* 输  入  : - pDT:
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
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

    /* 得到日历时间，从标准时间点到现在的秒数 */
    tp = time(NULL);

    /* 格式化日历时间 */
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
* 函数名  : SAL_getCurrentTimeUs
* 描  述  : 获取当前时间，以 us 单位输出
* 输  入  : - pCurTime:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
UINT64 SAL_getCurrentTimeUs(void)
{
    struct timeval timeval;

    gettimeofday(&timeval, NULL);

    return (UINT64)timeval.tv_sec*1000000 + timeval.tv_usec;
}

/*******************************************************************************
* 函数名  : SAL_getCurrentTimeMs
* 描  述  : 获取当前时间，以 ms 单位输出
* 输  入  : - pCurTime:  以 ms 单位输出
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
UINT64 SAL_getCurrentTimeMs(void)
{
    struct timeval timeval;

    gettimeofday(&timeval, NULL);

    return (UINT64)timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
}
