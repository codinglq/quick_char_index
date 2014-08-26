#include "index_memory_ctrl.h"

MEMORY_INDEX_QUEUE * get_memory_index_queue()//����һ�����ƶ���  ���ҳ�ʼ��
{//�����ʼ��ûɶ�����
	MEMORY_INDEX_QUEUE * temp=new MEMORY_INDEX_QUEUE;
	temp->index_queue_curr_size=0;
	temp->over_queue=0;
	temp->index_queue_max_size=MAX_INDEX_QUEUE_SIZE;//�������
	temp->index_queue_max_ctrl_size=MAX_CTRL_QUEUE_INDEX_SIZE;//�����ƿ��С
	return temp;
}

void init_index_memory_dom(MEMORY_INDEX_QUEUE * the_memory_queue)//��ʼ����Ԫ�صĶ���
{
	//ȫ����ʼ���ɽ�β�Ķ���
	size_t count=0;
	while (count<(the_memory_queue->index_queue_max_size))
	{
		the_memory_queue->the_ctrl_queue[count].count_times=0;
		the_memory_queue->the_ctrl_queue[count].p_index__queue=NULL;
		the_memory_queue->the_ctrl_queue[count].the_char=L'\0';
		++count;
	}
	printf("�ڴ���Ʊ��ʼ���ɹ�\n");
}

int get_min_times_queue_dom(MEMORY_INDEX_QUEUE * the_queue,TCHAR the_char)//��ȡ��������������С��Ԫ�ص��±�
{/**��Ƶĺ�ʧ�����ģ��*/
	if (!the_queue)
	{
		printf("��������the_queue�ǿյİ�\n");
		return -1;//��ʶ����ʧ����
	}
	//�Ȱ����ַ����� �ҵ���ȵ��ַ�
	int count=0;
	int loc=0;

	while (count<(the_queue->index_queue_curr_size))
	{
		if (the_queue->the_ctrl_queue[count].the_char==the_char)
		{
			return count;//���ҵ���
		}
		++count;
	}
	//�����ǲ���ʧ����  Ȼ�������С���������±�
	if (the_queue->index_queue_curr_size<the_queue->index_queue_max_ctrl_size)
	{
		return the_queue->index_queue_curr_size;
	}
	count=0;

	int temp_times=the_queue->the_ctrl_queue[0].count_times;//��temp���ڵ�һ��Ԫ�ص����ô���
	while (count<(the_queue->index_queue_curr_size))
	{//��ȡ��С���ô�����Ԫ���±�
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

int index_memory_ctrl(TCHAR the_char,MEMORY_INDEX_QUEUE * the_queue,MAP_QUARY_NEXT_INDEX_AREAR * map_index)//�ڴ����ӿ���ƾ�����ô�򵥵�
{//�ڴ�����㷨
	/*����ط��о޴��bug Ӧ�ð����ַ����Ҳ����𣿣�����������*/
	//�����ж����������û��
	MAP_QUARY_NEXT_INDEX_AREAR::iterator itr=map_index->find(the_char);
	
	if (!the_queue)
	{
		printf("�ڴ���ƶ���Ϊ��Ŷ  �������ڴ����ģ�麯����\n");
		return -1;
	}

	if (the_queue->over_queue)
	{//����������˵����
		int min_loc=get_min_times_queue_dom(the_queue,the_char);
		if (min_loc==-1)
		{
			printf("��ȡ�ַ��±�ʧ��\n");
			return -1;
		}

		if (the_queue->the_ctrl_queue[min_loc].the_char==the_char)
		{//�����������Ѿ����� ���Լ�һ������������
			++(the_queue->the_ctrl_queue[min_loc].count_times);
			return 1;
		}

		//��ʼ�ͷ�һ�������ڴ� Ȼ�����Ԫ��
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
		//�����û���������
		//++the_queue->index_queue_curr_size;
		int min_loc=get_min_times_queue_dom(the_queue,the_char);
		if (the_queue->the_ctrl_queue[min_loc].the_char==the_char)
		{//�����������Ѿ����� ���Լ�һ������������
			++(the_queue->the_ctrl_queue[min_loc].count_times);
			return 1;
		}

		the_queue->the_ctrl_queue[the_queue->index_queue_curr_size].count_times=1;
		the_queue->the_ctrl_queue[the_queue->index_queue_curr_size].the_char=the_char;
		the_queue->the_ctrl_queue[the_queue->index_queue_curr_size].p_index__queue=&(itr->second);
		++the_queue->index_queue_curr_size;
		if (the_queue->index_queue_curr_size==the_queue->index_queue_max_ctrl_size)
		{
			the_queue->over_queue=1;//��������� ����λ1
		}
	}//ò����ͨ��������  �ڴ���Ʊ���д���ر��Ұ�
	return 1;
}
