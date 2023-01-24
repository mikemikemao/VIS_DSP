/*******************************************************************************
 * sal_list.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2018��12��19�� Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/


/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>
#include <sal_linkedList.h>

/*******************************************************************************
* ������  : SAL_memListAddNode
* ��  ��  :����ڴ������Ա
* ��  ��  : - ptNew: Ҫ��ӵĽڵ�
			-ptList :����ͷ
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_memListAddNode(List_head *ptListNew, List_head *ptListHead)
{
    List_head *ptList = NULL;
    if (NULL == ptListNew)
    {
        SAL_ERROR("ptListNews is NULL\n");
        return SAL_FAIL;
    }    
     if (NULL == ptListHead)
    {
        SAL_ERROR("ptListHead is NULL\n");
        return SAL_FAIL;
    }
 
    ptList = ptListHead;
    while (ptList->next)
    {
        ptList = ptList->next;
    }
    ptList->next = ptListNew;
    ptListNew->pre  = ptList;
    return SAL_SOK;
}
/*******************************************************************************
* ������  : SAL_memListDelNode
* ��  ��  : ɾ���ڴ������Ա
* ��  ��  : - ptDel: Ҫɾ���Ľڵ�
			-ptList :����ͷ
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_memListDelNode(List_head *ptListDel, List_head *ptList)
{
    List_head * ptPre  = NULL;
    List_head * ptNext = NULL;

    if (NULL == ptListDel)
    {
        SAL_ERROR("ptDel is NULL\n");
        return SAL_FAIL;
    }
    if (NULL == ptList)
    {
        SAL_ERROR("ptList is NULL\n");
        return SAL_FAIL;
    }


    if (ptList == ptListDel)
    {
        ptList = ptListDel->next;
        return SAL_FAIL;
    }
    else
    {
        ptList = ptList->next;
        while (ptList)
        {
            if (ptList == ptListDel)
            {
                /* ��������ɾ�� */
                ptPre  = ptList->pre;
                ptNext = ptList->next;
                ptPre->next = ptNext;
                if (ptNext)
                {
                    ptNext->pre = ptPre;
                }
                
                break;
            }
            else
            {
                ptList = ptList->next;
            }
        }
    }
    return SAL_SOK;
}

/*******************************************************************************
* ������  : SAL_memListAddNode
* ��  ��  :���ڴ������Ա��������
* ��  ��  : 
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
List_head * SAL_memListSortNode(Sort Sortfunc)
{
    List_head *ptCur = NULL;
    List_head* p1,*p2,*pre;
    INT32 firstFlag =0;
    ptCur = SAL_memList();
    if(NULL == ptCur)
    {
        return NULL;
    }
    List_head* rear=ptCur;
    List_head* L=(List_head*)calloc(1,sizeof(List_head));
    if(L==NULL)
    {
        return NULL;
    }

    L->next=ptCur;
    while(rear->next!=NULL)
    {
        rear=rear->next;
    }

    while(rear!=ptCur)
    {
        pre=L;
        p1=L->next;
        p2=p1->next;
        //һ������
        while(p2!=rear->next&&p2!=NULL)
        {
            if(SAL_SOK == Sortfunc(p1,p2))
            {
                //swap p1,p2
                pre->next=p1->next;
                p1->next=p2->next;
                p2->next=p1;
                if(!firstFlag)
                {
                    //��һ������н�������ͷָ��任
                    ptCur = pre->next;
                    firstFlag = 1;
                }
                //next node
                pre=pre->next;
                p2=p1->next;
            }
            else
            {
                pre=pre->next;
                p1=p1->next;
                p2=p2->next;
            }
        }
        firstFlag = 0;
        rear=pre;
    }
    SAL_memSetListHead(L->next);
    free(L);
    return SAL_memList();
}

