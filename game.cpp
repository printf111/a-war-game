#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "polygon.h"
/*
			35排
			3*35=105连
4*105=420营
4*420=1260团
4*1260=5040旅
4*5040=20160师
*/
/*
	注意：本程序使用了itoa函数
*/
//宏定义
#define PLAYER1 true
#define PLAYER2 false

#define CITY 1
#define COUNTRYSIDE 2
#define MOUNTAIN 3
#define NOPEOPLESLAND 4

//函数宏定义
#define VKISDOWN(vkey) (GetAsyncKeyState(vkey)&0x8000)//判断按键是否被按下

//部分全局变量
unsigned short sumying=0;//ying的总数目
unsigned short sumtuan=0;//tuan的总数目
unsigned short sumlv=0;//lv的总数目
unsigned short sumshi=0;//shi的总数目

//图片全局变量
IMAGE background;//存放背景图片
IMAGE p1armyimage,p2armyimage;//存放军队图片
IMAGE p1image,p2image;//
IMAGE arrowimage1,maskarrow,arrowimage2;//箭头图片
IMAGE trectangle(600,75);//一个临时图片

void transparentimage(int x, int y, IMAGE *srcimg, IMAGE *maskimg);
void Workoutmoveunit(float *moveunitx, float *moveunity , short movex,short movey,float speed);

class PLACE;//声明一个类，因为我需要将place指向ying
PLACE ***PixelPointer;	//用于每一个像素指向的地区指针，主要用于检测士兵所在位置，放到堆区里是因为放到栈区里会报错

class ying
{
public:
	int numberying;
	int soldier;
	int morale;
	int fire; 
	 float x;
	 float y;
	float speed;	//提醒自己要设置速度*****************************************************************************************************************

	short movex;//根据目的地计算的向量的x
	short movey;//根据目的地计算的向量的y
	float moveunitx;//一次移动的x距离
	float moveunity;//一次移动的y距离

	int tox,toy;//目的地
	ying()
	{
		this->numberying=sumying+1;
		sumying++;
		this->soldier=35;
		this->morale=100;
		this->fire=50;
		this->x=30;	
		this->y=30;
		movex=-9999;//假设这是负无穷
		movey=-9999;//假设这是负无穷
		speed=1;

	}
	void setdestination(short dx,short dy)
	{
		this->tox=dx;
		this->toy=dy;
	}
	bool move()//返回值是确定是否到地方
	{
		/*
			移动军队
		*/

		if( ( (this->movex) < -600) && ( (this->movey) < -600) )//如果是第一次移动
		{
			movex=tox-x;
			movey=toy-y;
			Workoutmoveunit(&moveunitx,&moveunity,movex,movey,speed);//计算单位移动距离/方向（向量）
		}
		if( ( abs(abs(this->x)-abs(tox)) <= abs(moveunitx) )   ||  	( abs(abs(this->y)-abs(toy)) <= abs(moveunity) ))//加这个if是为了下面的else
		{
			if( ( abs(abs(this->x)-abs(tox)) <= abs(moveunitx) )    )//如果没法斜着移动，快到地方了
			{
				this->x=tox;
				moveunity=speed;//x不需要移动了，就重新设置一下y，不然只移动y，相当于和速度变慢
	
			}
			if(	( abs(abs(this->y)-abs(toy)) <= abs(moveunity) )    )//分开写是为了纠正误差，因为移动的都是整数，所以可能斜着移动的时候不能直接到达目的地
			{
				this->y=toy;
				moveunitx=speed;//y不需要移动了，就重新设置一下x，不然只移动y，相当于和速度变慢
			}
		}
		else
		{
			this->x+=moveunitx;
			this->y+=moveunity;
		}



		if(x<=0) x=0;
		if(x>=590) x=590;
		if(y<=0) y=0;
		if(y>=590) y=590;



		if( (this->x == tox)   &&  (this->y == toy) )//如果到地方了
		{
			return true;
		}

		return false;
	}

};

//把每一个军种看为军队组
//警告：此类内存泄漏概率较大，使用时记得回收内存
class Armygroup
{
public:
	unsigned char number;//军队组里的军队数量，或者认为是等级
	ying ** grouppoint;

public:
	float x;//X坐标
	float y;//y坐标

