/*******************************************************************************
 * sal_contentmessage.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhangyulong8 <zhangyulong8@hikvision.com>
 * Version: V1.0.0  2021年3月16日 Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/
 
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "sal_contentmessage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define     CONTENT_BOUNDARY            "boundary"

#define     JSON_CONTENT_TYPE_VALUE     "application/json"
#define     BINARY_CONTENT_TYPE_VALUE   "application/octet-stream"
#define     HANDLE_CONTENT_TYPE_VALUE   "application/x-handle"

#define     BINARY_DATA_TAIL            "\r\n"

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct
{
    void* data;
    UINT32 dataLen;
    
    UINT32 contentHeadLen;
    INT8 contentHead[512];
}StContentInfo;

typedef struct
{
    UINT32 currentID;
    StContentBodyData bodyData;
}StUnpacketContentInfo;

typedef struct
{
    void* handle;
    int   handleType;
    
    UINT32 contentHeadLen;
    INT8 contentHead[512];
}StHandleInfo;

typedef struct
{
    UINT32 currentID;
    StHandleBodyData bodyData;
}StUnpacketHandleInfo;

/* ========================================================================== */
/*                            全局变量定义区                                  */
/* ========================================================================== */
static const INT8* g_sContentHead = "Content-Type: multipart/form-data; boundary=%s\r\n" \
                                    "Content-Length: %u\r\n\r\n";

static const INT8* g_sContentContinue = "--%s\r\n" \
                                        "Content-Disposition: form-data; id=%x; name=%s\r\n" \
                                        "Content-Type: %s\r\n" \
                                        "Content-Length: %u\r\n\r\n";
/* 私有协议自定义写法 */
static const INT8* g_sHandleContinue = "--%s\r\n" \
                                        "Content-Disposition: form-data; id=%x; name=%s\r\n" \
                                        "Content-Type: %s\r\n" \
                                        "Handle-Type: %u\r\n\r\n";

