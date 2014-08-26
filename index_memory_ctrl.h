#ifndef _INDEX_MEMORY_CTRL_H__
#define _INDEX_MEMORY_CTRL_H__
#include "index_def.h"
#include "index_memory_ctrl.h"
/************************************************************************/
/* �ڴ����  ��ʱȷ����ʹ��LRU�㷨�������������ڴ��С          */
/************************************************************************/
#define MAX_INDEX_QUEUE_SIZE 20//���е���󳤶�
#define MAX_CTRL_QUEUE_INDEX_SIZE 19//���������ٸ��ַ��������ڴ���

typedef struct index_memory_dom_struct
{//������Ԫ�صĶ���
	TCHAR the_char;
	QUERY_NEXT_INDEX * p_index__queue;//������ǿյľ�˵���Ѿ����ص��ڴ���  ���Բ���Ҫ����flgȥ�ж���
	size_t count_times;
	//����Ҫ��ʼ����ѯ����ָ��
}INDEX_MEMORY_DOM;

typedef struct memory_index_queue_struct 
{//���еĶ���  �Ҿ�����ô�ľ�̬ ��ô�İ�
	size_t index_queue_curr_size;//�����е�ǰ��Ԫ�ظ���
	size_t index_queue_max_ctrl_size;//�����е�Ҫ�����Ԫ�صĸ���
	size_t index_queue_max_size;
	short over_queue;//��������1 ������0
	INDEX_MEMORY_DOM the_ctrl_queue[MAX_INDEX_QUEUE_SIZE];
}MEMORY_INDEX_QUEUE;
/*��ѯ�����кܶ�����Ż��ĵط�*/

MEMORY_INDEX_QUEUE * get_memory_index_queue();//����һ�����ƶ���  ���ҳ�ʼ��

void init_index_memory_dom(MEMORY_INDEX_QUEUE * the_memory_queue);//��ʼ����Ԫ�صĶ���

int index_memory_ctrl(TCHAR the_char,MEMORY_INDEX_QUEUE * the_queue,MAP_QUARY_NEXT_INDEX_AREAR * map_index);//�ڴ����ӿ���ƾ�����ô�򵥵�

int get_min_times_queue_dom(MEMORY_INDEX_QUEUE * the_queue);//��ȡ��������������С��Ԫ�ص��±�

#endif