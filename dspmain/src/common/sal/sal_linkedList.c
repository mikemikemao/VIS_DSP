/*******************************************************************************
 * sal_list.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2018年12月19日 Create
 *
 * Description : 
 * Modification: 
 *******************************************************************************/


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>
#include <sal_linkedList.h>

/*******************************************************************************
* 函数名  : SAL_memListAddNode
* 描  述  :添加内存链表成员
* 输  入  : - ptNew: 要添加的节点
			-ptList :链表头
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
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
* 函数名  : SAL_memListDelNode
* 描  述  : 删除内存链表成员
* 输  入  : - ptDel: 要删除的节点
			-ptList :链表头
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
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
                /* 从链表中删除 */
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
* 函数名  : SAL_memListAddNode
* 描  述  :对内存链表成员进行排序
* 输  入  : 
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
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
        //一次排序
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
                    //第一次如果有交换，则头指针变换
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

