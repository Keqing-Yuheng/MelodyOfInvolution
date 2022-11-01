#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<windows.h>

const int internal_upright = 0;
const char* MOI_TITLE = "Melody Of Involution [R1.1]";
const char* ADVENTUREMODE_REGID = "MOI_Main_Registration";
const char* ADVENTURE_EXE = "MOI_AdventureMode.exe";
const char* IMGDISPLAY = "MOI_ImgDisplay.exe";
const char* UPRIGHT_EXE = "MOI_SpeedUpright.exe";
const char* UPRIGHT_DATA = "MOI_SpeedUpright_Data.ini";
const char* CONFIG_DATA = "MOI_Config.ini";
const char* INDEX_PLAY = "MOI_INDEX_Play.lst";

bool cheat_disabled = 1, show_audio = 1, show_image = 1;
char player[257];
char space[20][5];
int consuming = 40;

void play(char item[], int audio_config);
void complete(char item[], char lrcfile[], int len_int, int note_int, int target_speed, int t_start, int t_end, int clear, int lost, int click, bool constant_no_debugger, int pause, bool upright_failed);

inline void moi_full()
{
	printf("Melody Of Involution\n"
		   "Version Release 1.1\n"
		   "GitHub @ Keqing-Yuheng\n"
		  );
}
inline void moi_brief()
{
	printf("Melody Of Involution\n"
		   "Version Release 1.1\n"
		  );
}

