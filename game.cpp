#include "gameclass.cpp"
#include "gamefuction.cpp"
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

//����ȫ�ֱ���
unsigned short sumtuan=0;//tuan������Ŀ
unsigned short sumlv=0;//lv������Ŀ
unsigned short sumshi=0;//shi������Ŀ



int main() {
	int i,j;
	//initgraph(600,600);
	initgraph(1260, 680, EW_SHOWCONSOLE);
	//initgraph(1260,680);
	
	IMAGE maskarrow;
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
	ARROW arrow1(&arrowimage1,&maskarrow);
	p1.arrow=&arrow1;
	arrow1.x=50;
	arrow1.y=50;
	arrow1.place=PixelPointer[50][50];
	//p2�ļ�ͷ
	ARROW arrow2(&arrowimage2,&maskarrow);
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


