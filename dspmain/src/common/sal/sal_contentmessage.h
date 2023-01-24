/*******************************************************************************
 * sal_contentmessage.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhangyulong8 <zhangyulong8@hikvision.com>
 * Version: V1.0.0  2021年3月16日 Create
 *
 * Description :
 * Modification:
 *******************************************************************************/

#ifndef __SAL_CONTENTMESSAGE_H__
#define __SAL_CONTENTMESSAGE_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "sal.h"


#ifdef __cplusplus
extern "C"
{
#endif //

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define     MAX_CONTENTBODY_CNT         10
#define     MAX_CONTENTNAME_LEN         128
#define     MAX_HANDLEBODY_CNT          10

/*对应StHandleBodyData handleType变量*/
#define     HANDLE_TYPE_SURFACE         0    /* 安卓显示窗口句柄 */
#define     HANDLE_TYPE_GRAPHIC         1    /* 安卓图像句柄     */

typedef struct
{
    void* data;
    UINT32 dataLen;
    INT8 dataName[MAX_CONTENTNAME_LEN];     // 用于区分二进制数据
}StContentBodyData;

typedef struct
{
    void* handle;
    int   handleType;
    INT8 handleName[MAX_CONTENTNAME_LEN];     // 用于区分句柄数据
}StHandleBodyData;

typedef struct
{
    INT32 currentID;
    INT8* jsonData;
    UINT32 jsonDataLen;
    INT8 jsonDataName[MAX_CONTENTNAME_LEN]; // 协议的URL
    INT8 method[MAX_CONTENTNAME_LEN]; // 协议的method

    UINT32 contentBodyCnt;
    StContentBodyData* contentBodyData;
    UINT32 handleBodyCnt;
    StHandleBodyData* handleBodyData;
}StContentData;

/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */
/*******************************************************************************
* 函数名  : SAL_packageMessage
* 描  述  : 将JSON消息和二进制内容打包成新的通信信息
* 输  入  : - pContentData: 包含JSON消息和二进制信息的结构体地址
* 输  出  : - outBuf: 封装成功后带出数据
*         : - outBufLen: 封装成功后带出的数据长度
* 返回值  : SAL_SOK:        操作成功
*           SAL_FAIL:       操作失败
*******************************************************************************/
INT32 SAL_packageMessage(StContentData* pContentData, void** outBuf, UINT32* outBufLen);

/*******************************************************************************
* 函数名  : SAL_releasePackageData
* 描  述  : 资源释放
* 输  入  : - data: 该数据地址为SAL_packageMessage带出的数据地址
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
void SAL_releasePackageData(void* data);

/*******************************************************************************
* 函数名  : SAL_unpackageMessage
* 描  述  : 将通信信息解包成JSON消息和二进制内容
* 输  入  : - data: 通信信息数据地址
* 输  出  : - dataLen: 通信信息数据的长度
*         : - outContentData: 解封装成功后带出包含JSON消息和二进制信息的结构体地址
* 返回值  : SAL_SOK:        操作成功
*           SAL_FAIL:       操作失败
*******************************************************************************/
INT32 SAL_unpackageMessage(void* data, UINT32 dataLen, StContentData** outContentData);

/*******************************************************************************
* 函数名  : SAL_releaseMessage
* 描  述  : 资源释放
* 输  入  : - contentData: 该数据地址为SAL_unpackageMessage带出的结构体数据地址
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
void SAL_releaseMessage(StContentData* contentData);

#ifdef __cplusplus
}
#endif //

#endif // __SAL_CONTENTMESSAGE_H__

