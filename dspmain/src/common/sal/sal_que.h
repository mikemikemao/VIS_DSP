
#ifndef _SAL_QUE_H_
#define _SAL_QUE_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

    
/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_queCreate
* 描  述  : 创建一个队列
            队列本体是一个指针数组，内部可存放maxLen个数的指针数据
* 输  入  : - phQue  : 队列结构体
*         : - maxLen : 最大队列长度
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QueCreate(Handle *phQue, unsigned int maxElems);

/*******************************************************************************
* 函数名  : SAL_queDelete
* 描  述  : 销毁一个队列
* 输  入  : - hQue: 队列结构体
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QueDelete(Handle hQue);

/*******************************************************************************
* 函数名  : SAL_quePut
* 描  述  : 往对列中放数据
* 输  入  : - hQue   : 队列句柄。
*         : - pElem  : 放入队列的数据指针。队列成员。
*         : - timeout: 超时时间。单位是毫秒。SAL_TIMEOUT_NONE 表示不等待，SAL_TIMEOUT_FOREVER 表示无限等待。
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QuePut(Handle hQue, void *pElem, unsigned int timeout);


/*******************************************************************************
* 函数名  : SAL_queGet
* 描  述  : 从队列中获取一个成员数据
* 输  入  : - hQue   : 队列句柄。
*         : - ppElem : 从队列中获取地址的二级指针。获取上来的成员。
*         : - timeout: 超时时间。单位是毫秒。SAL_TIMEOUT_NONE 表示不等待，SAL_TIMEOUT_FOREVER 表示无限等待。
* 输  出  : 无
* 返回值  : SAL_SOK  : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
int SAL_QueGet(Handle hQue, void **ppElem, unsigned int timeout);

/*******************************************************************************
* 函数名  : SAL_QueGetQueuedCount
* 描  述  : 获取队列当前已经缓存的成员数量。          
* 输  入  : - hQue: 队列句柄。
* 输  出  : 无。
* 返回值  : SAL_SOK  : 成功。
*           SAL_FAIL: 失败。
*******************************************************************************/
unsigned int SAL_QueGetQueuedCount(Handle hQue);


/*******************************************************************************
* 函数名  : SAL_queIsEmpty
* 描  述  : 判断队列是否为空。          
* 输  入  : - hQue: 队列句柄。
* 输  出  : 无。
* 返回值  : SAL_SOK  : 成功。
*           SAL_FAIL: 失败。
*******************************************************************************/
int SAL_QueIsEmpty(Handle hQue);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _SAL_QUE_H_ */

