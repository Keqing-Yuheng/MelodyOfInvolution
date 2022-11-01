#define _CRT_SECURE_NO_WARNINGS
#include"ImgDisplay.h"
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<windows.h>

const char* CONFIG_DATA = "MOI_Config.ini";
const char* BRIEF_HELP = "MOI_BriefHelp.md";

int select_l();

inline void moi()
{
	printf("Melody Of Involution\n"
		   "Version Release 1.1\n"
		   "GitHub @ Keqing-Yuheng\n"
		  );
}

int main(int argc, char* argv[])
{
	char get_title[255] = {}, color_set[9] = "color 0f";
	FILE *fconfig = NULL, *fscript = NULL, *fh = NULL;
	fconfig = fopen(CONFIG_DATA, "r");
	if (fconfig != NULL)
	{
		color_set[6] = fgetc(fconfig);	 //第1位 背景色-Launcher
		color_set[7] = fgetc(fconfig);	 //第2位 前景色-Launcher
		fclose(fconfig);
	}
	system("mode con cols=60 lines=17");
	system(color_set);
	SetConsoleTitleA("Melody Of Involution [R1.1]");  //SetConsoleTitleA()不会显示"管理员:..." 而title命令会显示"管理员:...
	system("cls");
	printf("Melody Of Involution");	 //注意不要换行 以免光标遮挡图片 可以考虑并把光标放远一点而美观
	ImgDisplay_Wipe("MOI_Welcome.bmp", "#DEFAULT_TITLE");
	Sleep(500);
	fscript = fopen("MOI_ScriptStart.cmd", "r");
	system("mode con cols=62 lines=35");
	if (fscript != NULL)
	{
		fclose(fscript);
		system("MOI_ScriptStart.cmd");
	}
	switch (select_l())
	{
	case 1:
		system("MelodyOfInvolution.exe");
		return 0;
	case 2:
		system("MOI_AdventureMode.exe");
		return 0;
	case 3:
		system("start https://github.com/Keqing-Yuheng/MelodyOfInvolution");
		return 0;
	case 4:
		return 0;
	}
	return 0;
}

int select_l()
{
	bool loop;
	char in_char;
	char c_front[4] = { '>', ' ', ' ', ' ' }, c_back[4] = { '<', ' ', ' ', ' ' };
	int location = 0, n = 0;  //location为数组序号以0起始 return为其+1以1起始
	while (1)
	{
		loop = 1;
		system("cls");
		moi();
		printf("\n"
			"使用W/S键上下移动选择光标\"> <\"\n"
			"然后使用空格键选择\n"
			"或键入项目前的序号以执行对应操作\n"
			"\n"
			" %c [1]开始游戏 %c\n"
			" %c [2]冒险模式 %c\n"
			" %c [3]项目网址 %c\n"
			" %c [0]退出程序 %c\n"
			, c_front[0], c_back[0], c_front[1], c_back[1], c_front[2], c_back[2], c_front[3], c_back[3]);
		do
		{
			fflush(stdin);
			in_char = _getch();
			fflush(stdin);
		} while (in_char != ' '&&in_char != 'w'&&in_char != 'W'&&in_char != 's'&&in_char != 'S'&&in_char != '1'&&in_char != '2'&&in_char != '3'&&in_char != '0');
		while (loop)
		{
			switch (in_char)
			{
			case 's':
				loop = 0;
				for (n = 0; c_front[n] != '>'; n++){}
				c_front[n] = ' ';
				c_back[n] = ' ';
				n = n >= 3 ? 0 : n + 1;	//循环滚动
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
				n = n <= 0 ? 3 : n - 1;
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
			case '3':return 3;
			case '0':return 4;
			}
		}
		
	}
}