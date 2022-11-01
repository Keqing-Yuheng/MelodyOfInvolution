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
	//���ö�ȡ
	//Config
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
		fgetc(fconfig);	 //������1�е�\n �Ի�ȡ��2�еĲ�����Ŀ¼
		fscanf(fconfig, "%[^\n]", player);
		if (!strcmp(player, "#NULL"))	memset(player,0,sizeof(player));
		fclose(fconfig);
	}
	system(color_set);
	//��׼����
	//Speed Upright
	system("cls");
	fupright = fopen(UPRIGHT_DATA, "r");
	while (fupright == NULL)
	{
		system("cls");
		moi_brief();
		printf("��ӭ����Melody Of Involution!\n");
		printf("�״�������Ҫ����һЩ����\n");
		printf("����������-1���Զ���ɻ�׼���ʲ���\n");
		printf("Ҳ���Լ���[0,9999]�е�����ֵ���ֶ����û�׼����(��msΪ��λ)\n");
		scanf("%d", &in_int);
		fflush(stdin);
		if (in_int < -1 || in_int>9999)
		{
			printf("����:�Ƿ�����:%d\n", in_int);
			printf("��׼����ֵδ����\n");
			printf("��ǰ��׼����:%d ms\n", consuming);
			printf("��Ҫ�ٴ����� ��������������\n");
			printf("�����������...");
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
				printf("����:δ�ҵ���׼���ٲ��Գ���:%s\n", UPRIGHT_EXE);
				printf("��׼����ֵδ����\n");
				printf("��ǰ��׼����:%d ms\n", consuming);
				printf("���������������������\n");
				printf("��Ҫ�ٴ����� ��������������\n");
				printf("�����������...");
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
		printf("���óɹ�:%d\n", consuming);
		printf("�Ƿ�д���ļ�?[Y/N]\n");
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
			printf("\nд��ɹ�\n");
			printf("�����������...");
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
	//�������
	//Environment Check
	system("cls");
	moi_brief();
	printf("���ڼ�⻷��...\n");
	if (IsDebuggerPresent()*cheat_disabled)
	{
		environment_error = 1;
		printf("<Info>[�����쳣:��⵽������]\n");
		printf("����ʱ���õ��������ڽ�������ע\n");
	}
	if (!system("tasklist|findstr \"HipsDaemon.exe\""))
	{
		environment_error = 1;
		printf("<Warn>[�����쳣:��⵽���ް�ȫ�������]\n");
		printf("���������ٶȽ��ή��\n");
	}
	if (!system("tasklist|findstr \"360tray.exe\""))
	{
		environment_error = 1;
		printf("<Warn>[�����쳣:��⵽360��ȫ��ʿ����]\n");
		printf("���������ٶȽ��ή��\n");
	}
	if (!system("tasklist|findstr \"360sd.exe\""))
	{
		environment_error = 1;
		printf("<Warn>[�����쳣:��⵽360ɱ������]\n");
		printf("���������ٶȿ��ܽ���\n");
	}
	if (environment_error == 1)
	{
		printf("------------------------------------------------------------\n"
		       "����:��⵽���������쳣\n"
		       "Ϊ��֤��Ϸ���� �뾡���޸�\n"
			   "\n"
			   "*ע��:MOI�������ж������\n"
			   "      �ر�ɱ�������Ŀ����Ϊ�˱���������ʧ��������ʴ���½�\n"
			   "      ����ɱ�����ͬʱ������Ѷ��׿��ܳ�������У��ʧ��\n"
			   "      Ҳ��ʹ��Windows Defender/MSE�ȼ�����Ӱ�����ʵ�ɱ�����\n"
		       "�����������..."
			  );
		fflush(stdin);
		_getch();
		fflush(stdin);
	}
	//������
	//Home
	do
	{
		system("cls");
		moi_full();
		printf("\n--------------------------------------------------\n\n");
		printf("[������Ϣ]\n");
		printf("��׼����:%d ms per Line\n", consuming);
		printf("���ֲ�����:%s", player);
		if (!strlen(player))	printf("[Ĭ��]");
		printf("\n");
		printf("��Ƶ:");
		if (show_audio)	printf("[ON]");
		else printf("[OFF]");
		printf(" | ����:");
		if (show_image)	printf("[ON]");
		else printf("[OFF]");
		printf("\n\n--------------------------------------------------\n\n");
		printf("����M.O.I.��Ŀ�ļ�(*.txt) �������Ŀ�ļ�·��\n"
			   "Ȼ��Enter�Խ���\n"
			   "(����*�г������������)\n"
			  );
		scanf("%[^\n]", item);  //����
		fflush(stdin);
		len = strlen(item);
		if (item[0] == '*')	 //��������Ϊ���
		{
			bool found = 0;
			FILE *fp = NULL;
			fp = fopen(INDEX_PLAY, "r");
			if (fp == NULL)
			{
				printf("δ�ҵ������ļ�:%s!\n"
					"���������������\n"
					"�����������..."
					, INDEX_PLAY);
				_getch();
				fflush(stdin);
				continue;
			}
			if (!strcmp(item, "*"))  //�г�����
			{
				while (!feof(fp))
				{
					fgets(item, 129, fp);
					fputs(item, stdout);
					memset(item, 0, sizeof(item));
				}
				printf("\n�����������...");
				fclose(fp);
				_getch();
				fflush(stdin);
				continue;
			}
			while (!feof(fp))  //��������
			{
				memset(item, 0, sizeof(item));
				fscanf(fp, "%[^\n]", item);		  //������1��Ϊ��� �ڶ���Ϊ·��
				if (!strcmp(item, item))			  //ʶ��ÿһ�� ֱ������Ҫ�ҵ����
				{
					found = 1;
					break;
				}
			}
			if (!found)
			{
				printf("��Ч���\n"
					"�����������..."
					);
				fclose(fp);
				_getch();
				fflush(stdin);
				continue;
			}
			else
			{
				fscanf(fp, "%[^\n]", item);			  //�ٶ�һ�о��Ƕ�Ӧ·��
				fclose(fp);
			}
		}  //���ʶ�����
		if (item[0] == '$' && strcmp(item,"$$"))
		{
			int t_start, t_end, result;
			printf("-- MOI Terminal --\n");
			printf("MOI>");
			scanf("%[^\n]", item);
			fflush(stdin);
			printf("����:%s\n", item);
			if (!strcmp(item, "$back"))	continue;
			if (!strcmp(item, "$exit"))	return 0;
			if (!strcmp(item, "$get-info"))
			{
				printf("����·��:%s\n", _pgmptr);
				printf("������ɫ:%c(����) | %c(ǰ��)\n", color_set[6], color_set[7]);
				printf("��ǰʱ��:%d\n", time(0));
				printf("��׼����:%d\n", consuming);
				printf("�������ٲ���:+ %d ms per Line\n", internal_upright);
				printf("�����������...");
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
					printf("MOI:����У�������ļ���ɾ��\n");
					printf("�����������...");
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
					printf("MOI:��׼���ٸ���Ϊ:%d ms per Line\n", consuming);
					printf("�Ƿ�д���ļ�?[Y/N]\n");
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
						printf("д��ɹ�\n");
					}
					printf("�����������...");
					fflush(stdin);
					_getch();
					fflush(stdin);
					continue;
				}
			}
			printf("--��ʼ����--\n\n");
			t_start = clock();
			result = system(item);
			t_end = clock();
			printf("\n--��������--\n");
			printf("��ʼ = %d\n", t_start);
			printf("���� = %d\n", t_end);
			printf("��ʱ = %dms = %.3fs\n", t_end - t_start, (t_end - t_start)*0.001);
			printf("���ش��� = %d\n\n\n", result);
			printf("�������Ȼ��Enter�Է���...");
			fflush(stdin);
			getchar();
			fflush(stdin);
			system("cls");
			continue;
		}
		if (!strcmp(item, "$$"))
		{
			printf("�ù������ڿ����� �����ڴ�...");
			fflush(stdin);
			_getch();
			fflush(stdin);
			continue;
		}
		if (item[0] == '\"'&&item[len - 1] == '\"')  //��������Ĵ��ո�·�� ���������ַ���
		{
			for (int imov = 1; imov < len; imov++)	item[imov - 1] = item[imov];  //ǰ��1λ�Ը��ǿ�ͷ������
			item[len - 2] = 0;	//ɾȥ��ǰ�Ƶ�ĩβ����
		}
		fitem = fopen(item, "r");
		if (fitem == NULL)
		{
			printf("����:δ�ҵ�:%s!\n", item);
			printf("����ļ�·���Ƿ���ȷ���Ƿ��пո�\n");
			printf("�������Ȼ��Enter���˳�...");
			getchar();
			return 0;
		}
		else
		{
			printf("��Ŀ:%s\n", item);
			fclose(fitem);
			play(item, audio_config);
		}
		printf("�������Ȼ��Enter���˳�...");
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
	moi_brief();
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
		moi_brief();
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
	printf("*|�÷�:%07d\n", score);
	printf("*|�ȼ�:");
	if (score < 200000)	printf("F  [�ٽ�����]");
	if (score >= 200000 && score < 350000)	printf("D  [��������]");
	if (score >= 350000 && score < 500000)	printf("C  [��������]");
	if (score >= 500000 && score < 650000)	printf("B  [��������]");
	if (score >= 650000 && score < 800000)	printf("A  [��������]");
	if (score >= 800000 && score < 920000)	printf("S  [�߶�����]");
	if (score >= 920000 && score < 1000000)	printf("V  [�߶�����]");
	if (score == 1000000)	printf("EX [��������]");
	if (score > 1000000)	printf("<Error> Level Undefined [��������*]");
	printf("\n*|PERFECT %d\n", clear);
	printf("*|LOST    %d\n", lost);
	if (clear == note_int)	printf("[FULL COMBO / ALL PERFECT]\n");
	printf("------------------------------------------------------------\n");
	printf("�������:%d\n", click);
	if (click != 0)	printf("�������:%.2lf%%\n", clear_double / click * 100);
	printf("��Ч�������:%.2f Note(s) per Minute\n", clear_double / t * 60000);
	printf("��ʱ:%.3lf Second(s) [%d ms - %d ms]\n", t*0.001, t_end, t_start);
	printf("------------------------------------------------------------\n");
	printf("��Ŀ:%s\n", item);
	printf("��׼����:%d\n", consuming);
	printf("����У��:%d\n", pause);
	actual_speed_double = t / len_double * 0.001;
	printf("����:%d\n", len_int);
	printf("ÿ��ʱ��:%0.3lf Second(s) per Line", actual_speed_double);
	if (int(actual_speed_double * 1000) < target_speed - 200)	printf(" [ƫ��]");
	if (upright_failed)	printf("[����У��ʧ��]");
	printf("\nĿ��ÿ��ʱ��:%.3lf Second(s) per Line\n", target_speed * 0.001);
	printf("������:%d\n", note_int);
	printf("��������:%.3lf Note(s) per Minute\n", note_double / t * 60000);
	printf("[У����Ϣ]--------------------------------------------------\n");
	printf("��ǰʱ��:%d\n", time(0));
	printf("�Ƿ��⵽������:%d\n", !constant_no_debugger * cheat_disabled);
	r = system(chk);
	if (r)	printf("����:У��δִ��(���ش���:%d)\n", r);
	printf("------------------------------------------------------------\n");
	switch (!constant_no_debugger * cheat_disabled)
	{
	case 0:
		printf("�Ƿ��¼ð��ģʽ����ֵ?[Y/N]");
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
				printf("��¼ʧ��!\n");
				break;
			default:
				printf("��¼�ɹ�!\n");
				printf("�÷�:%s (%lld)\n", score_str, r);
				break;
			}
		}
	}
	printf("�������Ȼ��Enter���˳�...");
	getchar();
}