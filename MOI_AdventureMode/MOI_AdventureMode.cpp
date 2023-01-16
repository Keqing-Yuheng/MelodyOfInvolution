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
		fgetc(fconfig);	 //������1λ ����ɫ-Launcher
		fgetc(fconfig);	 //������2λ ǰ��ɫ-Launcher
		fgetc(fconfig);	 //������3λ ����ɫ-Main
		fgetc(fconfig);	 //������4λ ǰ��ɫ-Main
		color_set[6] = fgetc(fconfig);	 //��5λ ����ɫ-Adventure
		color_set[7] = fgetc(fconfig);	 //��6λ ǰ��ɫ-Adventure
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
	char c_front[3] = { '>', ' ', ' ' }, c_back[3] = { '<', ' ', ' ' }, draw[21] = "                    ";	 //draw:20�ַ�;
	int location = 0, n = 0;  //locationΪ���������0��ʼ returnΪ��+1��1��ʼ
	long long exp_ll;
	FILE *fexp = NULL;
	while (1)
	{
		loop = 1;
		system("cls");
		moi_a();
		printf("\n[�ȼ���Ϣ]\n");
		fexp = fopen(EXP_DATA, "r");
		if (fexp == NULL)
		{
			printf("δ�ҵ��������� ����\"�����¼\"�Դ���\n"
				   "�����������..."
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
				   "����ֵ����|"
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
			"ʹ��W/S�������ƶ�ѡ����\"> <\"\n"
			"Ȼ��ʹ�ÿո��ѡ��\n"
			"�������Ŀǰ�������ִ�ж�Ӧ����\n"
			"\n"
			" %c [1]ð�չ��� %c\n"
			" %c [2]�����¼ %c\n"
			" %c [0]�˳����� %c\n"
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
				n = n >= 2 ? 0 : n + 1;	//ѭ������
				c_front[n] = '>';
				c_back[n] = '<';
				for (location = 0; c_front[location] != '>'; location++){}	//�˴���ʵ������ �ı�locationֵ
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
		if (i == MAX_ID)	return -1;	//reg_id���ݲ��Ϸ�
	}
	fexp = fopen(EXP_DATA, "r");
	if (fexp == NULL)	return -2;	//�ļ���ʧ��
	fgets(exp_str, 21, fexp);  //��Ҫʹ��������˽���޸� ����ܲ��ڴ˴�֮�����
	exp_ll = atoll(exp_str);  
	fclose(fexp);  //�ر��ļ���׼���л��򿪷�ʽ
	exp_ll = exp_ll < 0 ? 0 : exp_ll;
	exp_ll = exp_ll + score_ll;
	fexp = fopen(EXP_DATA, "w+");
	if (fexp == NULL)	return -2;	//�ļ�д��ʧ��
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
		printf("ð��ģʽ:����\n"
			"����M.O.I.�����ļ� ��������·��\n"
			"Ȼ��Enter�Խ���\n"
			"(����*�г������������ | ����*exit����)\n"
			);
		scanf("%[^\n]", item);
		if (item[0] == '*')	 //��������Ϊ���
		{
			if (!strcmp(item, "*exit"))	return;
			fp = fopen(INDEX_ADVENTURE, "r");
			if (fp == NULL)
			{
				printf("δ�ҵ������ļ�:%s!\n"
					   "���������������\n"
					   "�����������..."
					  , INDEX_ADVENTURE);
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
		reader(item);  //�������·��
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
	printf("ð��ģʽ:����\n");
	fexp = fopen(EXP_DATA, "r");
	if (fexp == NULL)
	{
		printf("δ�ҵ��������� ����\"�����¼\"�Դ���\n"
			   "�����������..."
			   );
		_getch();
		fflush(stdin);
		return;
	}
	if (item[0] == '\"'&&item[len - 1] == '\"')  //��������Ĵ��ո�·�� ���������ַ���
	{
		for (int imov = 1; imov < len; imov++)	item[imov - 1] = item[imov];  //ǰ��1λ�Ը��ǿ�ͷ������
		item[len - 2] = 0;	//ɾȥ��ǰ�Ƶ�ĩβ����
	}
	fp = fopen(item, "r");
	if (fp == NULL)
	{
		printf("����:δ�ҵ�����:%s!\n"
			   "�����������..."
			  , item);
		_getch();
		fflush(stdin);
		return;
	}
	fscanf(fp, "%[^\n]", fin_str);
	printf("����:%s", fin_str);
	fscanf(fexp, "%lld", &exp_ll);
	fscanf(fp, "%d", &fin_int);
	if (fin_int > exp_ll / EXP_PER_LV)
	{
		printf("�ȼ������Բ鿴�þ���!\n"
			"��ǰ%d | ��Ҫ%d\n"
			"�����ɹؿ��Ի��۾���\n"
			"�����������..."
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
		case '\n':	//�Ի���һ��Ϊһ����λ ����\n��׼��������һ����λ
			printf("\n\n--------------------------------------------------\n\n");
			fflush(stdin);	//��ֹ���������ֲ��ϳ���
			_getch();
			continue;
		case '\\':	//������\��ͷ
			fin_char = fgetc(fp);  //�жϾ��幦��
			switch (fin_char)
			{
			case '!':  //\!+1λ��+��һ��ͼƬ·������+���ͼƬ
				height = fgetc(fp) - '0';
				fscanf(fp, "%s", fin_str);
				ImgDisplay_Wipe(fin_str, "#DEFAULT");
				for (int iprint = 0; iprint <= height; iprint++)	putchar('\n');
				continue;
			case ':':  //\:���ڷָ��������뽲������
				printf(":\n");
				continue;
			case '|':  //\|���ڻ���
				printf("\n");
				continue;
			default:  //����\Ŀ�� ������Ҫ���� �������\�ַ�
				printf("\\");
				continue;
			}
		default:  //���
			putchar(fin_char);
			continue;
		}
		Sleep(10);	//������һ��һ����ʾ
	}
	printf("\n--------------------------------------------------\n"
		   "������� �����������..."
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
		printf("δ�ҵ����������ļ�!\n"
			   "�Ƿ񴴽��ļ�?[Y/N]"
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
				printf("�ļ�����ʧ��!\n"
					   "�����������..."
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
		   "��ǰ����:%lld\n"
		   "����<1/2> ����Ҫ��¼�ĳɼ�ֵ\n"
		   "ע��:����ֵ����Ϊ2^63-1\n"
		  ,exp_ll);
	scanf("%s", in_str);
	fflush(stdin);
	printf("����<2/2> ��ʹ��%s��Ϊ��¼ID\n"
		   "�������ȷ��..."
		   ,id_llist[1]);
	_getch();
	fclose(fexp);
	r = registry(in_str, (char*)id_llist[1]);
	switch (r)
	{
	case -1:
		printf("\n����:�Ƿ�����(%lld)\n", r);
		break;
	case -2:
		printf("\n����:�ļ���дʧ��(%lld)\n", r);
	default:
		printf("\nִ�гɹ�(%lld)\n", r);
		break;
	}
	printf("�����������...");
	_getch();
	fflush(stdin);
}