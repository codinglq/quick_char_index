#include "index_file_operator.h"
#include "index_elem_operator.h"
#define MAX_FILE_NAMEPATH 100

int read_file_to_buf(TCHAR * file_name,TCHAR * buf)//读取文件到缓冲区  文本文件
{
	FILE *fp;
	//DWORD len;
	char FileNameTemp[MAX_FILE_NAMEPATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_name,-1,FileNameTemp,MAX_FILE_NAMEPATH,NULL,FALSE);
	fp=fopen(FileNameTemp,"r");
	if (NULL==fp)
	{
		wcout<<"文件打开失败"<<endl;
		return -1;
	}
	size_t FileSize=0;
	fseek(fp,0L,SEEK_END);
	FileSize=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	//在这里不做缓冲区溢出处理
	buf=(TCHAR*)malloc(sizeof(TCHAR)*(FileSize+1));
	char * MulitCharBuf=(char*)malloc(sizeof(char)*(FileSize+1));
	memset(buf,'\0',sizeof(TCHAR)*FileSize);
	memset(MulitCharBuf,'\0',FileSize);
	//发现设置成内存是\0依然不影响后面的莫名的乱码的产生
	//Resource leak: fp
	fread(MulitCharBuf,FileSize,1L,fp);
	MultiByteToWideChar(CP_ACP,0,MulitCharBuf,-1,buf,sizeof(TCHAR)*FileSize);
	fclose(fp);
	return 0;
}

int read_file_to_buf_string(TCHAR * file_name,BUF_STRING *buf_string)//这个更好，直接读取到bufstring中去了
{

	FILE *fp=nullptr;
	//DWORD len;
	char FileNameTemp[MAX_FILE_NAMEPATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_name,-1,FileNameTemp,MAX_FILE_NAMEPATH,NULL,FALSE);
	fp=fopen(FileNameTemp,"r");
	if (NULL==fp)
	{
		wcout<<"文件打开失败"<<endl;
		return -1;
	}
	size_t FileSize=0;
	fseek(fp,0L,SEEK_END);
	FileSize=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	//在这里不做缓冲区溢出处理
	buf_string->the_char_buf=(TCHAR*)calloc(sizeof(TCHAR),FileSize+1);
	char * MulitCharBuf=(char*)calloc(sizeof(char),FileSize+1);
	// 	memset(buf_string->the_char_buf,'\0',sizeof(TCHAR)*FileSize);
	// 	memset(MulitCharBuf,'\0',FileSize);
	//发现设置成内存是\0依然不影响后面的莫名的乱码的产生
	//切记 对于类和结构体 不能随意内存置空
	//上述的bug已经解决 解决时间20140727 原因是动态分配的字节数太少了  导致字符串结尾时候不是'\0'
	fread(MulitCharBuf,FileSize,1,fp);
	MultiByteToWideChar(CP_ACP,0,MulitCharBuf,-1,buf_string->the_char_buf,/*sizeof(TCHAR)**/FileSize);
	buf_string->buf_size=wcslen(buf_string->the_char_buf);//这个才是真正的缓冲区中的字符个数  原来的写成了缓冲区长度 这个事不应该的哦
	free(MulitCharBuf);
	if (FileSize<1)
	{
		wcout<<L"文本这么短，建个屁索引"<<endl;
		fclose(fp);
		return -1;
	}
	//初始化滑动窗口
	buf_string->splid_wnd_char.first_char=buf_string->the_char_buf[0];
	buf_string->splid_wnd_char.last_char=buf_string->the_char_buf[1];
	buf_string->splid_wnd_char.first_char_loc=0;
	fclose(fp);
	//.......
	return 0;
}

