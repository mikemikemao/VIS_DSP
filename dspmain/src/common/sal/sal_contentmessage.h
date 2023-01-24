/*******************************************************************************
 * sal_contentmessage.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhangyulong8 <zhangyulong8@hikvision.com>
 * Version: V1.0.0  2021��3��16�� Create
 *
 * Description :
 * Modification:
 *******************************************************************************/

#ifndef __SAL_CONTENTMESSAGE_H__
#define __SAL_CONTENTMESSAGE_H__

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */

#include "sal.h"


#ifdef __cplusplus
extern "C"
{
#endif //

/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
#define     MAX_CONTENTBODY_CNT         10
#define     MAX_CONTENTNAME_LEN         128
#define     MAX_HANDLEBODY_CNT          10

/*��ӦStHandleBodyData handleType����*/
#define     HANDLE_TYPE_SURFACE         0    /* ��׿��ʾ���ھ�� */
#define     HANDLE_TYPE_GRAPHIC         1    /* ��׿ͼ����     */

typedef struct
{
    void* data;
    UINT32 dataLen;
    INT8 dataName[MAX_CONTENTNAME_LEN];     // �������ֶ���������
}StContentBodyData;

typedef struct
{
    void* handle;
    int   handleType;
    INT8 handleName[MAX_CONTENTNAME_LEN];     // �������־������
}StHandleBodyData;

typedef struct
{
    INT32 currentID;
    INT8* jsonData;
    UINT32 jsonDataLen;
    INT8 jsonDataName[MAX_CONTENTNAME_LEN]; // Э���URL
    INT8 method[MAX_CONTENTNAME_LEN]; // Э���method

    UINT32 contentBodyCnt;
    StContentBodyData* contentBodyData;
    UINT32 handleBodyCnt;
    StHandleBodyData* handleBodyData;
}StContentData;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */
/*******************************************************************************
* ������  : SAL_packageMessage
* ��  ��  : ��JSON��Ϣ�Ͷ��������ݴ�����µ�ͨ����Ϣ
* ��  ��  : - pContentData: ����JSON��Ϣ�Ͷ�������Ϣ�Ľṹ���ַ
* ��  ��  : - outBuf: ��װ�ɹ����������
*         : - outBufLen: ��װ�ɹ�����������ݳ���
* ����ֵ  : SAL_SOK:        �����ɹ�
*           SAL_FAIL:       ����ʧ��
*******************************************************************************/
INT32 SAL_packageMessage(StContentData* pContentData, void** outBuf, UINT32* outBufLen);

/*******************************************************************************
* ������  : SAL_releasePackageData
* ��  ��  : ��Դ�ͷ�
* ��  ��  : - data: �����ݵ�ַΪSAL_packageMessage���������ݵ�ַ
* ��  ��  : ��
* ����ֵ  : ��
*******************************************************************************/
void SAL_releasePackageData(void* data);

/*******************************************************************************
* ������  : SAL_unpackageMessage
* ��  ��  : ��ͨ����Ϣ�����JSON��Ϣ�Ͷ���������
* ��  ��  : - data: ͨ����Ϣ���ݵ�ַ
* ��  ��  : - dataLen: ͨ����Ϣ���ݵĳ���
*         : - outContentData: ���װ�ɹ����������JSON��Ϣ�Ͷ�������Ϣ�Ľṹ���ַ
* ����ֵ  : SAL_SOK:        �����ɹ�
*           SAL_FAIL:       ����ʧ��
*******************************************************************************/
INT32 SAL_unpackageMessage(void* data, UINT32 dataLen, StContentData** outContentData);

/*******************************************************************************
* ������  : SAL_releaseMessage
* ��  ��  : ��Դ�ͷ�
* ��  ��  : - contentData: �����ݵ�ַΪSAL_unpackageMessage�����Ľṹ�����ݵ�ַ
* ��  ��  : ��
* ����ֵ  : ��
*******************************************************************************/
void SAL_releaseMessage(StContentData* contentData);

#ifdef __cplusplus
}
#endif //

#endif // __SAL_CONTENTMESSAGE_H__

