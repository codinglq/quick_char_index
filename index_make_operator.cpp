#include "index_make_operator.h"
#include "index_file_operator.h"
#include "index_elem_operator.h"
#include "index_make_file_table_operator.h"
#include "index_usr_interface.h"
MemoryIndexTree * make_memory_index_tree(TCHAR * file_name,char * over_come_file_name,unsigned int file_number)//����һ���ڴ�������
{//�ļ����أ�������������������������������������������������������������������������
	//�ļ����Ѿ����20140729  ��ô��͵ڶ����˰�
	//����buf_string
	BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
	/************************************************************************/
	/* ��Ҫ���������Ӷ�ȡ�ı��ŵ�ģ��                                          */
	/************************************************************************/
	read_file_to_buf_string(file_name,buf_string);
	//�������

	//����������
	MemoryIndexTree * temp_memory_index_tree=new MemoryIndexTree;//��Ȼʹ����stl  ���õ�c++��
	//���������������

	//��ʼ�������ַ�һ������
	MemoryIndexTree::iterator itr_to_find_first_char;
	MemoryIndexTree::iterator itr_to_find_next_char;
	//20140729 0:50��ʼ�Ż�
	//��ʵ���ﻹ������û�н��
	/****
	���ַ���Ӧ����������ȫ���ı���������ַ��������� ���Գ��ȿ��ܻᳬ��1023ѽ
	��˵����������1023���ĵ� �ĵ���ȫ����һ���ַ����ظ�����
	������bug  �ȴ����
	�ѵ�Ҫ����next_char_loc �ĳ��ȣ�
	***/
	OVER_COME_SET * temp_over_set=make_over_come_set(over_come_file_name);
	do 
	{
#if 0//��ʱ�Ȳ����ַ��ų�����  ̫������

		/************************************************************************/
		/* �����ַ��ų�����ʹ��20140731                                           */
		/************************************************************************/
		if (judge_over_come_char(buf_string->splid_wnd_char.first_char,temp_over_set))
		{
			continue;
		}
#endif
		itr_to_find_first_char=temp_memory_index_tree->find(buf_string->splid_wnd_char.first_char);
		itr_to_find_next_char=temp_memory_index_tree->find(buf_string->splid_wnd_char.last_char);

		if (buf_string->splid_wnd_char.first_char==buf_string->splid_wnd_char.last_char)
		{
			if (itr_to_find_first_char==temp_memory_index_tree->end())
			{//��ǰ�ַ�����һ���ַ���� ��������ַ���û�г��ֹ������
				//����������
				MemoryIndexArear * memory_index_arear=new MemoryIndexArear;
				Index__ temp_index__ ;
				init_index_elem(&temp_index__);
				//���������ʱ���������ж��� �����Լ��ĳ���Ҳ����һ�����������
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,1);
				set_filenumber_index__(&temp_index__,file_number);
				memory_index_arear->push_back(temp_index__);
				temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,memory_index_arear));
			}

			if (itr_to_find_first_char!=temp_memory_index_tree->end())
			{//��ǰ�ַ�����һ���ַ���� �����Ѿ����ֹ���

				Index__ temp_index__;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size()+1);
				set_filenumber_index__(&temp_index__,file_number);
				//������Ǿ仰�Ķ���˼�� ��ǰ�ַ��Ѿ����ֹ��� ���ֱ�ӽ���ǰ�ַ�������������������ʱ�����ѭ��
				//������Ԥ�������ط�
				itr_to_find_first_char->second->push_back(temp_index__);
			}
			continue;
		}

		if (itr_to_find_first_char==temp_memory_index_tree->end())
		{//�ַ�����û�е�ǰ�ַ�����Ҫ����ǰ�ַ���ӵ��ַ�����
			if (itr_to_find_next_char==temp_memory_index_tree->end())
			{//��ǰ�ַ�����Ҳû����һ���ַ�
				//���������� ������ ��ӽ�ȥ
				MemoryIndexArear *index_memory_arear=new MemoryIndexArear;
				Index__ temp_index__ ;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,0);
				set_filenumber_index__(&temp_index__,file_number);
				//res��Ϊ�˲��Զ�����
				int res=get_currcharloc_index__(&temp_index__);
				index_memory_arear->push_back(temp_index__);
				temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,index_memory_arear));
				continue;
			}

			if (itr_to_find_next_char!=temp_memory_index_tree->end())
			{//˵����ǰ�ַ������ַ����� ������һ���ַ����ַ�����
				MemoryIndexArear * temp_memory_arear=new MemoryIndexArear;
				Index__ temp_index__;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size());
				set_filenumber_index__(&temp_index__,file_number);
				temp_memory_arear->push_back(temp_index__);
				temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,temp_memory_arear));
				continue;
			}
		}

		if (itr_to_find_first_char!=temp_memory_index_tree->end())
		{//˵����ǰ�ַ����ַ�����
			if (itr_to_find_next_char==temp_memory_index_tree->end())
			{//��һ���ַ������ַ�����
				Index__ temp_index__;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,0);//��һ���ַ�û�г��ֹ� ��������λ�ÿ϶���0
				set_filenumber_index__(&temp_index__,file_number);
				itr_to_find_first_char->second->push_back(temp_index__);
				continue;
			}
		}

		if (itr_to_find_first_char!=temp_memory_index_tree->end())
		{//
			if (itr_to_find_next_char!=temp_memory_index_tree->end())
			{
				Index__ temp_index__;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size());
				set_filenumber_index__(&temp_index__,file_number);
				itr_to_find_first_char->second->push_back(temp_index__);
				continue;
			}
		}
	} while (get_next_splid_wnd(buf_string)!=-1);//���ǻ�������û������Ҫһֱ��������

	free(buf_string->the_char_buf);
	free(buf_string);//���Ӧ���ͷ���?
	return temp_memory_index_tree;
}

