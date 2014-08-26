#ifndef _INDEX_MAKE_OPERATOR_H__
#define _INDEX_MAKE_OPERATOR_H__
/*创建索引 一级索引 二级索引 等操作*/
/*20140728 coding by iamwall*/
#include "index_def.h"
typedef vector<Index__> MemoryIndexArear;//一级索引域
typedef map<TCHAR,MemoryIndexArear*> MemoryIndexTree;//一级索引树
typedef set<TCHAR> OVER_COME_SET;//字符排除集
//二级索引的定义在def文件中

MemoryIndexTree * make_memory_index_tree(TCHAR * file_name,char * over_come_file_name,unsigned int file_number);//建立一级内存索引树

MemoryIndexTree * make_memory_index_tree_all_file(TCHAR * file_path,char * over_come_file_name);//这个是最终本版的创建某个路径下的所有的文本文件的索引树的程序模块


MEMORY_INDEX_ARRAY_BUF * get_memory_index_array(MemoryIndexTree *index_tree);//建立索引项的内存线性缓冲区

int write_index_to_file(MemoryIndexTree * index_tree,char * file_name,char *flg);//将一级索引文件写入文件

OVER_COME_SET * make_over_come_set(char * over_come_file_name);//通过文件创建排除集
//注意内存管理  释放内存啊

int judge_over_come_char(TCHAR the_char,OVER_COME_SET * over_come_set);//单词排除集  注意要释放排除集中的内存

THE_NEXT_INDEX * make_next_index__(MemoryIndexTree * index_tree);//生成二级索引

MAP_QUARY_NEXT_INDEX_AREAR * make_query_index(char * next_file_file_name);//建立用于查询的内存二级索引
//上面的那个建立二级内存检索索引的函数是通过文件和前面的程序关联的 所以 已经可以独立啦！！！！

int read_index_to_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index,FILE * index_fp);//读取一级索引到二级索引

int free_index_in_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index);//释放某一个字符的二级索引上挂载的缓冲区
#endif

