#include "index_make_operator.h"
#include "index_file_operator.h"
#include "index_elem_operator.h"
#include "index_make_file_table_operator.h"
#include "index_usr_interface.h"
MemoryIndexTree * make_memory_index_tree(TCHAR * file_name,char * over_come_file_name,unsigned int file_number)//建立一级内存索引树
{//文件号呢？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	//文件好已经解决20140729  这么快就第二天了啊
	//建立buf_string
	BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
	/************************************************************************/
	/* 需要在这里增加读取文本号的模块                                          */
	/************************************************************************/
	read_file_to_buf_string(file_name,buf_string);
	//建立完成

	//申请索引树
	MemoryIndexTree * temp_memory_index_tree=new MemoryIndexTree;//既然使用了stl  就用点c++吧
	//申请所索引树完成

	//开始建立单字符一级索引
	MemoryIndexTree::iterator itr_to_find_first_char;
	MemoryIndexTree::iterator itr_to_find_next_char;
	//20140729 0:50开始优化
	//其实这里还有问题没有解决
	/****
	单字符对应的索引域是全部文本的这个单字符的索引域 所以长度可能会超过1023呀
	最极端的情况就是有1023个文档 文档中全都是一个字符的重复出现
	超级大bug  等待解决
	难道要扩大next_char_loc 的长度？
	***/
	OVER_COME_SET * temp_over_set=make_over_come_set(over_come_file_name);
	do 
	{
#if 0//暂时先不用字符排除集了  太复杂了

		/************************************************************************/
		/* 考虑字符排除集的使用20140731                                           */
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
			{//当前字符和下一个字符相等 但是这个字符还没有出现过的情况
				//生成索引域
				MemoryIndexArear * memory_index_arear=new MemoryIndexArear;
				Index__ temp_index__ ;
				init_index_elem(&temp_index__);
				//在这里就暂时不做出错判断了 相信自己的程序也许是一件错误的事情
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,1);
				set_filenumber_index__(&temp_index__,file_number);
				memory_index_arear->push_back(temp_index__);
				temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,memory_index_arear));
			}

			if (itr_to_find_first_char!=temp_memory_index_tree->end())
			{//当前字符和下一个字符相等 但是已经出现过了

				Index__ temp_index__;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size()+1);
				set_filenumber_index__(&temp_index__,file_number);
				//上面的那句话的额意思是 当前字符已经出现过了 如果直接将当前字符加入索引项会引起查找时候的死循环
				//所以先预留出来地方
				itr_to_find_first_char->second->push_back(temp_index__);
			}
			continue;
		}

		if (itr_to_find_first_char==temp_memory_index_tree->end())
		{//字符表中没有当前字符，就要将当前字符添加到字符表中
			if (itr_to_find_next_char==temp_memory_index_tree->end())
			{//当前字符表中也没有下一个字符
				//生成索引域 索引项 添加进去
				MemoryIndexArear *index_memory_arear=new MemoryIndexArear;
				Index__ temp_index__ ;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,0);
				set_filenumber_index__(&temp_index__,file_number);
				//res是为了测试而生的
				int res=get_currcharloc_index__(&temp_index__);
				index_memory_arear->push_back(temp_index__);
				temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,index_memory_arear));
				continue;
			}

			if (itr_to_find_next_char!=temp_memory_index_tree->end())
			{//说明当前字符不再字符表中 但是下一个字符在字符表中
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
		{//说明当前字符在字符表中
			if (itr_to_find_next_char==temp_memory_index_tree->end())
			{//下一个字符不再字符表中
				Index__ temp_index__;
				init_index_elem(&temp_index__);
				set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
				set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
				set_nextcharloc_index__(&temp_index__,0);//下一个字符没有出现过 所以索引位置肯定是0
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
	} while (get_next_splid_wnd(buf_string)!=-1);//就是滑动窗口没结束就要一直建立索引

	free(buf_string->the_char_buf);
	free(buf_string);//这个应该释放吗?
	return temp_memory_index_tree;
}

MemoryIndexTree * make_memory_index_tree_all_file(TCHAR * file_path,char * over_come_file_name)//这个是最终本版的创建某个路径下的所有的文本文件的索引树的程序模块
{
	//首先 创建这个路径下的文件映射表
	FILE_TABLE * file_table=create_file_table(file_path);
	//尝试将文件映射表写入文件
	TCHAR file_table_name[MAX_PATH]={};
	wcscpy(file_table_name,file_path);
	wcscat(file_table_name,FILE_TABLE_DB_NAME);
	write_file_table_to_file(file_table_name,file_table);
	read_file_table_to_buf(file_table_name);
	//文件映射表创建完成
	//分配一个内存索引树
	//申请索引树
	MemoryIndexTree * temp_memory_index_tree=new MemoryIndexTree;//既然使用了stl  就用点c++吧
	MemoryIndexTree::iterator itr_to_find_first_char;
	MemoryIndexTree::iterator itr_to_find_next_char;
	//申请所索引树完成
	size_t file_table_size_count=0;
	
	while (file_table_size_count<(file_table->file_table_size))
	{
		//读取对应的文本到缓冲区
		BUF_STRING *buf_string=(BUF_STRING*)malloc(sizeof(BUF_STRING));
		/************************************************************************/
		/* 需要在这里增加读取文本号的模块                                          */
		/************************************************************************/
		if (-1==read_file_to_buf_string((WCHAR *)(file_table->the_file_table[file_table_size_count].c_str()),buf_string))
		{
			printf("最终版本的文件索引树创建失败 失败原因是文件读取失败吧\n");
			return NULL;
		}
		
		do 
		{
			itr_to_find_first_char=temp_memory_index_tree->find(buf_string->splid_wnd_char.first_char);
			itr_to_find_next_char=temp_memory_index_tree->find(buf_string->splid_wnd_char.last_char);

			if (buf_string->splid_wnd_char.first_char==buf_string->splid_wnd_char.last_char)
			{
				if (itr_to_find_first_char==temp_memory_index_tree->end())
				{//当前字符和下一个字符相等 但是这个字符还没有出现过的情况
					//生成索引域
					MemoryIndexArear * memory_index_arear=new MemoryIndexArear;
					Index__ temp_index__ ;
					init_index_elem(&temp_index__);
					//在这里就暂时不做出错判断了 相信自己的程序也许是一件错误的事情
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,1);
					set_filenumber_index__(&temp_index__,file_table_size_count);
					memory_index_arear->push_back(temp_index__);
					temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,memory_index_arear));
				}

				if (itr_to_find_first_char!=temp_memory_index_tree->end())
				{//当前字符和下一个字符相等 但是已经出现过了

					Index__ temp_index__;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,itr_to_find_next_char->second->size()+1);
					set_filenumber_index__(&temp_index__,file_table_size_count);
					//上面的那句话的额意思是 当前字符已经出现过了 如果直接将当前字符加入索引项会引起查找时候的死循环
					//所以先预留出来地方
					itr_to_find_first_char->second->push_back(temp_index__);
				}
				continue;
			}

			if (itr_to_find_first_char==temp_memory_index_tree->end())
			{//字符表中没有当前字符，就要将当前字符添加到字符表中
				if (itr_to_find_next_char==temp_memory_index_tree->end())
				{//当前字符表中也没有下一个字符
					//生成索引域 索引项 添加进去
					MemoryIndexArear *index_memory_arear=new MemoryIndexArear;
					Index__ temp_index__ ;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,0);
					set_filenumber_index__(&temp_index__,file_table_size_count);
					//res是为了测试而生的
					int res=get_currcharloc_index__(&temp_index__);
					index_memory_arear->push_back(temp_index__);
					temp_memory_index_tree->insert(make_pair(buf_string->splid_wnd_char.first_char,index_memory_arear));
					continue;
				}

				if (itr_to_find_next_char!=temp_memory_index_tree->end())
				{//说明当前字符不再字符表中 但是下一个字符在字符表中
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
			{//说明当前字符在字符表中
				if (itr_to_find_next_char==temp_memory_index_tree->end())
				{//下一个字符不再字符表中
					Index__ temp_index__;
					init_index_elem(&temp_index__);
					set_currcharloc_index__(&temp_index__,buf_string->splid_wnd_char.first_char_loc);
					set_nextchar_index__(&temp_index__,buf_string->splid_wnd_char.last_char);
					set_nextcharloc_index__(&temp_index__,0);//下一个字符没有出现过 所以索引位置肯定是0
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
		} while (get_next_splid_wnd(buf_string)!=-1);//就是滑动窗口没结束就要一直建立索引

		free(buf_string->the_char_buf);
		free(buf_string);//这个应该释放吗?
		++file_table_size_count;
	}
	free(file_table);//这个也该释放了吧
	return temp_memory_index_tree;
}