MemoryIndexTree * make_memory_index_tree_all_file(TCHAR * file_path,char * over_come_file_name)//��������ձ���Ĵ���ĳ��·���µ����е��ı��ļ����������ĳ���ģ��
{
	//���� �������·���µ��ļ�ӳ���
	FILE_TABLE * file_table=create_file_table(file_path);
	//���Խ��ļ�ӳ���д���ļ�
	TCHAR file_table_name[MAX_PATH]={};
	wcscpy(file_table_name,file_path);
	wcscat(file_table_name,FILE_TABLE_DB_NAME);
	write_file_table_to_file(file_table_name,file_table);
	read_file_table_to_buf(file_table_name);
	//�ļ�ӳ��������
	//����һ���ڴ�������
	//����������
	MemoryIndexTree * temp_memory_index_tree=new MemoryIndexTree;//��Ȼʹ����stl  ���õ�c++��
	MemoryIndexTree::iterator itr_to_find_first_char;
	MemoryIndexTree::iterator itr_to_find_next_char;
	//���������������
	size_t file_table_size_count=0;
	
	while (file_table_size_count<(file_table->file_table_size))
	{
		//��ȡ��Ӧ���ı���������
		BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
		/************************************************************************/
		/* ��Ҫ���������Ӷ�ȡ�ı��ŵ�ģ��                                          */
		/************************************************************************/
		if (-1==read_file_to_buf_string((WCHAR *)(file_table->the_file_table[file_table_size_count].c_str()),buf_string))
		{
			printf("���հ汾���ļ�����������ʧ�� ʧ��ԭ�����ļ���ȡʧ�ܰ�\n");
			return NULL;
		}
		
		do 
		{
			itr_to_find_first_char=temp_memory_index_tree->find(buf_string->splid_wnd_char.first_char);
			itr_to_find_next_char=temp_memory_index_tree->find(buf_string->splid_wnd_char.last_char);

			if (buf_string->splid_wnd_char.first_char==buf_string->splid_wnd_char.last_char)
			{
				if (itr_to_find_first_char==temp_memory_index_tree->end())
				{//��ǰ�ַ�����һ���ַ���� ��������ַ���û�г��ֹ������
					//����������
					MemoryIndexArear * memory_index_arear=new MemoryIndexArear;
					Index__ temp_index__ ;
					init_index_elem(&temp_index__);
					//���������ʱ���������ж��� �����Լ��ĳ���Ҳ����һ�����������
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,1);
					set_filenumber_index__(&temp_index__,file_table_size_count);
					memory_index_arear->push_back(temp_index__);
					temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,memory_index_arear));
				}

				if (itr_to_find_first_char!=temp_memory_index_tree->end())
				{//��ǰ�ַ�����һ���ַ���� �����Ѿ����ֹ���

					Index__ temp_index__;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size()+1);
					set_filenumber_index__(&temp_index__,file_table_size_count);
					//������Ǿ仰�Ķ���˼�� ��ǰ�ַ��Ѿ����ֹ��� ���ֱ�ӽ���ǰ�ַ�������������������ʱ�����ѭ��
					//������Ԥ�������ط�
					itr_to_find_first_char->second->push_back(temp_index__);
				}
				continue;
			}

			if (itr_to_find_first_char==temp_memory_index_tree->end())
			{//�ַ�����û�е�ǰ�ַ�����Ҫ����ǰ�ַ���ӵ��ַ�����
				if (itr_to_find_next_char==temp_memory_index_tree->end())
				{//��ǰ�ַ�����Ҳû����һ���ַ�
					//���������� ������ ��ӽ�ȥ
					MemoryIndexArear *index_memory_arear=new MemoryIndexArear;
					Index__ temp_index__ ;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,0);
					set_filenumber_index__(&temp_index__,file_table_size_count);
					//res��Ϊ�˲��Զ�����
					int res=get_currcharloc_index__(&temp_index__);
					index_memory_arear->push_back(temp_index__);
					temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,index_memory_arear));
					continue;
				}

				if (itr_to_find_next_char!=temp_memory_index_tree->end())
				{//˵����ǰ�ַ������ַ����� ������һ���ַ����ַ�����
					MemoryIndexArear * temp_memory_arear=new MemoryIndexArear;
					Index__ temp_index__;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size());
					set_filenumber_index__(&temp_index__,file_table_size_count);
					temp_memory_arear->push_back(temp_index__);
					temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,temp_memory_arear));
					continue;
				}
			}

			if (itr_to_find_first_char!=temp_memory_index_tree->end())
			{//˵����ǰ�ַ����ַ�����
				if (itr_to_find_next_char==temp_memory_index_tree->end())
				{//��һ���ַ������ַ�����
					Index__ temp_index__;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,0);//��һ���ַ�û�г��ֹ� ��������λ�ÿ϶���0
					set_filenumber_index__(&temp_index__,file_table_size_count);
					itr_to_find_first_char->second->push_back(temp_index__);
					continue;
				}
			}

			if (itr_to_find_first_char!=temp_memory_index_tree->end())
			{//
				if (itr_to_find_next_char!=temp_memory_index_tree->end())
				{
					Index__ temp_index__;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size());
					set_filenumber_index__(&temp_index__,file_table_size_count);
					itr_to_find_first_char->second->push_back(temp_index__);
					continue;
				}
			}
		} while (get_next_splid_wnd(buf_string)!=-1);//���ǻ�������û������Ҫһֱ��������

		free(buf_string->the_char_buf);
		free(buf_string);//���Ӧ���ͷ���?
		++file_table_size_count;
	}
	free(file_table);//���Ҳ���ͷ��˰�
	return temp_memory_index_tree;
}

