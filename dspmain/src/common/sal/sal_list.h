/*******************************************************************************
* sal_list.h
* Description: �������ӿڡ�
*******************************************************************************/


#ifndef _SAL_LIST_H_
#define _SAL_LIST_H_


/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */


/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
static inline void prefetch() {;}

#define SAL_ContainerOf(ptr, type, member) ({        \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#define SAL_ListEntry(ptr, type, member) \
    SAL_ContainerOf(ptr, type, member)

#define SAL_ListFirstEntry(ptr, type, member) \
    SAL_ListEntry((ptr)->next, type, member)

#define SAL_ListForEachEntry(pos, head, member)        \
    for (pos = SAL_ListEntry((head)->next, typeof(*pos), member);    \
         prefetch(), &pos->member != (head);    \
         pos = SAL_ListEntry(pos->member.next, typeof(*pos), member))

#define SAL_ListForEachEntryReverse(pos, head, member)        \
    for (pos = SAL_ListEntry((head)->prev, typeof(*pos), member);    \
         prefetch(), &pos->member != (head);    \
         pos = SAL_ListEntry(pos->member.prev, typeof(*pos), member))

#define SAL_ListForEachEntrySafe(pos, n, head, member)        \
    for (pos = SAL_ListEntry((head)->next, typeof(*pos), member),    \
    n = SAL_ListEntry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head);            \
         pos = n, n = SAL_ListEntry(n->member.next, typeof(*n), member))

#define SAL_ListForEachEntrySafeReverse(pos, n, head, member)    \
    for (pos = SAL_ListEntry((head)->prev, typeof(*pos), member),    \
    n = SAL_ListEntry(pos->member.prev, typeof(*pos), member);    \
         &pos->member != (head);            \
         pos = n, n = SAL_ListEntry(n->member.prev, typeof(*n), member))



/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */
/* �����㶨�� */
typedef struct SAL_ListNode
{
    struct SAL_ListNode* next;
    struct SAL_ListNode* prev;
}SAL_ListNode;

/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */
/* ����̬��ʼ�� */
void SAL_listInit(SAL_ListNode *list);

/* ��ӳ�Ա�Ľӿڣ�head��nextָ���β��head��prevָ����һ��Ԫ�ء� */
void SAL_listAdd(SAL_ListNode *newEle, SAL_ListNode *head);

/* ��ӳ�Ա�Ľӿڣ�head��nextָ����һ��Ԫ�أ�head��prevָ���β�� */
void SAL_listAddTail(SAL_ListNode *newEle, SAL_ListNode *head);

/* ��������ɾ����Ա��ͨ�ýӿڣ�ָ���ó�Ա�� */
void SAL_listDel(SAL_ListNode *entry);

/* �ж������Ƿ�Ϊ�յĽӿڡ� */
int SAL_listEmpty(const SAL_ListNode *head);

/* ��ȡ������ */
int SAL_listLength(const SAL_ListNode* head);

#endif    /* _SAL_LIST_H_ */


