/*******************************************************************************
* _SAL_MEMLIST_H_
* Description: �����ڴ�����ӿڡ�
*******************************************************************************/
#include <sal.h>
#include <sal_linkedList.h>

#ifndef _SAL_MEMLIST_H_
#define _SAL_MEMLIST_H_

/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */


/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
#define LIST_CONT_LEN   32   /* ������ڴ�����Ƴ��� */   

typedef enum
{
    CHECK_PATTERN_ORDER   = 0,
    CHECK_PATTERN_SIZE    = 1,
    CHECK_PATTERN_MODULE  = 2,
    CHECK_PATTERN_MAX,                /* Ԥ�� */
}CHECK_PATTERN;

/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */
/* ����ͷ */
typedef struct SAL_ListMHead
{
    char modelName[LIST_CONT_LEN]; /* ������ڴ������ */

    SAL_MemBlockObject salListHead;         /* �ڴ��ͷ��Ϣ�ṹ�� */
    SAL_MemBlockObject salListEnd;          /* �ڴ��β��Ϣ�ṹ�� */
  
    List_head salList;
}TSAL_ListMemHead;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_memListCheck
* ��  ��  : ���������ӿ�
* ��  ��  : - void:
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
void SAL_memListCheck(INT32 checkPattern);

/*******************************************************************************
* ������  : SAL_memListAdd
* ��  ��  : �������ӽڵ�
* ��  ��  : - pBufHead: 
*           - pBufEnd: 
*           - size��
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_memListAdd(SAL_MemBlockObject *pBufHead, SAL_MemBlockObject *pBufEnd,  UINT32 size, const char *modId);

/*******************************************************************************
* ������  : SAL_memListDel
* ��  ��  : ɾ���ڵ�
* ��  ��  : - pBufHead: 
* ��  ��  : �ޡ�
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_memListDel(SAL_MemBlockObject *pBufHead);

 /*******************************************************************************
 * ������  : SAL_memList
 * ��  ��  : ��ȡ�ڴ��������
 * ��  ��  : - VOID:
 * ��  ��  : ��
 * ����ֵ  : SAL_SOK  : �ɹ�
 *           SAL_FAIL : ʧ��
 *******************************************************************************/
 List_head *SAL_memList(VOID);

 /*******************************************************************************
 * ������  : SAL_memSetListHead
 * ��  ��  : ��������ͷ���
 * ��  ��  : - VOID:
 * ��  ��  : ��
 * ����ֵ  : SAL_SOK  : �ɹ�
 *           SAL_FAIL : ʧ��
 *******************************************************************************/
 void SAL_memSetListHead(List_head * pListHead);

 /*******************************************************************************
 * ������  : SAL_memListSortSize
 * ��  ��  :���ڴ�������Ա��������
 * ��  ��  : 
 * ��  ��  : ��
 * ����ֵ  : HIK_SOK  : �ɹ�
 *             HIK_FAIL : ʧ��
 *******************************************************************************/
UINT32 SAL_memListSortSize (List_head* p1,List_head* p2);
 /*******************************************************************************
* ������    : SAL_memListSortName
* ��    ��    :���ڴ�����name��������
* ��    ��    : 
* ��    ��    : ��
* ����ֵ    : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
 UINT32 SAL_memListSortName(List_head* p1,List_head* p2);
 
 /*******************************************************************************
 * ������  : SAL_memListCheckToFile
 * ��  ��  : ��������
 * ��  ��  : ��
 * ��  ��  : ��
 * ����ֵ  : HIK_SOK  : �ɹ�
 *           HIK_FAIL : ʧ��
 *******************************************************************************/
 void SAL_memListCheckToFile(INT32 checkPattern);

#endif                                  /* _SAL_MEMLIST_H_*/


