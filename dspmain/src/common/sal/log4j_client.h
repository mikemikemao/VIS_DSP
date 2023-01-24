/**	@file : log4j_client.h
  *  @note : HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  *  @brief : 日志公共模块。
  *
  *  @author : 
  *  @date : 2018年12月1日
  *
  *  @note : #
  *  @record : 
  *      2018年12月1日  创建 
  *      #
  *
  *  @warning : 
*/
#ifndef __LOG4J_CLIENT_H__
#define __LOG4J_CLIENT_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

enum logtype {
	APP_LOG = 0,
	BSP_LOG = 1,
	DSP_LOG = 2,
};

typedef enum{
    log4j_log_level_dbg = 0,
    log4j_log_level_info = 1,
    log4j_log_level_warning = 2,
    log4j_log_level_error = 3,
    log4j_log_level_record = 4,
    log4j_log_level_cmd = 5,
} LOG4J_LOG_LEVEL_E;

#define log4j_msg_write(level, model,format,...)	\
	log4j_msg_write_with_type(APP_LOG, level, model,format, ##__VA_ARGS__)

#define HK_LOG4J_DBG(model,format,...)      \
	log4j_msg_write(log4j_log_level_dbg,model,"|%s|%d|" format,__func__,__LINE__,##__VA_ARGS__)

#define HK_LOG4J_INFO(model,format,...)     \
	log4j_msg_write(log4j_log_level_info,model,"|%s|%d|" format,__func__,__LINE__,##__VA_ARGS__)

#define HK_LOG4J_WARN(model,format,...)     \
	log4j_msg_write(log4j_log_level_warning,model,"|%s|%d|" format,__func__,__LINE__,##__VA_ARGS__)

#define HK_LOG4J_ERR(model,format,...)      \
	log4j_msg_write(log4j_log_level_error,model,"|%s|%d|" format,__func__,__LINE__,##__VA_ARGS__)

#define HK_LOG4J_RECORD(model,format,...)      \
	log4j_msg_write(log4j_log_level_record,model,"|%s|%d|" format,__func__,__LINE__,##__VA_ARGS__)

#define log4j_dsp_record(format,...)			\
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_record, "DSP", format,##__VA_ARGS__)

#define log4j_dsp_dbg(format,...)			\
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_dbg, "DSP","|%s|%d|" format,__func__,__LINE__,##__VA_ARGS__)

#define log4j_dsp_info(format,...)			\
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_info, "DSP","|%s|%s|%d|" format,__FILENAME__,__func__,__LINE__,##__VA_ARGS__)

#define log4j_dsp_warn(format,...)			\
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_warning, "DSP","|%s|%s|%d|" format,__FILENAME__,__func__,__LINE__,##__VA_ARGS__)

#define log4j_dsp_error(format,...)			\
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_error, "DSP","|%s|%s|%d|" format,__FILENAME__,__func__,__LINE__,##__VA_ARGS__)

#define log4j_dsp_sync() \
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_cmd, "DSP", "sync")

#define log4j_dsp_init(format,...)          \
    log4j_msg_write_with_type(DSP_LOG, log4j_log_level_cmd, "DSP", format)

void  log4j_msg_write_with_type(int type, LOG4J_LOG_LEVEL_E level, const char *modelName, const char *format, ...);

#include <stdio.h>
/*
// DSP日志初始化，logsize日志大小单位字节，超出后循环覆盖，syncsize同步数据大小单位字节，超出后自动写入到flash或者eMMC中
// logsize和syncsize需要大于4096字节，logsize需要大于syncsize
// loglevel日志打印等级（0-3）
static void log4j_msg_dsp_init(int logsize, int syncsize, int loglevel)
{
	char msg[128] = "dsp_init";
	char mesg[256] = {0};
	sprintf(mesg, "%s %d %d %d", msg, logsize, syncsize, loglevel);
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_cmd, "DSP", mesg);
}
// DSP日志修改打印等级
// loglevel日志打印等级（0-3）
static void log4j_dsp_change_level(int loglevel)
{
	char msg[128] = "level";
	char mesg[256] = {0};
	sprintf(mesg, "%s%d", msg, loglevel);
	log4j_msg_write_with_type(DSP_LOG, log4j_log_level_cmd, "DSP", mesg);
}

// DSP日志初始化，logsize日志大小单位字节，sync_sec同步周期单位秒
// logsize需要大于4096字节，sync_sec需要大于0秒
static void log4j_msg_bsp_init(int logsize, int sync_sec)
{
	char msg[128] = "bsp_init";
	
	sprintf(msg, "%s %d %d", msg, logsize, sync_sec);
	log4j_msg_write_with_type(BSP_LOG, log4j_log_level_cmd, "BSP", msg);
}
*/
// 获取日志管理版本信息
// version版本信息缓存，ver_len版本缓存大小
// build编译日期信息缓存，bld_len编译日期缓存大小
void log4j_get_version(char *version, int ver_len, char *build, int bld_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
