#include "index_usr_interface.h"

int create_index_on(TCHAR * file_path)//建立索引的接口 命令行和图形界面通用
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
		temp_next_index->length/* 这个地方的length是字节数不是个数*/
		,next_index_file_name,"wb");
	if (res<=0)
	{
		printf("二级索引写入失败\n");
	}else
	{
		printf("二级索引写入成功\n");
	}

	if (write_index_to_file(temp_index,index_file_name,"wb")>0)
	{
		printf("一级索引写入成功\n");
	}
	return 0;
}

int query_index_on(TCHAR * file_path,TCHAR * query_string)//这个是命令行的用户接口
{
	if (!file_path||(!query_string))
	{
		printf("索引路径配置错误\n");
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

	MAP_QUARY_NEXT_INDEX_AREAR * tttemp=make_query_index(next_index_file_name);//通过了测试啦
	//read_index_to_next_index((L'a',tttemp,);
	FILE * fp=fopen(index_file_name,"rb");

	if (!fp)
	{
		printf("索引文件打开失败了\n");
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
		printf("输入查询串来查询：");
		wcin>>query_str;
		
		temp_file_num=query_iamwall((TCHAR*)query_str.c_str(),tttemp,fp,temp_file_num);
		query_file_table(temp_file_num,temp_res);//测试结果集函数
		//测试两次调用
	}
	return -1;
}