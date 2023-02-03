#define _CRT_SECURE_NO_WARNINGS
#include"ImgDisplay.h"
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<conio.h>

const char* CONFIG_DATA = "MOI_Config.ini";

int main(int argc, char* argv[])
{
	char color_set[9] = "color 0f";
	FILE *fconfig = NULL;
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
	switch (argc)
	{
	case 1:
		ImgDisplay_Wipe("MOI_Welcome.bmp", "Melody Of Involution");
		break;
	case 2:
		ImgDisplay_Wipe(argv[1], "Melody Of Involution");
		break;
	case 3:
		ImgDisplay_Wipe(argv[1], argv[2]);
		break;
	case 4:
		if (!strcmp(argv[3], "Still"))	ImgDisplay_Still(argv[1], argv[2]);	 //Still
		else	ImgDisplay_Wipe(argv[1], argv[2]);	//Wipe
		break;
	default:
		ImgDisplay_Wipe(argv[1], argv[2]);
		break;
	}
	_getch();
	fflush(stdin);
	return 0;
}