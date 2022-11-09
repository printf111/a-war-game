#pragma once
#include "gameclass.h"
void transparentimage(int x, int y, IMAGE *srcimg, IMAGE *maskimg);
void Workoutmoveunit(float *moveunitx, float *moveunity , short movex,short movey,float speed);

ying::ying()
{
	this->numberying=sumying+1;
	sumying++;
	this->soldier=35;
	this->morale=100;
	this->fire=50;
	this->x=30;	
	this->y=30;
	
}

PLACE::PLACE()
{
	army=new Armygroup*[15];/********************************************我只想设置到15*********************************************************/
	memset(army,NULL,4*15);
}
void PLACE::setarmypoint(Armygroup *t)
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
PLACE::~PLACE()
{
	delete []army;
}

ARROW::ARROW(IMAGE *image,IMAGE *mask)
{
	this->image=image;
	this->maskarrow=mask;
}

void ARROW::SetPointPlace(PLACE *t)
{
	place=t;
}
void ARROW::setdestination(PLACE *t)//设置目的地
{
	place=t;
}
void ARROW::draw()
{
	transparentimage(this->x,this->y, this->image, this->maskarrow);
}
myqueue::myqueue()
{
	queuenext=NULL;
	armyqueue = new Armygroup*[50];
	for(int i=0;i<50;i++)
	{
		armyqueue[i]=NULL;
	}
}
void myqueue::addqueue(Armygroup* t)//****************************************************************************************************************************************************
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
void myqueue::movearmy()
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
void myqueue::draw(IMAGE *army,player *p)//移动之后刷新图片
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
myqueue::~myqueue()
{

	delete []armyqueue;
	if(queuenext!=NULL)
	{
		delete queuenext;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Armygroup::	Armygroup(ying *a1)//只有一个ying军队时
{
	speed=1;
	movex=-9999;//假设这是负无穷
	movey=-9999;//假设这是负无穷
	number = 1;
	grouppoint=new ying*[1];
	grouppoint[0]=a1;
	x=a1->x;
	y=a1->y;
}
Armygroup::	Armygroup(Armygroup *a1,Armygroup *a2,Armygroup *a3,Armygroup *a4)//tuan和lv和shi（四进制）
{
	speed=1;
	movex=-9999;//假设这是负无穷
	movey=-9999;//假设这是负无穷
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
Armygroup::	~Armygroup()
{
	delete []grouppoint;
}
void Armygroup::setdestination(short dx,short dy)
{
	this->tox=dx;
	this->toy=dy;
}
bool Armygroup::move()//返回值是确定是否到地方
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