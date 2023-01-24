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
 * Version: V1.0.1  2022年11月8日  修改数据结构命名，新增获取链表长度接口
 *******************************************************************************/


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>

/* 链表动态初始化 */
void SAL_listInit(SAL_ListNode *list)
{
    list->next = list;
    list->prev = list;
}

/* 通用添加成员的接口，需指定队列的前节点和后节点。 */
static void __SAL_listAdd(SAL_ListNode *newEle, SAL_ListNode *prev, SAL_ListNode *next)
{
    next->prev      = newEle;
    newEle->next    = next;
    newEle->prev    = prev;
    prev->next      = newEle;
}

/* 添加成员的接口，从链表头部插入，head的next指向队尾，head的prev指向下一个元素。 */
void SAL_listAdd(SAL_ListNode *newEle, SAL_ListNode *head)
{
    __SAL_listAdd(newEle, head, head->next);
}

/* 添加成员的接口，链表尾部插入，head的next指向下一个元素，head的prev指向队尾。 */
void SAL_listAddTail(SAL_ListNode *newEle, SAL_ListNode *head)
{
    __SAL_listAdd(newEle, head->prev, head);
}

/* 从链表中删除成员的通用接口，需要改变该节点前驱节点的后继结点和后继结点的前驱节点，需要指定该成员的队头和队尾。 */
static void __SAL_listDel(SAL_ListNode *prev, SAL_ListNode *next)
{
    next->prev = prev;
    prev->next = next;
}

/*******************************************************************************
* 函数名  : SAL_listDel
* 描  述  : 从链表中删除成员的通用接口，指定要删除的该成员。
* 输  入  : - entry: 删除的成员
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
void SAL_listDel(SAL_ListNode *entry)
{
    __SAL_listDel(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

/* 判断链表是否为空的接口。 */
int SAL_listEmpty(const SAL_ListNode *head)
{
    return head->next == head;
}

/* 获取链表长度 */
int SAL_listLength(const SAL_ListNode* head)
{
    SAL_ListNode* tmp = head->next;
    int count = 0;
    while (NULL != tmp && head != tmp)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}

