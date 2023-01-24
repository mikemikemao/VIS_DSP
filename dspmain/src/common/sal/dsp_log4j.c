/*******************************************************************************
 * log4j.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : huangshuxin <huangshuxin@hikvision.com.cn>
 * Version: V1.0.0  2020年1月8日 Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/



/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "dsp_log4j.h"

#include <sal.h>

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

#define LOG4J_MAX_SIZE      (10*1024*1024)
#define LOG4J_UPDATE_SIZE   (4*1024)


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct _DSP_LOG4J_
{
    INT32 log4jLevel;
    INT32 log4jStatus;    
    INT32 log4jSyncStatus;    
    INT32 log4jSyncCnt;    
}DSP_LOG4J;

/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

static DSP_LOG4J g_dspLog4j ;

/*******************************************************************************
* 函数名  : Log4j_recordLevel
* 描  述  : 
* 输  入  : - level:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 Log4j_recordLevel(UINT32 level)
{    
    INT32  result = SAL_FAIL;
    UINT32 log4jLevel = LOG4J_LOG_LV_ERR;

    log4jLevel = g_dspLog4j.log4jLevel;
    
    if(level <= log4jLevel)
    {
        result = SAL_SOK;
    }

    return result;
}

/*******************************************************************************
* 函数名  : Log4j_updateSyncStatus
* 描  述  : 
* 输  入  : - level:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 Log4j_updateSyncStatus(UINT32 status)
{
    if(status != g_dspLog4j.log4jSyncStatus)
    {
        g_dspLog4j.log4jSyncStatus = status;
    }
    else
    {
        /* 预留 */
    }

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : Log4j_getSyncStatus
* 描  述  : 
* 输  入  : - VOID:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
UINT32 Log4j_getSyncStatus(VOID)
{
    return g_dspLog4j.log4jSyncStatus;
}

/*******************************************************************************
* 函数名  : Log4j_setRecordLevel
* 描  述  : 
* 输  入  : - level:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 Log4j_setRecordLevel(UINT32 level)
{
    if(LOG4J_LOG_LV_MAX <= level)
    {
        SAL_ERROR("Set Log4j level %d fail!\n",level);        
        return SAL_FAIL;
    }

    if(level != g_dspLog4j.log4jLevel)
    {
        g_dspLog4j.log4jLevel = level;
        SAL_INFO("Set debug level %d success!\n",level);
    }
    else
    {
        SAL_WARN("no need to set log4j level!\n");
    }
    
    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : Log4j_procProcess
* 描  述  : 
* 输  入  : - FunctionStr:
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
void Log4j_procProcess(INT8 * FunctionStr)
{
    INT32   i32Ret = SAL_SOK;
    INT32   cmdNum = 0;
    INT32   value = 0;
    INT8   *cmd[5] = {NULL};

    cmdNum = SAL_strtok(FunctionStr, " ", cmd);
    if(SAL_FAIL == cmdNum)
    {
        SAL_ERROR("Input cmd error!\n");
        return ;
    }
    
    if(is_str_equal(cmd[0], "level"))
    {
        /* value 在Log4j_setRecordLevel中判断有效性，此处不重复判断 */
        value = atoi(cmd[1]);
        
        i32Ret = Log4j_setRecordLevel(value);
        if(SAL_FAIL == i32Ret)
        {
            SAL_ERROR("Input cmd error!\n");
            return ;
        }    
    }
    else
    {
        SAL_WARN("Input cmd nou suport !\n");
    }

    return;
}


/*******************************************************************************
* 函数名  : Log4j_helpInfo
* 描  述  : 
* 输  入  : 无
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
void Log4j_helpInfo(VOID)
{
    SAL_logPrintf("dspinfo log4j:level 0/1/2/3/4                   :    Set log4j record level \n");

    return;
}


/*******************************************************************************
* 函数名  : Log4j_thrMain
* 描  述  : 
* 输  入  : 无
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
VOID *Log4j_thrMain()
{    
    UINT32 sycStatus = SAL_FALSE;

	while(1)
	{	
        SAL_msleep(5000);

        sycStatus = Log4j_getSyncStatus();
        
        if(sycStatus)
        {
            log4j_dsp_sync();

            g_dspLog4j.log4jSyncCnt++;

            Log4j_updateSyncStatus(SAL_FALSE);
            
            //SAL_logPrintf("log4j_dsp_sync !!!\n");
        }
        
	}

    return NULL;
}

/*******************************************************************************
* 函数名  : Log4j_init
* 描  述  : 
* 输  入  : 无
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 Log4j_init(VOID)
{
    INT8 version[64] = {0};
    INT8 build[64]   = {0};
	char msg[128] = "dsp_init";
	char mesg[256] = {0};

	pthread_t stThrHandl;

    SAL_memSetSafe(&g_dspLog4j , 0x0 , sizeof(DSP_LOG4J));
    if(SAL_TRUE == g_dspLog4j.log4jStatus)
    {
        SAL_WARN("log4j is running !\n");

        return SAL_SOK;
    }
    
    SAL_INFO("log4j init start !\n");

	/*log4j   DSP日志初始化*/
	sprintf_s(mesg, sizeof(mesg)-1, "%s %d %d %d", msg, LOG4J_MAX_SIZE, LOG4J_UPDATE_SIZE, 1);

    log4j_dsp_init(mesg);

    /* 增加获取版本号地方 */    
    log4j_get_version(version, sizeof(version),  build, sizeof(build));

    SAL_INFO("Log4j version : %s  build : %s \n",version, build);

	/* 创建同步日志线程 */
    pthread_create(&stThrHandl, NULL, Log4j_thrMain, NULL);

    g_dspLog4j.log4jStatus      = SAL_TRUE;

    g_dspLog4j.log4jLevel       = LOG4J_LOG_LV_INFO;
    
    g_dspLog4j.log4jSyncStatus  = SAL_FALSE;

    g_dspLog4j.log4jSyncCnt     = 0;

    SAL_INFO("log4j init ok !!!\n");

	return SAL_SOK;
}

