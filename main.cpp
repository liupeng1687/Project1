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
struct zhiwu {
	int type;
	int frameindex;

};
struct zhiwu map[3][9];

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
	memset(map, 0, sizeof(map));
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
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (map[i][j].type > 0)
			{
				int x = 256 + j * 81;
				int y = 179 + i * 102+14;
				int zhiWuType = map[i][j].type-1;
				int index = map[i][j].frameindex;
				putimagePNG(x, y, imgZhiWu[zhiWuType][index]);

			}
		}
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

			//判断是否属于非法位置
			if (msg.x > 256 && msg.y > 179 && msg.y < 489) {
				int row = (msg.y - 179) / 102;
				int col = (msg.x - 256) / 81;
				if (map[row][col].type == 0)
				{
					map[row][col].type = curZhiWu;
					map[row][col].frameindex = 0;
				}
			}
			curZhiWu = 0;
			status = 0;

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