//������������ڴ����Ի�����
MEMORY_INDEX_ARRAY_BUF * get_memory_index_array(MemoryIndexTree *index_tree)//������������ڴ����Ի�����
{
	if (!index_tree)
	{
		printf("�ڴ�������ΪNULL\n");
		return NULL;
	}

	size_t index_size=0;
	//index_size=index_tree->size();//����ط��д��� ͳ�����ַ����ֵĸ��� ����û��ͳ��ȫ��������ĸ���
	//ͳ��ȫ��������ĸ���
	MemoryIndexTree::iterator itr_start=index_tree->begin();
	MemoryIndexTree::iterator itr_end=index_tree->end();
	while (itr_start!=itr_end)
	{
		// 		MemoryIndexArear::iterator itr_arear_start=itr_start->second->begin();
		// 		MemoryIndexArear::iterator itr_arear_end=itr_start->second->end();
		index_size+=itr_start->second->size();
		++itr_start;
	}
	//��������
	size_t count_index__=0;
	MEMORY_INDEX_ARRAY_BUF * temp_index_memory_buf=(MEMORY_INDEX_ARRAY_BUF*)malloc(sizeof(MEMORY_INDEX_ARRAY_BUF)+sizeof(Index__)*index_size);
	temp_index_memory_buf->length=index_size;
	//˳�����map ��ȡ���е�������
	itr_start=index_tree->begin();
	itr_end=index_tree->end();

	while (itr_start!=itr_end)
	{
		MemoryIndexArear::iterator itr_arear_start=itr_start->second->begin();
		MemoryIndexArear::iterator itr_arear_end=itr_start->second->end();

		while (itr_arear_start!=itr_arear_end)
		{
			*((temp_index_memory_buf->index_head)+(count_index__++))=*(itr_arear_start++);//���д��̫�����רҵ�� ��ָ�������Ҫ�ľ��ǿ���
			//++temp_index_memory_buf->length;
			//++itr_arear_start;
		}
		++itr_start;
	}
#if 0
	size_t counter=0;
	while (counter<count_index__)
	{
		printf("currcharloc%d \n",get_currcharloc_index__(&(temp_index_memory_buf->index_head[counter])));
		wcout<<"next_char:"<<get_nextchar_index__(&(temp_index_memory_buf->index_head[counter]))<<endl;
		printf("next_char_loc:%d\n",get_nextcharloc_index__(&(temp_index_memory_buf->index_head[counter])));
		++counter;
		printf("\n");
	}
	printf("index_size=%u\n",temp_index_memory_buf->length);
#endif
	return temp_index_memory_buf;
	//�ⲿ��Ҫ�����ͷ��ڴ�
}

