#ifndef __INDEX_ELEM_OPERATOR__
#define __INDEX_ELEM_OPERATOR__
/****操作索引的定义***/
#include "index_def.h"

/*GET函数的设计*/
unsigned int get_currcharloc_index__(const Index__*index_elem);//获取当前字符在索引中位置的函数

TCHAR get_nextchar_index__(const Index__ *index_elem);//在索引中获取下一个字符

unsigned int get_nextcharloc_index__(const Index__ * index_elem);//获取下一个字符在索引中的位置

unsigned int get_filenumber_index__(const Index__ * index_elem);//获取索引中的文件号

/*SET函数的设计*/
unsigned int  set_currcharloc_index__(Index__ * index_elem,unsigned int curr_char_loc);//设置索引项中的当前字符位置

int set_nextchar_index__(Index__ * index_elem,TCHAR next_char);//设置索引项中的下一个字符

unsigned int set_nextcharloc_index__(Index__ * index_elem,unsigned int next_char_loc);//设置索引项中的下一个字符位置

unsigned int set_filenumber_index__(Index__ * index_elem,unsigned int file_number);//设置索引项中的文件号


#endif