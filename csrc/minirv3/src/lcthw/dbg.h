#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
//打印变量值
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

//纯记录错误原因，不做检查
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__,__func__, clean_errno(), ##__VA_ARGS__)

//纯做警告
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__,__func__, clean_errno(), ##__VA_ARGS__)

//记录正常执行流程
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d:%s) " M "\n", __FILE__, __LINE__,__func__, ##__VA_ARGS__)

//这个好像只要返回指针都可以用，第一个参数是条件 比如ret == 0
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

//用在switch的分支里，本身也不做任何检查，只是做个记录
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

//检查内存分配是否成功
#define check_mem(A) check((A), "Out of memory.")

//跟check一样，只是可以通过定义宏来消除
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif