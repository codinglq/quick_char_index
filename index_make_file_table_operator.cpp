#include "index_make_file_table_operator.h"
//#define GET_FILE_SIZE(info) (info->nFileSizeHigh)*(MAXDWORD+1)+info->nFileSizeLow)/1024)
void init_file_table(FILE_TABLE * the_file_table)//初始化文件映射表的函数
{
	the_file_table->file_table_size=0;
#if 0
		printf("文件映射表初始化完成\n");
#endif
}

size_t get_file_size(WIN32_FIND_DATA* find_data)//获取文件大小
{
	if (!find_data)
	{
		printf("获取文件大小失败，失败原因文件信息数据为空\n");
		return 0;
	}
	return (find_data->nFileSizeHigh*(MAXDWORD+1)+find_data->nFileSizeLow);
}

void scanf_file(wstring dir_path,FILE_TABLE * file_table)//扫描文件夹
{//system("cmd /c dir c:\\windows\\*.* /a-d /b /s >c:\\allfiles.txt");这个留着备用
	//然后从文件c:\\allfiles.txt里面逐行读。
	stack<wstring>find_file;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	wstring RealPath;
	int RealPos=0;
	find_file.push(dir_path);
	/************************************************************************/
	/* 真的很费时  20140805凌晨coding by lq                               */
	/************************************************************************/
	while (!find_file.empty())
	{
		wstring find_path = find_file.top();
		find_file.pop();
		wstring dir;
		//int index_of_slash = find_path.rfind(L'\\');
		//dir = find_path.substr(index_of_slash + 1 ,find_path.length() - index_of_slash);//string substr(int pos = 0,int n = npos) const;//返回pos开始的n个字符组成的字符串
		find_path = find_path + L"\\*.*";

		hFind = ::FindFirstFile(find_path.c_str(),&FindFileData);
		RealPos=find_path.find(L"\\*.*");
		find_path.replace(RealPos,4,L"");
		if (hFind == INVALID_HANDLE_VALUE)
			continue;
		do
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(FindFileData.cFileName,L"." )==0||wcscmp(FindFileData.cFileName,L"..")==0){//是当前文件夹或者是上级目录
					//RealPath=find_path+FindFileData.cFileName/*+JudgeFileAttributes(&FindFileData)*/;//输出文件夹
					continue ;

				}else{
					//这个地方是查找到文件夹的情况
					//find_path = find_path.assign(find_path, 0 ,find_path.rfind(L'\\'));//string &assign(const string &s,int start,int n);//把字符串s中从start开始的n个字符赋给当前字符
					find_path = find_path +L"\\" + FindFileData.cFileName;
					find_file.push(find_path);
					int pos=find_path.rfind(L"\\");
					find_path.replace(pos,find_path.length()-pos,L"");
					//find_path = find_path.assign(find_path, 0 ,find_path.rfind(L'\\'));
				}
			}
			else
			{//这个地方是查找到文件的情况
				RealPath=L"";
				RealPath=find_path+L"\\"+FindFileData.cFileName/*+JudgeFileAttributes(&FindFileData)*/;
				//在这里过滤掉不是txt的文件  根据文件的后缀判断
#if 1
				if ((RealPath.substr(RealPath.length()-3)==L"txt"||RealPath.substr(RealPath.length()-3)==L"TXT")
					&&(get_file_size(&FindFileData)))
				{//根据后缀名过滤文本文件
					file_table->the_file_table.push_back(RealPath);
					++file_table->file_table_size;
				}
#endif
			}
		}
		while(FindNextFile(hFind,&FindFileData));    
	}
}

FILE_TABLE * create_file_table(TCHAR * dir_path)//获得一个文件映射表 扫面dir下的文件
{//创建一个文件映射表  注意释放内存哦
	FILE_TABLE * file_table=new FILE_TABLE;
	init_file_table(file_table);
	//这里是开始创建文件映射表
	scanf_file(dir_path,file_table);
	return file_table;
}

