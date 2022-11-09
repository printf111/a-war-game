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
	army=new Armygroup*[15];/********************************************��ֻ�����õ�15*********************************************************/
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
	/*************************�˴�ע�⣬ֻ�ܵ�ʮ�壬ʮ��֮���޷��ڸõ������в���������ڸõ���ֹͣ�����⴦��*****************************************/
	/**************************���⴦���Ժ�ӣ���������������о���**********************************************************************************/
	/*							���ڿ���ʹ����ʽ����																								*/
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
void ARROW::setdestination(PLACE *t)//����Ŀ�ĵ�
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
	for(int i=0;i<50;i++)//�ڶ������ҿյ�
	{
		if(armyqueue[i]==NULL)
		{
			armyqueue[i]=t;
			return ;
		}
	}
	if(queuenext==NULL)//������������������½�һ������
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
			if( reach )//���ط��˾Ͱ�����������
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
	//�����Ƿ�����һ������
	if(queuenext!=NULL)
	{
		this->queuenext->movearmy();
	}
}
void myqueue::draw(IMAGE *army,player *p)//�ƶ�֮��ˢ��ͼƬ
{
	IMAGE t;//��Ż���ͼ��
	SetWorkingImage(&background);//��ȡ����ͼ
	getimage(&t,0,0,600,600);
	SetWorkingImage(&t);
	//��������λ��
	for(int i=0;i<360;i++)
	{
//			putimage(p->army.yingarmy[i].x,p->army.yingarmy[i].y,army);//��������
	}
	p->arrow->draw();
	SetWorkingImage(NULL);//���λ�øĻ�ȥ
	if(p->number==PLAYER1)
	{
		putimage(0,0,&t);
		putimage(0,600,&trectangle);//p1��״̬��ʾλ��
	}
	if(p->number==PLAYER2)
	{
		putimage(675,0,&t);
		putimage(675,600,&trectangle);//p2��״̬��ʾλ��
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

Armygroup::	Armygroup(ying *a1)//ֻ��һ��ying����ʱ
{
	speed=1;
	movex=-9999;//�������Ǹ�����
	movey=-9999;//�������Ǹ�����
	number = 1;
	grouppoint=new ying*[1];
	grouppoint[0]=a1;
	x=a1->x;
	y=a1->y;
}
Armygroup::	Armygroup(Armygroup *a1,Armygroup *a2,Armygroup *a3,Armygroup *a4)//tuan��lv��shi���Ľ��ƣ�
{
	speed=1;
	movex=-9999;//�������Ǹ�����
	movey=-9999;//�������Ǹ�����
	int i=0;
	Armygroup *unitarmyarr[4];//����һ����ʱ���飬���ݷ�������������forѭ�����������
	unitarmyarr[0]=a1;
	unitarmyarr[1]=a2;
	unitarmyarr[2]=a3;
	unitarmyarr[3]=a4;
	number = a1->number+a2->number+a3->number+a4->number;
	
	grouppoint=new ying*[number];
	
	//�˲�����iΪthis�еģ�jΪԭ���ģ�kΪԭ���д洢��ÿһ��
	int j;
	for(j=0;j<4;j++)
	{
		for(int k=0; k < unitarmyarr[j]->number; k++)
		{
			grouppoint[i]=unitarmyarr[j]->grouppoint[k];
			i++;
		}
	}
	
	/*w:�����ӽ���Щ�𽥻㼯һ��Ĺ��ܣ���xy����ͬʱ�ϲ�*/
	
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
bool Armygroup::move()//����ֵ��ȷ���Ƿ񵽵ط�
{
/*
�ƶ�����
	*/
	
	if( ( (this->movex) < -600) && ( (this->movey) < -600) )//����ǵ�һ���ƶ�
	{
		movex=tox-x;
		movey=toy-y;
		Workoutmoveunit(&moveunitx,&moveunity,movex,movey,speed);//���㵥λ�ƶ�����/����������
	}
	if( ( abs(abs(this->x)-abs(tox)) <= abs(moveunitx) )   ||  	( abs(abs(this->y)-abs(toy)) <= abs(moveunity) ))//�����if��Ϊ�������else
	{
		if( ( abs(abs(this->x)-abs(tox)) <= abs(moveunitx) )    )//���û��б���ƶ����쵽�ط���
		{
			this->x=tox;
			moveunity=speed;//x����Ҫ�ƶ��ˣ�����������һ��y����Ȼֻ�ƶ�y���൱�ں��ٶȱ���
			
		}
		if(	( abs(abs(this->y)-abs(toy)) <= abs(moveunity) )    )//�ֿ�д��Ϊ�˾�������Ϊ�ƶ��Ķ������������Կ���б���ƶ���ʱ����ֱ�ӵ���Ŀ�ĵ�
		{
			this->y=toy;
			moveunitx=speed;//y����Ҫ�ƶ��ˣ�����������һ��x����Ȼֻ�ƶ�y���൱�ں��ٶȱ���
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
	
	
	
	if( (this->x == tox)   &&  (this->y == toy) )//������ط���
	{
		return true;
	}
	
	return false;
}

void transparentimage(int x, int y, IMAGE *srcimg, IMAGE *maskimg)//���͸������
{
	putimage(x, y, maskimg, SRCAND);
	putimage(x, y, srcimg, SRCPAINT);
}

void Workoutmoveunit(float *moveunitx, float *moveunity , short movex,short movey,float speed)//�����ƶ��ĵ�λ����
{
	//���ÿ���ƶ���x�����y����
	//�൱�������Σ�����Ŀ����A����������B��speed���ȵ�������AB��ΪAP
	//P�����´�����x��+y��=t�����������������Ƶ�t/speed=unitx/movex����unitx=(movex*speed)/t
	int tt=movex*movex+movey*movey;
	double t=sqrt(tt);//tt���ڼ���t��t�Ǽ���б�߳�
	*moveunitx=(movex*speed)/t;
	*moveunity=(movey*speed)/t;
}