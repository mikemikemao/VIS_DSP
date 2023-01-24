/*******************************************************************************
 * sal_mem.c
 *
 * HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *
 * Author : heshengyuan <heshengyuan@hikvision.com>
 * Version: V1.0.0  2015年2月9日 Create
 *
 * Description : 系统抽象层的内存管理文件, 进行内存分配、内存拷贝等内存操作的封装
 * Modification: DSP程序使用malloc申请用户态内存，并进行统计
 *******************************************************************************/
 
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <sal.h>
#include <malloc.h>
#include <sal_memList.h>

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                            全局变量定义区                                  */
/* ========================================================================== */

static UINT32 mallocLen = 0;

/* ========================================================================== */
/*                          函数定义区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : SAL_memUAllocAlign
* 描  述  : 该函数负责在linux用户态中申请内存
*
* 输  入  : - size:  要申请的内存大小
*           - align: 用户自定义对齐的字节数, 若为0表示不进行自定义对齐
*                    该参数必须为4的整数倍, 否则函数将返回NULL
*        - modId: 模块索引
* 输  出  : 无。
* 返回值  : 非NULL: 申请成功
*           NULL:   申请失败
*******************************************************************************/
static Ptr SAL_memUAllocAlign(UINT32 size, UINT32 align, const char* modId)
{
    UINT32 allocSize = 0;
    UINT32 totalSize = 0;
    UINT32 headSize = 0;
    UINT32 endSize  = 0;
    Ptr pBufBase = NULL;
    Ptr pBufData = NULL;
    SAL_MemBlockObject *pBufHead = NULL;
    SAL_MemBlockObject *pBufEnd = NULL;
    if(0 == size)
    {
        SAL_ERROR("size must > 0\n");
        return NULL;
    }

    /* 检查align 必须为4的倍数 */
    if(0 != (align & 3))
    {
        SAL_ERROR("invalid align:%d\n", align);
        return NULL;
    }

    /* 计算头大小 */
    headSize = sizeof(SAL_MemBlockObject);

    /* 计算尾大小 */
    endSize  = sizeof(SAL_MemBlockObject);

    /* 总的要申请的内存大小 */
    totalSize = size + align + headSize + endSize;

    allocSize = SAL_align(totalSize, align);

    /* 申请内存 */
    pBufBase = malloc(allocSize);
    if(NULL == pBufBase)
    {
        SAL_ERROR("alloc failed, size:%d, align:%d\n", size, align);
        return NULL;
    }

    SAL_memSetSafe(pBufBase, 0, allocSize);
    /* 计算数据段的位置，内存的前面存放内存使用的类型，后面才是数据区 */
    pBufData = (void *)((PhysAddr)pBufBase + headSize);
    if(0 != align)
    {
        pBufData = (Ptr)SAL_align((PhysAddr)pBufData, align);
    }

    /* 获取头的位置 */
    pBufHead = SAL_memDataToHead(pBufData);

    /* 填充头 */
    pBufHead->nMgicNum  = SAL_MEM_MAGIC;
    pBufHead->size      = allocSize;
    pBufHead->pBufBase  = pBufBase;

    /* 获取尾的位置 */
    pBufEnd = (SAL_MemBlockObject *)((PhysAddr)pBufBase + (allocSize - endSize));

    /* 填充尾*/
    pBufEnd->nMgicNum  = SAL_MEM_MAGIC;
    pBufEnd->size      = allocSize;
    pBufEnd->pBufBase  = pBufBase;
    
    #ifdef MEMLISTON
    if (SAL_FAIL == SAL_memListAdd(pBufHead, pBufEnd, size, modId))
    {
        SAL_ERROR("SAL_memListAdd error\n");
    }
    #endif

    mallocLen += (allocSize);

    /*返回数据段指针*/
    return pBufData;
}

/*******************************************************************************
* 函数名  : SAL_memAlloc
* 描  述  : 软件抽象层统一的内存申请函数
* 输  入  : - size: 内存大小
*        - modId: 模块索引
* 输  出  : 无
* 返回值  : HIK_SOK  : 成功
*           HIK_FAIL : 失败
*******************************************************************************/
Ptr SAL_memAlloc(UINT32 size, const char *modId)
{
    return SAL_memUAllocAlign((size), 4, modId);
}

