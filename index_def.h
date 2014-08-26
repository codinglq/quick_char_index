#ifndef __INDEX_DEF_H__
#define __INDEX_DEF_H__
/************************************************************************/
/*   ��Ҫ��ͷ�ļ������ͷ�ļ��������������ļ������ж�����һ��������   ������Ҫ�Ķ���ͻ����Ķ��嶼������  */
/*20140726
coding by iamwall*/
/************************************************************************/
#include <windows.h>//���ͷ�ļ������н�TCHAR�����wchar_t
#include <winbase.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
using namespace std;

#define INDEX_DEBUG
typedef struct Index_ 
{
	TCHAR next_char;//�����ַ�
	unsigned int index_info_more;//�������һ���汾�Ļ�������취�ѵ�ǰ�ַ�λ��ȡ����ͨ���㷨�������ǰ�ַ�λ��
	//CurrCharLoc 10λ NextCharLoc10λ FIleNumber10λ
}Index__;//������Ԫ��

#ifdef _BIG_INDEX__
//��Ҫ��дget��set������
	typedef struct Index_
	{//һ�������е��������������next_char_loc�޷�������������Ҫ�������Ҫ�ӳ�next_char_loc��λ����21λ
		//��index_more_infp�ĺ���λ�����next_char_loc��
		TCHAR next_char;//�����ַ�
		unsigned int index_info_more;
	};
#endif
void init_index_elem(Index__ *index_elem);//��ʼ��������  ����ʼ��������޷���ȷ�������λ�õ�����

//˫�ַ���������
typedef struct splid_wnd_char_struct 
{
	TCHAR first_char;
	TCHAR last_char;
	size_t first_char_loc;
}SPLID_WND_CHAR;

void init_splid_wnd_char(SPLID_WND_CHAR * the_splid_wnd_char);//��ʼ��˫�ַ��������ڵĺ���

typedef struct buf_string_struct 
{
	TCHAR * the_char_buf;//ָ���ַ���������ָ��
	size_t buf_size;//��������С
	SPLID_WND_CHAR splid_wnd_char;
}BUF_STRING;//���ڽ�����һ���ڴ��������ַ�������

TCHAR get_next_char(TCHAR *input_string,size_t *curr_loc);//��BUF_STRING�л����һ���ַ��ĺ���

int get_next_splid_wnd(BUF_STRING * buf_string);//��ȡ��һ���������� ���ǻ��������е�˫�ַ���Ҫ�����ƶ�

//�ڴ����������Ա�  ��map�е�һ��������������������������м��ص����buf��  ����Ҫ����ʲô˳���أ�
//��Ϊ�������Ҫ��ϵ�����������İ����յ�������˳�����  Ȼ�����ν�value�е�������ŵ�buf��
typedef struct memory_index_arear_array_struct
{
	size_t length;
	Index__ index_head[0];
}MEMORY_INDEX_ARRAY_BUF;
//......'\n'Ӧ����Ϊ�ļ�������

typedef struct next_index__struct 
{
	TCHAR curr_char;
	unsigned int index_file_offset;//���������ǲ���Ҫ��size_t���Ͱ�
	unsigned int index_file_length;
}NEXT_INDEX;

//�ٷ�װһ��
typedef struct the_next_index__struct
{
	size_t length;
	NEXT_INDEX next_index__[0];
}THE_NEXT_INDEX;
//������Ǹ��ݵ�һ���ڴ��������ɵĶ�������
//������Ǽ���ʱ���Ҫ�õ��Ķ������� Ҳ���Ǹ������������ļ����ɵĲ�ѯ��������

typedef struct query_next_index__struct
{
	unsigned int index_file_offset;//һ���������ļ��е�ƫ����
	unsigned int index_file_length;//һ���������ļ��еĳ���
	unsigned short in_memory_flg;//�Ƿ��Ѿ���ȡ����������
	size_t index_arear_array_length;//һ�������򳤶ȣ�Ӧ���Ǹ�����
	Index__ *index_arear_array;//һ��������Ӧ���������  ����������ַ���ʱ��Ž��������ɻ�����   ����make_pairʱ�� ����������ó�0
}QUERY_NEXT_INDEX;//���ڼ����Ķ�������

typedef map<TCHAR,QUERY_NEXT_INDEX> MAP_QUARY_NEXT_INDEX_AREAR;//���������� �������Һͽ���
//                  ���ַ�     �����������һ��������

//�ļ�ӳ�������
typedef map<size_t,string> MAP_FILE_NUMBER;//�ļ���ӳ�����ʱ��ʹ���������ݽṹ ��һ������ʹ���ַ�������϶��ֲ���
//string����uncode������  ��֪����

#endif