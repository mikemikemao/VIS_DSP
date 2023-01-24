/*******************************************************************************
* sal_list.h
* Description: 链表管理接口。
*******************************************************************************/


#ifndef _SAL_LIST_H_
#define _SAL_LIST_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
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
/*                          数据结构定义区                                    */
/* ========================================================================== */
/* 链表结点定义 */
typedef struct SAL_ListNode
{
    struct SAL_ListNode* next;
    struct SAL_ListNode* prev;
}SAL_ListNode;

/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */
/* 链表动态初始化 */
void SAL_listInit(SAL_ListNode *list);

/* 添加成员的接口，head的next指向队尾，head的prev指向下一个元素。 */
void SAL_listAdd(SAL_ListNode *newEle, SAL_ListNode *head);

/* 添加成员的接口，head的next指向下一个元素，head的prev指向队尾。 */
void SAL_listAddTail(SAL_ListNode *newEle, SAL_ListNode *head);

/* 从链表中删除成员的通用接口，指定该成员。 */
void SAL_listDel(SAL_ListNode *entry);

/* 判断链表是否为空的接口。 */
int SAL_listEmpty(const SAL_ListNode *head);

/* 获取链表长度 */
int SAL_listLength(const SAL_ListNode* head);

#endif    /* _SAL_LIST_H_ */


