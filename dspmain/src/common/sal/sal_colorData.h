#ifndef _SAL_COLORDATA_H_
#define _SAL_COLORDATA_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

//YUV 2 RGB tab
extern short R_Y[256];
extern short R_V[256];
extern short G_U[256];
extern short G_V[256];
extern short B_U[256];
//RGB 2 YUV tab
extern short Y_R[256];
extern short Y_G[256];
extern short Y_B[256];
extern short U_R[256];
extern short U_G[256];
extern short U_B[256];
extern short V_R[256];
extern short V_G[256];
extern short V_B[256];

// RGB 2 HSV 倒数查找表
extern const int div_table[];
//RGB 2 HSV h倒数查找表
extern const int hdiv_table180[];

extern unsigned int R_TO_Y[256];
extern unsigned int G_TO_Y[256];
extern unsigned int B_TO_Y[256];

extern unsigned int R_TO_U[256];
extern unsigned int G_TO_U[256];
extern unsigned int B_TO_U[256];

extern unsigned int R_TO_V[256];
extern unsigned int G_TO_V[256];
extern unsigned int B_TO_V[256];

#endif

