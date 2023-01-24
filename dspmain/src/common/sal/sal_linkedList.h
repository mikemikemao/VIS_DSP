/*******************************************************************************
* sal_list.h
* Description: 链表管理接口。
*******************************************************************************/


#ifndef _SAL_LINKEDLIST_H_
#define _SAL_LINKEDLIST_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
/* 链表头 */
typedef struct list_head
{  
    struct list_head *pre;              /* 链表的前一个节点 */  
    struct list_head *next;             /* 链表的后一个节点 */ 
}List_head;

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

typedef UINT32 (*Sort)(List_head* p1,List_head* p2);

/*******************************************************************************
* 函数名  : SAL_memListAddNode
* 描  述  :对内存链表成员进行排序
* 输  入  : 
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
List_head * SAL_memListSortNode(Sort Sortfunc);

/*******************************************************************************
* 函数名  : SAL_memListAddNode
* 描  述  : 添加成员的接口
* 输  入  : - ptNew:
			-ptList :链表头
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_memListAddNode(List_head *ptListNew,List_head *ptList);

/*******************************************************************************
* 函数名  : SAL_memListDelNode
* 描  述  : 从链表中删除成员的通用接口，指定该成员。
* 输  入  : - ptDel:
			-ptList :链表头
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_memListDelNode(List_head * ptDel,List_head *ptList);


#endif       