//建立索引项的内存线性缓冲区
MEMORY_INDEX_ARRAY_BUF * get_memory_index_array(MemoryIndexTree *index_tree)//建立索引项的内存线性缓冲区
{
	if (!index_tree)
	{
		printf("内存索引树为NULL\n");
		return NULL;
	}

	size_t index_size=0;
	//index_size=index_tree->size();//这个地方有错误 统计了字符出现的个数 但是没有统计全部索引项的个数
	//统计全部索引项的个数
	MemoryIndexTree::iterator itr_start=index_tree->begin();
	MemoryIndexTree::iterator itr_end=index_tree->end();
	while (itr_start!=itr_end)
	{
		// 		MemoryIndexArear::iterator itr_arear_start=itr_start->second->begin();
		// 		MemoryIndexArear::iterator itr_arear_end=itr_start->second->end();
		index_size+=itr_start->second->size();
		++itr_start;
	}
	//。。。。
	size_t count_index__=0;
	MEMORY_INDEX_ARRAY_BUF * temp_index_memory_buf=(MEMORY_INDEX_ARRAY_BUF*)malloc(sizeof(MEMORY_INDEX_ARRAY_BUF)+sizeof(Index__)*index_size);
	temp_index_memory_buf->length=index_size;
	//顺序遍历map 获取所有的索引项
	itr_start=index_tree->begin();
	itr_end=index_tree->end();

	while (itr_start!=itr_end)
	{
		MemoryIndexArear::iterator itr_arear_start=itr_start->second->begin();
		MemoryIndexArear::iterator itr_arear_end=itr_start->second->end();

		while (itr_arear_start!=itr_arear_end)
		{
			*((temp_index_memory_buf->index_head)+(count_index__++))=*(itr_arear_start++);//这句写的太他妈的专业了 玩指针嘛！最重要的就是开心
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
	//外部不要忘记释放内存
}

int write_index_to_file(MemoryIndexTree * index_tree,char * file_name,char *flg)//将一级索引文件写入文件
{//将一级索引写入文件 不要忘记释放内存
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
		printf("写入失败 请检查\n");
		return -1;
	}
	//printf("写入成功！\n");成功的情况在外层应用判断
	free(buf);
	return res;
}

OVER_COME_SET * make_over_come_set(char * over_come_file_name)//通过文件创建排除集
{
	FILE * fp=fopen(over_come_file_name,"r");
	if (!fp)
	{
		printf("排除集文件读取失败！\n");
		return NULL;
	}

	OVER_COME_SET * temp_over_set=new OVER_COME_SET;
	if (!temp_over_set)
	{
		printf("排除集内存分配失败\n");
		return NULL;
	}
	//出错检查

	size_t file_size=0;
	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	char * char_buf=(char*)calloc(1,(sizeof(char)*file_size)+sizeof(char));
	int res=fread(char_buf,file_size*sizeof(char),1,fp);
	if (1!=res)
	{
		printf("排除集文件读取失败\n");
		perror("");
		return NULL;
	}
	//转换成宽字节的
	TCHAR * tchar_buf=(TCHAR *)calloc(1,(sizeof(TCHAR)*file_size)+sizeof(TCHAR));
	MultiByteToWideChar(CP_ACP,0,char_buf,-1,tchar_buf,/*sizeof(TCHAR)**/file_size);
	//这里是不是需要调试一下啊
	fclose(fp);//不要忘记关闭文件 我是遵守几率的同学
	// 下面开始建立set
	size_t count=0;
	while (count<file_size)
	{
		temp_over_set->insert(tchar_buf[count++]);
	}
	printf("set建立完成\n");

	return temp_over_set;
}
//注意内存管理  释放内存啊

int judge_over_come_char(TCHAR the_char,OVER_COME_SET * over_come_set)//单词排除集  注意要释放排除集中的内存
{
	if (!over_come_set)
	{
		printf("字符排除集错误！\n");
		return 0;
	}

	if (over_come_set->count(the_char))
	{
		return 1;
	}
	return 0;
}

THE_NEXT_INDEX * make_next_index__(MemoryIndexTree * index_tree)//生成二级索引
{
	if (!index_tree)
	{
		printf("一级内存索引树为空\n");
		return NULL;
	}
	size_t index_size=0;
	//index_size=index_tree->size();//这个地方有错误 统计了字符出现的个数 但是没有统计全部索引项的个数
	//统计全部索引项的个数
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
	//分配适当的内存哦
	index_size=index_tree->size();
	THE_NEXT_INDEX * temp_next_index=(THE_NEXT_INDEX*)malloc(sizeof(THE_NEXT_INDEX)+sizeof(NEXT_INDEX)*index_size);
	temp_next_index->length=index_size*sizeof(NEXT_INDEX);//这个是字节数

	itr_start=index_tree->begin();
	itr_end=index_tree->end();
	size_t file_offset=0;
	index_size=0;//将index_size置0 当作下标计数器
	//再次遍历所有的一级内存索引域，获取索引域长度（单个字符对应的索引长度）和当前总长度（一级索引文件的偏移量），然后搞定啊？
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
	/////////一定要注意释放内存  记得代码review
}

MAP_QUARY_NEXT_INDEX_AREAR * make_query_index(char * next_file_file_name)//建立用于查询的内存二级索引
{

	//先将二级索引文件读取到内存 然后释放它
	THE_NEXT_INDEX * the_next_index=NULL;
	int res=read_next_index_to_buf(next_file_file_name,&the_next_index);
	if (res<=0)
	{
		printf("二级索引读取失败\n");
		return NULL;
	}
	if (!the_next_index)
	{
		printf("二级索引建立失败\n");
		return NULL;
	}
	//开始建立内存检索二级索引

	MAP_QUARY_NEXT_INDEX_AREAR *temp_map_index=new MAP_QUARY_NEXT_INDEX_AREAR;
	size_t count=0;
	size_t big_count=the_next_index->length/sizeof(NEXT_INDEX);
	while (count<big_count)
	{/*这个地方有bug*/
		QUERY_NEXT_INDEX temp_quary_index;
		temp_quary_index.index_file_length=the_next_index->next_index__[count].index_file_length;
		temp_quary_index.index_file_offset=the_next_index->next_index__[count].index_file_offset;
		temp_quary_index.in_memory_flg=0;
		temp_quary_index.index_arear_array_length=0;
		temp_quary_index.index_arear_array=NULL;//加上这个初始化 要不然释放时候容易出错
		temp_map_index->insert(make_pair(the_next_index->next_index__[count].curr_char,temp_quary_index));
		//	wcout<<the_next_index->next_index__[count].curr_char<<endl;//貌似是读取到的二级索引有些问题
		++count;
	}
	free(the_next_index);
	return temp_map_index;
}

int read_index_to_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index,FILE * index_fp)//读取一级索引到二级索引
{
	if (!map_next_index)
	{
		printf("二级索引树为空失败\n");
		return 0;
	}
	if (!index_fp)
	{
		printf("一级索引的文件指针为空失败\n");
		return 0;
	}

	//首先通过字符查找二级索引的节点
	MAP_QUARY_NEXT_INDEX_AREAR::iterator next_index_itr;
	QUERY_NEXT_INDEX * quary_next_index;
	next_index_itr=map_next_index->find(the_char);
	if (next_index_itr==map_next_index->end())
	{
		printf("字符的二级索引查找失败\n");
		return 0;
	}
	quary_next_index=&(next_index_itr->second);//找到字符的二级索引 获取字符的一级索引文件的信息
	quary_next_index->index_arear_array_length=quary_next_index->index_file_length/sizeof(Index__);//存储索引域中索引项的个数
	if (1==quary_next_index->in_memory_flg)
	{
		//如果原来加载过  就不用加载了
		return 1;
	}
	//quary_next_index->index_arear_array=(Index__*)malloc(quary_next_index->index_file_length);
	int res=read_iamwall_send((void**)&(quary_next_index->index_arear_array),
		index_fp,quary_next_index->index_file_offset,
		quary_next_index->index_file_length);

	if (res<=0)
	{
		printf("一级索引加载失败\n");
		return 0;
	}//千万注意释放内存
	quary_next_index->in_memory_flg=1;
	//注意不要关闭文件
	return 1;
}

