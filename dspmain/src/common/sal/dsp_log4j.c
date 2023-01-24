/*******************************************************************************
 * log4j.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : huangshuxin <huangshuxin@hikvision.com.cn>
 * Version: V1.0.0  2020��1��8�� Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/



/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */

#include "dsp_log4j.h"

#include <sal.h>

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */

#define LOG4J_MAX_SIZE      (10*1024*1024)
#define LOG4J_UPDATE_SIZE   (4*1024)


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */
typedef struct _DSP_LOG4J_
{
    INT32 log4jLevel;
    INT32 log4jStatus;    
    INT32 log4jSyncStatus;    
    INT32 log4jSyncCnt;    
}DSP_LOG4J;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

static DSP_LOG4J g_dspLog4j ;

/*******************************************************************************
* ������  : Log4j_recordLevel
* ��  ��  : 
* ��  ��  : - level:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
* ������  : Log4j_updateSyncStatus
* ��  ��  : 
* ��  ��  : - level:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 Log4j_updateSyncStatus(UINT32 status)
{
    if(status != g_dspLog4j.log4jSyncStatus)
    {
        g_dspLog4j.log4jSyncStatus = status;
    }
    else
    {
        /* Ԥ�� */
    }

    return SAL_SOK;
}

/*******************************************************************************
* ������  : Log4j_getSyncStatus
* ��  ��  : 
* ��  ��  : - VOID:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
UINT32 Log4j_getSyncStatus(VOID)
{
    return g_dspLog4j.log4jSyncStatus;
}

/*******************************************************************************
* ������  : Log4j_setRecordLevel
* ��  ��  : 
* ��  ��  : - level:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
* ������  : Log4j_procProcess
* ��  ��  : 
* ��  ��  : - FunctionStr:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
        /* value ��Log4j_setRecordLevel���ж���Ч�ԣ��˴����ظ��ж� */
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
* ������  : Log4j_helpInfo
* ��  ��  : 
* ��  ��  : ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
void Log4j_helpInfo(VOID)
{
    SAL_logPrintf("dspinfo log4j:level 0/1/2/3/4                   :    Set log4j record level \n");

    return;
}


/*******************************************************************************
* ������  : Log4j_thrMain
* ��  ��  : 
* ��  ��  : ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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
* ������  : Log4j_init
* ��  ��  : 
* ��  ��  : ��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
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

	/*log4j   DSP��־��ʼ��*/
	sprintf_s(mesg, sizeof(mesg)-1, "%s %d %d %d", msg, LOG4J_MAX_SIZE, LOG4J_UPDATE_SIZE, 1);

    log4j_dsp_init(mesg);

    /* ���ӻ�ȡ�汾�ŵط� */    
    log4j_get_version(version, sizeof(version),  build, sizeof(build));

    SAL_INFO("Log4j version : %s  build : %s \n",version, build);

	/* ����ͬ����־�߳� */
    pthread_create(&stThrHandl, NULL, Log4j_thrMain, NULL);

    g_dspLog4j.log4jStatus      = SAL_TRUE;

    g_dspLog4j.log4jLevel       = LOG4J_LOG_LV_INFO;
    
    g_dspLog4j.log4jSyncStatus  = SAL_FALSE;

    g_dspLog4j.log4jSyncCnt     = 0;

    SAL_INFO("log4j init ok !!!\n");

	return SAL_SOK;
}