static const INT8* g_sContentEnd = "--%s\r\n";

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
INT32 SAL_packageMessage(StContentData* pContentData, void** outBuf, UINT32* outBufLen)
{
    char* content = NULL;
    UINT32 contentOffset = 0;

    StContentInfo jsonContent;
    StContentInfo binaryContent[MAX_CONTENTBODY_CNT];
    StHandleInfo handleContent[MAX_HANDLEBODY_CNT];
    INT8 contentHead[512] = {0};
    UINT32 contentHeadLen = 0;
    INT8 contentEnd[64] = {0};
    UINT32 contentEndLen = 0;

    UINT32 tailLen = strlen(BINARY_DATA_TAIL);
    UINT32 allContentLen = 0;
    
    INT32 i = 0;

    if (NULL == pContentData || NULL == outBuf || NULL == outBufLen)
    {
        SAL_ERROR("input args error!\n");
        return SAL_FAIL;
    }

    if (NULL == pContentData->jsonData || MAX_CONTENTBODY_CNT < pContentData->contentBodyCnt
        || MAX_HANDLEBODY_CNT < pContentData->handleBodyCnt)
    {
        //printf("--- jsonData %p, kjcontentBodyCnt %u\n", pContentData->jsonData, pContentData->contentBodyCnt);
        SAL_ERROR("input contentData jsonData or bodyCnt error!\n");
        return SAL_FAIL;
    }

    SAL_memSetSafe(&jsonContent, 0, sizeof(jsonContent));
    SAL_memSetSafe(&binaryContent, 0, sizeof(binaryContent));

    jsonContent.data = pContentData->jsonData;
    jsonContent.dataLen = pContentData->jsonDataLen;
    snprintf(jsonContent.contentHead, sizeof(jsonContent.contentHead), g_sContentContinue,
        CONTENT_BOUNDARY, pContentData->currentID, pContentData->jsonDataName, JSON_CONTENT_TYPE_VALUE, jsonContent.dataLen);
    jsonContent.contentHeadLen = strlen(jsonContent.contentHead);

    allContentLen += jsonContent.contentHeadLen;
    allContentLen += jsonContent.dataLen;
    allContentLen += tailLen;

    for (i = 0; i < pContentData->contentBodyCnt; ++i)
    {
        if (NULL == pContentData->contentBodyData || NULL == (pContentData->contentBodyData + i)->data)
        {
            break;
        }

        binaryContent[i].data = (pContentData->contentBodyData + i)->data;
        binaryContent[i].dataLen = (pContentData->contentBodyData + i)->dataLen;

        snprintf(binaryContent[i].contentHead, sizeof(binaryContent[i].contentHead), g_sContentContinue,
            CONTENT_BOUNDARY, pContentData->currentID, (pContentData->contentBodyData + i)->dataName,
            BINARY_CONTENT_TYPE_VALUE, binaryContent[i].dataLen);
        binaryContent[i].contentHeadLen = strlen(binaryContent[i].contentHead);
        
        allContentLen += binaryContent[i].contentHeadLen;
        allContentLen += binaryContent[i].dataLen;
        allContentLen += tailLen;
    }

    // 存在操作失败
    if (i != pContentData->contentBodyCnt)
    {
        SAL_ERROR("input contentBodyData error! %d\n", i);
        return SAL_FAIL;
    }

    for (i = 0; i < pContentData->handleBodyCnt; ++i)
    {
        if (NULL == pContentData->handleBodyData || NULL == (pContentData->handleBodyData + i)->handle)
        {
            break;
        }

        handleContent[i].handle = (pContentData->handleBodyData + i)->handle;
        handleContent[i].handleType = (pContentData->handleBodyData + i)->handleType;

        snprintf(handleContent[i].contentHead, sizeof(handleContent[i].contentHead), g_sHandleContinue,
            CONTENT_BOUNDARY, pContentData->currentID, (pContentData->handleBodyData + i)->handleName,
            HANDLE_CONTENT_TYPE_VALUE, handleContent[i].handleType);
        handleContent[i].contentHeadLen = strlen(handleContent[i].contentHead);
        
        allContentLen += handleContent[i].contentHeadLen;
        allContentLen += tailLen;
    }

    // 存在操作失败
    if (i != pContentData->handleBodyCnt)
    {
        SAL_ERROR("input handleBodyData error! %d\n", i);
        return SAL_FAIL;
    }

    snprintf(contentEnd, sizeof(contentEnd), g_sContentEnd, CONTENT_BOUNDARY);
    contentEndLen = strlen(contentEnd);
    allContentLen += contentEndLen;

    snprintf(contentHead, sizeof(contentHead), g_sContentHead,
        CONTENT_BOUNDARY, allContentLen);
    contentHeadLen = strlen(contentHead);


    // 消息组装
    content = (char*)calloc(1, strlen(contentHead) + allContentLen + 1);
    if (NULL == content)
    {
        SAL_ERROR("memory not enough!\n");
        return SAL_FAIL;
    }
    
    contentOffset = 0;
    SAL_memCpySafe(content + contentOffset, contentHead, contentHeadLen);
    contentOffset += contentHeadLen;

    SAL_memCpySafe(content + contentOffset, jsonContent.contentHead, jsonContent.contentHeadLen);
    contentOffset += jsonContent.contentHeadLen;
    SAL_memCpySafe(content + contentOffset, jsonContent.data, jsonContent.dataLen);
    contentOffset += jsonContent.dataLen;
    SAL_memCpySafe(content + contentOffset, BINARY_DATA_TAIL, tailLen);
    contentOffset += tailLen;

    for (i = 0; i < pContentData->contentBodyCnt; ++i)
    {
        SAL_memCpySafe(content + contentOffset, binaryContent[i].contentHead, binaryContent[i].contentHeadLen);
        contentOffset += binaryContent[i].contentHeadLen;
        SAL_memCpySafe(content + contentOffset, binaryContent[i].data, binaryContent[i].dataLen);
        contentOffset += binaryContent[i].dataLen;
        SAL_memCpySafe(content + contentOffset, BINARY_DATA_TAIL, tailLen);
        contentOffset += tailLen;
    }

    for (i = 0; i < pContentData->handleBodyCnt; ++i)
    {
        SAL_memCpySafe(content + contentOffset, handleContent[i].contentHead, handleContent[i].contentHeadLen);
        contentOffset += handleContent[i].contentHeadLen;
        SAL_memCpySafe(content + contentOffset, BINARY_DATA_TAIL, tailLen);
        contentOffset += tailLen;
    }

    SAL_memCpySafe(content + contentOffset, contentEnd, contentEndLen);
    contentOffset += contentEndLen;

    *outBuf = content;
    *outBufLen = contentOffset;

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_releasePackageData
* 描  述  : 资源释放
* 输  入  : - data: 该数据地址为SAL_packageMessage带出的数据地址
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
void SAL_releasePackageData(void* data)
{
    if (NULL == data)
    {
        return;
    }

    free(data);
    data = NULL;
}



// 解析初始报文头,返回值为解析成功后需要偏移的长度
static UINT32 unpackageContentHead(const INT8* data, UINT32 dataLen, UINT32* outAllContentLen)
{
    INT8 boundary[64] = {0};
    INT8 contentHead[512] = {0};
    UINT32 contentHeadLen = 0;
    UINT32 allContentLen = 0;
    INT32 ret = 0;

    if (NULL == data || 0 == dataLen || NULL == outAllContentLen)
    {
        SAL_ERROR("input args error!\n");
        return 0;
    }

    ret = sscanf_s(data, g_sContentHead, boundary, sizeof(boundary), &allContentLen);
    if (2 != ret)
    {
        SAL_ERROR("data format error!\n");
        return 0;
    }

    if (0 != strcmp(boundary, CONTENT_BOUNDARY))
    {
        SAL_ERROR("boundary error!\n");
        return 0;
    }

    snprintf(contentHead, sizeof(contentHead), g_sContentHead, boundary, allContentLen);
    contentHeadLen = strlen(contentHead);

    *outAllContentLen = allContentLen;

    return contentHeadLen;
}

// 解析每一块的报文头,返回值为解析成功后需要偏移的长度
static UINT32 unpackageContentContinue(INT8* data, UINT32 dataLen, StUnpacketContentInfo* unpacketContentBodyData, BOOL* isJson)
{
    INT8 contentHead[512] = {0};
    INT8 boundary[64] = {0};
    INT32 currentID = 0;
    INT8 name[MAX_CONTENTNAME_LEN] = {0};
    INT8 contentType[64] = {0};
    UINT32 contentHeadLen = 0;
    UINT32 contentBodyLen = 0;
    UINT32 allContentLen = 0;

    int ret = 0;

    if (NULL == data || 0 == dataLen || NULL == unpacketContentBodyData || NULL == isJson)
    {
        SAL_ERROR("input args error!\n");
        return 0;
    }

    ret = sscanf_s(data, g_sContentContinue, boundary, sizeof(boundary), &currentID, name, sizeof(name),contentType, sizeof(contentType),&contentBodyLen);
    if (5 != ret)
    {
        SAL_DEBUG("data format error!\n");
        return 0;
    }

    if (0 != strcmp(boundary, CONTENT_BOUNDARY))
    {
        SAL_ERROR("boundary error!\n");
        return 0;
    }

    snprintf(contentHead, sizeof(contentHead), g_sContentContinue, boundary, currentID, name, contentType, contentBodyLen);
    contentHeadLen = strlen(contentHead);

    // 数据报文头，数据内容，数据尾部BINARY_DATA_TAIL
    allContentLen = contentHeadLen + contentBodyLen + strlen(BINARY_DATA_TAIL);
    if (allContentLen > dataLen)
    {
        SAL_ERROR("data out of range!\n");
        return 0;
    }

    // JSON内容
    if (0 == strcmp(contentType, JSON_CONTENT_TYPE_VALUE))
    {
        *isJson = SAL_TRUE;
    }
    // 二进制数据
    else if (0 == strcmp(contentType, BINARY_CONTENT_TYPE_VALUE))
    {
        *isJson = SAL_FALSE;
    }
    else
    {
        SAL_ERROR("unsupport data type!\n");
        return 0;
    }

    // 数据提末尾BINARY_DATA_TAIL格式判断
    if (0 != strncmp(BINARY_DATA_TAIL, data + contentHeadLen + contentBodyLen, strlen(BINARY_DATA_TAIL)))
    {
        SAL_ERROR("data body tail error!\n");
        return 0;
    }

    unpacketContentBodyData->currentID = currentID;
    unpacketContentBodyData->bodyData.data = data + contentHeadLen;
    unpacketContentBodyData->bodyData.dataLen = contentBodyLen;
    SAL_memCpySafe(unpacketContentBodyData->bodyData.dataName, name, sizeof(name));

    return allContentLen;
}

// 解析每一块的报文头,返回值为解析成功后需要偏移的长度
static UINT32 unpackageHandleContinue(INT8* data, UINT32 dataLen, StUnpacketHandleInfo* unpacketHandleBodyData)
{
    INT8 contentHead[512] = {0};
    INT8 boundary[64] = {0};
    INT32 currentID = 0;
    INT8 name[MAX_CONTENTNAME_LEN] = {0};
    INT8 contentType[64] = {0};
    UINT32 contentHeadLen = 0;
    UINT32 handleType = 0;
    UINT32 allContentLen = 0;

    int ret = 0;

    if (NULL == data || 0 == dataLen || NULL == unpacketHandleBodyData)
    {
        SAL_ERROR("input args error!\n");
        return 0;
    }

    ret = sscanf_s(data, g_sHandleContinue, boundary, sizeof(boundary), &currentID, name, sizeof(name),contentType, sizeof(contentType),&handleType);
    if (5 != ret)
    {
        SAL_ERROR("data format error!\n");
        return 0;
    }

    if (0 != strcmp(boundary, CONTENT_BOUNDARY))
    {
        SAL_ERROR("boundary error!\n");
        return 0;
    }

    snprintf(contentHead, sizeof(contentHead), g_sHandleContinue, boundary, currentID, name, contentType, handleType);
    contentHeadLen = strlen(contentHead);

    // 数据报文头，数据内容，数据尾部BINARY_DATA_TAIL
    allContentLen = contentHeadLen + strlen(BINARY_DATA_TAIL);
    if (allContentLen > dataLen)
    {
        SAL_ERROR("data out of range!\n");
        return 0;
    }

    // 安卓句柄数据
    if (0 != strcmp(contentType, HANDLE_CONTENT_TYPE_VALUE))
    {
        SAL_ERROR("unsupport data type!\n");
        return 0;
    }

    // 数据提末尾BINARY_DATA_TAIL格式判断
    if (0 != strncmp(BINARY_DATA_TAIL, data + contentHeadLen, strlen(BINARY_DATA_TAIL)))
    {
        SAL_ERROR("data body tail error!\n");
        return 0;
    }

    unpacketHandleBodyData->currentID = currentID;
    unpacketHandleBodyData->bodyData.handle = NULL;
    unpacketHandleBodyData->bodyData.handleType = handleType;
    SAL_memCpySafe(unpacketHandleBodyData->bodyData.handleName, name, sizeof(name));

    return allContentLen;
}

/*******************************************************************************
* 函数名  : SAL_unpackageMessage
* 描  述  : 将通信信息解包成JSON消息和二进制内容
* 输  入  : - data: 通信信息数据地址
* 输  出  : - dataLen: 通信信息数据的长度
*         : - outContentData: 解封装成功后带出包含JSON消息和二进制信息的结构体地址
* 返回值  : SAL_SOK:        操作成功
*           SAL_FAIL:       操作失败
*******************************************************************************/
INT32 SAL_unpackageMessage(void* data, UINT32 dataLen, StContentData** outContentData)
{
    StContentData* outData = NULL;
    INT8* jsonData = NULL;
    INT8* binaryData = NULL;
    StContentBodyData* tmpContentBodyData = NULL;
    StHandleBodyData*  tmpHandleBodyData = NULL;

    INT8* tmpData = NULL;
    UINT32 tmpDataLen = 0;
    UINT32 allContentLen = 0;
    UINT32 ret = 0;

    BOOL isJson = SAL_TRUE;
    StUnpacketContentInfo unpacketContentBodyData;
    INT8 contentEnd[128] = {0};
    UINT32 contentEndLen = 0;

    StUnpacketContentInfo unpacketContentBodyDataArray[MAX_CONTENTBODY_CNT];
    StUnpacketHandleInfo unpacketHandleBodyDataArray[MAX_HANDLEBODY_CNT];
    INT32 contentBodyCnt = 0;
    INT32 handleBodyCnt = 0;
    INT32 i = 0;

    if (NULL == data || 0 == dataLen || NULL == outContentData)
    {
        SAL_ERROR("input args error!\n");
        return SAL_FAIL;
    }

    SAL_memSetSafe(&unpacketContentBodyData, 0, sizeof(unpacketContentBodyData));
    SAL_memSetSafe(unpacketContentBodyDataArray, 0, sizeof(StUnpacketContentInfo)*MAX_CONTENTBODY_CNT);
    SAL_memSetSafe(unpacketHandleBodyDataArray, 0, sizeof(StUnpacketHandleInfo)*MAX_HANDLEBODY_CNT);

    snprintf(contentEnd, sizeof(contentEnd), g_sContentEnd, CONTENT_BOUNDARY);
    contentEndLen = strlen(contentEnd);

    // 消息头解析
    tmpData = (char*)data;
    tmpDataLen = dataLen;
    while (' ' == *tmpData)
    {
        tmpDataLen--;
        tmpData++;
    }

    // 解析消息头
    ret = unpackageContentHead((const char*)tmpData, dataLen, &allContentLen);
    if (0 >= ret)
    {
        SAL_ERROR("data parse error!\n");
        return SAL_FAIL;
    }

    tmpData = tmpData + ret;
    tmpDataLen = tmpDataLen - ret;

    // 解析JSON
    ret = unpackageContentContinue(tmpData, tmpDataLen, &unpacketContentBodyData, &isJson);
    if (0 == ret)
    {
        SAL_ERROR("data parse error!\n");
        return SAL_FAIL;
    }

    if (SAL_FALSE == isJson)
    {
        SAL_ERROR("data format error!\n");
        return SAL_FAIL;
    }
    
    tmpData = tmpData + ret;
    tmpDataLen = tmpDataLen - ret;
    if (contentEndLen > tmpDataLen)
    {
        SAL_ERROR("data not enough.\n");
        return SAL_FAIL;
    }

    // 解析二进制数据
    do{
        // 首先判断二进制数据处理后是否只有尾部
        if (contentEndLen == tmpDataLen)
        {
            if (0 == strncmp(tmpData, contentEnd, tmpDataLen))
            {
                // 数据到达尾部
                break;
            }
            else
            {
                SAL_ERROR("data tail error! %s\n", tmpData);
                return SAL_FAIL;
            }
        }

        ret = unpackageContentContinue(tmpData, tmpDataLen, &unpacketContentBodyDataArray[contentBodyCnt], &isJson);
        if (0 == ret)
        {
            SAL_DEBUG("data parse error!\n");
            break;/* 可能接下来需要解析安卓句柄数据 */
        }

        if (unpacketContentBodyDataArray[contentBodyCnt].currentID != unpacketContentBodyData.currentID)
        {
            SAL_ERROR("data parse id error!\n");
            return SAL_FAIL;
        }

        contentBodyCnt++;
        if (contentBodyCnt >= MAX_CONTENTBODY_CNT)
        {
            SAL_ERROR("octet-stream out of cnt!\n");
            return SAL_FAIL;
        }

        if (SAL_TRUE == isJson)
        {
            SAL_ERROR("data format error!\n");
            return SAL_FAIL;
        }
        
        tmpData = tmpData + ret;
        tmpDataLen = tmpDataLen - ret;
        if (contentEndLen > tmpDataLen)
        {
            SAL_ERROR("data not enough.\n");
            return SAL_FAIL;
        }
    }while(1);

    // 解析安卓句柄数据
    do{
        // 首先判断二进制数据处理后是否只有尾部
        if (contentEndLen == tmpDataLen)
        {
            if (0 == strncmp(tmpData, contentEnd, tmpDataLen))
            {
                // 数据到达尾部
                break;
            }
            else
            {
                SAL_ERROR("data tail error! %s\n", tmpData);
                return SAL_FAIL;
            }
        }

        ret = unpackageHandleContinue(tmpData, tmpDataLen, &unpacketHandleBodyDataArray[handleBodyCnt]);
        if (0 == ret)
        {
            SAL_ERROR("data parse error!\n");
            return SAL_FAIL;
        }

        if (unpacketHandleBodyDataArray[handleBodyCnt].currentID != unpacketContentBodyData.currentID)
        {
            SAL_ERROR("data parse id error!\n");
            return SAL_FAIL;
        }

        handleBodyCnt++;
        if (handleBodyCnt >= MAX_HANDLEBODY_CNT)
        {
            SAL_ERROR("x-handle out of cnt!\n");
            return SAL_FAIL;
        }
        
        tmpData = tmpData + ret;
        tmpDataLen = tmpDataLen - ret;
        if (contentEndLen > tmpDataLen)
        {
            SAL_ERROR("data not enough.\n");
            return SAL_FAIL;
        }
    }while(1);

    outData = (StContentData*)calloc(1, sizeof(StContentData));
    if (NULL == outData)
    {
        SAL_ERROR("memory not enough! %lu\n", sizeof(StContentData));
        return SAL_FAIL;
    }
    /* 复制json报文数据 */
    outData->jsonData = (char*)calloc(1, unpacketContentBodyData.bodyData.dataLen + 1);
    if (NULL == outData->jsonData)
    {
        SAL_ERROR("memory not enough! %u\n", unpacketContentBodyData.bodyData.dataLen + 1);
        SAL_releaseMessage(outData);
        outData = NULL;
        return SAL_FAIL;
    }
    SAL_memCpySafe(outData->jsonData, unpacketContentBodyData.bodyData.data, unpacketContentBodyData.bodyData.dataLen);
    outData->jsonData[unpacketContentBodyData.bodyData.dataLen] = '\0';
    outData->currentID = unpacketContentBodyData.currentID;
    outData->jsonDataLen = unpacketContentBodyData.bodyData.dataLen;
    SAL_memCpySafe(outData->jsonDataName, unpacketContentBodyData.bodyData.dataName, sizeof(outData->jsonDataName));
    /* 复制二进制数据 */
    outData->contentBodyCnt = contentBodyCnt;
    if (contentBodyCnt != 0)
    {
        outData->contentBodyData = (StContentBodyData*)calloc(contentBodyCnt, sizeof(StContentBodyData));
        if (NULL == outData->contentBodyData)
        {
            SAL_ERROR("memory not enough!\n");
            SAL_releaseMessage(outData);
            outData = NULL;
            return SAL_FAIL;
        }
        for (i = 0; i < contentBodyCnt; ++i)
        {
            tmpContentBodyData = outData->contentBodyData + i;
            binaryData = calloc(1, unpacketContentBodyDataArray[i].bodyData.dataLen);
            if (NULL == binaryData)
            {
                SAL_ERROR("memory not enough! %u\n", unpacketContentBodyDataArray[i].bodyData.dataLen);
                SAL_releaseMessage(outData);
                outData = NULL;
                return SAL_FAIL;
            }
            SAL_memCpySafe(binaryData, unpacketContentBodyDataArray[i].bodyData.data, unpacketContentBodyDataArray[i].bodyData.dataLen);
            SAL_memCpySafe(tmpContentBodyData->dataName, &unpacketContentBodyDataArray[i].bodyData.dataName,
                sizeof(unpacketContentBodyDataArray[i].bodyData.dataName));
            tmpContentBodyData->data = binaryData;
            tmpContentBodyData->dataLen = unpacketContentBodyDataArray[i].bodyData.dataLen;
        }
    }
    /* 复制安卓句柄数据 */
    outData->handleBodyCnt = handleBodyCnt;
    if (handleBodyCnt != 0)
    {
        outData->handleBodyData = (StHandleBodyData*)calloc(handleBodyCnt, sizeof(StHandleBodyData));
        if (NULL == outData->handleBodyData)
        {
            SAL_ERROR("memory not enough!\n");
            SAL_releaseMessage(outData);
            outData = NULL;
            return SAL_FAIL;
        }
        for (i = 0; i < handleBodyCnt; ++i)
        {
            tmpHandleBodyData = outData->handleBodyData + i;
            SAL_memCpySafe(tmpHandleBodyData->handleName, &unpacketHandleBodyDataArray[i].bodyData.handleName,
                sizeof(unpacketHandleBodyDataArray[i].bodyData.handleName));
            tmpHandleBodyData->handle = unpacketHandleBodyDataArray[i].bodyData.handle;
            tmpHandleBodyData->handleType = unpacketHandleBodyDataArray[i].bodyData.handleType;
        }
    }

    *outContentData = outData;

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_releaseMessage
* 描  述  : 资源释放
* 输  入  : - contentData: 该数据地址为SAL_unpackageMessage带出的结构体数据地址
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
void SAL_releaseMessage(StContentData* contentData)
{
    StContentBodyData* tmpBodyData = NULL;
    StHandleBodyData*  tmpHandleData = NULL;
    INT32 i = 0;
    if (NULL == contentData)
    {
        return;
    }

    if (NULL != contentData->jsonData)
    {
        free(contentData->jsonData);
        contentData->jsonData = NULL;
    }

    if (NULL != contentData->contentBodyData)
    {
        for (i = 0; i < contentData->contentBodyCnt; ++i)
        {
            tmpBodyData = contentData->contentBodyData + i;
            if (NULL != tmpBodyData->data)
            {
                free(tmpBodyData->data);
                SAL_memSetSafe(tmpBodyData, 0, sizeof(*tmpBodyData));
            }
        }
        free(contentData->contentBodyData);
        contentData->contentBodyData = NULL;
    }

    if (NULL != contentData->handleBodyData)
    {
        free(contentData->handleBodyData);
        contentData->handleBodyData = NULL;
    }

    free(contentData);
    contentData = NULL;
}