/*******************************************************************************
* 函数名  : SAL_memCalloc
* 描  述  : 该函数在SAL_memUAllocAlign的基础上,增加对内存清0的动作
* 输  入  : - size:  要申请的内存大小
*        - modId: 模块索引
* 输  出  : 无。
* 返回值  : 非NULL:  申请成功
*           NULL:    申请失败
*******************************************************************************/
Ptr SAL_memCalloc(UINT32 size)
{
    Ptr pPtr = SAL_memUAllocAlign(size, 4, "Sal");
    if(pPtr)
    {
        SAL_clearSize(pPtr, size);
    }
    return pPtr;
}

/*******************************************************************************
* 函数名  : SAL_allocMemFree
* 描  述  : 系统抽象层下的内存申请封装函数，负责负责释放内存并统计更新当前系统总计使用的内存
* 输  入  : - pPtr:    要释放的内存地址
* 输  出  : 无
* 返回值  : SAL_SOK:   成功,内存已释放
*           SAL_FAIL: 失败, 内存未释放
*******************************************************************************/
INT32 SAL_allocMemFree(Ptr pPtr)
{
    SAL_MemBlockObject *pBufHead = NULL;

    if(NULL == pPtr)
    {
        return SAL_FAIL;
    }

    pBufHead = SAL_memDataToHead(pPtr);

    /* 校验魔数 */
    if(SAL_MEM_MAGIC != pBufHead->nMgicNum)
    {
        SAL_ERROR("invalid magic:0x%x\n", pBufHead->nMgicNum);
        return SAL_FAIL;
    }

    /* 释放内存块时把魔数置为-1 */
    pBufHead->nMgicNum = (UINT32) - 1;

    mallocLen -= (pBufHead->size);
    #ifdef MEMLISTON
    
    if (SAL_FAIL == SAL_memListDel(pBufHead))
    {
        SAL_ERROR("SAL_memListDel ERROR!!!\n");
    }
    #endif
    
    /* 释放内存 */
    free(pBufHead->pBufBase);

    return SAL_SOK;
}

/*******************************************************************************
* 函数名  : SAL_memAlign
* 描  述  : 系统抽象层下的内存申请封装函数，负责申请内存并统计下来，能够保证申请的内存有字节对齐的效果
* 输  入  : - alignSize: 强制字节对齐的长度
*         : - size     : 申请大小
*         : - modId    : 模块标识
* 输  出  : 无
* 返回值  : ptr  : 申请成功的内存地址
*           NULL : 申请失败
*******************************************************************************/
void *SAL_memAlign(UINT32 size, UINT32 alignSize, const char *modId)
{
    UINT32 allocSize = 0;
    UINT32 totalSize = 0;
    UINT32 headSize = 0;
    UINT32 endSize  = 0;
    Ptr pBufBase = NULL;
    Ptr pBufData = NULL;
    Ptr pBufDataAlign = NULL;
    SAL_MemBlockObject *pBufHead = NULL;
    SAL_MemBlockObject *pBaseHead = NULL;
    SAL_MemBlockObject *pBufEnd = NULL;

    if(0 == size)
    {
        SAL_ERROR("size must > 0\n");
        return NULL;
    }

    /* 检查align 必须为4的倍数 */
    if(0 != (alignSize & 3))
    {
        SAL_ERROR("invalid align:%d\n", alignSize);
        return NULL;
    }

    /* 计算头大小 */
    headSize = sizeof(SAL_MemBlockObject);


    /* 计算尾大小 */
    endSize  = sizeof(SAL_MemBlockObject);


    /* 总的要申请的内存大小 */
    totalSize = size + alignSize + headSize*2 + endSize;
    allocSize = SAL_align(totalSize, alignSize);

    /* 申请内存 */
    pBufBase = memalign(alignSize, allocSize);
    if(NULL == pBufBase)
    {
        SAL_ERROR("alloc failed, size:%d, align:%d\n", size, alignSize);
        return NULL;
    }
    SAL_memSetSafe(pBufBase, 0, allocSize);
    
    /* 计算数据段的位置，内存的前面存放内存使用的类型，后面才是数据区 */
    pBufData = (void *)((PhysAddr)pBufBase + headSize*2);
    if(0 != alignSize)
    {
        pBufDataAlign = (Ptr)SAL_align((PhysAddr)pBufData, alignSize);
    }

    /* 获取头的位置 */
    pBufHead = SAL_memDataToHead(pBufDataAlign);

    /* 填充头 */
    pBufHead->nMgicNum  = SAL_MEM_MAGIC;
    pBufHead->size      = allocSize;
    pBufHead->pBufBase  = pBufBase;

    pBaseHead = (SAL_MemBlockObject *)pBufBase;
    pBaseHead->nMgicNum  = SAL_MEM_MAGIC;
    pBaseHead->size      = allocSize;
    pBaseHead->pBufBase  = pBufBase;
    
    /* 获取尾的位置 */
    //pBufEnd = (SAL_MemBlockObject *)( (unsigned long)(pBufData) + size);
    pBufEnd = (SAL_MemBlockObject *)((PhysAddr)pBufData + (size + alignSize) + (allocSize - totalSize));

    /* 填充尾*/
    pBufEnd->nMgicNum  = SAL_MEM_MAGIC;
    pBufEnd->size      = allocSize;
    pBufEnd->pBufBase  = pBufBase;

    #ifdef MEMLISTON
    if (SAL_FAIL == SAL_memListAdd(pBaseHead, pBufEnd, size, modId))
    {
        SAL_ERROR("SAL_memListAdd error\n");
    }
    
    #endif
    mallocLen += (allocSize);

    /*返回数据段指针*/
    return pBufDataAlign;
}

