#ifndef __INDEX_QUERY_OPERATOR_H__
#define __INDEX_QUERY_OPERATOR_H__
#include "index_def.h"
#include "index_elem_operator.h"
#include "index_file_operator.h"
#include "index_make_operator.h"
#define MAX_QUERY_STRING_LENGTH 100
/************************************************************************/
/* ����coding by iamwall 20140801                                        */
/************************************************************************/
typedef list<string> FILE_RES_LIST;
typedef vector<string>FILE_RES_VECTOR;

typedef list<size_t> FILE_RES_FILENUMBER_LIST;

FILE_RES_FILENUMBER_LIST * query_iamwall(TCHAR * source_str,//������
	MAP_QUARY_NEXT_INDEX_AREAR * map_quary,//���ڼ����Ķ�������
	FILE * index_fp,FILE_RES_FILENUMBER_LIST  *the_list_file_number);//ָ��һ�������ļ���ָ��
//���ض�˵ �����������ţ�Ƶļ����㷨

TCHAR get_next_char_in_index(MAP_QUARY_NEXT_INDEX_AREAR::iterator itr,
	size_t next_char_loc);//�ڶ����������Լ������л�ȡ��һ���ַ�
#endif