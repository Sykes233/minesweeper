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
*		�궨��			*
*************************/
#define ROW 16			//�ж�����
#define COL 16			//�ж�����
#define NUM 50			//����
#define MINESIZE 30		//ÿ�����ӵĳ��Ϳ�

int count;//�㿪�ķ��׷�����Ŀ
int map[ROW+2][COL+2];
IMAGE img[3];
clock_t t1,t2,t3,t4;
int onoff;//��Ϸ��ͣ�ı�־
Time t;


void init();//��ʼ����ͼ�����ף����ܣ���ϸ�����ĵ���˵��
int first();//��Ϸ�׽���
void stop();//��ͣ��ť
void menu();//�Ҳ�˵�
void lightGrey(int i,int j);//ǳ��ɫ����Ļ���
void darkGrey(int i,int j);//���ɫ����Ļ���
void number(int n, int i, int j);//���ֵĻ���
void draw();//����
void openZero(int i, int j);//�Կ�������Ϊ0���ӵĴ���
void fail();//ʧ�ܵ���
void calTime();//������Ϸ����ʱ��
void printTime();//��ӡʱ��
void keepon();//��ӡ������ť
int game();//����������Ϸ�������������
void judge();//�ж���Ϸ�ĳɹ���ʧ��


int main()
{
	initgraph(COL*MINESIZE+160,ROW*MINESIZE);
//	initgraph(ROW*MINESIZE+160,COL*MINESIZE,SHOWCONSOLE);//�����һ��ʼ��֪���׵�λ�ã��Ͱ����ע��ȥ�����������Ǹ�ע�͵�

	if(first())//�ܽ��溯������һ�Ϳ�ʼ��Ϸ
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
	//ȫ������
	for(i = 0; i < ROW+2; i++)
		for(j = 0; j < COL+2; j++)
			map[i][j]=0;
	//����
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
	//����ÿ������������֣�����Χ�����Ƕ��٣�
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
	//����
	for(i = 1; i <= ROW; i++)
		for(j = 1; j <= COL; j++)
			map[i][j]+=20;
}