int read_iamwall_get(void * buf,FILE * fp,int file_off_set,int file_length)//随机读取文件
{/*一定要确保buf的大小能够放得下读取的文件  这个又涉及到是我这个函数分配空间还是调用者分配的问题了*/
	//fseek(fp,0L,SEEK_SET);//把文件指针移动到头部
	fseek(fp,file_off_set,SEEK_SET);
	int res=fread(buf,file_length,1,fp);
	if (1==res)
	{
		return 1;
	}else
	{
		return -1;//读取失败了
	}
}

int read_iamwall_send(void **p_buf,FILE * fp,int file_off_set,int file_length)//缓冲区由这个函数分配
{/* 这个是本函数分配的缓冲区 所以调用函数一定要记得释放空间 否则会内存泄漏*/
	/*注意了哈  比如一个文件有四个字节 要想读取最后一个字节就要偏移到文件长度减去1的位置然后读取一个字节*/
	void * the_buf=calloc(file_length+1,sizeof(char));
	*p_buf=the_buf;
	if (feof(fp))
	{
		printf("遇到文件结束 看来文件偏移量设置有点问题\n");
		return -1;
	}

	fseek(fp,0L,SEEK_SET);
	fseek(fp,file_off_set,SEEK_SET);
	int now=ftell(fp);
	printf("file_off_set=%d,file_length=%d\n",file_off_set,file_length);
	int res=fread(the_buf,file_length,1,fp);
	fseek(fp,0L,SEEK_SET);
	if (1==res)
	{
		return 1;
	}
	else
	{
		return -1;//读取失败了
	}
	/*该函数通过测试了*/
}

int write_buf_to_file(void *buf,size_t buf_length,char * file_name,char *write_flg/*写入时候的模式*/)
{//将缓冲区数据写入文件中
	if (!buf)
	{
		printf("缓冲区为null 请检查缓冲区！\n");
		return -1;
	}
	if (!write_flg)
	{
		printf("文件写入模式设置错误！\n");
		return -1;
	}

	FILE * fp=fopen(file_name,write_flg);//创建文件
	if (!fp)
	{
		perror("文件打开失败 检查文件名称路径，文件写入标志位！");
		return -1;
	}

	if (0>=buf_length)
	{
		printf("缓冲区长度太小 怎么能是0 呢！\n");
		return -1;
	}


	//错误检查

#if 0
	Index__* temp=(Index__*)buf;
	int coun=0;
	while (temp->next_char!='\0')
	{
		printf("test of file write\n");
		printf("curr_char_loc:%d\n",get_currcharloc_index__(temp));
		wcout<<"next_char:"<<get_nextchar_index__(temp)<<endl;
		printf("next_char_loc:%d\n",get_nextcharloc_index__(temp));
		printf("\n");
		++temp;
		++coun;
	}
	printf("%d\n",coun);
	printf("sizeof(Index__)=%u\n",sizeof(Index__));
#endif

	int res=0;
	res=fwrite(buf,buf_length,1,fp);
	fflush(fp);//同步文件
	fclose(fp);
	fp=NULL;
	return buf_length;
	//就这么简单吗？

}

int read_next_index_to_buf(char * next_file_name,THE_NEXT_INDEX **next_buf)//读取二级索引到内存的缓冲区
{
	if (!next_file_name)
	{
		printf("二级索引文件路径出错！请检查\n");
		return -1;
	}
	FILE * fp=fopen(next_file_name,"rb");
	if (!fp)
	{
		printf("二级索引文件打开失败！请检查文件的权限\n");
		return -1;
	}
	size_t file_size=0;
	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);//这个地方查看文件大小
	fseek(fp,0L,SEEK_SET);//将文件指针回溯

	THE_NEXT_INDEX *temp_next_index=(THE_NEXT_INDEX*)malloc(sizeof(THE_NEXT_INDEX)+file_size);
	fread(temp_next_index->next_index__,file_size,1,fp);
	fclose(fp);
	temp_next_index->length=file_size;
	*next_buf=temp_next_index;

	/************************************************************************/
	/* 一定要注意释放内存啊                                                         */
	/************************************************************************/
	return file_size;
}