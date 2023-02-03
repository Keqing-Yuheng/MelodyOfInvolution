#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _STDIO_H_
#define _STDIO_H_
#include<cstdio>
#endif

#ifndef _STRING_H_
#define _STRING_H_
#include<cstring>
#endif

#ifndef _CONIO_H_
#define _CONIO_H_
#include<conio.h>
#endif

#ifndef _WINDOWS_H_
#define _WINDOWS_H_
#include<windows.h>
#endif

#ifndef __IMGDISPLAY_H__
#define __IMGDISPLAY_H__
#include"ImgDisplay.h"
#endif

int ImgDisplay_Still(char* img_path, char* set_title)
{
	char window_title[255] = {};
	FILE *fp = NULL;
	if (strcmp(set_title, "#DEFAULT_TITLE"))	SetConsoleTitleA(set_title);
	fp = fopen(img_path, "r");	//打开位图文件
	if (fp == NULL)	return -1;	//文件打开失败
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	LPBITMAPINFOHEADER bi = (LPBITMAPINFOHEADER)malloc(size);
	if (!bi)	return -2;	//内存分配失败
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	fread(bi, 1, size, fp);
	GetConsoleTitleA(window_title, 255);
	HDC dc = GetDC(FindWindowA(0, window_title));
	SetDIBitsToDevice(dc, 0, 15, bi->biWidth, bi->biHeight, 0, 0, 0, bi->biHeight, bi + 1, (LPBITMAPINFO)bi, 0);	 //8,0让出第一列 0,15让出第一行
	return 0;
}

int ImgDisplay_Wipe(char* img_path, char* set_title)
{
	FILE* fp;
	char* buf;  //文件读取缓冲区
	char *p;    //死循环辅助指针
	int r, g, b;  //RGB
	DWORD w, h;   //读取图像长宽
	WORD biBitCount;  //每像素位深度，其值为1、4、8、16、24、或32
	BITMAPFILEHEADER bf;  //文件头
	UINT bftype;  //图像类型
	BITMAPINFOHEADER bi;  //文件头信息
	if (strcmp(set_title, "#DEFAULT_TITLE"))	SetConsoleTitleA(set_title);
	fp = fopen(img_path, "rb");	 //打开位图文件
	if (fp == NULL)	return -1;	//文件打开失败
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);  //读取BMP头文件
	bftype = bf.bfType;
	if (bftype != 0x4D42)	return -3;	//文件格式错误
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);  //读取BMP头文件信息
	w = bi.biWidth;  //宽
	h = bi.biHeight;  //高 若为正则图像为倒向 若为负则图像为正向
	biBitCount = bi.biBitCount; //获取图像位深度
	if (biBitCount != 24)	return -4;  //位深度错误 若位深度为24则表示位图为1670万色 在位数组中每3个字节(RGB)代表一个像素
	buf = (char*)malloc(w * h * 3); //分配缓冲区大小 在位数组中每3个字节代表一个像素
	fseek(fp, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);  //定位到位图数据像素起始位置
	fread(buf, 1, w * h * 3, fp);  //开始读取
	HWND consoleWindow = GetConsoleWindow();  //获取窗口句柄
	HDC dc = GetDC(consoleWindow);
	int x = 0;  //腾出位置
	int y = 15;  //腾出位置
	do
	{
		p = buf;
		for (DWORD j = 0; j < h; j++)
		{
			for (DWORD i = 0; i < w; i++)
			{
				b = *p++;
				g = *p++;
				r = *p++;
				SetPixel(dc, x + i, y + h - j, RGB(r, g, b));    //  图像倒向
			}
		}
	} while (0);
	fclose(fp);
}