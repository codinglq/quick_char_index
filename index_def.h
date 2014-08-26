#ifndef __INDEX_DEF_H__
#define __INDEX_DEF_H__
/************************************************************************/
/*   主要的头文件，别的头文件都必须包含这个文件，其中定义了一级索引项   各种重要的定义和基本的定义都在这里  */
/*20140726
coding by iamwall*/
/************************************************************************/
#include <windows.h>//这个头文件里面有将TCHAR定义成wchar_t
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
	TCHAR next_char;//单个字符
	unsigned int index_info_more;//如果有下一个版本的话，聚想办法把当前字符位置取消，通过算法计算出当前字符位置
	//CurrCharLoc 10位 NextCharLoc10位 FIleNumber10位
}Index__;//索引三元组

#ifdef _BIG_INDEX__
//需要重写get和set函数了
	typedef struct Index_
	{//一级索引中的索引域过长导致next_char_loc无法工作的问题需要解决，需要延长next_char_loc的位数到21位
		//把index_more_infp的后两位空余给next_char_loc吧
		TCHAR next_char;//单个字符
		unsigned int index_info_more;
	};
#endif
void init_index_elem(Index__ *index_elem);//初始化索引项  不初始化会出现无法正确获得索引位置的问题

//双字符滑动窗口
typedef struct splid_wnd_char_struct 
{
	TCHAR first_char;
	TCHAR last_char;
	size_t first_char_loc;
}SPLID_WND_CHAR;

void init_splid_wnd_char(SPLID_WND_CHAR * the_splid_wnd_char);//初始化双字符滑动窗口的函数

typedef struct buf_string_struct 
{
	TCHAR * the_char_buf;//指向字符缓冲区的指针
	size_t buf_size;//缓冲区大小
	SPLID_WND_CHAR splid_wnd_char;
}BUF_STRING;//用于建立第一次内存索引的字符缓冲区

TCHAR get_next_char(TCHAR *input_string,size_t *curr_loc);//在BUF_STRING中获得下一个字符的函数

int get_next_splid_wnd(BUF_STRING * buf_string);//获取下一个滑动窗口 就是滑动窗口中的双字符都要向下移动

//内存索引的线性表  将map中的一级索引的所有索引项按照线性排列加载到这个buf中  可是要按照什么顺序呢？
//因为这个可是要关系到二级索引的啊按照迭代器的顺序遍历  然后依次将value中的索引项放到buf中
typedef struct memory_index_arear_array_struct
{
	size_t length;
	Index__ index_head[0];
}MEMORY_INDEX_ARRAY_BUF;
//......'\n'应该作为文件结束符

typedef struct next_index__struct 
{
	TCHAR curr_char;
	unsigned int index_file_offset;//考虑这里是不是要用size_t类型啊
	unsigned int index_file_length;
}NEXT_INDEX;

//再封装一下
typedef struct the_next_index__struct
{
	size_t length;
	NEXT_INDEX next_index__[0];
}THE_NEXT_INDEX;
//上面的是根据第一次内存索引生成的二级索引
//下面的是检索时候的要用到的二级索引 也就是根绝二级索引文件生成的查询二级索引

typedef struct query_next_index__struct
{
	unsigned int index_file_offset;//一级索引在文件中的偏移量
	unsigned int index_file_length;//一级索引在文件中的长度
	unsigned short in_memory_flg;//是否已经读取到缓冲区？
	size_t index_arear_array_length;//一级索引域长度（应该是个数）
	Index__ *index_arear_array;//一级索引对应的柔和数组  检索到这个字符的时候才将这里生成缓冲区   调用make_pair时候 这个不能设置成0
}QUERY_NEXT_INDEX;//用于检索的二级索引

typedef map<TCHAR,QUERY_NEXT_INDEX> MAP_QUARY_NEXT_INDEX_AREAR;//二级索引表 包括查找和建立
//                  单字符     二级索引域和一级索引域

//文件映射表的设计
typedef map<size_t,string> MAP_FILE_NUMBER;//文件号映射表暂时先使用这种数据结构 下一步考虑使用字符数组加上二分查找
//string能用uncode编码吗  不知道了

#endif