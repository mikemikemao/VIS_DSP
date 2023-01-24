/*******************************************************************************
 * sal_json.h
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : zhangyulong8 <zhangyulong8@hikvision.com>
 * Version: V1.0.0  2020年12月9日 Create
 *
 * Description : JSON字符串的解析与创建功能
 * Modification:
 *******************************************************************************/

#ifndef _SAL_JSON_H_
#define _SAL_JSON_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "sal_macro.h"
#include "sal_type.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
// JSON Types
#define JSON_False 0
#define JSON_True 1
#define JSON_NULL 2
#define JSON_Number 3
#define JSON_String 4
#define JSON_Array 5
#define JSON_Object 6


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct _SAL_Json {
    struct _SAL_Json *next;
    struct _SAL_Json *prev;
    struct _SAL_Json *child;    // 该Json的值是一个对象或者数组数据

    INT32 jsonType;             // 当前Json的数据类型

    INT8 *strValue;             // JSON键值对的字符串值，该内存由内部分配
    INT32 intValue;             // JSON键值对的int值
    FLOAT64 doubleValue;        // JSON键值对的double值

    INT8 *key;                  // Json键值对的键信息，该内存由内部分配
}SAL_Json;


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
/*******************************************************************************
* 函数名  : SAL_jsonFree
* 描  述  : 释放JSON的地址
* 输  入  : - ptr : 需要释放的内存你地址
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
VOID SAL_jsonFree(void* ptr);

/*******************************************************************************
* 函数名  : SAL_jsonDelete
* 描  述  : 销毁JSON对象
* 输  入  : - item   : JSON对象
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
VOID SAL_jsonDelete(SAL_Json* item);

/*******************************************************************************
* 函数名  : SAL_jsonRead
* 描  述  : JSON字符串解析为JSON对象
* 输  入  : - str   : JSON字符串
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值JSON对象
*           NULL : 失败
*******************************************************************************/
SAL_Json* SAL_jsonRead(const INT8 *str);

/*******************************************************************************
* 函数名  : SAL_jsonWrite
* 描  述  : JSON对象转为字符串
* 输  入  : - root   : JSON对象
*         : - fmt : 是否需要格式化输出(Json字符串有空格和换行，方便目测数据)，0表示不格式化输出(省内存)，非0表示需要格式化输出
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值字符串使用完毕后，需要调用函数释放资源
*           NULL : 失败
*******************************************************************************/
INT8* SAL_jsonWrite(const SAL_Json* root, INT32 fmt);