UINT32 SAL_memTotalLen(void){return mallocLen;}

/*******************************************************************************
* 函数名  : SAL_memPageSize
* 描  述  : 获取系统页大大小
* 输  入  : - void:
* 输  出  : 无
* 返回值  : 页大小
*******************************************************************************/
UINT32 SAL_memPageSize(void)
{
    return (UINT32)getpagesize();
}


/*******************************************************************************
* 函数名  : SAL_memArray2D
* 描  述  : 给二维数组申请内存
* 输  入  : - row: 行
*         : - col: 列
* 输  出  : 无
* 返回值  : 二维数组内存
*******************************************************************************/
UINT32** SAL_memArray2D(UINT32 row, UINT32 col)
{
    UINT32 size = sizeof(UINT32);
    UINT32 point_size = sizeof(UINT32*);

    /* 先申请内存，其中point_size * row表示存放row个行指针 */
    UINT32 **arr = (UINT32 **) SAL_memAlloc(point_size * row + size * row * col, "Sal");
    if (arr != NULL)
    {
        SAL_memSetSafe(arr, 0, point_size * row + size * row * col);
        UINT32 *head = (UINT32*)((PhysAddr)arr + point_size * row);
        while (row--)
            arr[row] = (UINT32*)((PhysAddr)head + row * col * size);
    }
    return (UINT32**)arr;
}


/*******************************************************************************
* Function      : SAL_memCpy
* Description   : 封装 memcpy 函数，进行四字节对齐式的快速拷贝
* Input         : - dest  : 目标地址
*               : - src   : 源
*               : - nBytes: 长度
* Output        : NONE
* Return        : HIK_SOK  : Success
*                 HIK_FAIL : Fail
*******************************************************************************/
void SAL_memCpy(INT8 *dest, INT8 *src, UINT32 nBytes)
{
    PhysAddr desRes, srcRes;

    desRes = (PhysAddr)dest & 0x3;
    srcRes = (PhysAddr)src & 0x3;
    if (srcRes == 0 && desRes != 0)
    {
        SAL_memCpySafe(dest, src, nBytes);
    }
    else if (srcRes != 0 && desRes == 0)
    {
        SAL_memCpySafe(dest, src, nBytes);
    }
    else if ((srcRes != 0 && desRes != 0) && (srcRes == desRes))
    {
        while (srcRes > 0)
        {
            *dest++ = *src++;
            srcRes--;
        }
    }
    else if ((srcRes != 0 && desRes != 0) && (srcRes != desRes))
    {
        SAL_memCpySafe(dest, src, nBytes);
    }

    if ((((PhysAddr)dest & 0x3) == 0) && (((PhysAddr)src & 0x3) == 0))
    {

        while (nBytes >= 4)
        {
            *((UINT32 *)dest) = *((UINT32 *)src);
            dest += 4;
            src += 4;
            nBytes -= 4;
        }
    }

    while (nBytes > 0)
    {
        *dest++ = *src++;
        nBytes--;
    }
}


