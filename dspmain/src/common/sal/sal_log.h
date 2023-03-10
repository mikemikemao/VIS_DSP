/*******************************************************************************
 * sal_log.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2019年3月21日 Create
 *
 * Description : 对日志打印的管理，在线调整打印级别，是否进行存储
 * Modification: 
 *******************************************************************************/

#ifndef _SAL_LOG_H_
#define _SAL_LOG_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal_type.h>

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define SAVEDATAPATH "/data/dspdata/"


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_logLevel
* 描  述  : 设置打印等级
* 输  入  : - level: SAL_LogLevel 0 1 2 3
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_logLevel(UINT32 level);

/*******************************************************************************
* 函数名  : SAL_logAllowed
* 描  述  : 是否允许打印
* 输  入  : - level: SAL_logAllowed 0 1 2 3
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_logAllowed(UINT32 level);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

