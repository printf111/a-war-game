#pragma once
#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
DWORD WINAPI threadfuction(LPVOID Lthreadstruct);
DWORD WINAPI threadfuction2(LPVOID Lthreadstruct);



DWORD WINAPI threadfuction(LPVOID Lthreadstruct)//刷新屏幕的线程函数
{
	THREADSTRUCT* threadstruct = (THREADSTRUCT *)Lthreadstruct;
	threadstruct=(THREADSTRUCT *)threadstruct;
	while(1)
	{
		char out[30]="帧率";
		long t1=clock();
		threadstruct->tqueuemovep1->movearmy();
		threadstruct->tqueuemovep1->draw(&p1armyimage,threadstruct->tp1);
		threadstruct->tqueuemovep2->movearmy();
		threadstruct->tqueuemovep2->draw(&p2armyimage,threadstruct->tp2);
		//threadstruct->tarrow1->draw();
		long t2=clock();
		if(t2-t1<=33)//控制帧率为定值
		{
			Sleep(33-(t2-t1));
		}
		/*输出帧率部分*/
		char fps[3];
		itoa(clock()-t1,fps,10);
		strcat(out,fps);
		outtextxy(600,60,out);
		/*输出P1属性位置*/
		
		/*输出p2属性位置*/
	}
}

DWORD WINAPI threadfuction2(LPVOID Lthreadstruct)//侦测键盘线程函数
{
	THREADSTRUCT* threadstruct = (THREADSTRUCT *)Lthreadstruct;
	threadstruct=(THREADSTRUCT *)threadstruct;
	bool ischange1=false;
	bool ischange2=false;
	while(1)	
	{
		if(VKISDOWN('W') && (threadstruct->tarrow1->y-25+30>=0))
		{
			threadstruct->tarrow1->y-=25;
			ischange1=true;
		}
		if(VKISDOWN('S') && (threadstruct->tarrow1->y+25+30<=600))//减/加是要留出图片位置，箭头实际位置是在图片左上角
		{
			threadstruct->tarrow1->y+=25;	
			ischange1=true;
		}
		if(VKISDOWN('A') && (threadstruct->tarrow1->x-25+15>=0))
		{
			threadstruct->tarrow1->x-=25;
			ischange1=true;
		}
		if(VKISDOWN('D') && (threadstruct->tarrow1->x+25+15<=600))
		{
			threadstruct->tarrow1->x+=25;
			ischange1=true;
		}
		if(ischange1)
		{
			threadstruct->tarrow1->SetPointPlace(PixelPointer[threadstruct->tarrow1->x+15][threadstruct->tarrow1->y+30]);
			ischange1=false;
			Sleep(150);//p1操作
		}
		if(VKISDOWN(VK_UP) && (threadstruct->tarrow2->y-25+30>=0))
		{
			threadstruct->tarrow2->y-=25;
			ischange2=true;
		}
		if(VKISDOWN(VK_DOWN) && (threadstruct->tarrow2->y+25+30<=600))//减/加是要留出图片位置，箭头实际位置是在图片左上角
		{
			threadstruct->tarrow2->y+=25;
			ischange2=true;
		}
		if(VKISDOWN(VK_LEFT) && (threadstruct->tarrow2->x-25+15>=0))
		{
			threadstruct->tarrow2->x-=25;
			ischange2=true;
		}
		if(VKISDOWN(VK_RIGHT) && (threadstruct->tarrow2->x+25+15<=600))
		{
			threadstruct->tarrow2->x+=25;
			ischange2=true;
		}
		if(ischange2)
		{
			threadstruct->tarrow2->SetPointPlace(PixelPointer[threadstruct->tarrow2->x+15][threadstruct->tarrow2->y+30]);
			ischange2=false;
			Sleep(150);//p1操作
		}	
	}
}
