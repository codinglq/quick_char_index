#include "index_quary_operator.h"
#include "index_memory_ctrl.h"

TCHAR get_next_char_in_index(MAP_QUARY_NEXT_INDEX_AREAR::iterator itr,size_t next_char_loc)//在二级索引的以及索引中获取下一个字符
{//这个地方真的好难啊
	return get_nextchar_index__(&(itr->second.index_arear_array[next_char_loc]));
}

FILE_RES_FILENUMBER_LIST * query_iamwall(TCHAR * source_str,
	MAP_QUARY_NEXT_INDEX_AREAR * map_quary,
	FILE * index_fp,FILE_RES_FILENUMBER_LIST  *the_list_file_number/*原来这个是传进来的在外面申请的变量 这样做的原因是可以加快程序速度*/)//不必多说 这个就是最最牛逼的检索算法
{//这个算法应该提供多个版本的  因为这个可以优化的范伟实在是太大了
	/*var 0.1  这个版本提供的是静态检索串*/

	if (!the_list_file_number)
	{
		printf("加速文件号表为空\n");
		return the_list_file_number;
	}
	//清空传进来的加速文件号表变量
	the_list_file_number->clear();
	if (!source_str)
	{
		printf("检索传为空");
		return the_list_file_number;
	}
	if (!map_quary)
	{
		printf("二级索引为空 检索失败\n");
		return the_list_file_number;
	}
	if (!index_fp)
	{
		printf("一级索引指针为空 检索失败");
		return the_list_file_number;
	}
	//加载对应的字符索引到二级索引
	size_t source_str_length=wcslen(source_str);
	if (source_str_length> MAX_QUERY_STRING_LENGTH)
	{
		printf("输入的检索传太长了\n");
		return the_list_file_number;
	}
	size_t count_load_index=0;
	/************************************************************************/
	/* 测试内存控制                                                                  */
	/************************************************************************/
	MEMORY_INDEX_QUEUE * temp_mem=get_memory_index_queue();
	init_index_memory_dom(temp_mem);//这个是测试内存管理的

	while (count_load_index<source_str_length)
	{//循环加载二级索引树对应的一级索引
		if(!read_index_to_next_index(source_str[count_load_index],map_quary,index_fp))
		{
			printf("字符串查找失败 没有这个东西\n");
			return the_list_file_number;
		}//貌似内存控制这里也通过测试了  还没有优化过 会没有效率的
		index_memory_ctrl(source_str[count_load_index],temp_mem,map_quary);//这个是测试内存管理的
		++count_load_index;
	}
	size_t count_get_next_char=source_str_length;//读取nextchar的次数
	size_t count=0;//控制读取次数

	int success_flg=0;//是否检索到这个字符串的标志位 因为要判断是否成功索引  就是获得的索引数多余一个就行

	size_t count_index_size=0;
	MAP_QUARY_NEXT_INDEX_AREAR::iterator itr=map_quary->find(source_str[0]);//;查找开始字符的二级索引
	if (itr==map_quary->end())
	{
		return the_list_file_number;//没有在索引中找到这个字符  检索失败
	}
	size_t count_index=itr->second.index_arear_array_length;//该字符的索引域中的索引项的长度
	TCHAR temp_next_char[MAX_QUERY_STRING_LENGTH]={};//读取到的nextchar存储到这里

	size_t last_file_number=0XFFFFFFFF;
	while (count_index_size<count_index)
	{
		//	size_t temp_count=count_index_size;
		//	TCHAR temp_char=L'\0';
		//	size_t temp_count_index_size=count_index_size;//获取第一个字符的索引的下标
		size_t next_char_loc=count_index_size;
		while (count<count_get_next_char)
		{//依次获取下一个字符 然后和索引串比较 若相等 则ok
			if (!count)
			{
				temp_next_char[0]=source_str[0];
				++count;
				continue;
			}
			//在这里还要判断当前字符的索引是否已经加载到内存了 如果没有加载则加载一下来一下
			//这个地方可以优化掉好多语句
			if (source_str[count]!=get_nextchar_index__(&(itr->second.index_arear_array[next_char_loc])))
			{//这样多省事啊
				break;
			}
			if (!itr->second.in_memory_flg)
			{
				printf("有个字符串君的索引没有调入内存啊 我来加载一下\n");
				read_index_to_next_index(itr->first,map_quary,index_fp);
			}
			temp_next_char[count]=get_next_char_in_index(itr,next_char_loc);//在这处的bug
			next_char_loc=get_nextcharloc_index__(&(itr->second.index_arear_array[next_char_loc]));
			//这个地方有bug
			itr=map_quary->find(temp_next_char[count]);
			if (temp_next_char[count]==L'\0')
			{//检索到文件结束了 没有找到
				return the_list_file_number;
			}
			//itr=map_quary->find(temp_next_char[count]);
			if (itr==map_quary->end())
			{//这说明检索不到下一个字符了
				break;
			}
			++count;
		}
		//wcscpy(temp_next_char,L"");
		itr=map_quary->find(source_str[0]);
		if (!wcscmp(temp_next_char,source_str))
		{
			++success_flg;
#if 0
			printf("检索传在文本%u中查找成功！\n",get_filenumber_index__(&(itr->second.index_arear_array[count_index_size])));
#endif
			if (last_file_number!=get_filenumber_index__(&(itr->second.index_arear_array[count_index_size])))
			{
				the_list_file_number->push_back(get_filenumber_index__(&(itr->second.index_arear_array[count_index_size])));
			}//如果上次的查询结果和这次的不一样 不是一个文本 则标记它
			
			last_file_number=get_filenumber_index__(&(itr->second.index_arear_array[count_index_size]));
		}
		memset(temp_next_char,L'\0',sizeof(temp_next_char));
		++count_index_size;
		count=0;
	}//明天继续测试
	printf("共查询到%d个位置\n",success_flg);
	return the_list_file_number;
}

