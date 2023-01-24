/*******************************************************************************
 * sal_str.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhanglincheng <zhanglincheng@hikvision.com>
 * Version: V1.0.0  2019年7月19日 Create
 *
 * Description :
 * Modification:
 *******************************************************************************/

#ifndef _SAL_STR_H_
#define _SAL_STR_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <string.h>

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

#define is_str_equal(a,b) ((strlen(a) == strlen(b)) && (0 == strcasecmp(a,b)))


/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

INT32 SAL_strtok(INT8 *InputStr, const INT8 *delim, INT8 **OutputStr);

#ifdef __cplusplus
}
#endif

#endif /* _SAL_STR_H_ */

