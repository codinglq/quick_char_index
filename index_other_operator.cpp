#include "index_def.h"

void init_splid_wnd_char(SPLID_WND_CHAR * the_splid_wnd_char)//��ʼ��˫�ַ��������ڵĺ���
{
	//�������ò��û��ʲô�ð�
}

TCHAR get_next_char(TCHAR *input_string,size_t *curr_loc)//��BUF_STRING�л����һ���ַ��ĺ���
{
	if (!input_string)
	{
		printf("�����ַ�Ϊ�� ʧ��getnext\n");
		return (TCHAR)L'\0';
	}
	if (!curr_loc)
	{
		printf("locΪ�� ʧ��\n");
		return (TCHAR)L'\0';
	}
	if (wcslen(input_string)>(*curr_loc))
	{
		return input_string[(*curr_loc)++];
	}
	return (TCHAR)L'\0';
}

int get_next_splid_wnd(BUF_STRING * buf_string)//��ȡ��һ���������� ���ǻ��������е�˫�ַ���Ҫ�����ƶ�
{//ò�����һ���ַ��޷�����������  ������Ҫ���ı����������һ��������ʾ��
	//�㶨 ֻ�����󻬶�һ������ 20140728 ͨ��������
	if ((buf_string->splid_wnd_char.first_char_loc+1)==buf_string->buf_size)
	{
#if 0
		printf("������ͷ�ˣ�������\n");
#endif
		return -1;
	}
	++(buf_string->splid_wnd_char.first_char_loc);
	buf_string->splid_wnd_char.first_char=buf_string->the_char_buf[buf_string->splid_wnd_char.first_char_loc];
	buf_string->splid_wnd_char.last_char=buf_string->the_char_buf[buf_string->splid_wnd_char.first_char_loc+1];
	return 0;
}

void init_index_elem(Index__ *index_elem)//��ʼ��������
{
	memset(index_elem,0,sizeof(Index__));
}