int first()
{
	loadimage(&img[2],"first.jpg",COL*MINESIZE+160,ROW*MINESIZE);//�׽���ͼƬ
	putimage(0,0,&img[2]);
	settextstyle(MINESIZE, 0, _T("����"));
	setbkmode(TRANSPARENT);
	while(1)
	{
		
		setcolor(WHITE);
		outtextxy(MINESIZE*COL/2-45,(ROW-2)*MINESIZE,"����������ʼ��Ϸ");
		Sleep(300);
		setcolor(BLACK);
		outtextxy(MINESIZE*COL/2-45,(ROW-2)*MINESIZE,"����������ʼ��Ϸ");
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
	outtextxy(MINESIZE*COL+30,260,"��ͣ");
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
	outtextxy(MINESIZE*COL+30,60,"����");
	outtextxy(MINESIZE*COL+30,160,"�˳�");
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
//*����ϵͳ�������һ��ʼ��֪���׵�λ�ã��Ͱ���Щע��ȥ��
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
						openZero(m,n);//�ݹ����
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
	MessageBox(NULL,"��Ϸʧ��","ʧ��",MB_OK);
	closegraph();
	exit(0);
}

void calTime()
{
	t.hour=0;
	t.minute=0;
	while(t.second >= 60)//�����������60������Ӧ����ת���ɷ���
	{
		t.second-=60;
		t.minute++;
	}
	while(t.minute >= 60)//����60�ķ���ת����Сʱ
	{
		t.minute -= 60;
		t.hour++;
	}
}

void printTime()
{
	char str[20];
	setfillcolor(BLACK);
	bar(MINESIZE*COL+10,350,MINESIZE*COL+160,480);//������һ�δ�ӡ��ʱ��
	setcolor(WHITE);
	t2=clock();//��Ϸ���ڵ�ʱ��
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
	outtextxy(MINESIZE*COL+30,260,"����");
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
		else if(onoff == 1)//��������ͣ
		{
			t1=t1+(t4-t3);//�ֲ���ͣ��ʱ���
			printTime();
		}
		onoff = 0;//�ָ��ɼ���״̬

		m=GetMouseMsg();
		if(m.x > 0 && m.x < MINESIZE*COL && m.y > 0 && m.y < MINESIZE*ROW)//������ָ���������Ϸ��
		{
			//Ų����꣬�����ɫ
			static int lastx=1,lasty=1;//lastx��lasty��¼��һ�α��ǳɫ�ĸ��ӣ������临ԭ
			if(map[lasty/MINESIZE+1][lastx/MINESIZE+1] >= 19 && map[lasty/MINESIZE+1][lastx/MINESIZE+1] <= 28)
			darkGrey(lasty/MINESIZE+1,lastx/MINESIZE+1);
			if(map[m.y/MINESIZE+1][m.x/MINESIZE+1] >= 19 && map[m.y/MINESIZE+1][m.x/MINESIZE+1] <= 28)//��Ŀǰ�����ָ�ĸ��ӱ��ǳɫ
			{
				lightGrey(m.y/MINESIZE+1,m.x/MINESIZE+1);
				lastx=m.x;
				lasty=m.y;
			}

			//�������
			if(m.uMsg == WM_LBUTTONDOWN)
			{
				row=m.y/MINESIZE+1;
				col=m.x/MINESIZE+1;
				if(map[row][col] >= 1 && map[row][col] <= 8)//������Ѿ����������ַ���
				{
					int i,j,flagNum = 0, match=0;//flagNum��¼��Χ������Ŀ��match��¼�������׵���Ŀ
					for(i = row-1; i <= row+1; i++)
						for(j = col-1; j <= col+1; j++)//������Χ�ĸ���
						{
							if(i == row && j == col)//��ȥ�м�ĸ���
								continue;
							if(i >= 1 && i <= ROW && j >= 1 && j <= COL)
							{
								if(map[i][j] > 30)
									flagNum++;
								if(map[i][j] == 69)
									match++;
							}
						}
					if(flagNum == match && map[row][col] == flagNum && match == map[row][col])//�����Χ���ӵ�����=���Ӻ���ƥ�������=��Χ�׵�����
					{
						for(i = row-1; i <= row+1; i++)
							for(j = col-1; j <= col+1; j++)//������Χ�ĸ���
							{
								if(i == row && j == col)//��ȥ�м�ĸ���
									continue;
								if(i >= 1 && i <= ROW && j >= 1 && j <= COL)//��ֹԽ��
									if(map[i][j] >= 20 && map[i][j] <= 28)//�����δ����������
									{
										if(map[i][j] == 20)//���������0
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
					else if(flagNum == map[row][col] && match < flagNum)//�����Χ������Ŀ������Χ�׵���Ŀ����ƥ�����ĿС��������Ŀ��˵�������ӱ���ˣ�
						fail();
					else//������������֣���������Χδ�����ķ����ǳ���
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
								if(i >= 1 && i <= ROW && j >= 1 && j <= COL && map[i][j] >= 19 && map[i][j] <= 28)//��ֹԽ��+�޶���Χ��δ�����ķ�����
									darkGrey(i,j);
							}
					}
				}
				if(map[row][col] >= 19 && map[row][col] <= 28)//����㿪����δ�����ĸ���
				{
					if(map[row][col] == 20)
					{
						openZero(row,col);//0�Ļ����ÿ�0����
					}
					else
					{
						map[row][col]-=20;
						count++;
					}
					
				}
				return map[row][col];//����ֵ�����ж�ʤ��
			}
			else if(m.uMsg == WM_RBUTTONDOWN)//�Ҽ����£��������
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
		else if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 50 && m.y < 100)//������������ť��
		{
			if(m.uMsg == WM_LBUTTONDOWN)
			{
				init();
				draw();
				t1=clock();
			}
		}
		else if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 250 && m.y < 300 && m.uMsg == WM_LBUTTONDOWN)//����������ͣ
		{
			printf("\a");
			onoff = 1;//1��������ͣ
			t3 = clock();//��¼�����ͣ��ʱ��
			setcolor(BLACK);//�ú�ɫ����ԭ������ͣ��ť
			setfillcolor(BLACK);
			fillrectangle(MINESIZE*COL+20,250,MINESIZE*COL+100,300);
			keepon();//��ӡ������ť
			MOUSEMSG m;
			while(1)
			{
				m=GetMouseMsg();
				if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 250 && m.y < 300 && m.uMsg == WM_LBUTTONDOWN)//����������
				{
					printf("\a");
					t4=clock();//��¼������ʱ��
					setcolor(BLACK);
					setfillcolor(BLACK);
					fillrectangle(MINESIZE*COL+20,250,MINESIZE*COL+100,300);//���ǵ�ԭ���ļ�����ť
					stop();//��ӡ��ͣ��ť
					break;
				}
			}
		}
		else if(m.x > MINESIZE*COL+20 && m.x < MINESIZE*COL+100 && m.y > 150 && m.y < 200 && m.uMsg == WM_LBUTTONDOWN)//����˳�
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
		sprintf(str,"��ϲ�㴴���¼�¼��%02d:%02d:%02d",t.hour,t.minute,t.second);
		if((fp1=fopen("record.txt","r")) == NULL)//�ж���ǰ��û����ʷ��¼���ļ�
		{
			fp2=fopen("record.txt","w");//û�оʹ���һ��
			MessageBox(NULL,TEXT(str),"�¼�¼",MB_OK);//��������¼
			fprintf(fp2,"%d",t2-t1);
			fclose(fp2);
		}
		else
		{
			fscanf(fp1,"%d",&old);//�û�ȡ�ɵļ�¼
			if(t2-t1 < old)//������û�д��Ƽ�¼
			{	
				MessageBox(NULL,TEXT(str),"�¼�¼",MB_OK);
				fp2=fopen("record.txt","w");
				fprintf(fp2,"%d",t2-t1);
				fclose(fp2);
			}
			else//û���Ƽ�¼
			{
				MessageBox(NULL,TEXT("��ϲ��ʤ����"),"ʤ��",MB_OK);
			}
			fclose(fp1);
		}
		closegraph();
		exit(0);
	}
}
