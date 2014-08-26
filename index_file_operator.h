#ifndef __INDEX_FILE_OPERATOR_H__
#define __INDEX_FILE_OPERATOR_H__
/*定义了些文件操作*/
#include "index_def.h"

int read_file_to_buf(TCHAR * file_name,TCHAR * buf);//读取文件到缓冲区  文本文件

int read_file_to_buf_string(TCHAR * file_name,BUF_STRING *buf_string);//这个更好，直接读取到bufstring中去了

int read_iamwall_get(void * buf,FILE * fp,int file_off_set,int file_length);//随机读取文件  缓冲区由调用函数分配

int read_iamwall_send(void **p_buf,FILE * fp,int file_off_set,int file_length);//缓冲区由这个函数分配

int read_next_index_to_buf(char * next_file_name,THE_NEXT_INDEX **next_buf);//读取二级索引到内存的缓冲区

int write_buf_to_file(void *buf,size_t buf_length,char * file_name,char *write_flg/*写入时候的模式*/);//将缓冲区数据写入文件中

#endif