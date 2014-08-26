#include "index_elem_operator.h"
/*索引操作的各种实现*/
#define RIGTH_SHIFT(x,y) ((x)>>(y))//移位的宏定义 学以致用啊
#define LEFT_SHIFT(x,y) ((x)<<(y))

/************************************************************************/
/* 这些函数基本搞定20140727coding by iamwall                      */
/************************************************************************/

/*GET函数的设计*/
unsigned int get_currcharloc_index__(const Index__ * index_elem)
{//获取当前字符在索引中位置的函数
	unsigned int temp=0XFFC00000;
	temp=temp&index_elem->index_info_more;
//	temp=temp>>22;
	temp=RIGTH_SHIFT(temp,22);
	return temp;
}

TCHAR get_nextchar_index__(const Index__ * index_elem)
{//在索引中获取下一个字符

	return index_elem->next_char;
}

unsigned int get_nextcharloc_index__(const Index__ * index_elem)
{//获取下一个字符在索引中的位置
	unsigned int temp=index_elem->index_info_more;
	temp=LEFT_SHIFT(temp,10);
	temp=RIGTH_SHIFT(temp,22);
	return temp;
}

unsigned int get_filenumber_index__(const Index__ * index_elem)
{//获取索引中的文件号
	unsigned int temp=index_elem->index_info_more;
	temp=LEFT_SHIFT(temp,20);
	temp=RIGTH_SHIFT(temp,22);
	return temp;
}


/*SET函数的设计*/
unsigned int  set_currcharloc_index__(Index__ * index_elem,unsigned int curr_char_loc)
{//设置索引项中的当前字符位置
	if (curr_char_loc>1023)
	{
		printf("currcharloc过长\n");
		return -1;
	}
	curr_char_loc=curr_char_loc<<22;
	//curr_char_loc=curr_char_loc|(unsigned int)0X003FFFFF;
	index_elem->index_info_more=(index_elem->index_info_more)|(curr_char_loc);
	//index_elem->index_info_more=10;
	curr_char_loc=curr_char_loc>>22;
	return curr_char_loc;
}
int set_nextchar_index__(Index__ * index_elem,TCHAR next_char)
{//设置索引项中的下一个字符
	index_elem->next_char=next_char;
	return (int)next_char;
}
unsigned int set_nextcharloc_index__(Index__ * index_elem,unsigned int next_char_loc)
{//设置索引项中的下一个字符位置
	//index_elem->index_info_more=index_elem->index_info_more|0X003FF000;
	if (next_char_loc>1023)
	{
		printf("nextcharloc过长\n");
		return -1;
	}
	next_char_loc=LEFT_SHIFT(next_char_loc,12);
	index_elem->index_info_more=index_elem->index_info_more|next_char_loc;
	return RIGTH_SHIFT(next_char_loc,12);
}
unsigned int set_filenumber_index__(Index__ * index_elem,unsigned int file_number)
{
	if (file_number>1023)
	{
		printf("文件号过长\n");
		return -1;
	}
	file_number=LEFT_SHIFT(file_number,2);
	index_elem->index_info_more=index_elem->index_info_more|file_number;
	return RIGTH_SHIFT(file_number,2);
}//设置索引项中的文件号