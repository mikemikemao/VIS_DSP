 #ifndef _SAL_MAP_H_
#define _SAL_MAP_H_


/* ========================================================================== */
/*                             ͷ�ļ���                                       */
/* ========================================================================== */


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           ������Ͷ�����                                   */
/* ========================================================================== */
typedef enum
{
    SAL_SHM_ROLE_CLIENT = 0x01,
    SAL_SHM_ROLE_SERVER = 0x02,
} SAL_SHM_ROLE;


/* ========================================================================== */
/*                          ���ݽṹ������                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          ����������                                        */
/* ========================================================================== */

/*******************************************************************************
* ������  : SAL_mmapFileInit
* ��  ��  : ��װͨ���ļ���ʽ�������ڴ�ӳ���ͨ�ú���
* ��  ��  : - pFileName: �ļ�·��, ���ļ�ֱ�������ڴ�ռ��ϣ�ͨѶЧ�ʺ�ͬ�����
*         : - role     : ����˻��ǿͻ��ˣ�����ֻ�ǳ�ʼ��ʱ�Ƿ�����ļ����ݣ�����
                         �޲��
*         : - size     : ӳ���С��Ҳ�ǹ����ڴ�Ĵ�С
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
void *SAL_mmapFileInit(const INT8 *pFileName, UINT32 role, UINT32 size);


/*******************************************************************************
* ������  : SAL_mmapFileDeinit
* ��  ��  : �Ƿ��ļ�ӳ�乲���ڴ�
* ��  ��  : - pVaddr: �ͷŵ�ַ����Ҫʹ�������ַ
*         : - size  : �ڴ泤��
* ��  ��  : ��
* ����ֵ  : SAL_SOK  : �ɹ�
*           SAL_FAIL : ʧ��
*******************************************************************************/
INT32 SAL_mmapFileDeinit(void *pVaddr, UINT32 size);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif