#include "index_memory_ctrl.h"

MEMORY_INDEX_QUEUE * get_memory_index_queue()//创建一个控制队列  并且初始化
{//这个初始化没啥问题吧
	MEMORY_INDEX_QUEUE * temp=new MEMORY_INDEX_QUEUE;
	temp->index_queue_curr_size=0;
	temp->over_queue=0;
	temp->index_queue_max_size=MAX_INDEX_QUEUE_SIZE;//最大容量
	temp->index_queue_max_ctrl_size=MAX_CTRL_QUEUE_INDEX_SIZE;//最大控制块大小
	return temp;
}

void init_index_memory_dom(MEMORY_INDEX_QUEUE * the_memory_queue)//初始化表元素的东西
{
	//全部初始化成结尾的东东
	size_t count=0;
	while (count<(the_memory_queue->index_queue_max_size))
	{
		the_memory_queue->the_ctrl_queue[count].count_times=0;
		the_memory_queue->the_ctrl_queue[count].p_index__queue=NULL;
		the_memory_queue->the_ctrl_queue[count].the_char=L'\0';
		++count;
	}
	printf("内存控制表初始化成功\n");
}

int get_min_times_queue_dom(MEMORY_INDEX_QUEUE * the_queue,TCHAR the_char)//获取到表中引用数最小的元素的下标
{/**设计的很失败这个模块*/
	if (!the_queue)
	{
		printf("传进来的the_queue是空的啊\n");
		return -1;//标识查找失败了
	}
	//先按照字符查找 找到相等的字符
	int count=0;
	int loc=0;

	while (count<(the_queue->index_queue_curr_size))
	{
		if (the_queue->the_ctrl_queue[count].the_char==the_char)
		{
			return count;//查找到了
		}
		++count;
	}
	//下面是查找失败了  然后检索最小引用数的下标
	if (the_queue->index_queue_curr_size<the_queue->index_queue_max_ctrl_size)
	{
		return the_queue->index_queue_curr_size;
	}
	count=0;

	int temp_times=the_queue->the_ctrl_queue[0].count_times;//让temp等于第一个元素的引用次数
	while (count<(the_queue->index_queue_curr_size))
	{//获取最小引用次数的元素下标
		if (temp_times>(the_queue->the_ctrl_queue[count].count_times))
		{
			//++count;
			temp_times=the_queue->the_ctrl_queue[count].count_times;
			loc=count;
		}
		++count;
	}
	return loc;
}

int index_memory_ctrl(TCHAR the_char,MEMORY_INDEX_QUEUE * the_queue,MAP_QUARY_NEXT_INDEX_AREAR * map_index)//内存管理接口设计就是这么简单滴
{//内存控制算法
	/*这个地方有巨大的bug 应该按照字符查找不是吗？？？？？？？*/
	//首先判断这个队列满没满
	MAP_QUARY_NEXT_INDEX_AREAR::iterator itr=map_index->find(the_char);
	
	if (!the_queue)
	{
		printf("内存控制队列为空哦  发生在内存控制模块函数中\n");
		return -1;
	}

	if (the_queue->over_queue)
	{//这个就是满了的情况
		int min_loc=get_min_times_queue_dom(the_queue,the_char);
		if (min_loc==-1)
		{
			printf("获取字符下标失败\n");
			return -1;
		}

		if (the_queue->the_ctrl_queue[min_loc].the_char==the_char)
		{//本来表里面已经有了 所以加一次引用数就行
			++(the_queue->the_ctrl_queue[min_loc].count_times);
			return 1;
		}

		//开始释放一级索引内存 然后更改元素
		the_queue->the_ctrl_queue[min_loc].p_index__queue->index_arear_array_length=0;
		free(the_queue->the_ctrl_queue[min_loc].p_index__queue->index_arear_array);
		the_queue->the_ctrl_queue[min_loc].p_index__queue->index_arear_array=NULL;
		the_queue->the_ctrl_queue[min_loc].p_index__queue->in_memory_flg=0;

		the_queue->the_ctrl_queue[min_loc].count_times=1;
		the_queue->the_ctrl_queue[min_loc].p_index__queue=&(itr->second);
		the_queue->the_ctrl_queue[min_loc].the_char=the_char;
	}
	else
	{
		//这个是没有满的情况
		//++the_queue->index_queue_curr_size;
		int min_loc=get_min_times_queue_dom(the_queue,the_char);
		if (the_queue->the_ctrl_queue[min_loc].the_char==the_char)
		{//本来表里面已经有了 所以加一次引用数就行
			++(the_queue->the_ctrl_queue[min_loc].count_times);
			return 1;
		}

		the_queue->the_ctrl_queue[the_queue->index_queue_curr_size].count_times=1;
		the_queue->the_ctrl_queue[the_queue->index_queue_curr_size].the_char=the_char;
		the_queue->the_ctrl_queue[the_queue->index_queue_curr_size].p_index__queue=&(itr->second);
		++the_queue->index_queue_curr_size;
		if (the_queue->index_queue_curr_size==the_queue->index_queue_max_ctrl_size)
		{
			the_queue->over_queue=1;//如果表满了 就置位1
		}
	}//貌似是通过测试了  内存控制被我写的特别乱啊
	return 1;
}
