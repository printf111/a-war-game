#pragma once
#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//�궨��
#define PLAYER1 true
#define PLAYER2 false

#define CITY 1
#define COUNTRYSIDE 2
#define MOUNTAIN 3
#define NOPEOPLESLAND 4

//�����궨��
#define VKISDOWN(vkey) (GetAsyncKeyState(vkey)&0x8000)//�жϰ����Ƿ񱻰���

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
//��ÿһ�����ֿ�Ϊ������
//���棺�����ڴ�й©���ʽϴ�ʹ��ʱ�ǵû����ڴ�
class Armygroup
{
public:
	unsigned char number;//��������ľ���������������Ϊ�ǵȼ�
	unsigned short serial_number;//�������
	ying ** grouppoint;
	float x;//X����
	float y;//y����
	float speed;	//�����Լ�Ҫ�����ٶ�*****************************************************************************************************************
	short movex;//����Ŀ�ĵؼ����������x
	short movey;//����Ŀ�ĵؼ����������y
	float moveunitx;//һ���ƶ���x����
	float moveunity;//һ���ƶ���y����
	int tox,toy;//Ŀ�ĵ�
public:
	Armygroup(ying *a1);
	Armygroup(Armygroup *a1,Armygroup *a2,Armygroup *a3,Armygroup *a4);//tuan��lv��shi���Ľ��ƣ�
	~Armygroup();
	bool move();
    void setdestination(short,short);
};

class PLACE//���ƶ���ͷ��ʱ����
{
public:
	//����������
	bool belong;//���������ߣ�trueΪp1,falseΪp2
	//��������
	char kind;//��������,1Ϊ���У�2Ϊ��壬3Ϊɽ�أ�4Ϊ������
	//����
	Armygroup ** army;//���ڱ�����������о���
	ying ** other;//����ʮ��ķ�������


	PLACE();	//���캯��
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
	
	IMAGE *maskarrow;//��ͷͼƬ
	int tox,toy;//Ŀ�ĵ�
	ARROW(IMAGE *image,IMAGE *mask);
	void SetPointPlace(PLACE *t);
	void setdestination(PLACE *t);//����Ŀ�ĵ�
	void draw();
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

class myqueue
{
public:
	Armygroup** armyqueue;
	myqueue* queuenext;
	myqueue();
	void addqueue(Armygroup* t);
	void movearmy();
	void draw(IMAGE *army,player *p);//�ƶ�֮��ˢ��ͼƬ
	~myqueue();
};

PLACE ***PixelPointer;	//����ÿһ������ָ��ĵ���ָ�룬��Ҫ���ڼ��ʿ������λ�ã��ŵ�����������Ϊ�ŵ�ջ����ᱨ��

//ͼƬȫ�ֱ���
IMAGE background;//��ű���ͼƬ
IMAGE p1armyimage,p2armyimage;//��ž���ͼƬ
IMAGE p1image,p2image;//
IMAGE arrowimage1,arrowimage2;//��ͷͼƬ
IMAGE trectangle(600,75);//һ����ʱͼƬ

//����ȫ�ֱ���
unsigned short sumtuan=0;//tuan������Ŀ
unsigned short sumlv=0;//lv������Ŀ
unsigned short sumshi=0;//shi������Ŀ
unsigned int sumying=0;//ying ������Ŀ
int ERRORCODE;//һЩ���ܳ���Ķ�����ERRORCODE����ŵ�ERRORCODE_intruction.txt

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