	Armygroup(ying *a1)//只有一个ying军队时
	{
		number = 1;
		grouppoint=new ying*[1];
		grouppoint[0]=a1;
		x=a1->x;
		y=a1->y;
	}
	Armygroup(ying *a1,ying *a2,ying *a3,ying *a4)//4个ying是tuan
	{
		number = 4;
		grouppoint=new ying*[4];
		grouppoint[0]=a1;
		grouppoint[1]=a2;
		grouppoint[2]=a3;
		grouppoint[3]=a4;
		/*w:可增加将这些都汇集一起的功能*/
		x=a1->x;
		y=a1->y;
	}
	Armygroup(Armygroup *a1,Armygroup *a2,Armygroup *a3,Armygroup *a4)//tuan和lv和shi（四进制）
	{
		int i=0;

		Armygroup *unitarmyarr[4];//建立一个临时数组，数据放数组里，避免后面for循环的冗余代码
		unitarmyarr[0]=a1;
		unitarmyarr[1]=a2;
		unitarmyarr[2]=a3;
		unitarmyarr[3]=a4;
		number = a1->number+a2->number+a3->number+a4->number;

		grouppoint=new ying*[number];

		//此部分中i为this中的，j为原来的，k为原来中存储的每一个
		int j;
		for(j=0;j<4;j++)
		{
			for(int k=0; k < unitarmyarr[j]->number; k++)
			{
				grouppoint[i]=unitarmyarr[j]->grouppoint[k];
				i++;
			}
		}

		/*w:可增加将这些逐渐汇集一起的功能，当xy都相同时合并*/

		this->x=unitarmyarr[0]->grouppoint[0]->x;
		this->y=unitarmyarr[0]->grouppoint[0]->y;

		for(j=0;j<4;j++)
		{
			for(int k=0; k < unitarmyarr[j]->number; k++)
			{
				unitarmyarr[j]->grouppoint[k]->x=this->x;
				unitarmyarr[j]->grouppoint[k]->y=this->y;
				i++;
			}
		}
		
	}
	~Armygroup()
	{
		delete []grouppoint;
	}
};
class PLACE//在移动箭头的时候用
{
public:
//地区归属者
	bool belong;//地区归属者，true为p1,false为p2
//地区种类
	char kind;//地区种类,1为城市，2为乡村，3为山地，4为无人区
//军队
	ying ** army;//用于保存地区所含有军队
	ying ** other;//大于十五的放在这里
	//构造函数
	PLACE()
	{
		army=new ying*[15];/********************************************我只想设置到15*********************************************************/
		memset(army,NULL,4*15);
	}
	void setarmypoint(ying *t)
	{
		for(int i=0;i<15;i++)
		{
			if(army[i]!=NULL)
			{
				army[i]=t;
				return ;
			}
		}
		/*************************此处注意，只能到十五，十五之后无法在该地区进行操作，如果在该地区停止，特殊处理*****************************************/
		/**************************特殊处理以后加，现在先在这里进行警告**********************************************************************************/
		/*							后期考虑使用链式储存																								*/
	}
	/*
	void getarmy(Armygroup *return_armygroup,int *retur_nsize)
	{
		for()
	}
	*/
	~PLACE()
	{
		delete []army;
	}


};

class ARROW
{
public:
	int x;
	int y;

	IMAGE *image;
	PLACE *place;

	int tox,toy;//目的地
	ARROW(IMAGE *image)
	{
		this->image=image;
	}