int write_index_to_file(MemoryIndexTree * index_tree,char * file_name,char *flg)//��һ�������ļ�д���ļ�
{//��һ������д���ļ� ��Ҫ�����ͷ��ڴ�
	MEMORY_INDEX_ARRAY_BUF *buf=get_memory_index_array(index_tree);

#if 0

	size_t counter=0;
	size_t size_map=buf->length;
	while (counter<size_map)
	{
		printf("currcharloc%d \n",get_currcharloc_index__(&(buf->index_head[counter])));
		wcout<<"next_char:"<<get_nextchar_index__(&(buf->index_head[counter]))<<endl;
		printf("next_char_loc:%d\n",get_nextcharloc_index__(&(buf->index_head[counter])));
		++counter;
		printf("\n");
	}
	printf("index_size=%u\n",buf->length);
#endif

	int res=write_buf_to_file((buf->index_head),(buf->length)*sizeof(Index__),file_name,flg);

	if (0>=res)
	{
		printf("д��ʧ�� ����\n");
		return -1;
	}
	//printf("д��ɹ���\n");�ɹ�����������Ӧ���ж�
	free(buf);
	return res;
}

OVER_COME_SET * make_over_come_set(char * over_come_file_name)//ͨ���ļ������ų���
{
	FILE * fp=fopen(over_come_file_name,"r");
	if (!fp)
	{
		printf("�ų����ļ���ȡʧ�ܣ�\n");
		return NULL;
	}

	OVER_COME_SET * temp_over_set=new OVER_COME_SET;
	if (!temp_over_set)
	{
		printf("�ų����ڴ����ʧ��\n");
		return NULL;
	}
	//������

	size_t file_size=0;
	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	char * char_buf=(char*)calloc(1,(sizeof(char)*file_size)+sizeof(char));
	int res=fread(char_buf,file_size*sizeof(char),1,fp);
	if (1!=res)
	{
		printf("�ų����ļ���ȡʧ��\n");
		perror("");
		return NULL;
	}
	//ת���ɿ��ֽڵ�
	TCHAR * tchar_buf=(TCHAR *)calloc(1,(sizeof(TCHAR)*file_size)+sizeof(TCHAR));
	MultiByteToWideChar(CP_ACP,0,char_buf,-1,tchar_buf,/*sizeof(TCHAR)**/file_size);
	//�����ǲ�����Ҫ����һ�°�
	fclose(fp);//��Ҫ���ǹر��ļ� �������ؼ��ʵ�ͬѧ
	// ���濪ʼ����set
	size_t count=0;
	while (count<file_size)
	{
		temp_over_set->insert(tchar_buf[count++]);
	}
	printf("set�������\n");

	return temp_over_set;
}
//ע���ڴ����  �ͷ��ڴ氡

