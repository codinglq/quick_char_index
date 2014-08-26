#include "index_quary_operator.h"
#include "index_memory_ctrl.h"

TCHAR get_next_char_in_index(MAP_QUARY_NEXT_INDEX_AREAR::iterator itr,size_t next_char_loc)//�ڶ����������Լ������л�ȡ��һ���ַ�
{//����ط���ĺ��Ѱ�
	return get_nextchar_index__(&(itr->second.index_arear_array[next_char_loc]));
}

FILE_RES_FILENUMBER_LIST * query_iamwall(TCHAR * source_str,
	MAP_QUARY_NEXT_INDEX_AREAR * map_quary,
	FILE * index_fp,FILE_RES_FILENUMBER_LIST  *the_list_file_number/*ԭ������Ǵ�����������������ı��� ��������ԭ���ǿ��Լӿ�����ٶ�*/)//���ض�˵ �����������ţ�Ƶļ����㷨
{//����㷨Ӧ���ṩ����汾��  ��Ϊ��������Ż��ķ�ΰʵ����̫����
	/*var 0.1  ����汾�ṩ���Ǿ�̬������*/

	if (!the_list_file_number)
	{
		printf("�����ļ��ű�Ϊ��\n");
		return the_list_file_number;
	}
	//��մ������ļ����ļ��ű����
	the_list_file_number->clear();
	if (!source_str)
	{
		printf("������Ϊ��");
		return the_list_file_number;
	}
	if (!map_quary)
	{
		printf("��������Ϊ�� ����ʧ��\n");
		return the_list_file_number;
	}
	if (!index_fp)
	{
		printf("һ������ָ��Ϊ�� ����ʧ��");
		return the_list_file_number;
	}
	//���ض�Ӧ���ַ���������������
	size_t source_str_length=wcslen(source_str);
	if (source_str_length> MAX_QUERY_STRING_LENGTH)
	{
		printf("����ļ�����̫����\n");
		return the_list_file_number;
	}
	size_t count_load_index=0;
	/************************************************************************/
	/* �����ڴ����                                                                  */
	/************************************************************************/
	MEMORY_INDEX_QUEUE * temp_mem=get_memory_index_queue();
	init_index_memory_dom(temp_mem);//����ǲ����ڴ�����

	while (count_load_index<source_str_length)
	{//ѭ�����ض�����������Ӧ��һ������
		if(!read_index_to_next_index(source_str[count_load_index],map_quary,index_fp))
		{
			printf("�ַ�������ʧ�� û���������\n");
			return the_list_file_number;
		}//ò���ڴ��������Ҳͨ��������  ��û���Ż��� ��û��Ч�ʵ�
		index_memory_ctrl(source_str[count_load_index],temp_mem,map_quary);//����ǲ����ڴ�����
		++count_load_index;
	}
	size_t count_get_next_char=source_str_length;//��ȡnextchar�Ĵ���
	size_t count=0;//���ƶ�ȡ����

	int success_flg=0;//�Ƿ����������ַ����ı�־λ ��ΪҪ�ж��Ƿ�ɹ�����  ���ǻ�õ�����������һ������

	size_t count_index_size=0;
	MAP_QUARY_NEXT_INDEX_AREAR::iterator itr=map_quary->find(source_str[0]);//;���ҿ�ʼ�ַ��Ķ�������
	if (itr==map_quary->end())
	{
		return the_list_file_number;//û�����������ҵ�����ַ�  ����ʧ��
	}
	size_t count_index=itr->second.index_arear_array_length;//���ַ����������е�������ĳ���
	TCHAR temp_next_char[MAX_QUERY_STRING_LENGTH]={};//��ȡ����nextchar�洢������

	size_t last_file_number=0XFFFFFFFF;
	while (count_index_size<count_index)
	{
		//	size_t temp_count=count_index_size;
		//	TCHAR temp_char=L'\0';
		//	size_t temp_count_index_size=count_index_size;//��ȡ��һ���ַ����������±�
		size_t next_char_loc=count_index_size;
		while (count<count_get_next_char)
		{//���λ�ȡ��һ���ַ� Ȼ����������Ƚ� ����� ��ok
			if (!count)
			{
				temp_next_char[0]=source_str[0];
				++count;
				continue;
			}
			//�����ﻹҪ�жϵ�ǰ�ַ��������Ƿ��Ѿ����ص��ڴ��� ���û�м��������һ����һ��
			//����ط������Ż����ö����
			if (source_str[count]!=get_nextchar_index__(&(itr->second.index_arear_array[next_char_loc])))
			{//������ʡ�°�
				break;
			}
			if (!itr->second.in_memory_flg)
			{
				printf("�и��ַ�����������û�е����ڴ氡 ��������һ��\n");
				read_index_to_next_index(itr->first,map_quary,index_fp);
			}
			temp_next_char[count]=get_next_char_in_index(itr,next_char_loc);//���⴦��bug
			next_char_loc=get_nextcharloc_index__(&(itr->second.index_arear_array[next_char_loc]));
			//����ط���bug
			itr=map_quary->find(temp_next_char[count]);
			if (temp_next_char[count]==L'\0')
			{//�������ļ������� û���ҵ�
				return the_list_file_number;
			}
			//itr=map_quary->find(temp_next_char[count]);
			if (itr==map_quary->end())
			{//��˵������������һ���ַ���
				break;
			}
			++count;
		}
		//wcscpy(temp_next_char,L"");
		itr=map_quary->find(source_str[0]);
		if (!wcscmp(temp_next_char,source_str))
		{
			++success_flg;
#if 0
			printf("���������ı�%u�в��ҳɹ���\n",get_filenumber_index__(&(itr->second.index_arear_array[count_index_size])));
#endif
			if (last_file_number!=get_filenumber_index__(&(itr->second.index_arear_array[count_index_size])))
			{
				the_list_file_number->push_back(get_filenumber_index__(&(itr->second.index_arear_array[count_index_size])));
			}//����ϴεĲ�ѯ�������εĲ�һ�� ����һ���ı� ������
			
			last_file_number=get_filenumber_index__(&(itr->second.index_arear_array[count_index_size]));
		}
		memset(temp_next_char,L'\0',sizeof(temp_next_char));
		++count_index_size;
		count=0;
	}//�����������
	printf("����ѯ��%d��λ��\n",success_flg);
	return the_list_file_number;
}

