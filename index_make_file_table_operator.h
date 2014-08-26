#ifndef _INDEX_MAKE_FILE_TABLE_H__
#define _INDEX_MAKE_FILE_TABLE_H__

#include "index_def.h"
#include "index_quary_operator.h"
#include "index_file_operator.h"
/************************************************************************/
/* 建立文件映射表的相关操作                                                   */
/************************************************************************/

typedef vector<wstring> FILE_TABLE_VECTOR;//用来存储文件路径的线性表
typedef struct file_table_struct
{
	size_t file_table_size;
	FILE_TABLE_VECTOR the_file_table;
}FILE_TABLE;
//文件映射表

typedef struct file_table_file_struct 
{
	size_t file_name_length;
	TCHAR file_name[MAX_PATH];
}FILE_TABLE_FILE;
//文件映射表的文件存储节点  其实不用这样的  按照刚才写的就行 内次读取MAX_PATH长度的字符就可以了 虽然浪费空降
typedef vector<wstring> FILE_RES_FILEPATH_VECTOR;
//typedef list<size_t> FILE_RES_FILENUMBER_LIST;//文件号表
void init_file_table(FILE_TABLE * the_file_table);//初始化文件映射表的函数

FILE_TABLE * create_file_table(TCHAR * dir_path);//获得一个文件映射表 扫面dir下的文件

int write_file_table_to_file(TCHAR * file_name,FILE_TABLE * file_table);//将文件映射写入文件

FILE_RES_FILEPATH_VECTOR * read_file_table_to_buf(TCHAR * file_name);//将文件索引表文件中的内容读取到内存中的链表中

FILE_RES_FILEPATH_VECTOR * query_file_table(FILE_RES_FILENUMBER_LIST * file_number_list,FILE_RES_FILEPATH_VECTOR * file_table_list);//在文件映射表中根据文件号查询文件路径

int free_file_table(FILE_TABLE * the_file_table);//释放文件映射表

size_t get_file_size(WIN32_FIND_DATA* find_data);//获取文件大小

void scanf_file(wstring dir_path,FILE_TABLE * the_file_table);//扫描文件夹

#endif