#define _CRT_SECURE_NO_WARNINGS
#include"ImgDisplay.h"
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<windows.h>

const int MAX_ID = 3;
const int EXP_PER_LV = 1000000;
const char id_llist[MAX_ID][65] = { "MOI_Main_Registration",
								   "MOI_Adventure_Record",
								   "MOI_Score_Editor"
								  };
const char* EXP_DATA = "MOI_ExpData.ini";
const char* EXP_REC = "MOI_ExpRec.txt";
const char* ADVENTURE_EXE = "MOI_AdventureMode.exe";
const char* CONFIG_DATA = "MOI_Config.ini";
const char* INDEX_ADVENTURE = "MOI_Index_Adventure.lst";

long long registry(char* score_str, char* id);
void adventure();
void reader(char* item);
void record();
int select_a();

inline void moi_a()
{
	printf("Melody of Involution\n"
		   "Adventure Mode\n"
		   "Version Release 1.2\n");
}

int main(int argc, char* argv[])
{
	char color_set[9] = "color 0f";
	FILE *fconfig = NULL;
	if (argc == 3)	return registry(argv[1], argv[2]);
	SetConsoleTitleA("Melody Of Involution - Adventure Mode [R1.2]");
	fconfig = fopen(CONFIG_DATA, "r");
	if (fconfig != NULL)
	{
		fgetc(fconfig);	 //跳过第1位 背景色-Launcher
		fgetc(fconfig);	 //跳过第2位 前景色-Launcher
		fgetc(fconfig);	 //跳过第3位 背景色-Main
		fgetc(fconfig);	 //跳过第4位 前景色-Main
		color_set[6] = fgetc(fconfig);	 //第5位 背景色-Adventure
		color_set[7] = fgetc(fconfig);	 //第6位 前景色-Adventure
		fclose(fconfig);
	}
	system(color_set);
	while (1)
	{
		system("cls");
		switch (select_a())
		{
		case 1:
			adventure();
			break;
		case 2:
			record();
			break;
		case 3:return 0;
		}
	}
	return 0;
}
int select_a()
{
	bool loop;
	char in_char;
	char c_front[3] = { '>', ' ', ' ' }, c_back[3] = { '<', ' ', ' ' }, draw[21] = "                    ";	 //draw:20字符;
	int location = 0, n = 0;  //location为数组序号以0起始 return为其+1以1起始
	long long exp_ll;
	FILE *fexp = NULL;
	while (1)
	{
		loop = 1;
		system("cls");
		moi_a();
		printf("\n[等级信息]\n");
		fexp = fopen(EXP_DATA, "r");
		if (fexp == NULL)
		{
			printf("未找到经验数据 进入\"经验记录\"以创建\n"
				   "按任意键进入..."
				  );
			_getch();
			fflush(stdin);
			return 2;
		}
		else
		{
			int ndraw;
			fscanf(fexp, "%lld", &exp_ll);
			printf("Lv. %lld | Exp. %lld/%d\n"
				   "经验值进度|"
				  , exp_ll / EXP_PER_LV, exp_ll%EXP_PER_LV, EXP_PER_LV);
			ndraw = exp_ll%EXP_PER_LV / double(EXP_PER_LV) * 20;
			for (int idraw = 0; idraw < ndraw; idraw++)
			{
				draw[idraw] = '=';
			}
			printf("%s|\n", draw);
			fclose(fexp);
		}
		printf("\n"
			"使用W/S键上下移动选择光标\"> <\"\n"
			"然后使用空格键选择\n"
			"或键入项目前的序号以执行对应操作\n"
			"\n"
			" %c [1]冒险故事 %c\n"
			" %c [2]经验记录 %c\n"
			" %c [0]退出程序 %c\n"
			, c_front[0], c_back[0], c_front[1], c_back[1], c_front[2], c_back[2]);
		do
		{
			in_char = _getch();
			fflush(stdin);
		} while (in_char != ' '&&in_char != 'w'&&in_char != 'W'&&in_char != 's'&&in_char != 'S'&&in_char != '1'&&in_char != '2'&&in_char != '0');
		while (loop)
		{
			switch (in_char)
			{
			case 's':
				loop = 0;
				for (n = 0; c_front[n] != '>'; n++){}
				c_front[n] = ' ';
				c_back[n] = ' ';
				n = n >= 2 ? 0 : n + 1;	//循环滚动
				c_front[n] = '>';
				c_back[n] = '<';
				for (location = 0; c_front[location] != '>'; location++){}	//此处起实际作用 改变location值
				break;
			case 'S':
				in_char = 's';
				continue;
			case 'w':
				loop = 0;
				for (n = 0; c_front[n] != '>'; n++)
				{
				}
				c_front[n] = ' ';
				c_back[n] = ' ';
				n = n <= 0 ? 2 : n - 1;
				c_front[n] = '>';
				c_back[n] = '<';
				for (location = 0; c_front[location] != '>'; location++){}
				break;
			case 'W':
				in_char = 'w';
				continue;
			case ' ':return location + 1;
			case '1':return 1;
			case '2':return 2;
			case '0':return 3;
			}
		}

	}
}
long long registry(char* score_str, char* reg_id)  //MOI_Adventure [int64:score] [string:reg_id]	 (return int64:score)
{
	char exp_str[20] = {};
	int score_ll, exp_ll;
	FILE *fexp = NULL;
	score_ll = atoll(score_str);
	for (int i = 0; strcmp(reg_id, id_llist[i]); i++)
	{
		if (i == MAX_ID)	return -1;	//reg_id数据不合法
	}
	fexp = fopen(EXP_DATA, "r");
	if (fexp == NULL)	return -2;	//文件打开失败
	fgets(exp_str, 21, fexp);  //若要使数据难以私自修改 则加密并在此处之后解密
	exp_ll = atoll(exp_str);  
	fclose(fexp);  //关闭文件以准备切换打开方式
	exp_ll = exp_ll < 0 ? 0 : exp_ll;
	exp_ll = exp_ll + score_ll;
	fexp = fopen(EXP_DATA, "w+");
	if (fexp == NULL)	return -2;	//文件写入失败
	fprintf(fexp, "%s", _ltoa(exp_ll, exp_str, 10));
	fclose(fexp);
	return score_ll;
}
void adventure()
{
	bool found = 0;
	char item[129] = {};
	FILE *fp = NULL;
	while (1)
	{
		memset(item,0,sizeof(item));
		system("cls");
		moi_a();
		printf("冒险模式:故事\n"
			"拖入M.O.I.剧情文件 或键入剧情路径\n"
			"然后按Enter以进入\n"
			"(键入*列出剧情序号索引 | 键入*exit返回)\n"
			);
		scanf("%[^\n]", item);
		if (item[0] == '*')	 //输入类型为序号
		{
			if (!strcmp(item, "*exit"))	return;
			fp = fopen(INDEX_ADVENTURE, "r");
			if (fp == NULL)
			{
				printf("未找到索引文件:%s!\n"
					   "序号与索引不可用\n"
					   "按任意键返回..."
					  , INDEX_ADVENTURE);
				_getch();
				fflush(stdin);
				continue;
			}
			if (!strcmp(item, "*"))  //列出索引
			{
				while (!feof(fp))
				{
					fgets(item, 129, fp);
					fputs(item, stdout);
					memset(item, 0, sizeof(item));
				}
				printf("\n按任意键返回...");
				fclose(fp);
				_getch();
				fflush(stdin);
				continue;
			}
			while (!feof(fp))  //查找索引
			{
				memset(item, 0, sizeof(item));  	 
				fscanf(fp, "%[^\n]", item);		  //索引第1行为序号 第二行为路径
				if (!strcmp(item, item))			  //识别每一行 直至发现要找的序号
				{
					found = 1;
					break;
				}
			}
			if (!found)
			{
				printf("无效序号\n"
					   "按任意键返回..."
					  );
				fclose(fp);
				_getch();
				fflush(stdin);
				continue;
			}
			else
			{
				fscanf(fp, "%[^\n]", item);			  //再读一行就是对应路径
				fclose(fp);
			}
		}  //序号识别结束
		reader(item);  //传入的是路径
	}
}
void reader(char* item)
{
	char fin_char;
	char fin_str[129] = {};
	int exp_ll = 0, height = 0, fin_int = 0, len = strlen(item);
	FILE *fp = NULL, *fexp=NULL;
	system("cls");
	moi_a();
	printf("冒险模式:故事\n");
	fexp = fopen(EXP_DATA, "r");
	if (fexp == NULL)
	{
		printf("未找到经验数据 进入\"经验记录\"以创建\n"
			   "按任意键返回..."
			   );
		_getch();
		fflush(stdin);
		return;
	}
	if (item[0] == '\"'&&item[len - 1] == '\"')  //处理拖入的带空格路径 而不新增字符串
	{
		for (int imov = 1; imov < len; imov++)	item[imov - 1] = item[imov];  //前移1位以覆盖开头的引号
		item[len - 2] = 0;	//删去被前移的末尾引号
	}
	fp = fopen(item, "r");
	if (fp == NULL)
	{
		printf("错误:未找到剧情:%s!\n"
			   "按任意键返回..."
			  , item);
		_getch();
		fflush(stdin);
		return;
	}
	fscanf(fp, "%[^\n]", fin_str);
	printf("标题:%s", fin_str);
	fscanf(fexp, "%lld", &exp_ll);
	fscanf(fp, "%d", &fin_int);
	if (fin_int > exp_ll / EXP_PER_LV)
	{
		printf("等级不足以查看该剧情!\n"
			"当前%d | 需要%d\n"
			"请多完成关卡以积累经验\n"
			"按任意键返回..."
			, exp_ll/EXP_PER_LV,fin_int);
		_getch();
		fflush(stdin);
		return;
	}
	while (!feof(fp))
	{
		memset(fin_str, 0, sizeof(fin_str));
		fin_char = fgetc(fp);
		switch (fin_char)
		{
		case '\n':	//对话以一行为一个单位 遇到\n即准备进入下一个单位
			printf("\n\n--------------------------------------------------\n\n");
			fflush(stdin);	//防止按多了文字不断出来
			_getch();
			continue;
		case '\\':	//功能以\开头
			fin_char = fgetc(fp);  //判断具体功能
			switch (fin_char)
			{
			case '!':  //\!+1位数+下一行图片路径用于+输出图片
				height = fgetc(fp) - '0';
				fscanf(fp, "%s", fin_str);
				ImgDisplay_Wipe(fin_str, "#DEFAULT");
				for (int iprint = 0; iprint <= height; iprint++)	putchar('\n');
				continue;
			case ':':  //\:用于分隔讲述者与讲述内容
				printf(":\n");
				continue;
			case '|':  //\|用于换行
				printf("\n");
				continue;
			default:  //键入\目的 不是需要功能 而是输出\字符
				printf("\\");
				continue;
			}
		default:  //输出
			putchar(fin_char);
			continue;
		}
		Sleep(10);	//让文字一个一个显示
	}
	printf("\n--------------------------------------------------\n"
		   "剧情结束 按任意键返回..."
		  );
	_getch();
	return;
}
void record()
{
	char in_char;
	char in_str[16] = {}, run[98] = {};
	long long exp_ll, r;
	FILE *fexp = NULL;
	system("cls");
	moi_a();
	fexp = fopen(EXP_DATA, "r");
	if (fexp == NULL)
	{
		printf("未找到经验数据文件!\n"
			   "是否创建文件?[Y/N]"
			   );
		do
		{
			in_char = _getch();
			fflush(stdin);
		} while (in_char != 'Y'&&in_char != 'y'&&in_char != 'N'&&in_char != 'n');
		if (in_char == 'Y' || in_char == 'y')
		{
			fexp = fopen(EXP_DATA, "a");
			if (fexp == NULL)
			{
				printf("文件创建失败!\n"
					   "按任意键返回..."
					  );
				_getch();
				fflush(stdin);
				return;
			}
		}
		if (in_char == 'N' || in_char == 'n')	return;
		system("cls");
		moi_a();
	}
	fscanf(fexp,"%[^\n]", in_str);
	exp_ll = atoll(in_str);
	printf("MOI_Adventure [int64:score] [string:reg_id]\n"
		   "当前经验:%lld\n"
		   "参数<1/2> 键入要记录的成绩值\n"
		   "注意:经验值上限为2^63-1\n"
		  ,exp_ll);
	scanf("%s", in_str);
	fflush(stdin);
	printf("参数<2/2> 将使用%s作为记录ID\n"
		   "按任意键确认..."
		   ,id_llist[1]);
	_getch();
	fclose(fexp);
	r = registry(in_str, (char*)id_llist[1]);
	switch (r)
	{
	case -1:
		printf("\n错误:非法输入(%lld)\n", r);
		break;
	case -2:
		printf("\n错误:文件读写失败(%lld)\n", r);
	default:
		printf("\n执行成功(%lld)\n", r);
		break;
	}
	printf("按任意键返回...");
	_getch();
	fflush(stdin);
}