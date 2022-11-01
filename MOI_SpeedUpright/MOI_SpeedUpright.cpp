#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<conio.h>
#include<windows.h>

const char* IMGDISPLAY = "MOI_ImgDisplay.exe";
const char* UPRIGHT_DATA = "MOI_SpeedUpright_Data.ini";
const char* CONFIG_DATA = "MOI_Config.ini";
const char* UPRIGHT_PROJECT = "MOI_Test_Project.txt";

bool cheat_disabled = 1, show_audio = 1, show_image = 1;
char player[257];
char space[20][5];
char color_set[9] = "color 0f";
int consuming = 0, internal_upright = 0;

int play_su(char item[], int audio_config);
int complete_su(char item[], char lrcfile[], int len_int, int note_int, int target_speed, int t_start, int t_end, int clear, int lost, int click, bool constant_no_debugger, int pause, bool upright_failed);

inline void moi_u()
{
	printf("Melody Of Involution\n"
		   "Speed Upright\n"
		   "Version Release 1.1\n"
		  );
}

int main(int argc, char* argv[])
{
	char item[129] = {};
	FILE *fp = NULL, *fconfig = NULL;
	strcpy(item, UPRIGHT_PROJECT);
	if (argc >= 2)
	{
		memset(item, 0, sizeof(item));
		strcpy(item, argv[1]);
	}
	fp = fopen(item, "r");
	if (fp == NULL)
	{
		moi_u();
		printf("未找到测试项目:%s!\n"
			"传入参数以指定测试项目\n"
			"按任意键退出..."
			, item);
		_getch();
		fflush(stdin);
		return 0;
	}
	fclose(fp);
	SetConsoleTitleA("Melody Of Involution - Speed Upright [R1.1]");
	system("cls");
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
		fclose(fconfig);
	}
	system(color_set);
	moi_u();
	printf("--------------------------------------------------\n"
		"[警告]\n"
		"执行速率测定时，窗口可能会快速闪烁\n"
		"极少数人暴露在特定光影图案或闪光光亮下时，会引发癫痫\n"
		"为了您的健康，在此期间请不要注视窗口！\n"
		"窗口将换用对比度稍低的颜色以降低风险\n"
		);
	printf("测试项目:%s\n", item);
	printf("按任意键开始速率测试...");
	fflush(stdin);
	_getch();
	fflush(stdin);
	return play_su(item, 0);
}

int play_su(char item[], int audio_config)
{
	bool constant_no_debugger = 1, upright_failed = 0, show_audio_proj = 1, show_image_proj = 1;
	char in = 0, mp3file[136] = {}, lrcfile[133] = {}, bmpfile[136] = {}, fin_str[22] = {};
	char head_char[20] = { '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '|', '>', '|', '|', '>', '|', '|', '>', '|' }, warn_char[4] = { '_', '_', '_', '_' };
	int i = 0, clear = 0, lost = 0, len_int = 0, note_int = 0, target_speed = 0, pause = 0, click = 0, t_start = 0, t_end = 0;
	FILE *fitem = NULL, *fsong = NULL;
	system("color 08");	 //改变颜色
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
	moi_u();
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
		moi_u();
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
			case 'Z'://26
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
			case 'X'://24
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
			case 'V'://22
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
			case 'S'://19
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
			case 'D'://4
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
			case 'Q'://17
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
			case 'W'://23
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
			case 'E'://5
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
			case 'R'://18
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
	}
	t_end = clock();
	fclose(fsong);
	return complete_su(item, lrcfile, len_int, note_int, target_speed, t_start, t_end, clear, lost, click, constant_no_debugger, pause, upright_failed);
}
int complete_su(char item[], char lrcfile[], int len_int, int note_int, int target_speed, int t_start, int t_end, int clear, int lost, int click, bool constant_no_debugger, int pause, bool upright_failed)
{
	char in_char;
	double len_double = len_int*1.0000, note_double = note_int*1.00000000, clear_double = clear*1.0000, actual_speed_double;
	int score = clear_double / note_double * 1000000, t = t_end - t_start, r = 1;
	system(color_set);	//恢复颜色
	consuming = (t_end - t_start) / len_int;
	fflush(stdin);
	system("cls");
	moi_u();
	printf("*|---UPRIGHT DONE---|*\n");
	printf("*|基准配速:%04d ms\n", consuming);
	printf("------------------------------------------------------------\n");
	printf("得分:%07d\n", score);
	printf("等级:");
	if (score < 200000)	printf("F  [再接再厉]");
	if (score >= 200000 && score < 350000)	printf("D  [余音绕梁]");
	if (score >= 350000 && score < 500000)	printf("C  [余音绕梁]");
	if (score >= 500000 && score < 650000)	printf("B  [声动梁尘]");
	if (score >= 650000 && score < 800000)	printf("A  [声动梁尘]");
	if (score >= 800000 && score < 920000)	printf("S  [高遏行云]");
	if (score >= 920000 && score < 1000000)	printf("V  [高遏行云]");
	if (score == 1000000)	printf("EX [大音天籁]");
	if (score > 1000000)	printf("<Error> Level Undefined [大音天籁*]");
	printf("\nPERFECT %d\n", clear);
	printf("LOST    %d\n", lost);
	if (clear == note_int)	printf("[FULL COMBO / ALL PERFECT]\n");
	printf("------------------------------------------------------------\n");
	printf("点击次数:%d\n", click);
	if (click != 0)	printf("点击精度:%.2lf%%\n", clear_double / click * 100);
	printf("有效点击速率:%.2f Note(s) per Minute\n", clear_double / t * 60000);
	printf("用时:%.3lf Second(s) [%d ms - %d ms]\n", t*0.001, t_end, t_start);
	printf("------------------------------------------------------------\n");
	printf("测试项目:%s\n", item);
	printf("基准配速:%d\n", consuming);
	printf("速率校正:%d\n", pause);
	actual_speed_double = t / len_double * 0.001;
	printf("长度:%d\n", len_int);
	printf("每行时长:%0.3lf Second(s) per Line", actual_speed_double);
	printf("\n目标每行时长:%.3lf Second(s) per Line\n", target_speed * 0.001);
	printf("音符数:%d\n", note_int);
	printf("音符速率:%.3lf Note(s) per Minute\n", note_double / t * 60000);
	printf("------------------------------------------------------------\n");
	printf("是否记录配速校正数据?[Y/N]");
	do
	{
		fflush(stdin);
		in_char = _getch();
		fflush(stdin);
	} while (in_char != 'Y'&&in_char != 'N'&&in_char != 'y'&&in_char != 'n');
	if (in_char == 'Y' || in_char == 'y')
	{
		FILE *fupright = NULL;
		fupright = fopen(UPRIGHT_DATA, "w+");
		fprintf(fupright, "%d", consuming);
		fclose(fupright);
		printf("\n写入成功\n");
		printf("按任意键进入...");
		fflush(stdin);
		_getch();
		fflush(stdin);
	}
	return consuming;
}