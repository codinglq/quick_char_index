#include "index_def.h"

void init_splid_wnd_char(SPLID_WND_CHAR * the_splid_wnd_char)//初始化双字符滑动窗口的函数
{
	//这个函数貌似没有什么用啊
}

TCHAR get_next_char(TCHAR *input_string,size_t *curr_loc)//在BUF_STRING中获得下一个字符的函数
{
	if (!input_string)
	{
		printf("检索字符为空 失败getnext\n");
		return (TCHAR)L'\0';
	}
	if (!curr_loc)
	{
		printf("loc为空 失败\n");
		return (TCHAR)L'\0';
	}
	if (wcslen(input_string)>(*curr_loc))
	{
		return input_string[(*curr_loc)++];
	}
	return (TCHAR)L'\0';
}

int get_next_splid_wnd(BUF_STRING * buf_string)//获取下一个滑动窗口 就是滑动窗口中的双字符都要向下移动
{//貌似最后一个字符无法被索引到啊  看来需要对文本缓冲区添加一个结束标示符
	//搞定 只许多向后滑动一个就行 20140728 通过测试了
	if ((buf_string->splid_wnd_char.first_char_loc+1)==buf_string->buf_size)
	{
#if 0
		printf("滑动到头了！不滑了\n");
#endif
		return -1;
	}
	++(buf_string->splid_wnd_char.first_char_loc);
	buf_string->splid_wnd_char.first_char=buf_string->the_char_buf[buf_string->splid_wnd_char.first_char_loc];
	buf_string->splid_wnd_char.last_char=buf_string->the_char_buf[buf_string->splid_wnd_char.first_char_loc+1];
	return 0;
}

void init_index_elem(Index__ *index_elem)//初始化索引项
{
	memset(index_elem,0,sizeof(Index__));
}