#ifndef _INDEX_USR_INTERFACE_H__
#define _INDEX_USR_INTERFACE_H__
#include "index_def.h"
#include "index_elem_operator.h"
#include "index_file_operator.h"
#include "index_make_file_table_operator.h"
#include "index_make_operator.h"
#include "index_make_operator.h"
#include "index_memory_ctrl.h"
#include "index_quary_operator.h"
#include "index_usr_interface.h"

#define INDEX_DB_NAME "\\index__ddb.ddb"//����һ���������ļ���
#define NEXT_INDEX_DB_NAME "\\next_index.ddb"
#define FILE_TABLE_DB_NAME L"\\file_table.ddb"
/************************************************************************/
/* Ϊ�û��ṩ�Ľӿ�                                                               */
/************************************************************************/

int create_index_on(TCHAR * file_path);//���������Ľӿ� �����к�ͼ�ν���ͨ��

int query_index_on(TCHAR * file_path,TCHAR * query_string);//����������е��û��ӿ�
#endif