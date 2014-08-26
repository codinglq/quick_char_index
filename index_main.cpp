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
#if 0 //测试完毕20140727
	// 测试索引存入CurrCharLoc和读取CurrCharLoc函数
	Index__ *temp=(Index__*)calloc(1,sizeof(Index__));
	unsigned int loc=0;
	loc=set_currcharloc_index__(temp,1024);
	printf("设置进去的loc%d\n",loc);
	//测试写入NextChar
	set_nextchar_index__(temp,L'A');
	printf("%c\n",get_nextchar_index__(temp));

	loc=get_currcharloc_index__(temp);
	printf("获取到的loc%d\n",loc);
	//测试写入NextCharLoc和读取NextCharLoc

	loc=set_nextcharloc_index__(temp,1110);
	printf("设置进去的loc%d\n",loc);
	loc=get_nextcharloc_index__(temp);
	printf("读取出来的loc%d\n",loc);
	//测试写入文件号和读取文件号
	loc=set_filenumber_index__(temp,1230);
	printf("设置进去的filenumber%d\n",loc);
	loc=get_filenumber_index__(temp);
	printf("读取到的filenumber=%d\n",loc);

	loc=set_currcharloc_index__(temp,10230);
	printf("设置进去的loc%d\n",loc);
	//测试写入NextChar
	set_nextchar_index__(temp,L'A');
	printf("%c\n",get_nextchar_index__(temp));
#endif
	//测试文件操作的相关函数
#if 0

	FILE * fp=fopen("a.txt","r");
	char *buf=NULL;
	int size_file=read_iamwall_send((void**)&buf,fp,3,1);
	printf("%s\n",buf);
	fclose(fp);

	//测试读取到缓冲区的函数
	BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
	read_file_to_buf_string(_T("a.txt"),buf_string);
#endif

#if 0
	//测试get_next_splid_wnd
	BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
	read_file_to_buf_string(_T("a.txt"),buf_string);

	int res=get_next_splid_wnd(buf_string);
	while (res!=-1)
	{//貌似最后一个字符无法被索引到啊  看来需要对文本缓冲区添加一个结束标示符
		res=get_next_splid_wnd(buf_string);
	}//搞定 只许多向后滑动一个就行
#endif

//测试建立第一次内存索引的函数
	wcout.imbue(locale(""));//这句话是关键 关于宽字节输出的问题

#if 1
	create_index_on(L"D:\\file_scanf_test");//测试用户接口
	query_index_on(L"D:\\file_scanf_test",L":sasa");
#endif
#if 0
	//MemoryIndexTree * temp_index=make_memory_index_tree(_T("a.txt"),"b.txt",1);
	MemoryIndexTree * temp_index=make_memory_index_tree_all_file(L"D:\\file_scanf_test",NULL);
	//生成第一次的内存二级索引
	THE_NEXT_INDEX * temp_next_index=make_next_index__(temp_index);
	//貌似这个建立第一次二级索引的程序也成功了
	printf("%u\n",sizeof(NEXT_INDEX));
	int res=write_buf_to_file(temp_next_index->next_index__,
		temp_next_index->length/* 这个地方的length是字节数不是个数*/
		,"next_index.txt","wb");
	//写入文件看看对不对
	//free(temp_next_index);
	//free(temp_index);
	if (res<=0)
	{
		printf("二级索引写入失败\n");
	}
	//测试读取二级检索索引
	//THE_NEXT_INDEX * ttemp_next_index=NULL;
	//res=read_next_index_to_buf("next_index.txt",&ttemp_next_index);
	//注意要释放内存啊

	//测试生成二级内存检索索引
	write_index_to_file(temp_index,"index__ddb.ddb","wb");//通过测试

	//上面的都是写入建立索引用的程序。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。

	MAP_QUARY_NEXT_INDEX_AREAR * tttemp=make_query_index("next_index.txt");//通过了测试啦
	//read_index_to_next_index((L'a',tttemp,);
	FILE * fp=fopen("index__ddb.ddb","rb");
	
	if (!fp)
	{
		printf("索引文件打开失败了\n");
	}
	fseek(fp,0L,SEEK_END);
	int now=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	now=ftell(fp);
	FILE_RES_FILENUMBER_LIST *temp_file_num=new FILE_RES_FILENUMBER_LIST;
	temp_file_num=query_iamwall(L"abc",tttemp,fp,temp_file_num);
	FILE_RES_FILEPATH_VECTOR * temp_res=read_file_table_to_buf(L"file_table.txt");
	query_file_table(temp_file_num,temp_res);//测试结果集函数
	//测试两次调用
	temp_file_num=query_iamwall(L"d",tttemp,fp,temp_file_num);
	query_file_table(temp_file_num,temp_res);//测试结果集函数
	/*
	res=read_index_to_next_index(L'我',tttemp,fp);//通过二级索引加载一级索引到内存函数测试完毕

	if (res==1)
	{
		printf("通过二级索引加载的一级索引成功\n");
	}

	res=read_index_to_next_index(L'a',tttemp,fp);
	if (res==1)
	{
		printf("通过二级索引加载的一级索引成功\n");
	}
	//测试释放二级索引中加载的一级索引
	res=free_index_in_next_index(L'a',tttemp);
	if (res==1)
	{
		printf("二级索引加载的一级索引释放成功\n");
	}

	res=free_index_in_next_index(L'我',tttemp);
	if (res==1)
	{
		printf("二级索引加载的一级索引释放成功\n");
	}

	//输出索引来看看是不是正确的
	printf("%u\n",temp_index->size());

	//测试获取内存索引域线性表的函数

	//MEMORY_INDEX_ARRAY_BUF * temp=get_memory_index_array(temp_index);//通过测试 但是不要忘记释放内存 20140730
	write_index_to_file(temp_index,"index__ddb.ddb","w");//通过测试
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
	//测试写入文件函数
	char * buf="abcdefg";
	int res=write_buf_to_file(buf,strlen(buf)+1,"ttt.txt","w");
	if (res>0)
	{
		printf("success!\n");
		char *temp=nullptr;
		FILE * fp=fopen("ttt.txt","r");
		perror("fie error");
		int size_file=read_iamwall_send((void**)&temp,fp,0,strlen(buf)+1);
		printf("读取到的文件%s\n",temp);
	}
	else{
		printf("Error\n");
	}
	//20140730函数通过测试 这么看来上面的随机读取函数也通过了回归测试
#endif

#if 0
	OVER_COME_SET * temp_set=make_over_come_set("a.txt");//通过测试  下面就要在建立索引的函数中加入排除了
#endif

#if 0//测试扫描文件的模块

	FILE_TABLE flile_table;
	init_file_table(&flile_table);
	wstring file_path=L"D:\\kankan";
	scanf_file(file_path,&flile_table);

#endif
	return 0;
}