#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

typedef struct{
	int hour;
	int minute;
	int second;
}Time;


/************************
*		宏定义			*
*************************/
#define ROW 16			//有多少行
#define COL 16			//有多少列
#define NUM 50			//雷数
#define MINESIZE 30		//每个格子的长和宽

int count;//点开的非雷方块数目
int map[ROW+2][COL+2];
IMAGE img[3];
clock_t t1,t2,t3,t4;
int onoff;//游戏暂停的标志
Time t;


void init();//初始化贴图，布雷，加密（详细会在文档里说）
int first();//游戏首界面
void stop();//暂停按钮
void menu();//右侧菜单
void lightGrey(int i,int j);//浅灰色防块的绘制
void darkGrey(int i,int j);//深灰色方块的绘制
void number(int n, int i, int j);//数字的绘制
void draw();//画表
void openZero(int i, int j);//对开到数字为0格子的处理
void fail();//失败弹窗
void calTime();//计算游戏运行时间
void printTime();//打印时间
void keepon();//打印继续按钮
int game();//控制整个游戏的鼠标点击，流程
void judge();//判断游戏的成功与失败


int main()
{
	initgraph(COL*MINESIZE+160,ROW*MINESIZE);
//	initgraph(ROW*MINESIZE+160,COL*MINESIZE,SHOWCONSOLE);//如果想一开始就知道雷的位置，就把这个注释去掉并把上面那个注释掉

	if(first())//受界面函数返回一就开始游戏
	{	
		cleardevice();
		init();
		while(1)
		{
			menu();
			draw();
			judge();
		}
	}
	return 0;
}

void init()
{
	loadimage(&img[0],"mine.jpg",MINESIZE-1,MINESIZE-1);
	loadimage(&img[1],"flag.jpg",MINESIZE-1,MINESIZE-1);
	srand(time(NULL));
	int cnt = 0;
	int i,j,m,n;
	//全部清零
	for(i = 0; i < ROW+2; i++)
		for(j = 0; j < COL+2; j++)
			map[i][j]=0;
	//布雷
	while(cnt<NUM)
	{
		i=rand()%ROW+1;
		j=rand()%COL+1;
		if(map[i][j]==0)
		{
			map[i][j]=-1;
			cnt++;
		}
	}
	//计算每个方块里的数字（即周围雷数是多少）
	for(i = 1; i <= ROW; i++)
		for(j = 1; j <= COL; j++)
			if(map[i][j] != -1)
			{
				for(m = -1; m <= 1; m++)
					for(n = -1; n <= 1; n++)
					{
						if(m == 0 && n ==0)
							continue;
						if(map[i+m][j+n] == -1)
							map[i][j]++;
					}
			}
	//加密
	for(i = 1; i <= ROW; i++)
		for(j = 1; j <= COL; j++)
			map[i][j]+=20;
}

int first()
{
	loadimage(&img[2],"first.jpg",COL*MINESIZE+160,ROW*MINESIZE);//首界面图片
	putimage(0,0,&img[2]);
	settextstyle(MINESIZE, 0, _T("黑体"));
	setbkmode(TRANSPARENT);
	while(1)
	{
		
		setcolor(WHITE);
		outtextxy(MINESIZE*COL/2-45,(ROW-2)*MINESIZE,"点击任意键开始游戏");
		Sleep(300);
		setcolor(BLACK);
		outtextxy(MINESIZE*COL/2-45,(ROW-2)*MINESIZE,"点击任意键开始游戏");
		Sleep(300);
		if(kbhit())
		{
			getch();
			t1=clock();
			return 1;
		}
	}
}

void stop()
{
	setcolor(WHITE);
	setfillstyle(BS_HATCHED, HS_BDIAGONAL);
	setfillcolor(RGB(190,190,190));
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	fillrectangle(MINESIZE*COL+20,250,MINESIZE*COL+100,300);
	outtextxy(MINESIZE*COL+30,260,"暂停");
	setfillstyle(BS_SOLID);
}

