#ifndef __INDEX_FILE_OPERATOR_H__
#define __INDEX_FILE_OPERATOR_H__
/*������Щ�ļ�����*/
#include "index_def.h"

int read_file_to_buf(TCHAR * file_name,TCHAR * buf);//��ȡ�ļ���������  �ı��ļ�

int read_file_to_buf_string(TCHAR * file_name,BUF_STRING *buf_string);//������ã�ֱ�Ӷ�ȡ��bufstring��ȥ��

int read_iamwall_get(void * buf,FILE * fp,int file_off_set,int file_length);//�����ȡ�ļ�  �������ɵ��ú�������

int read_iamwall_send(void **p_buf,FILE * fp,int file_off_set,int file_length);//�������������������

int read_next_index_to_buf(char * next_file_name,THE_NEXT_INDEX **next_buf);//��ȡ�����������ڴ�Ļ�����

int write_buf_to_file(void *buf,size_t buf_length,char * file_name,char *write_flg/*д��ʱ���ģʽ*/);//������������д���ļ���

#endif