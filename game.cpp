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
			35��
			3*35=105��
4*105=420Ӫ
4*420=1260��
4*1260=5040��
4*5040=20160ʦ
*/
/*
	ע�⣺������ʹ����itoa����
*/
//�궨��
#define PLAYER1 true
#define PLAYER2 false

#define CITY 1
#define COUNTRYSIDE 2
#define MOUNTAIN 3
#define NOPEOPLESLAND 4

//�����궨��
#define VKISDOWN(vkey) (GetAsyncKeyState(vkey)&0x8000)//�жϰ����Ƿ񱻰���

//����ȫ�ֱ���
unsigned short sumying=0;//ying������Ŀ
unsigned short sumtuan=0;//tuan������Ŀ
unsigned short sumlv=0;//lv������Ŀ
unsigned short sumshi=0;//shi������Ŀ

//ͼƬȫ�ֱ���
IMAGE background;//��ű���ͼƬ
IMAGE p1armyimage,p2armyimage;//��ž���ͼƬ
IMAGE p1image,p2image;//
IMAGE arrowimage1,maskarrow,arrowimage2;//��ͷͼƬ
IMAGE trectangle(600,75);//һ����ʱͼƬ

void transparentimage(int x, int y, IMAGE *srcimg, IMAGE *maskimg);
void Workoutmoveunit(float *moveunitx, float *moveunity , short movex,short movey,float speed);

class PLACE;//����һ���࣬��Ϊ����Ҫ��placeָ��ying
PLACE ***PixelPointer;	//����ÿһ������ָ��ĵ���ָ�룬��Ҫ���ڼ��ʿ������λ�ã��ŵ�����������Ϊ�ŵ�ջ����ᱨ��

class ying
{
public:
	int numberying;
	int soldier;
	int morale;
	int fire; 
	 float x;
	 float y;
	float speed;	//�����Լ�Ҫ�����ٶ�*****************************************************************************************************************

	short movex;//����Ŀ�ĵؼ����������x
	short movey;//����Ŀ�ĵؼ����������y
	float moveunitx;//һ���ƶ���x����
	float moveunity;//һ���ƶ���y����

	int tox,toy;//Ŀ�ĵ�
	ying()
	{
		this->numberying=sumying+1;
		sumying++;
		this->soldier=35;
		this->morale=100;
		this->fire=50;
		this->x=30;	
		this->y=30;
		movex=-9999;//�������Ǹ�����
		movey=-9999;//�������Ǹ�����
		speed=1;

	}
	void setdestination(short dx,short dy)
	{
		this->tox=dx;
		this->toy=dy;
	}
	bool move()//����ֵ��ȷ���Ƿ񵽵ط�
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

};

//��ÿһ�����ֿ�Ϊ������
//���棺�����ڴ�й©���ʽϴ�ʹ��ʱ�ǵû����ڴ�
class Armygroup
{
public:
	unsigned char number;//��������ľ���������������Ϊ�ǵȼ�
	ying ** grouppoint;

public:
	float x;//X����
	float y;//y����

	Armygroup(ying *a1)//ֻ��һ��ying����ʱ
	{
		number = 1;
		grouppoint=new ying*[1];
		grouppoint[0]=a1;
		x=a1->x;
		y=a1->y;
	}
	Armygroup(ying *a1,ying *a2,ying *a3,ying *a4)//4��ying��tuan
	{
		number = 4;
		grouppoint=new ying*[4];
		grouppoint[0]=a1;
		grouppoint[1]=a2;
		grouppoint[2]=a3;
		grouppoint[3]=a4;
		/*w:�����ӽ���Щ���㼯һ��Ĺ���*/
		x=a1->x;
		y=a1->y;
	}
	Armygroup(Armygroup *a1,Armygroup *a2,Armygroup *a3,Armygroup *a4)//tuan��lv��shi���Ľ��ƣ�
	{
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
	~Armygroup()
	{
		delete []grouppoint;
	}
};
class PLACE//���ƶ���ͷ��ʱ����
{
public:
//����������
	bool belong;//���������ߣ�trueΪp1,falseΪp2
//��������
	char kind;//��������,1Ϊ���У�2Ϊ��壬3Ϊɽ�أ�4Ϊ������
//����
	ying ** army;//���ڱ�����������о���
	ying ** other;//����ʮ��ķ�������
	//���캯��
	PLACE()
	{
		army=new ying*[15];/********************************************��ֻ�����õ�15*********************************************************/
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
		/*************************�˴�ע�⣬ֻ�ܵ�ʮ�壬ʮ��֮���޷��ڸõ������в���������ڸõ���ֹͣ�����⴦��*****************************************/
		/**************************���⴦���Ժ�ӣ���������������о���**********************************************************************************/
		/*							���ڿ���ʹ����ʽ����																								*/
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

	int tox,toy;//Ŀ�ĵ�
	ARROW(IMAGE *image)
	{
		this->image=image;
	}

	void SetPointPlace(PLACE *t)
	{
		place=t;
	}
	void setdestination(PLACE *t)//����Ŀ�ĵ�
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
	int	MilitaryExploit;//����
	int Money;			//Ǯ
	byte post;			//ְ�񣨴��ţ�����Ϊ��ʡ�ڴ�ֻ��һ���ֽڵ����ִ��棩��3Ӫ��4�ţ�5�ã�6ʦ��7��

};

//�ƶ����ӵĶ��У��Լ�ˢ����Ļ�ĺ���
class myqueue//˼·��һ��ָ��ָ�������Ķ��У��������м��ҿյģ�����������������ٷ���һ���˶���ÿ�ε��ôӴ˶����queuqnext����ȥ
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
	void draw(IMAGE *army,player *p)//�ƶ�֮��ˢ��ͼƬ
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
//�̲߳����ṹ��
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