int judge_over_come_char(TCHAR the_char,OVER_COME_SET * over_come_set)//�����ų���  ע��Ҫ�ͷ��ų����е��ڴ�
{
	if (!over_come_set)
	{
		printf("�ַ��ų�������\n");
		return 0;
	}

	if (over_come_set->count(the_char))
	{
		return 1;
	}
	return 0;
}

THE_NEXT_INDEX * make_next_index__(MemoryIndexTree * index_tree)//���ɶ�������
{
	if (!index_tree)
	{
		printf("һ���ڴ�������Ϊ��\n");
		return NULL;
	}
	size_t index_size=0;
	//index_size=index_tree->size();//����ط��д��� ͳ�����ַ����ֵĸ��� ����û��ͳ��ȫ��������ĸ���
	//ͳ��ȫ��������ĸ���
	MemoryIndexTree::iterator itr_start=index_tree->begin();
	MemoryIndexTree::iterator itr_end=index_tree->end();
#if 0
	while (itr_start!=itr_end)
	{
		MemoryIndexArear::iterator itr_arear_start=itr_start->second->begin();
		//MemoryIndexArear::iterator itr_arear_end=itr_start->second->end();
		index_size+=itr_start->second->size();
		++itr_start;
	}
#endif
	//�����ʵ����ڴ�Ŷ
	index_size=index_tree->size();
	THE_NEXT_INDEX * temp_next_index=(THE_NEXT_INDEX*)malloc(sizeof(THE_NEXT_INDEX)+sizeof(NEXT_INDEX)*index_size);
	temp_next_index->length=index_size*sizeof(NEXT_INDEX);//������ֽ���

	itr_start=index_tree->begin();
	itr_end=index_tree->end();
	size_t file_offset=0;
	index_size=0;//��index_size��0 �����±������
	//�ٴα������е�һ���ڴ������򣬻�ȡ�����򳤶ȣ������ַ���Ӧ���������ȣ��͵�ǰ�ܳ��ȣ�һ�������ļ���ƫ��������Ȼ��㶨����
	while (itr_start!=itr_end)
	{
		temp_next_index->next_index__[index_size].curr_char=itr_start->first;
		temp_next_index->next_index__[index_size].index_file_length=itr_start->second->size()*sizeof(Index__);
		temp_next_index->next_index__[index_size].index_file_offset=file_offset;
		file_offset+=((itr_start->second->size())*sizeof(Index__));
		++index_size;
		++itr_start;
	}
	return temp_next_index;
	/////////һ��Ҫע���ͷ��ڴ�  �ǵô���review
}

MAP_QUARY_NEXT_INDEX_AREAR * make_query_index(char * next_file_file_name)//�������ڲ�ѯ���ڴ��������
{

	//�Ƚ����������ļ���ȡ���ڴ� Ȼ���ͷ���
	THE_NEXT_INDEX * the_next_index=NULL;
	int res=read_next_index_to_buf(next_file_file_name,&the_next_index);
	if (res<=0)
	{
		printf("����������ȡʧ��\n");
		return NULL;
	}
	if (!the_next_index)
	{
		printf("������������ʧ��\n");
		return NULL;
	}
	//��ʼ�����ڴ������������

	MAP_QUARY_NEXT_INDEX_AREAR *temp_map_index=new MAP_QUARY_NEXT_INDEX_AREAR;
	size_t count=0;
	size_t big_count=the_next_index->length/sizeof(NEXT_INDEX);
	while (count<big_count)
	{/*����ط���bug*/
		QUERY_NEXT_INDEX temp_quary_index;
		temp_quary_index.index_file_length=the_next_index->next_index__[count].index_file_length;
		temp_quary_index.index_file_offset=the_next_index->next_index__[count].index_file_offset;
		temp_quary_index.in_memory_flg=0;
		temp_quary_index.index_arear_array_length=0;
		temp_quary_index.index_arear_array=NULL;//���������ʼ�� Ҫ��Ȼ�ͷ�ʱ�����׳���
		temp_map_index->insert(make_pair(the_next_index->next_index__[count].curr_char,temp_quary_index));
		//	wcout<<the_next_index->next_index__[count].curr_char<<endl;//ò���Ƕ�ȡ���Ķ���������Щ����
		++count;
	}
	free(the_next_index);
	return temp_map_index;
}

