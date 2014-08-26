#include "index_make_file_table_operator.h"
//#define GET_FILE_SIZE(info) (info->nFileSizeHigh)*(MAXDWORD+1)+info->nFileSizeLow)/1024)
void init_file_table(FILE_TABLE * the_file_table)//��ʼ���ļ�ӳ���ĺ���
{
	the_file_table->file_table_size=0;
#if 0
		printf("�ļ�ӳ����ʼ�����\n");
#endif
}

size_t get_file_size(WIN32_FIND_DATA* find_data)//��ȡ�ļ���С
{
	if (!find_data)
	{
		printf("��ȡ�ļ���Сʧ�ܣ�ʧ��ԭ���ļ���Ϣ����Ϊ��\n");
		return 0;
	}
	return (find_data->nFileSizeHigh*(MAXDWORD+1)+find_data->nFileSizeLow);
}

void scanf_file(wstring dir_path,FILE_TABLE * file_table)//ɨ���ļ���
{//system("cmd /c dir c:\\windows\\*.* /a-d /b /s >c:\\allfiles.txt");������ű���
	//Ȼ����ļ�c:\\allfiles.txt�������ж���
	stack<wstring>find_file;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	wstring RealPath;
	int RealPos=0;
	find_file.push(dir_path);
	/************************************************************************/
	/* ��ĺܷ�ʱ  20140805�賿coding by lq                               */
	/************************************************************************/
	while (!find_file.empty())
	{
		wstring find_path = find_file.top();
		find_file.pop();
		wstring dir;
		//int index_of_slash = find_path.rfind(L'\\');
		//dir = find_path.substr(index_of_slash + 1 ,find_path.length() - index_of_slash);//string substr(int pos = 0,int n = npos) const;//����pos��ʼ��n���ַ���ɵ��ַ���
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
				if (wcscmp(FindFileData.cFileName,L"." )==0||wcscmp(FindFileData.cFileName,L"..")==0){//�ǵ�ǰ�ļ��л������ϼ�Ŀ¼
					//RealPath=find_path+FindFileData.cFileName/*+JudgeFileAttributes(&FindFileData)*/;//����ļ���
					continue ;

				}else{
					//����ط��ǲ��ҵ��ļ��е����
					//find_path = find_path.assign(find_path, 0 ,find_path.rfind(L'\\'));//string &assign(const string &s,int start,int n);//���ַ���s�д�start��ʼ��n���ַ�������ǰ�ַ�
					find_path = find_path +L"\\" + FindFileData.cFileName;
					find_file.push(find_path);
					int pos=find_path.rfind(L"\\");
					find_path.replace(pos,find_path.length()-pos,L"");
					//find_path = find_path.assign(find_path, 0 ,find_path.rfind(L'\\'));
				}
			}
			else
			{//����ط��ǲ��ҵ��ļ������
				RealPath=L"";
				RealPath=find_path+L"\\"+FindFileData.cFileName/*+JudgeFileAttributes(&FindFileData)*/;
				//��������˵�����txt���ļ�  �����ļ��ĺ�׺�ж�
#if 1
				if ((RealPath.substr(RealPath.length()-3)==L"txt"||RealPath.substr(RealPath.length()-3)==L"TXT")
					&&(get_file_size(&FindFileData)))
				{//���ݺ�׺�������ı��ļ�
					file_table->the_file_table.push_back(RealPath);
					++file_table->file_table_size;
				}
#endif
			}
		}
		while(FindNextFile(hFind,&FindFileData));    
	}
}

FILE_TABLE * create_file_table(TCHAR * dir_path)//���һ���ļ�ӳ��� ɨ��dir�µ��ļ�
{//����һ���ļ�ӳ���  ע���ͷ��ڴ�Ŷ
	FILE_TABLE * file_table=new FILE_TABLE;
	init_file_table(file_table);
	//�����ǿ�ʼ�����ļ�ӳ���
	scanf_file(dir_path,file_table);
	return file_table;
}

FILE_RES_FILEPATH_VECTOR * query_file_table(FILE_RES_FILENUMBER_LIST * file_number_list)//���ļ�ӳ����и����ļ��Ų�ѯ�ļ�·��
{
	return NULL;
}

int free_file_table(FILE_TABLE * the_file_table)//�ͷ��ļ�ӳ���
{
	return 0;
}

int write_file_table_to_file(TCHAR * file_name,FILE_TABLE * file_table)//���ļ�ӳ��д���ļ�
{
	/*���̫�˷ѿռ���  ��취�Ľ�Ҫ*/
	size_t file_name_buf_size=file_table->file_table_size;//��¼�ļ����� ����������ռ�
	TCHAR * file_name_buf=(TCHAR *)calloc(file_name_buf_size,MAX_PATH*sizeof(TCHAR));//������Ȼ�˷ѿռ� ���ǡ�����
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
			printf("�ļ�ӳ���д���ļ��ɹ�\n");
#endif
		return 1;
	}
	printf("�ļ�ӳ���д���ļ�ʧ��\n");
	return 0;
}

FILE_RES_FILEPATH_VECTOR * read_file_table_to_buf(TCHAR * file_name)//���ļ��������ļ��е����ݶ�ȡ���ڴ��е�������
{/************************************************************************/
	/* ���ģ��д��̫�˷��ڴ���                                                   */
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
		//f��ȡ��������
		TCHAR temp[MAX_PATH]={};
		wcscpy(temp,file_name_buf);
		file_name_buf+=MAX_PATH;
		//����ط����ļ�ָ�����ư�
		wstring temp_file_name(temp);
		temp_file_table_list->push_back(temp_file_name);
		++count_file_table_size;
	}
	return temp_file_table_list;
}

FILE_RES_FILEPATH_VECTOR * query_file_table(FILE_RES_FILENUMBER_LIST * file_number_list,FILE_RES_FILEPATH_VECTOR * file_table_list)
{//���ļ�ӳ����и����ļ��Ų�ѯ�ļ�·��     ����ǲ�ѯ�õ����ļ��ű�                                �����ɨ��Ӳ���ƶ�Ŀ¼���ɵ��ļ���

	if (!file_number_list)
	{
		printf("������ļ��ű��ǿյ�\n");
		return NULL;
	}
	if (!file_table_list)
	{
		printf("�ļ��������ǿյ�\n");
		return NULL;
	}

	FILE_RES_FILEPATH_VECTOR * temp_res=new FILE_RES_FILEPATH_VECTOR;
	//һ��Ҫ�ͷ��ڴ�
	FILE_RES_FILENUMBER_LIST::iterator itr_start=file_number_list->begin();

	while (itr_start!=file_number_list->end())
	{
		temp_res->push_back(file_table_list->at(*itr_start));
#if 1//�����������ǲ��԰汾ʹ�õ�
		wcout<<L"�ļ�·����"<<file_table_list->at(*itr_start)<<endl;
#endif
		++itr_start;
	}
	return temp_res;//ע���ͷ��ڴ氡������
}
