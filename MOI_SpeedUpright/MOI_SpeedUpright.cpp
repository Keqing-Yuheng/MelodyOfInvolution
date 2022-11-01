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
		printf("δ�ҵ�������Ŀ:%s!\n"
			"���������ָ��������Ŀ\n"
			"��������˳�..."
			, item);
		_getch();
		fflush(stdin);
		return 0;
	}
	fclose(fp);
	SetConsoleTitleA("Melody Of Involution - Speed Upright [R1.1]");
	system("cls");
	fconfig = fopen(CONFIG_DATA, "r");
	if (fconfig != NULL)  //����fseek(fp, 2, 0);����2
	{
		fgetc(fconfig);	 //������1λ ����ɫ-Launcher
		fgetc(fconfig);	 //������2λ ǰ��ɫ-Launcher
		color_set[6] = fgetc(fconfig);	 //��3λ ����ɫ-Main
		color_set[7] = fgetc(fconfig);	 //��4λ ǰ��ɫ-Main
		fgetc(fconfig);	 //������5λ ����ɫ-Adventure
		fgetc(fconfig);	 //������6λ ǰ��ɫ-Adventure
		show_audio = fgetc(fconfig) - 48;  //��7λ ��Ƶ
		show_image = fgetc(fconfig) - 48;  //��8λ ����
		fclose(fconfig);
	}
	system(color_set);
	moi_u();
	printf("--------------------------------------------------\n"
		"[����]\n"
		"ִ�����ʲⶨʱ�����ڿ��ܻ������˸\n"
		"�������˱�¶���ض���Ӱͼ�������������ʱ�����������\n"
		"Ϊ�����Ľ������ڴ��ڼ��벻Ҫע�Ӵ��ڣ�\n"
		"���ڽ����öԱȶ��Ե͵���ɫ�Խ��ͷ���\n"
		);
	printf("������Ŀ:%s\n", item);
	printf("���������ʼ���ʲ���...");
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
	system("color 08");	 //�ı���ɫ
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
	fsong = fopen(lrcfile, "r");  //�������ļ�
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
	system("cls");	//���������ϴ��� �ڴ��д�ϵ�
	moi_u();
	printf("��Ŀ:%s\n", item);
	printf("����:%d\n", len_int);
	printf("������:%d\n", note_int);
	printf("����������:%d ms per Line\n", target_speed);
	printf("����:%s\n", lrcfile);
	printf("��Ƶ:%s\n", mp3file);
	printf("����:%s\n", bmpfile);
	printf("��Ƶ:");
	if (show_audio)	printf("[ON]");
	else printf("[OFF]");
	printf(" | ����:");
	if (show_image)	printf("[ON]");
	else printf("[OFF]");
	printf("\n�뽫���뷨�л���Ӣ��\n");
	printf("������ʾ�����������ʼ...");
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
		if (len_int < 10 || len_int >= 10000)	printf("| [%3d%%] %d / %d\n", i * 100 / len_int, i, len_int);  //%3dΪ[  1%] %03dΪ[001%]
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
					if (space[0][igetcol] == '$')	loop = 1;  //����$�����������
					if (space[0][igetcol] == '\n')	loop = 1;  //�������������������
					if (space[0][igetcol] == '#')	space[0][igetcol] = ' ';  //����#�򵱳ɿո�
				}
			}
		}
		else	strcpy(space[0], "    ");
		for (int iprint = 0; iprint <= 19; iprint++)	printf("%c %c %c %c %c |\n", head_char[iprint], space[iprint][0], space[iprint][1], space[iprint][2], space[iprint][3]);
		printf("__%c_%c_%c_%c__________________\n", warn_char[0], warn_char[1], warn_char[2], warn_char[3]);
		for (int ichk = 0; ichk <= 3; ichk++)  //����20���Ƿ�����© �Դ��ж�LOST
		{
			if (space[19][ichk] != ' ')
			{
				lost++;
				warn_span[ichk] = iloop;  //������ڸ��������ɺ� ����һ�ֲŻᷴӳ����� �Ӷ���֤�����뿪���������ʾLOST����
			}
		}
		for (int iwarn = 0; iwarn <= 3; iwarn++)  //��Ȼ��1/2�ж��ߵ������ڵ�20��֮ǰ��Ӧ����ΪLOST ��������Լ���20������ ֻҪ�������㹻��(�������ĵ�Ҫ��)����׼ȷͳ��
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
			fflush(stdin);	//getchǰӦ�ò�����fflush�������̽��������ʱ��
			switch (in)	 //�߳����ж�����
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
	system(color_set);	//�ָ���ɫ
	consuming = (t_end - t_start) / len_int;
	fflush(stdin);
	system("cls");
	moi_u();
	printf("*|---UPRIGHT DONE---|*\n");
	printf("*|��׼����:%04d ms\n", consuming);
	printf("------------------------------------------------------------\n");
	printf("�÷�:%07d\n", score);
	printf("�ȼ�:");
	if (score < 200000)	printf("F  [�ٽ�����]");
	if (score >= 200000 && score < 350000)	printf("D  [��������]");
	if (score >= 350000 && score < 500000)	printf("C  [��������]");
	if (score >= 500000 && score < 650000)	printf("B  [��������]");
	if (score >= 650000 && score < 800000)	printf("A  [��������]");
	if (score >= 800000 && score < 920000)	printf("S  [�߶�����]");
	if (score >= 920000 && score < 1000000)	printf("V  [�߶�����]");
	if (score == 1000000)	printf("EX [��������]");
	if (score > 1000000)	printf("<Error> Level Undefined [��������*]");
	printf("\nPERFECT %d\n", clear);
	printf("LOST    %d\n", lost);
	if (clear == note_int)	printf("[FULL COMBO / ALL PERFECT]\n");
	printf("------------------------------------------------------------\n");
	printf("�������:%d\n", click);
	if (click != 0)	printf("�������:%.2lf%%\n", clear_double / click * 100);
	printf("��Ч�������:%.2f Note(s) per Minute\n", clear_double / t * 60000);
	printf("��ʱ:%.3lf Second(s) [%d ms - %d ms]\n", t*0.001, t_end, t_start);
	printf("------------------------------------------------------------\n");
	printf("������Ŀ:%s\n", item);
	printf("��׼����:%d\n", consuming);
	printf("����У��:%d\n", pause);
	actual_speed_double = t / len_double * 0.001;
	printf("����:%d\n", len_int);
	printf("ÿ��ʱ��:%0.3lf Second(s) per Line", actual_speed_double);
	printf("\nĿ��ÿ��ʱ��:%.3lf Second(s) per Line\n", target_speed * 0.001);
	printf("������:%d\n", note_int);
	printf("��������:%.3lf Note(s) per Minute\n", note_double / t * 60000);
	printf("------------------------------------------------------------\n");
	printf("�Ƿ��¼����У������?[Y/N]");
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
		printf("\nд��ɹ�\n");
		printf("�����������...");
		fflush(stdin);
		_getch();
		fflush(stdin);
	}
	return consuming;
}