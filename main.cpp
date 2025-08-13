#include<stdio.h>
#include<graphics.h>
#include"tools.h"
const int WIN_WIDTH = 900;
const int WIN_HEIGHT = 600;
IMAGE imgBg;
IMAGE imgBar;

enum {WAN_DOU,XIANG_RI_KUI,ZHI_WU_COUNT};
IMAGE imgCards[ZHI_WU_COUNT];
IMAGE* imgZhiWu[ZHI_WU_COUNT][20];
int curX, curY;
int curZhiWu;
bool fileExist(const char* name)
{
	FILE* fp = fopen(name, "r");
	if (fp == NULL)
	{
		return false;

	}
	else {
		fclose(fp);
		return true;
	}

}

void gameInit()
{
	loadimage(&imgBg, "res/bg.jpg");//将图片变量加载到内存变量
	loadimage(&imgBar, "res/bar5.png");
	memset(imgZhiWu, 0, sizeof(imgZhiWu));
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++)
	{
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i+1);
		loadimage(&imgCards[i], name);
		for (int j = 0; j < 20; j++)
		{
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i ,j+ 1);
			if (fileExist(name))
			{
				imgZhiWu[i][j] = new IMAGE;
				loadimage(imgZhiWu[i][j], name);

			}
			else {
				break;
			}

		}

	}
	curZhiWu = 0;
	initgraph(WIN_WIDTH, WIN_HEIGHT,1);


}
void UpdateWindow()
{
	BeginBatchDraw();
	putimage(0, 0, &imgBg);
	putimagePNG(250, 0, &imgBar);
	for (int i = 0; i < ZHI_WU_COUNT; i++)
	{
		int x = 338 + i * 65;
		int y = 5;
		putimage(x, y, &imgCards[i]);
	}
	if (curZhiWu>0) {
		IMAGE* img = imgZhiWu[curZhiWu - 1][0];
		putimagePNG(curX-img->getwidth()/2, curY-img->getheight()/2, img);

	}
	EndBatchDraw();

}
void userClick()
{
	ExMessage msg;
	static int status = 0;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT&&msg.y<96)
			{
				int index = (msg.x - 338)/65;
				status = 1;
				curZhiWu = index + 1;


			}

		}
		else if (msg.message==WM_MOUSEMOVE&&status==1)
		{
			curX = msg.x;
			curY = msg.y;


		}
		else if (msg.message == WM_LBUTTONUP)
		{

		}
	}

}
int main()
{
	gameInit();
	while (1)
	{
		userClick();
		UpdateWindow();
	}
	
	system("pause");
}