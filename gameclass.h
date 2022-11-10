#pragma once
#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//宏定义
#define PLAYER1 true
#define PLAYER2 false

#define CITY 1
#define COUNTRYSIDE 2
#define MOUNTAIN 3
#define NOPEOPLESLAND 4

//函数宏定义
#define VKISDOWN(vkey) (GetAsyncKeyState(vkey)&0x8000)//判断按键是否被按下

class ying
{
public:
	int numberying;
	int soldier;
	int morale;
	int fire; 
	float x;
	float y;
	ying();
	
};
//把每一个军种看为军队组
//警告：此类内存泄漏概率较大，使用时记得回收内存
class Armygroup
{
public:
	unsigned char number;//军队组里的军队数量，或者认为是等级
	unsigned short serial_number;//军队序号
	ying ** grouppoint;
	float x;//X坐标
	float y;//y坐标
	float speed;	//提醒自己要设置速度*****************************************************************************************************************
	short movex;//根据目的地计算的向量的x
	short movey;//根据目的地计算的向量的y
	float moveunitx;//一次移动的x距离
	float moveunity;//一次移动的y距离
	int tox,toy;//目的地
public:
	Armygroup(ying *a1);
	Armygroup(Armygroup *a1,Armygroup *a2,Armygroup *a3,Armygroup *a4);//tuan和lv和shi（四进制）
	~Armygroup();
	bool move();
    void setdestination(short,short);
};

class PLACE//在移动箭头的时候用
{
public:
	//地区归属者
	bool belong;//地区归属者，true为p1,false为p2
	//地区种类
	char kind;//地区种类,1为城市，2为乡村，3为山地，4为无人区
	//军队
	Armygroup ** army;//用于保存地区所含有军队
	ying ** other;//大于十五的放在这里


	PLACE();	//构造函数
	void setarmypoint(Armygroup *t);
	Armygroup* LookNumberArmy(char number);
	~PLACE();	
};


class ARROW
{
public:
	int x;
	int y;
	
	IMAGE *image;
	PLACE *place;
	
	IMAGE *maskarrow;//箭头图片
	int tox,toy;//目的地
	ARROW(IMAGE *image,IMAGE *mask);
	void SetPointPlace(PLACE *t);
	void setdestination(PLACE *t);//设置目的地
	void draw();
};

class player
{
public:
	bool number;
	ARROW* arrow;
	int	MilitaryExploit;//军功
	int Money;			//钱
	byte post;			//职务（代号），（为了省内存只存一个字节的数字代替），3营，4团，5旅，6师，7军
	
};

class myqueue
{
public:
	Armygroup** armyqueue;
	myqueue* queuenext;
	myqueue();
	void addqueue(Armygroup* t);
	void movearmy();
	void draw(IMAGE *army,player *p);//移动之后刷新图片
	~myqueue();
};

PLACE ***PixelPointer;	//用于每一个像素指向的地区指针，主要用于检测士兵所在位置，放到堆区里是因为放到栈区里会报错

//图片全局变量
IMAGE background;//存放背景图片
IMAGE p1armyimage,p2armyimage;//存放军队图片
IMAGE p1image,p2image;//
IMAGE arrowimage1,arrowimage2;//箭头图片
IMAGE trectangle(600,75);//一个临时图片

//部分全局变量
unsigned short sumtuan=0;//tuan的总数目
unsigned short sumlv=0;//lv的总数目
unsigned short sumshi=0;//shi的总数目
unsigned int sumying=0;//ying 的总数目
int ERRORCODE;//一些可能出错的东西，ERRORCODE报错放到ERRORCODE_intruction.txt

//线程参数结构体
typedef struct threadstruct
{
	player *tp1;
	player *tp2;
	IMAGE *tbackground;
	IMAGE *tp1armyimage,*tp2armyimage;
	IMAGE *tp1image,*tp2image;
	IMAGE *tarrowimage1,*tmaskarrow,*tarrowimage2;
	myqueue * tqueuemovep1;
	myqueue * tqueuemovep2;
	ARROW * tarrow1;
	ARROW * tarrow2;
	
}THREADSTRUCT;

