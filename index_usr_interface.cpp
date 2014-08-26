#include "index_usr_interface.h"

int create_index_on(TCHAR * file_path)//���������Ľӿ� �����к�ͼ�ν���ͨ��
{

	char mutil_file_path[MAX_PATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_path,-1,mutil_file_path,MAX_PATH,NULL,FALSE);
	MemoryIndexTree * temp_index=make_memory_index_tree_all_file(file_path,NULL);
	THE_NEXT_INDEX * temp_next_index=make_next_index__(temp_index);
	char index_file_name[MAX_PATH]={};
	strcpy(index_file_name,mutil_file_path);
	strcat(index_file_name,INDEX_DB_NAME);

	char next_index_file_name[MAX_PATH]={};
	strcpy(next_index_file_name,mutil_file_path);
	strcat(next_index_file_name,NEXT_INDEX_DB_NAME);
	int res=write_buf_to_file(temp_next_index->next_index__,
		temp_next_index->length/* ����ط���length���ֽ������Ǹ���*/
		,next_index_file_name,"wb");
	if (res<=0)
	{
		printf("��������д��ʧ��\n");
	}else
	{
		printf("��������д��ɹ�\n");
	}

	if (write_index_to_file(temp_index,index_file_name,"wb")>0)
	{
		printf("һ������д��ɹ�\n");
	}
	return 0;
}

int query_index_on(TCHAR * file_path,TCHAR * query_string)//����������е��û��ӿ�
{
	if (!file_path||(!query_string))
	{
		printf("����·�����ô���\n");
		return -1;
	}
	char mutil_file_path[MAX_PATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_path,-1,mutil_file_path,MAX_PATH,NULL,FALSE);
	char index_file_name[MAX_PATH]={};
	strcpy(index_file_name,mutil_file_path);
	strcat(index_file_name,INDEX_DB_NAME);

	char next_index_file_name[MAX_PATH]={};
	strcpy(next_index_file_name,mutil_file_path);
	strcat(next_index_file_name,NEXT_INDEX_DB_NAME);

	MAP_QUARY_NEXT_INDEX_AREAR * tttemp=make_query_index(next_index_file_name);//ͨ���˲�����
	//read_index_to_next_index((L'a',tttemp,);
	FILE * fp=fopen(index_file_name,"rb");

	if (!fp)
	{
		printf("�����ļ���ʧ����\n");
	}
	fseek(fp,0L,SEEK_END);
	int now=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	now=ftell(fp);
	TCHAR file_table_name[100]={};
	wcscpy(file_table_name,file_path);
	wcscat(file_table_name,FILE_TABLE_DB_NAME);
	wstring query_str;
	FILE_RES_FILEPATH_VECTOR * temp_res=read_file_table_to_buf(file_table_name);
	FILE_RES_FILENUMBER_LIST *temp_file_num=new FILE_RES_FILENUMBER_LIST;
	while (1)
	{
		printf("�����ѯ������ѯ��");
		wcin>>query_str;
		
		temp_file_num=query_iamwall((TCHAR*)query_str.c_str(),tttemp,fp,temp_file_num);
		query_file_table(temp_file_num,temp_res);//���Խ��������
		//�������ε���
	}
	return -1;
}