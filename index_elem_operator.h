#ifndef __INDEX_ELEM_OPERATOR__
#define __INDEX_ELEM_OPERATOR__
/****���������Ķ���***/
#include "index_def.h"

/*GET���������*/
unsigned int get_currcharloc_index__(const Index__*index_elem);//��ȡ��ǰ�ַ���������λ�õĺ���

TCHAR get_nextchar_index__(const Index__ *index_elem);//�������л�ȡ��һ���ַ�

unsigned int get_nextcharloc_index__(const Index__ * index_elem);//��ȡ��һ���ַ��������е�λ��

unsigned int get_filenumber_index__(const Index__ * index_elem);//��ȡ�����е��ļ���

/*SET���������*/
unsigned int  set_currcharloc_index__(Index__ * index_elem,unsigned int curr_char_loc);//�����������еĵ�ǰ�ַ�λ��

int set_nextchar_index__(Index__ * index_elem,TCHAR next_char);//�����������е���һ���ַ�

unsigned int set_nextcharloc_index__(Index__ * index_elem,unsigned int next_char_loc);//�����������е���һ���ַ�λ��

unsigned int set_filenumber_index__(Index__ * index_elem,unsigned int file_number);//�����������е��ļ���


#endif