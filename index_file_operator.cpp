#include "index_file_operator.h"
#include "index_elem_operator.h"
#define MAX_FILE_NAMEPATH 100

int read_file_to_buf(TCHAR * file_name,TCHAR * buf)//��ȡ�ļ���������  �ı��ļ�
{
	FILE *fp;
	//DWORD len;
	char FileNameTemp[MAX_FILE_NAMEPATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_name,-1,FileNameTemp,MAX_FILE_NAMEPATH,NULL,FALSE);
	fp=fopen(FileNameTemp,"r");
	if (NULL==fp)
	{
		wcout<<"�ļ���ʧ��"<<endl;
		return -1;
	}
	size_t FileSize=0;
	fseek(fp,0L,SEEK_END);
	FileSize=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	//�����ﲻ���������������
	buf=(TCHAR*)malloc(sizeof(TCHAR)*(FileSize+1));
	char * MulitCharBuf=(char*)malloc(sizeof(char)*(FileSize+1));
	memset(buf,'\0',sizeof(TCHAR)*FileSize);
	memset(MulitCharBuf,'\0',FileSize);
	//�������ó��ڴ���\0��Ȼ��Ӱ������Ī��������Ĳ���
	//Resource leak: fp
	fread(MulitCharBuf,FileSize,1L,fp);
	MultiByteToWideChar(CP_ACP,0,MulitCharBuf,-1,buf,sizeof(TCHAR)*FileSize);
	fclose(fp);
	return 0;
}

int read_file_to_buf_string(TCHAR * file_name,BUF_STRING *buf_string)//������ã�ֱ�Ӷ�ȡ��bufstring��ȥ��
{

	FILE *fp=nullptr;
	//DWORD len;
	char FileNameTemp[MAX_FILE_NAMEPATH]={};
	WideCharToMultiByte(CP_OEMCP,NULL,file_name,-1,FileNameTemp,MAX_FILE_NAMEPATH,NULL,FALSE);
	fp=fopen(FileNameTemp,"r");
	if (NULL==fp)
	{
		wcout<<"�ļ���ʧ��"<<endl;
		return -1;
	}
	size_t FileSize=0;
	fseek(fp,0L,SEEK_END);
	FileSize=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	//�����ﲻ���������������
	buf_string->the_char_buf=(TCHAR*)calloc(sizeof(TCHAR),FileSize+1);
	char * MulitCharBuf=(char*)calloc(sizeof(char),FileSize+1);
	// 	memset(buf_string->the_char_buf,'\0',sizeof(TCHAR)*FileSize);
	// 	memset(MulitCharBuf,'\0',FileSize);
	//�������ó��ڴ���\0��Ȼ��Ӱ������Ī��������Ĳ���
	//�м� ������ͽṹ�� ���������ڴ��ÿ�
	//������bug�Ѿ���� ���ʱ��20140727 ԭ���Ƕ�̬������ֽ���̫����  �����ַ�����βʱ����'\0'
	fread(MulitCharBuf,FileSize,1,fp);
	MultiByteToWideChar(CP_ACP,0,MulitCharBuf,-1,buf_string->the_char_buf,/*sizeof(TCHAR)**/FileSize);
	buf_string->buf_size=wcslen(buf_string->the_char_buf);//������������Ļ������е��ַ�����  ԭ����д���˻��������� ����²�Ӧ�õ�Ŷ
	free(MulitCharBuf);
	if (FileSize<1)
	{
		wcout<<L"�ı���ô�̣�����ƨ����"<<endl;
		fclose(fp);
		return -1;
	}
	//��ʼ����������
	buf_string->splid_wnd_char.first_char=buf_string->the_char_buf[0];
	buf_string->splid_wnd_char.last_char=buf_string->the_char_buf[1];
	buf_string->splid_wnd_char.first_char_loc=0;
	fclose(fp);
	//.......
	return 0;
}

int read_iamwall_get(void * buf,FILE * fp,int file_off_set,int file_length)//�����ȡ�ļ�
{/*һ��Ҫȷ��buf�Ĵ�С�ܹ��ŵ��¶�ȡ���ļ�  ������漰�����������������ռ仹�ǵ����߷����������*/
	//fseek(fp,0L,SEEK_SET);//���ļ�ָ���ƶ���ͷ��
	fseek(fp,file_off_set,SEEK_SET);
	int res=fread(buf,file_length,1,fp);
	if (1==res)
	{
		return 1;
	}else
	{
		return -1;//��ȡʧ����
	}
}

int read_iamwall_send(void **p_buf,FILE * fp,int file_off_set,int file_length)//�������������������
{/* ����Ǳ���������Ļ����� ���Ե��ú���һ��Ҫ�ǵ��ͷſռ� ������ڴ�й©*/
	/*ע���˹�  ����һ���ļ����ĸ��ֽ� Ҫ���ȡ���һ���ֽھ�Ҫƫ�Ƶ��ļ����ȼ�ȥ1��λ��Ȼ���ȡһ���ֽ�*/
	void * the_buf=calloc(file_length+1,sizeof(char));
	*p_buf=the_buf;
	if (feof(fp))
	{
		printf("�����ļ����� �����ļ�ƫ���������е�����\n");
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
		return -1;//��ȡʧ����
	}
	/*�ú���ͨ��������*/
}

int write_buf_to_file(void *buf,size_t buf_length,char * file_name,char *write_flg/*д��ʱ���ģʽ*/)
{//������������д���ļ���
	if (!buf)
	{
		printf("������Ϊnull ���黺������\n");
		return -1;
	}
	if (!write_flg)
	{
		printf("�ļ�д��ģʽ���ô���\n");
		return -1;
	}

	FILE * fp=fopen(file_name,write_flg);//�����ļ�
	if (!fp)
	{
		perror("�ļ���ʧ�� ����ļ�����·�����ļ�д���־λ��");
		return -1;
	}

	if (0>=buf_length)
	{
		printf("����������̫С ��ô����0 �أ�\n");
		return -1;
	}


	//������

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
	fflush(fp);//ͬ���ļ�
	fclose(fp);
	fp=NULL;
	return buf_length;
	//����ô����

}

int read_next_index_to_buf(char * next_file_name,THE_NEXT_INDEX **next_buf)//��ȡ�����������ڴ�Ļ�����
{
	if (!next_file_name)
	{
		printf("���������ļ�·����������\n");
		return -1;
	}
	FILE * fp=fopen(next_file_name,"rb");
	if (!fp)
	{
		printf("���������ļ���ʧ�ܣ������ļ���Ȩ��\n");
		return -1;
	}
	size_t file_size=0;
	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);//����ط��鿴�ļ���С
	fseek(fp,0L,SEEK_SET);//���ļ�ָ�����

	THE_NEXT_INDEX *temp_next_index=(THE_NEXT_INDEX*)malloc(sizeof(THE_NEXT_INDEX)+file_size);
	fread(temp_next_index->next_index__,file_size,1,fp);
	fclose(fp);
	temp_next_index->length=file_size;
	*next_buf=temp_next_index;

	/************************************************************************/
	/* һ��Ҫע���ͷ��ڴ氡                                                         */
	/************************************************************************/
	return file_size;
}