	//����ͼƬ
	loadimage(&background,_T(".\\image\\background.jpg"));//���ر���ͼƬ

	loadimage(&p2armyimage,_T(".\\image\\p2armyimage.png"));//���ؾ���ͼƬ
	loadimage(&p1armyimage,_T(".\\image\\p1armyimage.png"));

	loadimage(&arrowimage1,_T(".\\image\\arrow1.png"));//���ؼ�ͷ������ͼ
	loadimage(&arrowimage2,_T(".\\image\\arrow2.png"));
	loadimage(&maskarrow ,_T(".\\image\\maskarrow.png"));
/*
	//����ͼƬ
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
	//����ÿһ������ָ��ĵ���ָ�룬��Ҫ���ڼ��ʿ������λ�ã��ŵ�����������Ϊ�ŵ�ջ����ᱨ��,ע���������һ������ΪPLACE *���͵�����

	/*��ֵ���е�����Ϣ*/
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
//������û�и��õ��뷨���жϾ��ӵ�λ�ã���������и��õ��뷨�ٸ�
/*ѭ����ֵָ��******************************************************************************/
	for(i=0;i<150;i++)
	{
		for(j=0;j<150;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[0];//��0
			else PixelPointer[i][j]=&MapPlace[1];//��1
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=0;j<150;j++)
		{
			if(i-150<=j) PixelPointer[i][j]=&MapPlace[2];//��2
			else PixelPointer[i][j]=&MapPlace[3];//��3
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=0;j<150;j++)
		{
			PixelPointer[i][j]=&MapPlace[4];//��4
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=0;j<150;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[6];//��6
			else PixelPointer[i][j]=&MapPlace[5];//��5************ע�⣬����˳���Ƿ��ģ���Ϊ�Ҳ�����ѧ�ˣ��Ͳ�����
		}
	}
	for(i=0;i<150;i++)
	{
		for(j=150;j<300;j++)
		{
			if(i<=j-150)  PixelPointer[i][j]=&MapPlace[7];//��7
			else  PixelPointer[i][j]=&MapPlace[8];//��8
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=150;j<300;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[9];//��9
			else PixelPointer[i][j]=&MapPlace[10];//��10
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=150;j<300;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[12];//��12
			else PixelPointer[i][j]=&MapPlace[11];//��11************ע�⣬����˳���Ƿ��ģ���Ϊ�Ҳ�����ѧ�ˣ��Ͳ�����
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=150;j<300;j++)
		{
			PixelPointer[i][j]=&MapPlace[13];//��13
		}
	}
	for(i=0;i<150;i++)
	{
		for(j=300;j<450;j++)
		{
			PixelPointer[i][j]=&MapPlace[14];//��14
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=300;j<450;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[16];//��16
			else PixelPointer[i][j]=&MapPlace[15];//��15************ע�⣬����˳���Ƿ��ģ���Ϊ�Ҳ�����ѧ�ˣ��Ͳ�����
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=300;j<450;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[17];//��17
			else PixelPointer[i][j]=&MapPlace[18];//��18
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=300;j<450;j++)
		{
			if(i-150<=j) PixelPointer[i][j]=&MapPlace[19];//��19
			else PixelPointer[i][j]=&MapPlace[20];//��20
		}
	}
	for(i=0;i<150;i++)
	{
		for(j=450;j<600;j++)
		{
			if(-i+600<=j) PixelPointer[i][j]=&MapPlace[22];//��22
			else PixelPointer[i][j]=&MapPlace[21];//��21************ע�⣬����˳���Ƿ��ģ���Ϊ�Ҳ�����ѧ�ˣ��Ͳ�����
		}
	}
	for(i=150;i<300;i++)
	{
		for(j=450;j<600;j++)
		{
			PixelPointer[i][j]=&MapPlace[23];//��23
		}
	}
	for(i=300;i<450;i++)
	{
		for(j=450;j<600;j++)
		{
			if(i+150<=j) PixelPointer[i][j]=&MapPlace[24];//��24
			else PixelPointer[i][j]=&MapPlace[25];//��25
		}
	}
	for(i=450;i<600;i++)
	{
		for(j=450;j<600;j++)
		{
			if(i<=j) PixelPointer[i][j]=&MapPlace[26];//��26
			else PixelPointer[i][j]=&MapPlace[27];//��27
		}
	}
/*��ֵָ��******************************************************************************/
/*********************************************************************************************************************************************/
//�˴��ǲ���

	myqueue * queuemovep1= new myqueue();
	myqueue * queuemovep2= new myqueue();

//	p1.army.yingarmy[0].x=220;
//	p1.army.yingarmy[0].y=300;

//	queuemovep1->addqueue(&p1.army.yingarmy[0]);


//	p1.army.yingarmy[0].setdestination(100,100);

	//p1�ļ�ͷ
	ARROW arrow1(&arrowimage1);
	p1.arrow=&arrow1;
	arrow1.x=50;
	arrow1.y=50;
	arrow1.place=PixelPointer[50][50];
	//p2�ļ�ͷ
	ARROW arrow2(&arrowimage2);
	p2.arrow=&arrow2;
	arrow2.x=50;
	arrow2.y=50;
	arrow2.place=PixelPointer[50][50];
	//��ʼ���̡߳�������Ҫ����ָ��
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
//����

	CreateThread(NULL,0,threadfuction,&threadstruct,0,NULL);
	CreateThread(NULL,0,threadfuction2,&threadstruct,0,NULL);

	while(1)
	{
	}

	



	_getch();		
	closegraph();
	return 0;
}


DWORD WINAPI threadfuction(LPVOID Lthreadstruct)//ˢ����Ļ���̺߳���
{
	THREADSTRUCT* threadstruct = (THREADSTRUCT *)Lthreadstruct;
	threadstruct=(THREADSTRUCT *)threadstruct;
	while(1)
	{
		char out[30]="֡��";
		long t1=clock();
		threadstruct->tqueuemovep1->movearmy();
		threadstruct->tqueuemovep1->draw(&p1armyimage,threadstruct->tp1);
		threadstruct->tqueuemovep2->movearmy();
		threadstruct->tqueuemovep2->draw(&p2armyimage,threadstruct->tp2);
		//threadstruct->tarrow1->draw();
		long t2=clock();
		if(t2-t1<=33)//����֡��Ϊ��ֵ
		{
			Sleep(33-(t2-t1));
		}
		/*���֡�ʲ���*/
		char fps[3];
		itoa(clock()-t1,fps,10);
		strcat(out,fps);
		outtextxy(600,60,out);
		/*���P1����λ��*/

		/*���p2����λ��*/
	}
}

DWORD WINAPI threadfuction2(LPVOID Lthreadstruct)//�������̺߳���
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
		if(VKISDOWN('S') && (threadstruct->tarrow1->y+25+30<=600))//��/����Ҫ����ͼƬλ�ã���ͷʵ��λ������ͼƬ���Ͻ�
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
			Sleep(150);//p1����
		}
		if(VKISDOWN(VK_UP) && (threadstruct->tarrow2->y-25+30>=0))
		{
			threadstruct->tarrow2->y-=25;
			ischange2=true;
		}
		if(VKISDOWN(VK_DOWN) && (threadstruct->tarrow2->y+25+30<=600))//��/����Ҫ����ͼƬλ�ã���ͷʵ��λ������ͼƬ���Ͻ�
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
			Sleep(150);//p1����
		}	
	}
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