int read_index_to_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index,FILE * index_fp)//��ȡһ����������������
{
	if (!map_next_index)
	{
		printf("����������Ϊ��ʧ��\n");
		return 0;
	}
	if (!index_fp)
	{
		printf("һ���������ļ�ָ��Ϊ��ʧ��\n");
		return 0;
	}

	//����ͨ���ַ����Ҷ��������Ľڵ�
	MAP_QUARY_NEXT_INDEX_AREAR::iterator next_index_itr;
	QUERY_NEXT_INDEX * quary_next_index;
	next_index_itr=map_next_index->find(the_char);
	if (next_index_itr==map_next_index->end())
	{
		printf("�ַ��Ķ�����������ʧ��\n");
		return 0;
	}
	quary_next_index=&(next_index_itr->second);//�ҵ��ַ��Ķ������� ��ȡ�ַ���һ�������ļ�����Ϣ
	quary_next_index->index_arear_array_length=quary_next_index->index_file_length/sizeof(Index__);//�洢��������������ĸ���
	if (1==quary_next_index->in_memory_flg)
	{
		//���ԭ�����ع�  �Ͳ��ü�����
		return 1;
	}
	//quary_next_index->index_arear_array=(Index__*)malloc(quary_next_index->index_file_length);
	int res=read_iamwall_send((void**)&(quary_next_index->index_arear_array),
		index_fp,quary_next_index->index_file_offset,
		quary_next_index->index_file_length);

	if (res<=0)
	{
		printf("һ����������ʧ��\n");
		return 0;
	}//ǧ��ע���ͷ��ڴ�
	quary_next_index->in_memory_flg=1;
	//ע�ⲻҪ�ر��ļ�
	return 1;
}

int free_index_in_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index)//�ͷ�ĳһ���ַ��Ķ��������Ϲ��صĻ�����
{//�ͷųɹ�����1   ���򷵻�0  ����㷨���ṩ��LRU�ڴ�����õ�
	//ͨ������20140801
	if (!map_next_index)
	{
		printf("����������Ϊ��ʧ��\n");
		return 0;
	}
	//����ͨ���ַ����Ҷ��������Ľڵ�
	MAP_QUARY_NEXT_INDEX_AREAR::iterator next_index_itr;
	QUERY_NEXT_INDEX * quary_next_index=NULL;
	next_index_itr=map_next_index->find(the_char);
	if (next_index_itr==map_next_index->end())
	{
		printf("�ַ��Ķ�����������ʧ��\n");
		return 0;
	}
	quary_next_index=&(next_index_itr->second);//�ҵ��ַ��Ķ������� ��ȡ�ַ���һ�������ļ�����Ϣ
	if (!quary_next_index->index_arear_array)
	{
		printf("���������еļ��ص�һ�������ǿյ�");
		return 1;
	}
	free(quary_next_index->index_arear_array);//����ط����ܳ���big  ��Ϊ���ܻ���������һ���������ָ����Ұָ�� ����Ҫ���ж�һ�� ����Ҫ���ϳ�ʼ�����������ڵ�
	quary_next_index->index_arear_array=NULL;
	quary_next_index->in_memory_flg=0;
	quary_next_index->index_arear_array_length=0;
	return 1;
}
