#ifndef _INDEX_MAKE_OPERATOR_H__
#define _INDEX_MAKE_OPERATOR_H__
/*�������� һ������ �������� �Ȳ���*/
/*20140728 coding by iamwall*/
#include "index_def.h"
typedef vector<Index__> MemoryIndexArear;//һ��������
typedef map<TCHAR,MemoryIndexArear*> MemoryIndexTree;//һ��������
typedef set<TCHAR> OVER_COME_SET;//�ַ��ų���
//���������Ķ�����def�ļ���

MemoryIndexTree * make_memory_index_tree(TCHAR * file_name,char * over_come_file_name,unsigned int file_number);//����һ���ڴ�������

MemoryIndexTree * make_memory_index_tree_all_file(TCHAR * file_path,char * over_come_file_name);//��������ձ���Ĵ���ĳ��·���µ����е��ı��ļ����������ĳ���ģ��


MEMORY_INDEX_ARRAY_BUF * get_memory_index_array(MemoryIndexTree *index_tree);//������������ڴ����Ի�����

int write_index_to_file(MemoryIndexTree * index_tree,char * file_name,char *flg);//��һ�������ļ�д���ļ�

OVER_COME_SET * make_over_come_set(char * over_come_file_name);//ͨ���ļ������ų���
//ע���ڴ����  �ͷ��ڴ氡

int judge_over_come_char(TCHAR the_char,OVER_COME_SET * over_come_set);//�����ų���  ע��Ҫ�ͷ��ų����е��ڴ�

THE_NEXT_INDEX * make_next_index__(MemoryIndexTree * index_tree);//���ɶ�������

MAP_QUARY_NEXT_INDEX_AREAR * make_query_index(char * next_file_file_name);//�������ڲ�ѯ���ڴ��������
//������Ǹ����������ڴ���������ĺ�����ͨ���ļ���ǰ��ĳ�������� ���� �Ѿ����Զ�������������

int read_index_to_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index,FILE * index_fp);//��ȡһ����������������

int free_index_in_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index);//�ͷ�ĳһ���ַ��Ķ��������Ϲ��صĻ�����
#endif