/*******************************************************************************
* 函数名  : SAL_jsonCreateArray
* 描  述  : 创建一个JSON数组类型空数据对象
* 输  入  : - 无
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值JSON数组类型空数据对象
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonCreateArray(VOID);

/*******************************************************************************
* 函数名  : SAL_jsonCreateObject
* 描  述  : 创建一个JSON对象类型空数据对象
* 输  入  : - 无
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值JSON对象类型空数据对象
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonCreateObject(VOID);

/*******************************************************************************
* 函数名  : SAL_jsonCreateIntArray
* 描  述  : 创建一个JSON整型数组类型对象
* 输  入  : - numbers : 整型数据数组
*         : - count : 整型数据个数
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值JSON整型数组类型对象
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonCreateIntArray(const INT32 *numbers, INT32 count);

/*******************************************************************************
* 函数名  : SAL_jsonCreateDoubleArray
* 描  述  : 创建一个JSON浮点数组类型对象
* 输  入  : - numbers : 浮点数据数组
*         : - count : 浮点数据个数
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值JSON浮点数组类型对象
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonCreateDoubleArray(const FLOAT64 *numbers, INT32 count);

/*******************************************************************************
* 函数名  : SAL_jsonCreateStringArray
* 描  述  : 创建一个JSON浮点数组类型对象
* 输  入  : - numbers : 浮点数据数组
*         : - count : 浮点数据个数
* 输  出  : 无
* 返回值  : 非NULL  : 成功，返回值JSON浮点数组类型对象
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonCreateStringArray(const INT8 **strings, INT32 count);

/*******************************************************************************
* 函数名  : SAL_jsonAddItemToArray
* 描  述  : JSON数组类型对象添加一个元素
* 输  入  : - array : JSON数组类型对象
*         : - item : JSON任意数据对象
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddItemToArray(SAL_Json *array, SAL_Json *item);

/*******************************************************************************
* 函数名  : SAL_jsonAddItemToObject
* 描  述  : JSON对象类型对象添加一个元素
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
*         : - item : JSON任意数据对象
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddItemToObject(SAL_Json *object, const INT8 *name, SAL_Json *item);

/*******************************************************************************
* 函数名  : SAL_jsonAddNullToObject
* 描  述  : JSON对象类型对象添加一个元素
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddNullToObject(SAL_Json *object, const INT8 *name);

/*******************************************************************************
* 函数名  : SAL_jsonAddTrueToObject
* 描  述  : JSON对象类型对象添加一个元素
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddTrueToObject(SAL_Json *object, const INT8 *name);

/*******************************************************************************
* 函数名  : SAL_jsonAddFalseToObject
* 描  述  : JSON对象类型对象添加一个元素
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddFalseToObject(SAL_Json *object, const INT8 *name);

/*******************************************************************************
* 函数名  : SAL_jsonAddBoolToObject
* 描  述  : JSON对象类型对象添加一个元素
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
*         : - b : bool值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddBoolToObject(SAL_Json *object, const INT8 *name, BOOL b);

/*******************************************************************************
* 函数名  : SAL_jsonAddIntNumberToObject
* 描  述  : JSON对象类型对象添加一个整型数
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
*         : - n : 整型值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddIntNumberToObject(SAL_Json *object, const INT8 *name, INT32 n);

/*******************************************************************************
* 函数名  : SAL_jsonAddDoubleNumberToObject
* 描  述  : JSON对象类型对象添加一个浮点数
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
*         : - n : 浮点值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddDoubleNumberToObject(SAL_Json *object, const INT8 *name, FLOAT64 n);

/*******************************************************************************
* 函数名  : SAL_jsonAddStringToObject
* 描  述  : JSON对象类型对象添加一个字符数据
* 输  入  : - object : JSON对象类型对象
*         : - name : 需要新增的对象数据key值
*         : - n : 字符数据
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonAddStringToObject(SAL_Json *object, const INT8 *name, const INT8 *s);

/*******************************************************************************
* 函数名  : SAL_jsonDetachItemFromArray
* 描  述  : 从JSON数组类型对象移出指定位置的JSON对象
* 输  入  : - array : JSON数组类型对象
*         : - which : 需要移出的数据位置
* 输  出  : 无
* 返回值  : 非NULL : 成功，移出的数据，需要手动释放资源
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonDetachItemFromArray(SAL_Json *array, INT32 which);

/*******************************************************************************
* 函数名  : SAL_jsonDeleteItemFromArray
* 描  述  : 从JSON数组类型对象删除指定位置的JSON对象
* 输  入  : - array : JSON数组类型对象
*         : - which : 需要移出的数据位置
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonDeleteItemFromArray(SAL_Json *array, INT32 which);

/*******************************************************************************
* 函数名  : SAL_jsonDetachItemFromArray
* 描  述  : 从JSON数组类型对象移出指定位置的JSON对象
* 输  入  : - array : JSON数组类型对象
*         : - name : 需要移出的数据key值
* 输  出  : 无
* 返回值  : 非NULL : 成功，移出的数据，需要手动释放资源
*           NULL : 失败
*******************************************************************************/
SAL_Json *SAL_jsonDetachItemFromObject(SAL_Json *object, const INT8 *name);

/*******************************************************************************
* 函数名  : SAL_jsonDeleteItemFromObject
* 描  述  : 从JSON数组类型对象删除指定位置的JSON对象
* 输  入  : - array : JSON数组类型对象
*         : - name : 需要删除的数据key值
* 输  出  : 无
* 返回值  : SAL_SOK : 成功
*           SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonDeleteItemFromObject(SAL_Json *object, const INT8 *name);

/*******************************************************************************
* 函数名  : SAL_jsonGetItemFromObject
* 描     述  : 从JSON对象中找到指定的子对象
* 输     入  : - *pObject : JSON对象
*                   : - pKeyName : 需要获取的子对象key值
* 输     出  : - **ppItem:获取得到的子对象
* 返回值  : SAL_SOK : 成功
*                     SAL_FAIL : 失败
*******************************************************************************/
INT32 SAL_jsonGetItemFromObject(SAL_Json *pObject, const INT8 *pKeyName, SAL_Json **ppItem);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _SAL_LIST_H_
