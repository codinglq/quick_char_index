#include "index_elem_operator.h"
/*���������ĸ���ʵ��*/
#define RIGTH_SHIFT(x,y) ((x)>>(y))//��λ�ĺ궨�� ѧ�����ð�
#define LEFT_SHIFT(x,y) ((x)<<(y))

/************************************************************************/
/* ��Щ���������㶨20140727coding by iamwall                      */
/************************************************************************/

/*GET���������*/
unsigned int get_currcharloc_index__(const Index__ * index_elem)
{//��ȡ��ǰ�ַ���������λ�õĺ���
	unsigned int temp=0XFFC00000;
	temp=temp&index_elem->index_info_more;
//	temp=temp>>22;
	temp=RIGTH_SHIFT(temp,22);
	return temp;
}

TCHAR get_nextchar_index__(const Index__ * index_elem)
{//�������л�ȡ��һ���ַ�

	return index_elem->next_char;
}

unsigned int get_nextcharloc_index__(const Index__ * index_elem)
{//��ȡ��һ���ַ��������е�λ��
	unsigned int temp=index_elem->index_info_more;
	temp=LEFT_SHIFT(temp,10);
	temp=RIGTH_SHIFT(temp,22);
	return temp;
}

unsigned int get_filenumber_index__(const Index__ * index_elem)
{//��ȡ�����е��ļ���
	unsigned int temp=index_elem->index_info_more;
	temp=LEFT_SHIFT(temp,20);
	temp=RIGTH_SHIFT(temp,22);
	return temp;
}


/*SET���������*/
unsigned int  set_currcharloc_index__(Index__ * index_elem,unsigned int curr_char_loc)
{//�����������еĵ�ǰ�ַ�λ��
	if (curr_char_loc>1023)
	{
		printf("currcharloc����\n");
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
{//�����������е���һ���ַ�
	index_elem->next_char=next_char;
	return (int)next_char;
}
unsigned int set_nextcharloc_index__(Index__ * index_elem,unsigned int next_char_loc)
{//�����������е���һ���ַ�λ��
	//index_elem->index_info_more=index_elem->index_info_more|0X003FF000;
	if (next_char_loc>1023)
	{
		printf("nextcharloc����\n");
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
		printf("�ļ��Ź���\n");
		return -1;
	}
	file_number=LEFT_SHIFT(file_number,2);
	index_elem->index_info_more=index_elem->index_info_more|file_number;
	return RIGTH_SHIFT(file_number,2);
}//�����������е��ļ���