int free_index_in_next_index(TCHAR the_char,MAP_QUARY_NEXT_INDEX_AREAR * map_next_index)//释放某一个字符的二级索引上挂载的缓冲区
{//释放成功返回1   否则返回0  这个算法是提供给LRU内存管理用的
	//通过测试20140801
	if (!map_next_index)
	{
		printf("二级索引树为空失败\n");
		return 0;
	}
	//首先通过字符查找二级索引的节点
	MAP_QUARY_NEXT_INDEX_AREAR::iterator next_index_itr;
	QUERY_NEXT_INDEX * quary_next_index=NULL;
	next_index_itr=map_next_index->find(the_char);
	if (next_index_itr==map_next_index->end())
	{
		printf("字符的二级索引查找失败\n");
		return 0;
	}
	quary_next_index=&(next_index_itr->second);//找到字符的二级索引 获取字符的一级索引文件的信息
	if (!quary_next_index->index_arear_array)
	{
		printf("二级索引中的加载的一级索引是空的");
		return 1;
	}
	free(quary_next_index->index_arear_array);//这个地方可能出现big  因为可能会这个里面的一级索引域的指针是野指针 索引要先判断一下 另外要加上初始化二级索引节点
	quary_next_index->index_arear_array=NULL;
	quary_next_index->in_memory_flg=0;
	quary_next_index->index_arear_array_length=0;
	return 1;
}