FILE_RES_FILEPATH_VECTOR * query_file_table(FILE_RES_FILENUMBER_LIST * file_number_list)//在文件映射表中根据文件号查询文件路径
{
	return NULL;
}

int free_file_table(FILE_TABLE * the_file_table)//释放文件映射表
{
	return 0;
}

int write_file_table_to_file(TCHAR * file_name,FILE_TABLE * file_table)//将文件映射写入文件
{
	/*这个太浪费空间了  想办法改进要*/
	size_t file_name_buf_size=file_table->file_table_size;//记录文件个数 好用来分配空间
	TCHAR * file_name_buf=(TCHAR *)calloc(file_name_buf_size,MAX_PATH*sizeof(TCHAR));//这样虽然浪费空间 但是。。。
	size_t count_file_name=0;
	TCHAR *temp_name_buf=file_name_buf;
	while (count_file_name<file_name_buf_size)
	{
		wcscpy(temp_name_buf,(TCHAR *)(file_table->the_file_table[count_file_name].c_str()));
		temp_name_buf+=MAX_PATH;
		++count_file_name;
	}

	char file_name_temp[MAX_PATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_name,-1,file_name_temp,MAX_PATH,NULL,FALSE);
	if (write_buf_to_file(file_name_buf,sizeof(TCHAR)*file_name_buf_size*MAX_PATH,file_name_temp,"wb")>0);
	{
#if 0
			printf("文件映射表写入文件成功\n");
#endif
		return 1;
	}
	printf("文件映射表写入文件失败\n");
	return 0;
}

FILE_RES_FILEPATH_VECTOR * read_file_table_to_buf(TCHAR * file_name)//将文件索引表文件中的内容读取到内存中的链表中
{/************************************************************************/
	/* 这个模块写的太浪费内存了                                                   */
	/************************************************************************/
	char file_name_temp[MAX_PATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_name,-1,file_name_temp,MAX_PATH,NULL,FALSE);
	FILE * fp=fopen(file_name_temp,"rb");
	if (!fp)
	{
		perror("file_table_file:");
		return NULL;
	}
	size_t file_size=0;
	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);

	TCHAR * file_name_buf=(TCHAR *)calloc(file_size,sizeof(TCHAR));
	fseek(fp,0L,SEEK_SET);
	fread(file_name_buf,file_size,1,fp);
	FILE_RES_FILEPATH_VECTOR * temp_file_table_list=new FILE_RES_FILEPATH_VECTOR;
	size_t count_file_table_size=0;
	while (count_file_table_size<(file_size/(MAX_PATH*sizeof(TCHAR))))
	{
		//f读取到链表中
		TCHAR temp[MAX_PATH]={};
		wcscpy(temp,file_name_buf);
		file_name_buf+=MAX_PATH;
		//这个地方的文件指针会后移吧
		wstring temp_file_name(temp);
		temp_file_table_list->push_back(temp_file_name);
		++count_file_table_size;
	}
	return temp_file_table_list;
}

FILE_RES_FILEPATH_VECTOR * query_file_table(FILE_RES_FILENUMBER_LIST * file_number_list,FILE_RES_FILEPATH_VECTOR * file_table_list)
{//在文件映射表中根据文件号查询文件路径     这个是查询得到的文件号表                                这个是扫描硬盘制定目录生成的文件表

	if (!file_number_list)
	{
		printf("传入的文件号表是空的\n");
		return NULL;
	}
	if (!file_table_list)
	{
		printf("文件索引表是空的\n");
		return NULL;
	}

	FILE_RES_FILEPATH_VECTOR * temp_res=new FILE_RES_FILEPATH_VECTOR;
	//一定要释放内存
	FILE_RES_FILENUMBER_LIST::iterator itr_start=file_number_list->begin();

	while (itr_start!=file_number_list->end())
	{
		temp_res->push_back(file_table_list->at(*itr_start));
#if 1//这个输出功能是测试版本使用的
		wcout<<L"文件路径："<<file_table_list->at(*itr_start)<<endl;
#endif
		++itr_start;
	}
	return temp_res;//注意释放内存啊啊啊啊
}