int main(int argc, char *argv[])
{
	bool environment_error = 0;
	char in_char;
	char item[129] = {}, color_set[9]="color 0f";
	int in_int = 0, len, score = 0, audio_config = 1;  //audio_config:0=mute 1=mp3 2=flac
	FILE *fupright = NULL, *fconfig = NULL, *fitem = NULL;
	SetConsoleTitleA(MOI_TITLE);
	if (argc >= 2)
	{
		if ((!strcmp(argv[1], "-mute")) || (!strcmp(argv[1], "-silent")))	audio_config = 0;
		if (argc >= 3 && (!strcmp(argv[2], "-cheat_enabled")))	cheat_disabled = 0;
		moi_brief();
		printf("*Advanced Config Enabled:\n");
		printf("Audio Config:[%d]", audio_config);
		switch (audio_config)
		{
		case 0:
			printf("Mute/Silent\n");
			break;
		case 1:
			printf("ON\n");
			break;
		}
		if (!cheat_disabled)	printf("Debugger Check Value Ignored\n");
		Sleep(1000);
	}
	//配置读取
	//Config
	fconfig = fopen(CONFIG_DATA, "r");
	if (fconfig != NULL)  //试用fseek(fp, 2, 0);更改2
	{
		fgetc(fconfig);	 //跳过第1位 背景色-Launcher
		fgetc(fconfig);	 //跳过第2位 前景色-Launcher
		color_set[6] = fgetc(fconfig);	 //第3位 背景色-Main
		color_set[7] = fgetc(fconfig);	 //第4位 前景色-Main
		fgetc(fconfig);	 //跳过第5位 背景色-Adventure
		fgetc(fconfig);	 //跳过第6位 前景色-Adventure
		show_audio = fgetc(fconfig) - 48;  //第7位 音频
		show_image = fgetc(fconfig) - 48;  //第8位 曲绘
		fgetc(fconfig);	 //跳过第1行的\n 以获取第2行的播放器目录
		fscanf(fconfig, "%[^\n]", player);
		if (!strcmp(player, "#NULL"))	memset(player,0,sizeof(player));
		fclose(fconfig);
	}
	system(color_set);
	//基准速率
	//Speed Upright
	system("cls");
	fupright = fopen(UPRIGHT_DATA, "r");
	while (fupright == NULL)
	{
		system("cls");
		moi_brief();
		printf("欢迎来到Melody Of Involution!\n");
		printf("首次启动需要进行一些配置\n");
		printf("建议您键入-1以自动完成基准速率测试\n");
		printf("也可以键入[0,9999]中的整数值来手动设置基准速率(以ms为单位)\n");
		scanf("%d", &in_int);
		fflush(stdin);
		if (in_int < -1 || in_int>9999)
		{
			printf("错误:非法输入:%d\n", in_int);
			printf("基准配速值未调整\n");
			printf("当前基准配速:%d ms\n", consuming);
			printf("若要再次设置 可重新启动程序\n");
			printf("按任意键进入...");
			fflush(stdin);
			_getch();
			fflush(stdin);
			system("cls");
			break;
		}
		if (in_int == -1)
		{
			FILE *fp = fopen(UPRIGHT_EXE, "r");
			if (fp == NULL)
			{
				printf("错误:未找到基准配速测试程序:%s\n", UPRIGHT_EXE);
				printf("基准配速值未调整\n");
				printf("当前基准配速:%d ms\n", consuming);
				printf("建议您检查程序组件完整性\n");
				printf("若要再次设置 可重新启动程序\n");
				printf("按任意键进入...");
				fflush(stdin);
				_getch();
				fflush(stdin);
				break;
			}
			else
			{
				fclose(fp);
				consuming = system("MOI_SpeedUpright.exe");
				SetConsoleTitleA(MOI_TITLE);
			}
		}
		else consuming = in_int;
		printf("设置成功:%d\n", consuming);
		printf("是否写入文件?[Y/N]\n");
		do
		{
			fflush(stdin);
			in_char = _getch();
			fflush(stdin);
		} while (in_char != 'Y'&&in_char != 'N'&&in_char != 'y'&&in_char != 'n');
		if (in_char == 'Y' || in_char == 'y')
		{
			fupright = fopen(UPRIGHT_DATA, "w+");
			fprintf(fupright, "%d", consuming);
			fclose(fupright);
			fupright = fopen(UPRIGHT_DATA, "r");
			printf("\n写入成功\n");
			printf("按任意键进入...");
			fflush(stdin);
			_getch();
			fflush(stdin);
		}
		break;
	}
	if (fupright != NULL)
	{
		fscanf(fupright, "%d", &consuming);
		fclose(fupright);
	}
	//环境检测
	//Environment Check
	system("cls");
	moi_brief();
	printf("正在检测环境...\n");
	if (IsDebuggerPresent()*cheat_disabled)
	{
		environment_error = 1;
		printf("<Info>[环境异常:检测到调试器]\n");
		printf("游玩时启用调试器会在结算界面标注\n");
	}
	if (!system("tasklist|findstr \"HipsDaemon.exe\""))
	{
		environment_error = 1;
		printf("<Warn>[环境异常:检测到火绒安全软件进程]\n");
		printf("程序运行速度将会降低\n");
	}
	if (!system("tasklist|findstr \"360tray.exe\""))
	{
		environment_error = 1;
		printf("<Warn>[环境异常:检测到360安全卫士进程]\n");
		printf("程序运行速度将会降低\n");
	}
	if (!system("tasklist|findstr \"360sd.exe\""))
	{
		environment_error = 1;
		printf("<Warn>[环境异常:检测到360杀毒进程]\n");
		printf("程序运行速度可能降低\n");
	}
	if (environment_error == 1)
	{
		printf("------------------------------------------------------------\n"
		       "警告:检测到上述环境异常\n"
		       "为保证游戏体验 请尽快修复\n"
			   "\n"
			   "*注意:MOI并不含有恶意代码\n"
			   "      关闭杀毒软件的目的是为了避免性能损失引起的速率大幅下降\n"
			   "      开启杀毒软件同时游玩高难度谱可能出现速率校正失败\n"
			   "      也可使用Windows Defender/MSE等几乎不影响速率的杀毒软件\n"
		       "按任意键进入..."
			  );
		fflush(stdin);
		_getch();
		fflush(stdin);
	}
	//主界面
	//Home
	do
	{
		system("cls");
		moi_full();
		printf("\n--------------------------------------------------\n\n");
		printf("[配置信息]\n");
		printf("基准配速:%d ms per Line\n", consuming);
		printf("音乐播放器:%s", player);
		if (!strlen(player))	printf("[默认]");
		printf("\n");
		printf("音频:");
		if (show_audio)	printf("[ON]");
		else printf("[OFF]");
		printf(" | 曲绘:");
		if (show_image)	printf("[ON]");
		else printf("[OFF]");
		printf("\n\n--------------------------------------------------\n\n");
		printf("拖入M.O.I.项目文件(*.txt) 或键入项目文件路径\n"
			   "然后按Enter以进入\n"
			   "(键入*列出剧情序号索引)\n"
			  );
		scanf("%[^\n]", item);  //输入
		fflush(stdin);
		len = strlen(item);
		if (item[0] == '*')	 //输入类型为序号
		{
			bool found = 0;
			FILE *fp = NULL;
			fp = fopen(INDEX_PLAY, "r");
			if (fp == NULL)
			{
				printf("未找到索引文件:%s!\n"
					"序号与索引不可用\n"
					"按任意键返回..."
					, INDEX_PLAY);
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
		if (item[0] == '$' && strcmp(item,"$$"))
		{
			int t_start, t_end, result;
			printf("-- MOI Terminal --\n");
			printf("MOI>");
			scanf("%[^\n]", item);
			fflush(stdin);
			printf("命令:%s\n", item);
			if (!strcmp(item, "$back"))	continue;
			if (!strcmp(item, "$exit"))	return 0;
			if (!strcmp(item, "$get-info"))
			{
				printf("程序路径:%s\n", _pgmptr);
				printf("窗口颜色:%c(背景) | %c(前景)\n", color_set[6], color_set[7]);
				printf("当前时间:%d\n", time(0));
				printf("基准配速:%d\n", consuming);
				printf("内置配速补偿:+ %d ms per Line\n", internal_upright);
				printf("按任意键继续...");
				fflush(stdin);
				_getch();
				fflush(stdin);
				continue;
			}
			if (!strncmp(item, "$set-upright:", 13))
			{
				if (!strcmp(item, "$set-upright:reset"))
				{
					char del_str[36] = "del /f /q ";
					strcat(del_str, UPRIGHT_DATA);
					system(del_str);
					printf("MOI:速率校正数据文件已删除\n");
					printf("按任意键继续...");
					fflush(stdin);
					_getch();
					fflush(stdin);
					continue;
				}
				else
				{
					int a = item[13] >= 48 ? item[13] - 48 : 0;
					int b = item[14] >= 48 ? item[14] - 48 : 0;
					int c = item[15] >= 48 ? item[15] - 48 : 0;
					int d = item[16] >= 48 ? item[16] - 48 : 0;
					consuming = 1000 * a + 100 * b + 10 * c + d;
					printf("MOI:基准配速更改为:%d ms per Line\n", consuming);
					printf("是否写入文件?[Y/N]\n");
					do
					{
						fflush(stdin);
						in_char = _getch();
					} while (in_char != 'Y'&&in_char != 'N'&&in_char != 'y'&&in_char != 'n');
					if (in_char == 'Y' || in_char == 'y')
					{
						fupright = fopen(UPRIGHT_DATA, "w+");
						fprintf(fupright, "%d", consuming);
						fclose(fupright);
						fupright = fopen(UPRIGHT_DATA, "r");
						printf("写入成功\n");
					}
					printf("按任意键继续...");
					fflush(stdin);
					_getch();
					fflush(stdin);
					continue;
				}
			}
			printf("--开始运行--\n\n");
			t_start = clock();
			result = system(item);
			t_end = clock();
			printf("\n--结束运行--\n");
			printf("开始 = %d\n", t_start);
			printf("结束 = %d\n", t_end);
			printf("用时 = %dms = %.3fs\n", t_end - t_start, (t_end - t_start)*0.001);
			printf("返回代码 = %d\n\n\n", result);
			printf("按任意键然后Enter以返回...");
			fflush(stdin);
			getchar();
			fflush(stdin);
			system("cls");
			continue;
		}
		if (!strcmp(item, "$$"))
		{
			printf("该功能仍在开发中 敬请期待...");
			fflush(stdin);
			_getch();
			fflush(stdin);
			continue;
		}
		if (item[0] == '\"'&&item[len - 1] == '\"')  //处理拖入的带空格路径 而不新增字符串
		{
			for (int imov = 1; imov < len; imov++)	item[imov - 1] = item[imov];  //前移1位以覆盖开头的引号
			item[len - 2] = 0;	//删去被前移的末尾引号
		}
		fitem = fopen(item, "r");
		if (fitem == NULL)
		{
			printf("错误:未找到:%s!\n", item);
			printf("检查文件路径是否正确及是否含有空格\n");
			printf("按任意键然后Enter以退出...");
			getchar();
			return 0;
		}
		else
		{
			printf("项目:%s\n", item);
			fclose(fitem);
			play(item, audio_config);
		}
		printf("按任意键然后Enter以退出...");
		getchar();
		break;
	} while (1);
	return 0;
}
void play(char item[], int audio_config)
{
	bool constant_no_debugger = 1, upright_failed = 0, show_audio_proj = 1, show_image_proj = 1;
	char in = 0, mp3file[136] = {}, lrcfile[136] = {}, bmpfile[136] = {}, fin_str[22] = {};
	char head_char[20] = { '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '>', '|', '|', '>', '|', '|', '>', '|' }, warn_char[4] = { '_', '_', '_', '_' };
	int i = 0, clear = 0, lost = 0, len_int = 0, note_int = 0, target_speed = 0, pause = 0, click = 0, t_start = 0, t_end = 0;
	FILE *fitem = NULL, *fsong = NULL;
	memset(space, ' ', sizeof(space));
	for (int isetline = 0; isetline <= 20; isetline++)	space[isetline][4] = 0;
	fitem = fopen(item, "r");
	fgets(fin_str, 23, fitem);
	fgets(lrcfile, 136, fitem);
	fgets(mp3file, 136, fitem);
	fgets(bmpfile, 136, fitem);
	fclose(fitem);
	lrcfile[strlen(lrcfile) - 1] = 0;
	mp3file[strlen(mp3file) - 1] = 0;
	bmpfile[strlen(bmpfile) - 1] = 0;
	fsong = fopen(lrcfile, "r");  //打开谱面文件
	target_speed = (fin_str[0] - 48) * 1000 + (fin_str[1] - 48) * 100 + (fin_str[2] - 48) * 10 + (fin_str[3] - 48);
	len_int = (fin_str[5] - 48) * 100000 + (fin_str[6] - 48) * 10000 + (fin_str[7] - 48) * 1000 + (fin_str[8] - 48) * 100 + (fin_str[9] - 48) * 10 + (fin_str[10] - 48);
	note_int = (fin_str[12] - 48) * 100000 + (fin_str[13] - 48) * 10000 + (fin_str[14] - 48) * 1000 + (fin_str[15] - 48) * 100 + (fin_str[16] - 48) * 10 + (fin_str[17] - 48);
	pause = target_speed - consuming;
	show_audio_proj = fin_str[19] - 48;
	show_image_proj = fin_str[20] - 48;
	show_audio = show_audio&&show_audio_proj;
	show_image = show_image&&show_image_proj;
	if (pause < internal_upright)  //pause = pause > internal_upright ? pause : internal_upright;
	{
		pause = internal_upright;
		upright_failed = 1;
	}
	if (audio_config != 0 || show_audio != 0)
	{
		char audio_transfer[142] = {};
		strcpy(audio_transfer, player);
		strcat(audio_transfer, " ");
		strcat(audio_transfer, mp3file);
		system(audio_transfer);
	}
	if (show_image != 0)
	{
		char img_transfer[142] = "start ";
		strcat(img_transfer, IMGDISPLAY);
		strcat(img_transfer, " ");
		strcat(img_transfer, bmpfile);
		system(img_transfer);
	}
	system("cls");	//欲调试以上代码 在此行打断点
	moi_brief();
	printf("项目:%s\n", item);
	printf("长度:%d\n", len_int);
	printf("音符数:%d\n", note_int);
	printf("理论行速率:%d ms per Line\n", target_speed);
	printf("谱面:%s\n", lrcfile);
	printf("音频:%s\n", mp3file);
	printf("曲绘:%s\n", bmpfile);
	printf("音频:");
	if (show_audio)	printf("[ON]");
	else printf("[OFF]");
	printf(" | 曲绘:");
	if (show_image)	printf("[ON]");
	else printf("[OFF]");
	printf("\n请将输入法切换至英语\n");
	printf("听见提示音后按任意键开始...");
	fflush(stdin);
	_getch();
	system("cls");
	t_start = clock();
	for (; i <= len_int; i++)
	{
		int iloop = 1, warn_span[4] = { 0, 0, 0, 0 };
		constant_no_debugger = constant_no_debugger*!bool(IsDebuggerPresent());
		system("cls");
		moi_brief();
		printf(" _________________________\n");
		printf("| %d PERFECT / ", clear);
		if (lost == 0)	printf("FULL COMBO\n");
		else  printf("%d LOST\n", lost);
		if (len_int < 10 || len_int >= 10000)	printf("| [%3d%%] %d / %d\n", i * 100 / len_int, i, len_int);  //%3d为[  1%] %03d为[001%]
		if (len_int >= 10 && len_int < 100)		printf("| [%3d%%] %2d / %2d\n", i * 100 / len_int, i, len_int);
		if (len_int >= 100 && len_int < 1000)	printf("| [%3d%%] %3d / %3d\n", i * 100 / len_int, i, len_int);
		if (len_int >= 1000 && len_int < 10000)	printf("| [%3d%%] %4d / %4d\n", i * 100 / len_int, i, len_int);
		printf("|_________________________\n");
		if (!feof(fsong))
		{
			for (int igetcol = 0; igetcol <= 3; igetcol++)
			{
				bool loop = 1;
				while (loop)
				{
					loop = 0;
					space[0][igetcol] = fgetc(fsong);
					if (space[0][igetcol] == '$')	loop = 1;  //遇到$则跳过不输出
					if (space[0][igetcol] == '\n')	loop = 1;  //遇到换行则跳过不输出
					if (space[0][igetcol] == '#')	space[0][igetcol] = ' ';  //遇到#则当成空格
				}
			}
		}
		else	strcpy(space[0], "    ");
		for (int iprint = 0; iprint <= 19; iprint++)	printf("%c %c %c %c %c |\n", head_char[iprint], space[iprint][0], space[iprint][1], space[iprint][2], space[iprint][3]);
		printf("__%c_%c_%c_%c__________________\n", warn_char[0], warn_char[1], warn_char[2], warn_char[3]);
		for (int ichk = 0; ichk <= 3; ichk++)  //检查第20行是否有遗漏 以此判定LOST
		{
			if (space[19][ichk] != ' ')
			{
				lost++;
				warn_span[ichk] = iloop;  //检测置于该轮输出完成后 故下一轮才会反映检测结果 从而保证音符离开数组后再显示LOST警告
			}
		}
		for (int iwarn = 0; iwarn <= 3; iwarn++)  //虽然第1/2判定线的音符在第20行之前就应被判为LOST 但简单起见仍检测第20行音符 只要最后空行足够多(按制谱文档要求)就能准确统计
		{
			if (warn_span[iwarn] >= iloop)	warn_char[iwarn] = '^';
			else  warn_char[iwarn] = '_';
		}
		for (int icy = 11; icy <= 19; icy++)
		{
			for (int icx = 0; icx <= 3; icx++)
			{
				if (space[icy][icx] == '+')	space[icy][icx] = ' ';
			}
		}
		while (_kbhit())
		{
			click++;
			in = _getch();
			fflush(stdin);	//getch前应该不宜用fflush以免缩短接受输入的时长
			switch (in)	 //冗长的判定代码
			{
			case 'z':
				if (space[18][0] == 'z')
				{
					space[18][0] = '+';
					clear++;
				}
				if (space[17][0] == 'z')
				{
					space[17][0] = '+';
					clear++;
				}
				break;
			case 'Z':
				if (space[18][0] == 'Z')
				{
					space[18][0] = '+';
					clear++;
				}
				if (space[17][0] == 'Z')
				{
					space[17][0] = '+';
					clear++;
				}
				break;
			case 'x':
				if (space[18][1] == 'x')
				{
					space[18][1] = '+';
					clear++;
				}
				if (space[17][1] == 'x')
				{
					space[17][1] = '+';
					clear++;
				}
				break;
			case 'X':
				if (space[18][1] == 'X')
				{
					space[18][1] = '+';
					clear++;
				}
				if (space[17][1] == 'X')
				{
					space[17][1] = '+';
					clear++;
				}
				break;
			case 'c':
				if (space[18][2] == 'c')
				{
					space[18][2] = '+';
					clear++;
				}
				if (space[17][2] == 'c')
				{
					space[17][2] = '+';
					clear++;
				}
				break;
			case 'C':
				if (space[18][2] == 'C')
				{
					space[18][2] = '+';
					clear++;
				}
				if (space[17][2] == 'C')
				{
					space[17][2] = '+';
					clear++;
				}
				break;
			case 'v':
				if (space[18][3] == 'v')
				{
					space[18][3] = '+';
					clear++;
				}
				if (space[17][3] == 'v')
				{
					space[17][3] = '+';
					clear++;
				}
				break;
			case 'V':
				if (space[18][3] == 'V')
				{
					space[18][3] = '+';
					clear++;
				}
				if (space[17][3] == 'V')
				{
					space[17][3] = '+';
					clear++;
				}
				break;
			case 'a':
				if (space[15][0] == 'a')
				{
					space[15][0] = '+';
					clear++;
				}
				if (space[14][0] == 'a')
				{
					space[14][0] = '+';
					clear++;
				}
				break;
			case 'A':
				if (space[15][0] == 'A')
				{
					space[15][0] = '+';
					clear++;
				}
				if (space[14][0] == 'A')
				{
					space[14][0] = '+';
					clear++;
				}
				break;
			case 's':
				if (space[15][1] == 's')
				{
					space[15][1] = '+';
					clear++;
				}
				if (space[14][1] == 's')
				{
					space[14][1] = '+';
					clear++;
				}
				break;
			case 'S':
				if (space[15][1] == 'S')
				{
					space[15][1] = '+';
					clear++;
				}
				if (space[14][1] == 'S')
				{
					space[14][1] = '+';
					clear++;
				}
				break;
			case 'd':
				if (space[15][2] == 'd')
				{
					space[15][2] = '+';
					clear++;
				}
				if (space[14][2] == 'd')
				{
					space[14][2] = '+';
					clear++;
				}
				break;
			case 'D':
				if (space[15][2] == 'D')
				{
					space[15][2] = '+';
					clear++;
				}
				if (space[14][2] == 'D')
				{
					space[14][2] = '+';
					clear++;
				}
				break;
			case 'f':
				if (space[15][3] == 'f')
				{
					space[15][3] = '+';
					clear++;
				}
				if (space[14][3] == 'f')
				{
					space[14][3] = '+';
					clear++;
				}
				break;
			case 'F':
				if (space[15][3] == 'F')
				{
					space[15][3] = '+';
					clear++;
				}
				if (space[14][3] == 'F')
				{
					space[14][3] = '+';
					clear++;
				}
				break;
			case 'q':
				if (space[12][0] == 'q')
				{
					space[12][0] = '+';
					clear++;
				}
				if (space[11][0] == 'q')
				{
					space[11][0] = '+';
					clear++;
				}
				break;
			case 'Q':
				if (space[12][0] == 'Q')
				{
					space[12][0] = '+';
					clear++;
				}
				if (space[11][0] == 'Q')
				{
					space[11][0] = '+';
					clear++;
				}
				break;
			case 'w':
				if (space[12][1] == 'w')
				{
					space[12][1] = '+';
					clear++;
				}
				if (space[11][1] == 'w')
				{
					space[11][1] = '+';
					clear++;
				}
				break;
			case 'W':
				if (space[12][1] == 'W')
				{
					space[12][1] = '+';
					clear++;
				}
				if (space[11][1] == 'W')
				{
					space[11][1] = '+';
					clear++;
				}
				break;
			case 'e':
				if (space[12][2] == 'e')
				{
					space[12][2] = '+';
					clear++;
				}
				if (space[11][2] == 'e')
				{
					space[11][2] = '+';
					clear++;
				}
				break;
			case 'E':
				if (space[12][2] == 'E')
				{
					space[12][2] = '+';
					clear++;
				}
				if (space[11][2] == 'E')
				{
					space[11][2] = '+';
					clear++;
				}
				break;
			case 'r':
				if (space[12][3] == 'r')
				{
					space[12][3] = '+';
					clear++;
				}
				if (space[11][3] == 'r')
				{
					space[11][3] = '+';
					clear++;
				}
				break;
			case 'R':
				if (space[12][3] == 'R')
				{
					space[12][3] = '+';
					clear++;
				}
				if (space[11][3] == 'R')
				{
					space[11][3] = '+';
					clear++;
				}
				break;
			}
			if ((!_kbhit()) && (iloop <= 5))	continue;
			break;
		}
		for (int imovline = 19; imovline >= 0; imovline--)
		{
			for (int imovcol = 0; imovcol <= 3; imovcol++)
				space[imovline + 1][imovcol] = space[imovline][imovcol];
		}
		Sleep(pause + internal_upright);
	}
	t_end = clock();
	fclose(fsong);
	complete(item, lrcfile, len_int, note_int, target_speed, t_start, t_end, clear, lost, click, constant_no_debugger, pause, upright_failed);
}
void complete(char item[], char lrcfile[], int len_int, int note_int, int target_speed, int t_start, int t_end, int clear, int lost, int click, bool constant_no_debugger, int pause, bool upright_failed)
{
	char in_char;
	char chk[157] = {};
	double len_double = len_int*1.0000, note_double = note_int*1.00000000, clear_double = clear*1.0000, actual_speed_double;
	int score = clear_double / note_double * 1000000, t = t_end - t_start, r = 1;
	fflush(stdin);
	strcpy(chk, "certutil -hashfile ");
	strcat(chk, lrcfile);
	strcat(chk, " SHA1");
	system("cls");
	moi_brief();
	printf("*|--------------------|*\n");
	printf("*|---TRACK COMPLETE---|*\n");
	printf("*|--------------------|*\n");
	printf("*|得分:%07d\n", score);
	printf("*|等级:");
	if (score < 200000)	printf("F  [再接再厉]");
	if (score >= 200000 && score < 350000)	printf("D  [余音绕梁]");
	if (score >= 350000 && score < 500000)	printf("C  [余音绕梁]");
	if (score >= 500000 && score < 650000)	printf("B  [声动梁尘]");
	if (score >= 650000 && score < 800000)	printf("A  [声动梁尘]");
	if (score >= 800000 && score < 920000)	printf("S  [高遏行云]");
	if (score >= 920000 && score < 1000000)	printf("V  [高遏行云]");
	if (score == 1000000)	printf("EX [大音天籁]");
	if (score > 1000000)	printf("<Error> Level Undefined [大音天籁*]");
	printf("\n*|PERFECT %d\n", clear);
	printf("*|LOST    %d\n", lost);
	if (clear == note_int)	printf("[FULL COMBO / ALL PERFECT]\n");
	printf("------------------------------------------------------------\n");
	printf("点击次数:%d\n", click);
	if (click != 0)	printf("点击精度:%.2lf%%\n", clear_double / click * 100);
	printf("有效点击速率:%.2f Note(s) per Minute\n", clear_double / t * 60000);
	printf("用时:%.3lf Second(s) [%d ms - %d ms]\n", t*0.001, t_end, t_start);
	printf("------------------------------------------------------------\n");
	printf("项目:%s\n", item);
	printf("基准配速:%d\n", consuming);
	printf("速率校正:%d\n", pause);
	actual_speed_double = t / len_double * 0.001;
	printf("长度:%d\n", len_int);
	printf("每行时长:%0.3lf Second(s) per Line", actual_speed_double);
	if (int(actual_speed_double * 1000) < target_speed - 200)	printf(" [偏低]");
	if (upright_failed)	printf("[速率校正失败]");
	printf("\n目标每行时长:%.3lf Second(s) per Line\n", target_speed * 0.001);
	printf("音符数:%d\n", note_int);
	printf("音符速率:%.3lf Note(s) per Minute\n", note_double / t * 60000);
	printf("[校验信息]--------------------------------------------------\n");
	printf("当前时间:%d\n", time(0));
	printf("是否检测到调试器:%d\n", !constant_no_debugger * cheat_disabled);
	r = system(chk);
	if (r)	printf("错误:校验未执行(返回代码:%d)\n", r);
	printf("------------------------------------------------------------\n");
	switch (!constant_no_debugger * cheat_disabled)
	{
	case 0:
		printf("是否记录冒险模式经验值?[Y/N]");
		do
		{
			fflush(stdin);
			in_char = _getch();
			fflush(stdin);
		} while (in_char != 'Y'&&in_char != 'N'&&in_char != 'y'&&in_char != 'n');
		if (in_char == 'Y' || in_char == 'y')
		{
			char score_str[8] = {}, transfer_str[49] = {};
			/*
			score_str[6] = score / 1000000 % 10 + 48;
			score_str[5] = score / 100000 % 10 + 48;
			score_str[4] = score / 10000 % 10 + 48;
			score_str[3] = score / 1000 % 10 + 48;
			score_str[2] = score / 100 % 10 + 48;
			score_str[1] = score / 10 % 10 + 48;
			score_str[0] = score % 10 + 48;
			*/
			itoa(score, score_str, 10);
			strcpy(transfer_str, ADVENTURE_EXE);
			strcat(transfer_str, " ");
			strcat(transfer_str, score_str);
			strcat(transfer_str, " ");
			strcat(transfer_str, ADVENTUREMODE_REGID);
			r = system(transfer_str);
			switch (r)
			{
			case -1:
				printf("记录失败!\n");
				break;
			default:
				printf("记录成功!\n");
				printf("得分:%s (%lld)\n", score_str, r);
				break;
			}
		}
	}
	printf("按任意键然后Enter以退出...");
	getchar();
}