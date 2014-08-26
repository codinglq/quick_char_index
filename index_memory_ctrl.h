#ifndef _INDEX_MEMORY_CTRL_H__
#define _INDEX_MEMORY_CTRL_H__
#include "index_def.h"
#include "index_memory_ctrl.h"
/************************************************************************/
/* 内存控制  暂时确定先使用LRU算法来控制索引的内存大小          */
/************************************************************************/
#define MAX_INDEX_QUEUE_SIZE 20//队列的最大长度
#define MAX_CTRL_QUEUE_INDEX_SIZE 19//允许最多多少个字符索引在内存中

typedef struct index_memory_dom_struct
{//队列中元素的定义
	TCHAR the_char;
	QUERY_NEXT_INDEX * p_index__queue;//这个不是空的就说明已经加载到内存了  所以不需要再有flg去判断了
	size_t count_times;
	//所以要初始化查询二级指针
}INDEX_MEMORY_DOM;

typedef struct memory_index_queue_struct 
{//队列的定义  我就是这么的静态 怎么的啊
	size_t index_queue_curr_size;//队列中当前的元素个数
	size_t index_queue_max_ctrl_size;//队列中的要求控制元素的个数
	size_t index_queue_max_size;
	short over_queue;//表满了置1 否则置0
	INDEX_MEMORY_DOM the_ctrl_queue[MAX_INDEX_QUEUE_SIZE];
}MEMORY_INDEX_QUEUE;
/*查询程序有很多可以优化的地方*/

MEMORY_INDEX_QUEUE * get_memory_index_queue();//创建一个控制队列  并且初始化

void init_index_memory_dom(MEMORY_INDEX_QUEUE * the_memory_queue);//初始化表元素的东西

int index_memory_ctrl(TCHAR the_char,MEMORY_INDEX_QUEUE * the_queue,MAP_QUARY_NEXT_INDEX_AREAR * map_index);//内存管理接口设计就是这么简单滴

int get_min_times_queue_dom(MEMORY_INDEX_QUEUE * the_queue);//获取到表中引用数最小的元素的下标

#endif