	void SetPointPlace(PLACE *t)
	{
		place=t;
	}
	void setdestination(PLACE *t)//设置目的地
	{
		place=t;
	}
	void draw()
	{
		transparentimage(this->x,this->y, image, &maskarrow);
	}

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

//移动军队的队列，以及刷新屏幕的函数
class myqueue//思路：一个指针指向这个类的队列，并在这中间找空的，如果队列已满，则再分配一个此对象，每次调用从此对象的queuqnext跳过去
{
public:
	ying** armyqueue;
	myqueue* queuenext;
	myqueue()
	{
		queuenext=NULL;
		armyqueue = new ying*[50];
		for(int i=0;i<50;i++)
		{
			armyqueue[i]=NULL;
		}
	}
	void addqueue(ying* t)//****************************************************************************************************************************************************
	{
		for(int i=0;i<50;i++)//在队列里找空的
		{
			if(armyqueue[i]==NULL)
			{
				armyqueue[i]=t;
				return ;
			}
		}
			if(queuenext==NULL)//如果队列已满，就再新建一个队列
			{
 				queuenext= new myqueue();
				queuenext->addqueue(t);
			}
			else
			{
				queuenext->addqueue(t);
			}
	}
	void addarmygroup(Armygroup *a)
	{
		int n=a->number;
		ying **arr=a->grouppoint;
		for(int i=0;i<n;i++)
		{
			this->addqueue(arr[i]);
		}

	}
	void movearmy()
	{
		int x,s,d;
		for(int i=0;i<50;i++)
		{
			if(armyqueue[i]!=NULL)
			{
				bool reach=armyqueue[i]->move();
				int tx,ty;
				if( reach )//到地方了就把这个对象出队
				{
					tx=armyqueue[i]->x;
					ty=armyqueue[i]->y;
					PixelPointer[tx][ty]->setarmypoint(armyqueue[i]);
					armyqueue[i]=NULL;
				}
				else
				{
					tx=(int)armyqueue[i]->x;
					ty=(int)armyqueue[i]->y;
					PixelPointer[tx][ty]->setarmypoint(armyqueue[i]);
				}
			}
		}
		//看看是否有下一个队列
		if(queuenext!=NULL)
		{
			this->queuenext->movearmy();
		}
	}
	void draw(IMAGE *army,player *p)//移动之后刷新图片
	{
		IMAGE t;//存放画的图像
		SetWorkingImage(&background);//读取背景图
		getimage(&t,0,0,600,600);
		SetWorkingImage(&t);
		//遍历军队位置
		for(int i=0;i<360;i++)
		{
//			putimage(p->army.yingarmy[i].x,p->army.yingarmy[i].y,army);//遍历军队
		}
		p->arrow->draw();
		SetWorkingImage(NULL);//输出位置改回去
		if(p->number==PLAYER1)
		{
			putimage(0,0,&t);
			putimage(0,600,&trectangle);//p1的状态显示位置
		}
		if(p->number==PLAYER2)
		{
			putimage(675,0,&t);
			putimage(675,600,&trectangle);//p2的状态显示位置
		}
	//	setorigin()
	}
	~myqueue()
	{

		delete []armyqueue;
		if(queuenext!=NULL)
		{
			delete queuenext;
		}
	}
};

DWORD WINAPI threadfuction(LPVOID Lthreadstruct);
DWORD WINAPI threadfuction2(LPVOID Lthreadstruct);
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

int main() {

	int i,j;
	//initgraph(600,600);
	initgraph(1260, 680, EW_SHOWCONSOLE);
	//initgraph(1260,680);


	//加载图片
	loadimage(&background,_T(".\\image\\background.jpg"));//加载背景图片

	loadimage(&p2armyimage,_T(".\\image\\p2armyimage.png"));//加载军队图片
	loadimage(&p1armyimage,_T(".\\image\\p1armyimage.png"));

	loadimage(&arrowimage1,_T(".\\image\\arrow1.png"));//加载箭头及掩码图
	loadimage(&arrowimage2,_T(".\\image\\arrow2.png"));
	loadimage(&maskarrow ,_T(".\\image\\maskarrow.png"));
/*
	//加载图片
	putimage(0,0,&background);
	putimage(650,0,&background);
	putimage(10,10,&p1armyimage);
	putimage(0,0,&p2armyimage);
*/
	IMAGE t(600,75);
	SetWorkingImage(&t);
	setorigin(0,0);
	setfillcolor(0x45BEF2);
	solidrectangle(0,0,600,75);
	getimage(&trectangle,0,0,600,75);
	SetWorkingImage(NULL);
//	transparentimage(1000,350, &arrowimage2, &maskarrow);

	player p1;
	player p2;

	p1.number=PLAYER1;
	p2.number=PLAYER2;

	PLACE MapPlace[28];
	PixelPointer = new PLACE**[600];
    for(i = 0;i < 600;i++)	PixelPointer[i] = new PLACE*[600];
	//用于每一个像素指向的地区指针，主要用于检测士兵所在位置，放到堆区里是因为放到栈区里会报错,注意我造的是一个类型为PLACE *类型的数组

	/*赋值所有地区信息*/
	MapPlace[0].kind=NOPEOPLESLAND;
	MapPlace[1].kind=NOPEOPLESLAND;
	MapPlace[2].kind=MOUNTAIN;
	MapPlace[3].kind=COUNTRYSIDE;
	MapPlace[4].kind=CITY;
	MapPlace[5].kind=COUNTRYSIDE;
	MapPlace[6].kind=CITY;
	MapPlace[7].kind=COUNTRYSIDE;
	MapPlace[8].kind=MOUNTAIN;
	MapPlace[9].kind=NOPEOPLESLAND;
	MapPlace[10].kind=NOPEOPLESLAND;
	MapPlace[11].kind=MOUNTAIN;
	MapPlace[12].kind=COUNTRYSIDE;
	MapPlace[13].kind=CITY;
	MapPlace[14].kind=CITY;
	MapPlace[15].kind=MOUNTAIN;
	MapPlace[16].kind=COUNTRYSIDE;
	MapPlace[17].kind=NOPEOPLESLAND;
	MapPlace[18].kind=NOPEOPLESLAND;
	MapPlace[19].kind=MOUNTAIN;
	MapPlace[20].kind=COUNTRYSIDE;
	MapPlace[21].kind=COUNTRYSIDE;
	MapPlace[22].kind=CITY;
	MapPlace[23].kind=CITY;
	MapPlace[24].kind=COUNTRYSIDE;
	MapPlace[25].kind=MOUNTAIN;
	MapPlace[26].kind=MOUNTAIN;
	MapPlace[27].kind=MOUNTAIN;

	MapPlace[0].belong=PLAYER1;
	MapPlace[1].belong=PLAYER2;
	MapPlace[2].belong=PLAYER2;
	MapPlace[3].belong=PLAYER2;
	MapPlace[4].belong=PLAYER2;
	MapPlace[5].belong=PLAYER2;
	MapPlace[6].belong=PLAYER2;
	MapPlace[7].belong=PLAYER1;
	MapPlace[8].belong=PLAYER1;
	MapPlace[9].belong=PLAYER1;
	MapPlace[10].belong=PLAYER2;
	MapPlace[11].belong=PLAYER2;
	MapPlace[12].belong=PLAYER2;
	MapPlace[13].belong=PLAYER2;
	MapPlace[14].belong=PLAYER1;
	MapPlace[15].belong=PLAYER1;
	MapPlace[16].belong=PLAYER1;
	MapPlace[17].belong=PLAYER1;
	MapPlace[18].belong=PLAYER2;
	MapPlace[19].belong=PLAYER2;
	MapPlace[20].belong=PLAYER2;
	MapPlace[21].belong=PLAYER1;
	MapPlace[22].belong=PLAYER1;
	MapPlace[23].belong=PLAYER1;
	MapPlace[24].belong=PLAYER1;
	MapPlace[25].belong=PLAYER1;
	MapPlace[26].belong=PLAYER1;
	MapPlace[27].belong=PLAYER2;
//这里我没有更好的想法来判断军队的位置，如果后面有更好的想法再改
/*循环赋值指针******************************************************************************/
	for(i=0;i<150;i++)
	{
		for(j=0;j<150;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[0];//块0
			else PixelPointer[i][j]=&MapPlace[1];//块1
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=0;j<150;j++)
		{
			if(i-150<=j) PixelPointer[i][j]=&MapPlace[2];//块2
			else PixelPointer[i][j]=&MapPlace[3];//块3
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=0;j<150;j++)
		{
			PixelPointer[i][j]=&MapPlace[4];//块4
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=0;j<150;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[6];//块6
			else PixelPointer[i][j]=&MapPlace[5];//块5************注意，这里顺序是反的，因为我不会数学了，就不改了
		}
	}
	for(i=0;i<150;i++)
	{
		for(j=150;j<300;j++)
		{
			if(i<=j-150)  PixelPointer[i][j]=&MapPlace[7];//块7
			else  PixelPointer[i][j]=&MapPlace[8];//块8
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=150;j<300;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[9];//块9
			else PixelPointer[i][j]=&MapPlace[10];//块10
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=150;j<300;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[12];//块12
			else PixelPointer[i][j]=&MapPlace[11];//块11************注意，这里顺序是反的，因为我不会数学了，就不改了
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=150;j<300;j++)
		{
			PixelPointer[i][j]=&MapPlace[13];//块13
		}
	}
	for(i=0;i<150;i++)
	{
		for(j=300;j<450;j++)
		{
			PixelPointer[i][j]=&MapPlace[14];//块14
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=300;j<450;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[16];//块16
			else PixelPointer[i][j]=&MapPlace[15];//块15************注意，这里顺序是反的，因为我不会数学了，就不改了
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=300;j<450;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[17];//块17
			else PixelPointer[i][j]=&MapPlace[18];//块18
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=300;j<450;j++)
		{
			if(i-150<=j) PixelPointer[i][j]=&MapPlace[19];//块19
			else PixelPointer[i][j]=&MapPlace[20];//块20
		}
	}
	for(i=0;i<150;i++)
	{
		for(j=450;j<600;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[22];//块22
			else PixelPointer[i][j]=&MapPlace[21];//块21************注意，这里顺序是反的，因为我不会数学了，就不改了
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=450;j<600;j++)
		{
			PixelPointer[i][j]=&MapPlace[23];//块23
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=450;j<600;j++)
		{
			if(i+150<=j) PixelPointer[i][j]=&MapPlace[24];//块24
			else PixelPointer[i][j]=&MapPlace[25];//块25
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=450;j<600;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[26];//块26
			else PixelPointer[i][j]=&MapPlace[27];//块27
		}
	}
/*赋值指针******************************************************************************/
/*********************************************************************************************************************************************/
//此处是测试

	myqueue * queuemovep1= new myqueue();
	myqueue * queuemovep2= new myqueue();

//	p1.army.yingarmy[0].x=220;
//	p1.army.yingarmy[0].y=300;

//	queuemovep1->addqueue(&p1.army.yingarmy[0]);


//	p1.army.yingarmy[0].setdestination(100,100);

	//p1的箭头
	ARROW arrow1(&arrowimage1);
	p1.arrow=&arrow1;
	arrow1.x=50;
	arrow1.y=50;
	arrow1.place=PixelPointer[50][50];
	//p2的箭头
	ARROW arrow2(&arrowimage2);
	p2.arrow=&arrow2;
	arrow2.x=50;
	arrow2.y=50;
	arrow2.place=PixelPointer[50][50];
	//初始化线程“可能需要”的指针
	THREADSTRUCT threadstruct;
	threadstruct.tarrowimage1=&arrowimage1;
	threadstruct.tarrowimage2=&arrowimage2;
	threadstruct.tbackground=&background;
	threadstruct.tmaskarrow=&maskarrow;
	threadstruct.tp1=&p1;
	threadstruct.tp1armyimage=&p1armyimage;
	threadstruct.tp1image=&p1image;
	threadstruct.tp2=&p2;
	threadstruct.tp2armyimage=&p2armyimage;
	threadstruct.tp2image=&p2image;
	threadstruct.tqueuemovep1=queuemovep1;
	threadstruct.tqueuemovep2=queuemovep2;
	threadstruct.tarrow1=&arrow1;
	threadstruct.tarrow2=&arrow2;
//测试

	CreateThread(NULL,0,threadfuction,&threadstruct,0,NULL);
	CreateThread(NULL,0,threadfuction2,&threadstruct,0,NULL);

	while(1)
	{
	}

	



	_getch();		
	closegraph();
	return 0;
}


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
void transparentimage(int x, int y, IMAGE *srcimg, IMAGE *maskimg)//输出透明函数
{
	putimage(x, y, maskimg, SRCAND);
	putimage(x, y, srcimg, SRCPAINT);
}

void Workoutmoveunit(float *moveunitx, float *moveunity , short movex,short movey,float speed)//计算移动的单位距离
{
	//求出每次移动的x坐标和y坐标
	//相当于三角形，假设目标点叫A，结束点在B，speed长度的向量在AB上为AP
	//P做向下垂，设x方+y方=t方，则两三角形相似得t/speed=unitx/movex，得unitx=(movex*speed)/t
	int tt=movex*movex+movey*movey;
	double t=sqrt(tt);//tt用于计算t，t是计算斜边长
	*moveunitx=(movex*speed)/t;
	*moveunity=(movey*speed)/t;
}