void menu()
{

	setcolor(WHITE);
	setfillstyle(BS_HATCHED, HS_BDIAGONAL);
	setfillcolor(RGB(190,190,190));
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	fillrectangle(MINESIZE*COL+20,50,MINESIZE*COL+100,100);
	fillrectangle(MINESIZE*COL+20,150,MINESIZE*COL+100,200);
	outtextxy(MINESIZE*COL+30,60,"重来");
	outtextxy(MINESIZE*COL+30,160,"退出");
	setfillstyle(BS_SOLID);
	stop();
}

void lightGrey(int i,int j)
{
	setcolor(WHITE);
	setfillcolor(RGB(211,211,211));
	fillrectangle((j-1)*MINESIZE,(i-1)*MINESIZE,(j-1)*MINESIZE+MINESIZE,(i-1)*MINESIZE+MINESIZE);
}

void darkGrey(int i,int j)
{
	setcolor(WHITE);
	setfillcolor(RGB(169,169,169));
	fillrectangle((j-1)*MINESIZE,(i-1)*MINESIZE,(j-1)*MINESIZE+MINESIZE,(i-1)*MINESIZE+MINESIZE);
}

void number(int n, int i, int j)
{
	char str[3];
	sprintf(str,"%d",n);
	settextstyle(MINESIZE, 0, _T("Consolas"));
	switch(n)
	{
		case 1:
		{
			setcolor(WHITE);
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 2:
		{
			setcolor(GREEN);
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 3:
		{
			setcolor(BLUE);
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 4:
		{
			setcolor(YELLOW);
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 5:
		{
			setcolor(RED);
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 6:
		{
			setcolor(BLUE);
			setbkmode(RGB(255,127,80));
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 7:
		{
			setcolor(RGB(139,69,0));
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
		case 8:
		{
			setcolor(RGB(255,20,147));
			setbkmode(TRANSPARENT);
			outtextxy((j-1)*MINESIZE+(MINESIZE/2)-10,(i-1)*MINESIZE,str);
		}break;
	}
}

void draw()
{
	int i,j;
	for(i = 1; i <= ROW; i++)
		for(j = 1; j <= COL; j++)
		{
			printf("%4d",map[i][j]);
			if(map[i][j] == -1)
			{
				putimage((j-1)*MINESIZE+1,(i-1)*MINESIZE+1,&img[0]);
			}
			else if(map[i][j] >= 0 && map[i][j] <= 8)
			{
				lightGrey(i,j);
				number(map[i][j],i,j);
			}
			else if(map[i][j] >= 19 && map[i][j] <= 28)
			{
				darkGrey(i,j);
			}
			else if(map[i][j] > 30)
			{
				putimage((j-1)*MINESIZE,(i-1)*MINESIZE,&img[1]);
			}
		}
//*作弊系统，如果想一开始就知道雷的位置，就把这些注释去掉
/*	system("cls");
	for(i = 1; i <= ROW; i++)
	{
		for(j = 1; j <= COL; j++)
			printf("%4d",map[i][j]);
		printf("\n");
	}
*/
}

void openZero(int i, int j)
{
	int m,n;
	map[i][j]-=20;
	count++;
	for(m = i-1; m <= i+1; m++)
		for(n = j-1; n <= j+1; n++)
		{
			if(m >=1 && m <= ROW && n >= 1 && n <= COL)
			{
				if(m == i && n == j)
					continue;
				if(map[m][n] >= 19 && map[m][n] <= 28)
					if(map[m][n] != 20)
					{
						map[m][n]-=20;
						count++;
					}
					else
					{
						openZero(m,n);//递归调用
					}
			}
		}
}

void fail()
{
	int i,j;
	for(i = 1; i <= ROW; i++)
		for(j = 1; j <= COL; j++)
			if(map[i][j] == 19 || map[i][j] == 69)
				map[i][j]=-1;
	draw();
	MessageBox(NULL,"游戏失败","失败",MB_OK);
	closegraph();
	exit(0);
}

void calTime()
{
	t.hour=0;
	t.minute=0;
	while(t.second >= 60)//如果秒数大于60，将对应秒数转换成分钟
	{
		t.second-=60;
		t.minute++;
	}
	while(t.minute >= 60)//超出60的分钟转换成小时
	{
		t.minute -= 60;
		t.hour++;
	}
}

void printTime()
{
	char str[20];
	setfillcolor(BLACK);
	bar(MINESIZE*COL+10,350,MINESIZE*COL+160,480);//覆盖上一次打印的时间
	setcolor(WHITE);
	t2=clock();//游戏现在的时间
	t.second =(t2-t1)/1000;
	calTime();
	sprintf(str,"%02d:%02d:%02d",t.hour,t.minute,t.second);
	outtextxy(MINESIZE*COL+20,350,str);
}

void keepon()
{
	setcolor(WHITE);
	setfillstyle(BS_HATCHED, HS_BDIAGONAL);
	setfillcolor(RGB(190,190,190));
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	fillrectangle(MINESIZE*COL+20,250,MINESIZE*COL+100,300);
	outtextxy(MINESIZE*COL+30,260,"继续");
	setfillstyle(BS_SOLID);
}

int game()
{
	MOUSEMSG m;
	while(1)
	{
		int row;
		int col;

		if(onoff == 0)
			printTime();
		else if(onoff == 1)//如果点过暂停
		{
			t1=t1+(t4-t3);//弥补暂停的时间差
			printTime();
		}
		onoff = 0;//恢复成继续状态

		m=GetMouseMsg();
		if(m.x > 0 && m.x < MINESIZE*COL && m.y > 0 && m.y < MINESIZE*ROW)//如果鼠标指针在左边游戏区
		{
			//挪动鼠标，区块变色
			static int lastx=1,lasty=1;//lastx，lasty记录上一次变成浅色的格子，并将其复原
			if(map[lasty/MINESIZE+1][lastx/MINESIZE+1] >= 19 && map[lasty/MINESIZE+1][lastx/MINESIZE+1] <= 28)
			darkGrey(lasty/MINESIZE+1,lastx/MINESIZE+1);
			if(map[m.y/MINESIZE+1][m.x/MINESIZE+1] >= 19 && map[m.y/MINESIZE+1][m.x/MINESIZE+1] <= 28)//把目前鼠标所指的格子变成浅色
			{
				lightGrey(m.y/MINESIZE+1,m.x/MINESIZE+1);
				lastx=m.x;
				lasty=m.y;
			}

			//左键按下
			if(m.uMsg == WM_LBUTTONDOWN)
			{
				row=m.y/MINESIZE+1;
				col=m.x/MINESIZE+1;
				if(map[row][col] >= 1 && map[row][col] <= 8)//如果是已经翻开的数字方块
				{
					int i,j,flagNum = 0, match=0;//flagNum记录周围旗子数目，match记录旗子有雷的数目
					for(i = row-1; i <= row+1; i++)
						for(j = col-1; j <= col+1; j++)//遍历周围的格子
						{
							if(i == row && j == col)//除去中间的格子
								continue;
							if(i >= 1 && i <= ROW && j >= 1 && j <= COL)
							{
								if(map[i][j] > 30)
									flagNum++;
								if(map[i][j] == 69)
									match++;
							}
						}
					if(flagNum == match && map[row][col] == flagNum && match == map[row][col])//如果周围旗子的数量=旗子和雷匹配的数量=周围雷的数量
					{
						for(i = row-1; i <= row+1; i++)
							for(j = col-1; j <= col+1; j++)//遍历周围的格子
							{
								if(i == row && j == col)//除去中间的格子
									continue;
								if(i >= 1 && i <= ROW && j >= 1 && j <= COL)//防止越界
									if(map[i][j] >= 20 && map[i][j] <= 28)//如果是未翻开的数字
									{
										if(map[i][j] == 20)//如果数字是0
										{
											openZero(i,j);
										}
										else
										{
											map[i][j]-=20;
											count++;
										}
									}
							}
					}
					else if(flagNum == map[row][col] && match < flagNum)//如果周围旗子数目等于周围雷的数目，而匹配的数目小于旗子数目（说明有旗子标错了）
						fail();
					else//点击翻开的数字，将数字周围未翻开的方块标记出来
					{
						for(i = row-1; i <= row+1; i++)
							for(j = col-1; j <= col+1; j++)
							{
								if(i == row && j == col)
									continue;
								if(i >= 1 && i <= ROW && j >= 1 && j <= COL && map[i][j] >= 19 && map[i][j] <= 28)
									lightGrey(i,j);
							}
						Sleep(150);
						for(i = row-1; i <= row+1; i++)
							for(j = col-1; j <= col+1; j++)
							{
								if(i == row && j == col)
									continue;
								if(i >= 1 && i <= ROW && j >= 1 && j <= COL && map[i][j] >= 19 && map[i][j] <= 28)//防止越界+限定范围在未翻开的方块上
									darkGrey(i,j);
							}
					}
				}
				if(map[row][col] >= 19 && map[row][col] <= 28)//如果点开的是未翻开的格子
				{
					if(map[row][col] == 20)
					{
						openZero(row,col);//0的话调用开0函数
					}
					else
					{
						map[row][col]-=20;
						count++;
					}
					
				}
				return map[row][col];//返回值用于判断胜利
			}
			else if(m.uMsg == WM_RBUTTONDOWN)//右键按下，标记旗子
			{
				row=m.y/MINESIZE+1;
				col=m.x/MINESIZE+1;
				if(map[row][col] >= 19 && map[row][col] <= 28)
					map[row][col]+=50;
				else if(map[row][col] > 30)
					map[row][col]-=50;
				return map[row][col];
			}

		}
		else if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 50 && m.y < 100)//如果鼠标重来按钮内
		{
			if(m.uMsg == WM_LBUTTONDOWN)
			{
				init();
				draw();
				t1=clock();
			}
		}
		else if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 250 && m.y < 300 && m.uMsg == WM_LBUTTONDOWN)//如果鼠标点击暂停
		{
			printf("\a");
			onoff = 1;//1代表点过暂停
			t3 = clock();//记录点击暂停的时间
			setcolor(BLACK);//用黑色覆盖原来的暂停按钮
			setfillcolor(BLACK);
			fillrectangle(MINESIZE*COL+20,250,MINESIZE*COL+100,300);
			keepon();//打印继续按钮
			MOUSEMSG m;
			while(1)
			{
				m=GetMouseMsg();
				if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 250 && m.y < 300 && m.uMsg == WM_LBUTTONDOWN)//如果点击继续
				{
					printf("\a");
					t4=clock();//记录继续的时间
					setcolor(BLACK);
					setfillcolor(BLACK);
					fillrectangle(MINESIZE*COL+20,250,MINESIZE*COL+100,300);//覆盖掉原来的继续按钮
					stop();//打印暂停按钮
					break;
				}
			}
		}
		else if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 150 && m.y < 200 && m.uMsg == WM_LBUTTONDOWN)//点击退出
		{
			closegraph();
			exit(0);
		}
	}
		
}

void judge()
{
	if(game()==-1)
	{
		fail();
	}
	if(ROW*COL-NUM == count)
	{
		FILE *fp1,*fp2;
		char str[50];
		long old;
		t.second=(t2-t1)/1000;
		calTime();
		sprintf(str,"恭喜你创下新纪录：%02d:%02d:%02d",t.hour,t.minute,t.second);
		if((fp1=fopen("record.txt","r")) == NULL)//判断先前有没有历史纪录的文件
		{
			fp2=fopen("record.txt","w");//没有就创建一个
			MessageBox(NULL,TEXT(str),"新纪录",MB_OK);//输出这个记录
			fprintf(fp2,"%d",t2-t1);
			fclose(fp2);
		}
		else
		{
			fscanf(fp1,"%d",&old);//用获取旧的记录
			if(t2-t1 < old)//看看有没有打破记录
			{	
				MessageBox(NULL,TEXT(str),"新纪录",MB_OK);
				fp2=fopen("record.txt","w");
				fprintf(fp2,"%d",t2-t1);
				fclose(fp2);
			}
			else//没打破记录
			{
				MessageBox(NULL,TEXT("恭喜你胜利了"),"胜利",MB_OK);
			}
			fclose(fp1);
		}
		closegraph();
		exit(0);
	}
}
