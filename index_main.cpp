#include "index_def.h"
#include "index_elem_operator.h"
#include "index_file_operator.h"
#include "index_make_operator.h"
#include "index_quary_operator.h"
#include "index_make_file_table_operator.h"
#include "index_usr_interface.h"
#define TEST_INDEX_ELEM
int main()
{

#if 0
	TCHAR str[]=L"abcd";
	size_t loc=0;
	TCHAR next_char=get_next_char(str,&loc);
	next_char=get_next_char(str,&loc);
	next_char=get_next_char(str,&loc);
	next_char=get_next_char(str,&loc);
	next_char=get_next_char(str,&loc);
#endif
#if 0 //�������20140727
	// ������������CurrCharLoc�Ͷ�ȡCurrCharLoc����
	Index__ *temp=(Index__*)calloc(1,sizeof(Index__));
	unsigned int loc=0;
	loc=set_currcharloc_index__(temp,1024);
	printf("���ý�ȥ��loc%d\n",loc);
	//����д��NextChar
	set_nextchar_index__(temp,L'A');
	printf("%c\n",get_nextchar_index__(temp));

	loc=get_currcharloc_index__(temp);
	printf("��ȡ����loc%d\n",loc);
	//����д��NextCharLoc�Ͷ�ȡNextCharLoc

	loc=set_nextcharloc_index__(temp,1110);
	printf("���ý�ȥ��loc%d\n",loc);
	loc=get_nextcharloc_index__(temp);
	printf("��ȡ������loc%d\n",loc);
	//����д���ļ��źͶ�ȡ�ļ���
	loc=set_filenumber_index__(temp,1230);
	printf("���ý�ȥ��filenumber%d\n",loc);
	loc=get_filenumber_index__(temp);
	printf("��ȡ����filenumber=%d\n",loc);

	loc=set_currcharloc_index__(temp,10230);
	printf("���ý�ȥ��loc%d\n",loc);
	//����д��NextChar
	set_nextchar_index__(temp,L'A');
	printf("%c\n",get_nextchar_index__(temp));
#endif
	//�����ļ���������غ���
#if 0

	FILE * fp=fopen("a.txt","r");
	char *buf=NULL;
	int size_file=read_iamwall_send((void**)&buf,fp,3,1);
	printf("%s\n",buf);
	fclose(fp);

	//���Զ�ȡ���������ĺ���
	BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
	read_file_to_buf_string(_T("a.txt"),buf_string);
#endif

#if 0
	//����get_next_splid_wnd
	BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
	read_file_to_buf_string(_T("a.txt"),buf_string);

	int res=get_next_splid_wnd(buf_string);
	while (res!=-1)
	{//ò�����һ���ַ��޷�����������  ������Ҫ���ı����������һ��������ʾ��
		res=get_next_splid_wnd(buf_string);
	}//�㶨 ֻ�����󻬶�һ������
#endif

//���Խ�����һ���ڴ������ĺ���
	wcout.imbue(locale(""));//��仰�ǹؼ� ���ڿ��ֽ����������

#if 1
	create_index_on(L"D:\\file_scanf_test");//�����û��ӿ�
	query_index_on(L"D:\\file_scanf_test",L":sasa");
#endif
#if 0
	//MemoryIndexTree * temp_index=make_memory_index_tree(_T("a.txt"),"b.txt",1);
	MemoryIndexTree * temp_index=make_memory_index_tree_all_file(L"D:\\file_scanf_test",NULL);
	//���ɵ�һ�ε��ڴ��������
	THE_NEXT_INDEX * temp_next_index=make_next_index__(temp_index);
	//ò�����������һ�ζ��������ĳ���Ҳ�ɹ���
	printf("%u\n",sizeof(NEXT_INDEX));
	int res=write_buf_to_file(temp_next_index->next_index__,
		temp_next_index->length/* ����ط���length���ֽ������Ǹ���*/
		,"next_index.txt","wb");
	//д���ļ������Բ���
	//free(temp_next_index);
	//free(temp_index);
	if (res<=0)
	{
		printf("��������д��ʧ��\n");
	}
	//���Զ�ȡ������������
	//THE_NEXT_INDEX * ttemp_next_index=NULL;
	//res=read_next_index_to_buf("next_index.txt",&ttemp_next_index);
	//ע��Ҫ�ͷ��ڴ氡

	//�������ɶ����ڴ��������
	write_index_to_file(temp_index,"index__ddb.ddb","wb");//ͨ������

	//����Ķ���д�뽨�������õĳ��򡣡�����������������������������������������������������������������������������������������������������������������������

	MAP_QUARY_NEXT_INDEX_AREAR * tttemp=make_query_index("next_index.txt");//ͨ���˲�����
	//read_index_to_next_index((L'a',tttemp,);
	FILE * fp=fopen("index__ddb.ddb","rb");
	
	if (!fp)
	{
		printf("�����ļ���ʧ����\n");
	}
	fseek(fp,0L,SEEK_END);
	int now=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	now=ftell(fp);
	FILE_RES_FILENUMBER_LIST *temp_file_num=new FILE_RES_FILENUMBER_LIST;
	temp_file_num=query_iamwall(L"abc",tttemp,fp,temp_file_num);
	FILE_RES_FILEPATH_VECTOR * temp_res=read_file_table_to_buf(L"file_table.txt");
	query_file_table(temp_file_num,temp_res);//���Խ��������
	//�������ε���
	temp_file_num=query_iamwall(L"d",tttemp,fp,temp_file_num);
	query_file_table(temp_file_num,temp_res);//���Խ��������
	/*
	res=read_index_to_next_index(L'��',tttemp,fp);//ͨ��������������һ���������ڴ溯���������

	if (res==1)
	{
		printf("ͨ�������������ص�һ�������ɹ�\n");
	}

	res=read_index_to_next_index(L'a',tttemp,fp);
	if (res==1)
	{
		printf("ͨ�������������ص�һ�������ɹ�\n");
	}
	//�����ͷŶ��������м��ص�һ������
	res=free_index_in_next_index(L'a',tttemp);
	if (res==1)
	{
		printf("�����������ص�һ�������ͷųɹ�\n");
	}

	res=free_index_in_next_index(L'��',tttemp);
	if (res==1)
	{
		printf("�����������ص�һ�������ͷųɹ�\n");
	}

	//��������������ǲ�����ȷ��
	printf("%u\n",temp_index->size());

	//���Ի�ȡ�ڴ����������Ա�ĺ���

	//MEMORY_INDEX_ARRAY_BUF * temp=get_memory_index_array(temp_index);//ͨ������ ���ǲ�Ҫ�����ͷ��ڴ� 20140730
	write_index_to_file(temp_index,"index__ddb.ddb","w");//ͨ������
	*/
	/*MemoryIndexTree::iterator itr=temp_index->begin();
	while (itr!=temp_index->end())
	{
		wcout<<"curr_char:"<<itr->first<<endl;
		printf("curr_char_loc:%d\n",get_currcharloc_index__(&(itr->second->at(0))));
		wcout<<"next_char:"<<get_nextchar_index__(&(itr->second->at(0)))<<endl;
		printf("next_char_loc:%d\n",get_nextcharloc_index__(&(itr->second->at(0))));
		printf("\n");
		itr++;
	}*/
#endif

#if 0
	MemoryIndexTree * index_tree=make_memory_index_tree_all_file(L"D:\\file_scanf_test",NULL);
#endif

#if 0
	//����д���ļ�����
	char * buf="abcdefg";
	int res=write_buf_to_file(buf,strlen(buf)+1,"ttt.txt","w");
	if (res>0)
	{
		printf("success!\n");
		char *temp=nullptr;
		FILE * fp=fopen("ttt.txt","r");
		perror("fie error");
		int size_file=read_iamwall_send((void**)&temp,fp,0,strlen(buf)+1);
		printf("��ȡ�����ļ�%s\n",temp);
	}
	else{
		printf("Error\n");
	}
	//20140730����ͨ������ ��ô��������������ȡ����Ҳͨ���˻ع����
#endif

#if 0
	OVER_COME_SET * temp_set=make_over_come_set("a.txt");//ͨ������  �����Ҫ�ڽ��������ĺ����м����ų���
#endif

#if 0//����ɨ���ļ���ģ��

	FILE_TABLE flile_table;
	init_file_table(&flile_table);
	wstring file_path=L"D:\\kankan";
	scanf_file(file_path,&flile_table);

#endif
	return 0;
}