#ifndef _INDEX_MAKE_FILE_TABLE_H__
#define _INDEX_MAKE_FILE_TABLE_H__

#include "index_def.h"
#include "index_quary_operator.h"
#include "index_file_operator.h"
/************************************************************************/
/* �����ļ�ӳ������ز���                                                   */
/************************************************************************/

typedef vector<wstring> FILE_TABLE_VECTOR;//�����洢�ļ�·�������Ա�
typedef struct file_table_struct
{
	size_t file_table_size;
	FILE_TABLE_VECTOR the_file_table;
}FILE_TABLE;
//�ļ�ӳ���

typedef struct file_table_file_struct 
{
	size_t file_name_length;
	TCHAR file_name[MAX_PATH];
}FILE_TABLE_FILE;
//�ļ�ӳ�����ļ��洢�ڵ�  ��ʵ����������  ���ող�д�ľ��� �ڴζ�ȡMAX_PATH���ȵ��ַ��Ϳ����� ��Ȼ�˷ѿս�
typedef vector<wstring> FILE_RES_FILEPATH_VECTOR;
//typedef list<size_t> FILE_RES_FILENUMBER_LIST;//�ļ��ű�
void init_file_table(FILE_TABLE * the_file_table);//��ʼ���ļ�ӳ���ĺ���

FILE_TABLE * create_file_table(TCHAR * dir_path);//���һ���ļ�ӳ��� ɨ��dir�µ��ļ�

int write_file_table_to_file(TCHAR * file_name,FILE_TABLE * file_table);//���ļ�ӳ��д���ļ�

FILE_RES_FILEPATH_VECTOR * read_file_table_to_buf(TCHAR * file_name);//���ļ��������ļ��е����ݶ�ȡ���ڴ��е�������

FILE_RES_FILEPATH_VECTOR * query_file_table(FILE_RES_FILENUMBER_LIST * file_number_list,FILE_RES_FILEPATH_VECTOR * file_table_list);//���ļ�ӳ����и����ļ��Ų�ѯ�ļ�·��

int free_file_table(FILE_TABLE * the_file_table);//�ͷ��ļ�ӳ���

size_t get_file_size(WIN32_FIND_DATA* find_data);//��ȡ�ļ���С

void scanf_file(wstring dir_path,FILE_TABLE * the_file_table);//ɨ���ļ���

#endif