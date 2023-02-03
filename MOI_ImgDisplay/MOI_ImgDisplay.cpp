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
		fgetc(fconfig);	 //跳过第1位 背景色-Launcher
		fgetc(fconfig);	 //跳过第2位 前景色-Launcher
		fgetc(fconfig);	 //跳过第3位 背景色-Main
		fgetc(fconfig);	 //跳过第4位 前景色-Main
		color_set[6] = fgetc(fconfig);	 //第5位 背景色-Adventure
		color_set[7] = fgetc(fconfig);	 //第6位 前景色-Adventure
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