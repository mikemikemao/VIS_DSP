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
 * Version: V1.0.1  2022��11��8��  �޸����ݽṹ������������ȡ�����Ƚӿ�
 *******************************************************************************/


/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */
#include <sal.h>

/* ����̬��ʼ�� */
void SAL_listInit(SAL_ListNode *list)
{
    list->next = list;
    list->prev = list;
}

/* ͨ����ӳ�Ա�Ľӿڣ���ָ�����е�ǰ�ڵ�ͺ�ڵ㡣 */
static void __SAL_listAdd(SAL_ListNode *newEle, SAL_ListNode *prev, SAL_ListNode *next)
{
    next->prev      = newEle;
    newEle->next    = next;
    newEle->prev    = prev;
    prev->next      = newEle;
}

/* ��ӳ�Ա�Ľӿڣ�������ͷ�����룬head��nextָ���β��head��prevָ����һ��Ԫ�ء� */
void SAL_listAdd(SAL_ListNode *newEle, SAL_ListNode *head)
{
    __SAL_listAdd(newEle, head, head->next);
}

/* ��ӳ�Ա�Ľӿڣ�����β�����룬head��nextָ����һ��Ԫ�أ�head��prevָ���β�� */
void SAL_listAddTail(SAL_ListNode *newEle, SAL_ListNode *head)
{
    __SAL_listAdd(newEle, head->prev, head);
}

/* ��������ɾ����Ա��ͨ�ýӿڣ���Ҫ�ı�ýڵ�ǰ���ڵ�ĺ�̽��ͺ�̽���ǰ���ڵ㣬��Ҫָ���ó�Ա�Ķ�ͷ�Ͷ�β�� */
static void __SAL_listDel(SAL_ListNode *prev, SAL_ListNode *next)
{
    next->prev = prev;
    prev->next = next;
}

/*******************************************************************************
* ������  : SAL_listDel
* ��  ��  : ��������ɾ����Ա��ͨ�ýӿڣ�ָ��Ҫɾ���ĸó�Ա��
* ��  ��  : - entry: ɾ���ĳ�Ա
* ��  ��  : ��
* ����ֵ  : HIK_SOK  : �ɹ�
*           HIK_FAIL : ʧ��
*******************************************************************************/
void SAL_listDel(SAL_ListNode *entry)
{
    __SAL_listDel(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

/* �ж������Ƿ�Ϊ�յĽӿڡ� */
int SAL_listEmpty(const SAL_ListNode *head)
{
    return head->next == head;
}

/* ��ȡ������ */
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

