#include "gameclass.cpp"
#include "gamefuction.cpp"
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

//部分全局变量
unsigned short sumtuan=0;//tuan的总数目
unsigned short sumlv=0;//lv的总数目
unsigned short sumshi=0;//shi的总数目



int main() {
	int i,j;
	//initgraph(600,600);
	initgraph(1260, 680, EW_SHOWCONSOLE);
	//initgraph(1260,680);
	
	IMAGE maskarrow;
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
	ARROW arrow1(&arrowimage1,&maskarrow);
	p1.arrow=&arrow1;
	arrow1.x=50;
	arrow1.y=50;
	arrow1.place=PixelPointer[50][50];
	//p2的箭头
	ARROW arrow2(&arrowimage2,&maskarrow);
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


