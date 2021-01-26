#include<graphics.h>            //调用easyx图形数据库
#include<stdio.h>
#include <conio.h>
#include<time.h>                //用于使用随机数
//玩家成绩
struct Player
{
	int grade;        //玩家成绩
	int len;          //记录蛇长度
	int speed;        //记录游戏速度
	int level;        //记录游戏关卡
	int num;		  //记录玩家序号
}player[100];
int n = 0;
//坐标
typedef struct point
{
	int x;
	int y;
}selfPOINT;
//🐍
struct Snake
{
	point xy[100];   //坐标
	int len;         //长度
	int position;    //方向
	int socre;
	point disspare;
	int speed;      //记录蛇的速度
	int bkcolor;
}snake;
//🍔
struct
{
	point fdxy;     //食物坐标
	point bigfdxy;  //设置奖励食物
	int flag;       //存在标记
	int grade;      //分数
}food;
//🚧
struct
{
	point fencexy[100]; //障碍物的坐标
	int fencenum;       //障碍物的数量
	int flag;			//障碍物的存在标记
}fence;

//枚举方向
enum position { up, down, left, right };
/*****************************************模块化功能设计***********************************/

//初始化🐍和玩家数据
void makesnake() {
	n++;
	snake.xy[2].x = 345;
	snake.xy[2].y = 285;

	snake.xy[1].x = 360;
	snake.xy[1].y = 285;

	snake.xy[0].x = 375;
	snake.xy[0].y = 285;

	snake.position = right;   //定义蛇头的初始方向为向右
	snake.len = 3;
	snake.socre = 0;
	player[n].num = n;
	player[n].grade = 0;
	player[n].len = 0;
}

//画🐍
void drawsnake() {
	for (int i = 0; i < snake.len; i++) {
		setlinecolor(BLACK);
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 15, snake.xy[i].y + 15);
	}

}

//让🐍动起来
void snakemove() {
	snake.disspare.x = snake.xy[snake.len - 1].x;
	snake.disspare.y = snake.xy[snake.len - 1].y;
	for (int i = snake.len; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}

	if (snake.bkcolor == 1) {
		setlinecolor(RGB(176, 196, 222));
		setfillcolor(RGB(176, 196, 222));
		fillrectangle(snake.disspare.x, snake.disspare.y, snake.disspare.x + 15, snake.disspare.y + 15);
	}
	if (snake.bkcolor == 2) {
		setlinecolor(RGB(255, 182, 193));
		setfillcolor(RGB(255, 182, 193));
		fillrectangle(snake.disspare.x, snake.disspare.y, snake.disspare.x + 15, snake.disspare.y + 15);
	}
	if (snake.bkcolor == 3) {
		setlinecolor(RGB(221, 160, 221));
		setfillcolor(RGB(221, 160, 221));
		fillrectangle(snake.disspare.x, snake.disspare.y, snake.disspare.x + 15, snake.disspare.y + 15);
	}
	//🐍头的运动
	switch (snake.position) {
	case up:
		snake.xy[0].y -= 15;
		break;
	case down:
		snake.xy[0].y += 15;
		break;
	case left:
		snake.xy[0].x -= 15;
		break;
	case right:
		snake.xy[0].x += 15;
		break;
	}
	if (snake.xy[0].x > 885) snake.xy[0].x = 0;
	if (snake.xy[0].x < 0) snake.xy[0].x = 885;
	if (snake.xy[0].y > 585) snake.xy[0].y = 0;
	if (snake.xy[0].y < 0) snake.xy[0].y = 585;
}

//按键操作
void keydown() {
	char tap = _getch();
	switch (tap) {
	case'W':
	case'w':
	case 72:
		if (snake.position != down) {
			snake.position = up;
		}
		break;
	case'S':
	case's':
	case'80':
		if (snake.position != up) {
			snake.position = down;
		}
		break;
	case'A':
	case'a':
	case 75:
		if (snake.position != right) {
			snake.position = left;
		}
		break;
	case'D':
	case'd':
	case 77:
		if (snake.position != left) {
			snake.position = right;
		}
		break;
	}
}

//初始化🚧
void setfence() {
	fence.flag = 1; //表示🚧已存在
	for (int i = 0; i <= fence.fencenum; i++) {
		fence.fencexy[i].x = rand() % 60 * 15;//防止🚧出现在窗口外
		fence.fencexy[i].y = rand() % 40 * 15;
	}
	for (int i = 0; i < snake.len; i++) //防止🚧出现在🐍的身上
	{
		if (fence.fencexy[i].x == snake.xy[i].x && fence.fencexy[i].y == snake.xy[i].y) {
			setfence();
		}
	}
}

//初始化🍔
void setfood() {
	food.fdxy.x = rand() % 30 * 15;//防止🍔出现在窗口外
	food.fdxy.y = rand() % 20 * 15;
	food.flag = 1;//表示🍔存在
	if (food.grade % 5 == 0 && food.grade != 0) {
		food.bigfdxy.x = rand() % 30 * 15;//防止🍔出现在窗口外
		food.bigfdxy.y = rand() % 20 * 15;
		for (int i = 0; i < snake.len; i++) //防止🍔出现在🐍的身上
		{
			if (food.bigfdxy.x == snake.xy[i].x && food.fdxy.y == snake.xy[i].y) {
				setfood();
			}
		}
		for (int j = 0; j < fence.fencenum; j++) {//防止🍔出现在路障上 
			if (food.bigfdxy.x == fence.fencexy[j].x && food.bigfdxy.y == fence.fencexy[j].y) {
				setfood();
			}
		}
	}
	for (int i = 0; i < snake.len; i++) //防止🍔出现在🐍的身上
	{
		if (food.fdxy.x == snake.xy[i].x && food.fdxy.y == snake.xy[i].y) {
			setfood();
		}
	}
	for (int j = 0; j < fence.fencenum; j++) {//防止🍔出现在路障上 
		if (food.fdxy.x == fence.fencexy[j].x && food.fdxy.y == fence.fencexy[j].y) {
			setfood();
		}
	}
}

//画🚧
void drawfence() {
	for (int i = 0; i < fence.fencenum; i++) {
		setlinecolor(BLACK);
		setfillcolor(RGB(120, 120, 120));
		fillrectangle(fence.fencexy[i].x, fence.fencexy[i].y, fence.fencexy[i].x + 15, fence.fencexy[i].y + 15);
	}
}


//画🍔
void drawfood() {
	setlinecolor(BLACK);
	setfillcolor(RGB(255, 165, 0));
	fillrectangle(food.fdxy.x, food.fdxy.y, food.fdxy.x + 15, food.fdxy.y + 15);
	if (food.grade % 5 == 0 && food.grade != 0) {
		setlinecolor(BLACK);
		setfillcolor(RED);
		fillrectangle(food.bigfdxy.x, food.bigfdxy.y, food.bigfdxy.x + 15, food.bigfdxy.y + 15);
	}
}


//吃🍔
void eatfood() {
	if (snake.xy[0].x == food.fdxy.x && snake.xy[0].y == food.fdxy.y) {
		food.flag = 0;
		snake.len++;
		snake.socre++;
		food.grade++;
	}
	if (snake.xy[0].x == food.bigfdxy.x && snake.xy[0].y == food.bigfdxy.y) {
		snake.len++;
		snake.socre += 10;
		food.grade++;
	}
}


//🐍死亡
int snakedie() {
	for (int i = 1; i <= snake.len; i++) {
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y) {
			food.grade = 0;
			return 1;
		}
	}
	for (int i = 0; i < fence.fencenum; i++) {
		if (snake.xy[0].x == fence.fencexy[i].x && snake.xy[0].y == fence.fencexy[i].y) {
			food.grade = 0;
			return 1;
		}
	}
	return 0;
}
//显示结果
void printresult() {
	void play();
	void menu();
	void mousectrl();
	settextcolor(RGB(106, 90, 205));
	settextstyle(100, 0, "黑体");
	outtextxy(230, 60, "GAME OVER");
	char grade[20] = "";
	sprintf_s(grade, "grade:%d", snake.socre);
	player[n].grade = snake.socre;
	player[n].len = snake.len;
	settextcolor(RGB(106, 90, 205));
	settextstyle(100, 0, "黑体");
	outtextxy(260, 130, grade);

	/*settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 250, 510, 300);
	settextstyle(100, 0, "微软雅黑");
	settextstyle(50, 0, "微软雅黑");
	outtextxy(350, 250, "再来一次");*/

	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 300, 510, 350);
	settextcolor(RGB(106, 90, 205));
	settextstyle(40, 0, "微软雅黑");
	outtextxy(350, 300, "返回主菜单");

	MOUSEMSG mou;
	while (1) {
		mou = GetMouseMsg();

		/*//再来一次
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 250 && mou.y <= 300) {
			setlinecolor(BLACK);
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 250, 510, 300);
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 250, "再来一次");
			if (mou.mkLButton == true)
			{
				play();
			}
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 250 || mou.y >= 300) {
			setlinecolor(WHITE);
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 250, 510, 300);
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 250, "再来一次");
		}*/

		//返回主菜单
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 300 && mou.y <= 350) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 300, 510, 350);
			settextcolor(RGB(106, 90, 205));
			settextstyle(40, 0, "微软雅黑");
			outtextxy(350, 300, "返回主菜单");
			if (mou.mkLButton == true)
			{
				menu();
				mousectrl();
			}
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 300 || mou.y >= 350) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 300, 510, 350);
			settextcolor(RGB(106, 90, 205));
			settextstyle(40, 0, "微软雅黑");
			outtextxy(350, 300, "返回主菜单");
		}
	}
}
/*//显示分数
void printgrade() {
	char grade[20] = "";
	sprintf_s(grade, "grade:%d", snake.socre);
	settextcolor(LIGHTRED);
	settextstyle(25, 0, "黑体");
	outtextxy(780, 30, grade);
}*/


//建立主菜单
void menu() {
	initgraph(900, 600);
	setbkmode(TRANSPARENT);			 // 设置文字输出模式为透明
	setbkcolor(RGB(255, 228, 181));  //设置背景颜色
	cleardevice();

	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 250, 510, 300);
	settextstyle(100, 0, "微软雅黑");
	outtextxy(180, 120, "Greedy Snake");
	settextstyle(50, 0, "微软雅黑");
	outtextxy(350, 250, "开始游戏");

	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 300, 510, 350);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(350, 300, "选择关卡");

	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 350, 510, 400);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(350, 350, "玩家记录");

	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 400, 510, 450);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(380, 400, "设置");

	setfillcolor(RGB(200, 200, 200));
	fillrectangle(340, 450, 510, 500);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(380, 450, "退出");

	settextcolor(RGB(170, 170, 170));
	settextstyle(25, 0, "微软雅黑");
	outtextxy(360, 550, "王睿涵倾情制作");
}

//建立关卡菜单
void checkpoint() {
	cleardevice();
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(100, 0, "微软雅黑");
	outtextxy(180, 120, "Greedy Snake");

	setfillcolor(RGB(200, 200, 200));
	setlinecolor(WHITE);
	fillrectangle(370, 350, 510, 400);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 350, "简单");

	setlinecolor(WHITE);
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(370, 400, 510, 450);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 400, "普通");

	setlinecolor(WHITE);
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(370, 450, 510, 500);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 450, "困难");
}

//鼠标控制（关卡）
void levelctrl() {
	void mousectrl();
	void play();
	MOUSEMSG level;
	while (1) {
		level = GetMouseMsg();
		//简单
		if (level.x >= 370 && level.x <= 510 && level.y >= 350 && level.y <= 400) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 350, 510, 400);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 350, "简单");
			if (level.mkLButton == true)
			{
				fence.fencenum = 10;
				snake.bkcolor = 1;
				player[n].level = 1;
				play();
			}
		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 350 || level.y >= 400) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 350, 510, 400);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 350, "简单");
		}
		//普通
		if (level.x >= 370 && level.x <= 510 && level.y >= 400 && level.y <= 450) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 400, 510, 450);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 400, "普通");
			if (level.mkLButton == true)
			{
				fence.fencenum = 20;
				snake.bkcolor = 2;
				player[n].level = 2;
				play();
			}

		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 400 || level.y >= 450) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 400, 510, 450);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 400, "普通");
		}
		//困难
		if (level.x >= 370 && level.x <= 510 && level.y >= 450 && level.y <= 500) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 450, 510, 500);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 450, "困难");
			if (level.mkLButton == true)
			{
				fence.fencenum = 30;
				snake.bkcolor = 3;
				player[n].level = 3;
				play();
			}

		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 450 || level.y >= 500) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 450, 510, 500);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 450, "困难");
		}
	}

}

//建立设置菜单
void setting() {
	cleardevice();
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(100, 0, "微软雅黑");
	outtextxy(180, 120, "Greedy Snake");

	setfillcolor(RGB(200, 200, 200));
	setlinecolor(WHITE);
	fillrectangle(370, 350, 510, 400);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 350, "慢速");

	setlinecolor(WHITE);
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(370, 400, 510, 450);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 400, "中速");

	setlinecolor(WHITE);
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(370, 450, 510, 500);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 450, "快速");

	setlinecolor(WHITE);
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(370, 500, 510, 550);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 500, "返回");
}

//鼠标控制（设置）
void settingctrl() {
	void mousectrl();
	void menu();
	MOUSEMSG level;
	while (1) {
		level = GetMouseMsg();
		//简单
		if (level.x >= 370 && level.x <= 510 && level.y >= 350 && level.y <= 400) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 350, 510, 400);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 350, "慢速");
			if (level.mkLButton == true)
			{
				snake.speed = 0;
			}
		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 350 || level.y >= 400) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 350, 510, 400);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 350, "慢速");
		}
		//普通
		if (level.x >= 370 && level.x <= 510 && level.y >= 400 && level.y <= 450) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 400, 510, 450);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 400, "中速");
			if (level.mkLButton == true)
			{
				snake.speed = 20;
			}

		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 400 || level.y >= 450) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 400, 510, 450);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 400, "中速");
		}
		//困难
		if (level.x >= 370 && level.x <= 510 && level.y >= 450 && level.y <= 500) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 450, 510, 500);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 450, "快速");
			if (level.mkLButton == true)
			{
				snake.speed = 40;
			}

		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 450 || level.y >= 500) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 450, 510, 500);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 450, "快速");
		}


		//返回
		if (level.x >= 370 && level.x <= 510 && level.y >= 500 && level.y <= 550) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 500, 510, 550);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 500, "返回");
			if (level.mkLButton == true)
			{
				cleardevice();
				menu();
				mousectrl();
			}

		}
		if (level.x <= 370 || level.x >= 510 || level.y <= 500 || level.y >= 550) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 500, 510, 550);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 500, "返回");
		}
	}
}

//鼠标控制(主菜单)
void mousectrl() {
	void play();
	void leaderboard();
	void learderboardctrl();
	MOUSEMSG mou;
	while (1)
	{
		mou = GetMouseMsg();
		//退出游戏（关闭窗口）
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 450 && mou.y <= 500) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 450, 510, 500);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(380, 450, "退出");
			if (mou.mkLButton == true)
				closegraph();
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 450 || mou.y >= 500) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 450, 510, 500);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(380, 450, "退出");
		}

		//开始游戏
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 250 && mou.y <= 300) {
			setlinecolor(BLACK);
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 250, 510, 300);
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 250, "开始游戏");
			if (mou.mkLButton == true)
			{
				checkpoint();
				levelctrl();
			}
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 250 || mou.y >= 300) {
			setlinecolor(WHITE);
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 250, 510, 300);
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 250, "开始游戏");
		}

		//关卡选择
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 300 && mou.y <= 350) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 300, 510, 350);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 300, "选择关卡");
			if (mou.mkLButton == true)
			{
				checkpoint();
				levelctrl();
			}
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 300 || mou.y >= 350) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 300, 510, 350);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 300, "选择关卡");
		}

		//排行榜
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 350 && mou.y <= 400) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 350, 510, 400);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 350, "玩家记录");
			if (mou.mkLButton == true)
			{
				leaderboard();
				learderboardctrl();
			}
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 350 || mou.y >= 400) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 350, 510, 400);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(350, 350, "玩家记录");
		}

		//设置
		if (mou.x >= 340 && mou.x <= 510 && mou.y >= 400 && mou.y <= 450) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 400, 510, 450);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(380, 400, "设置");
			if (mou.mkLButton == true)
			{
				setting();
				settingctrl();
			}
		}
		if (mou.x <= 340 || mou.x >= 510 || mou.y <= 400 || mou.y >= 450) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(340, 400, 510, 450);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(380, 400, "设置");
		}
	}
	_getch();
}


//玩家记录界面
void leaderboard() {
	cleardevice();
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(350, 10, "玩家记录");

	//序号Title
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(30, 0, "微软雅黑");
	outtextxy(100, 60, "序号");

	//得分Title
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(30, 0, "微软雅黑");
	outtextxy(250, 60, "得分");

	//长度Title
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(30, 0, "微软雅黑");
	outtextxy(400, 60, "长度");

	//难度Title
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(30, 0, "微软雅黑");
	outtextxy(550, 60, "难度");


	//速度Title
	settextcolor(RGB(106, 90, 205));
	setfillcolor(RGB(200, 200, 200));
	settextstyle(30, 0, "微软雅黑");
	outtextxy(700, 60, "速度");


	setlinecolor(WHITE);
	setfillcolor(RGB(200, 200, 200));
	fillrectangle(370, 550, 510, 600);
	settextcolor(RGB(106, 90, 205));
	settextstyle(50, 0, "微软雅黑");
	outtextxy(400, 550, "返回");

	//载入玩家数据
	char num[100][3], grade[100][10], len[100][10];
	for (int i = 1; i <= n; i++) {
		//序号
		sprintf_s(num[i], "%d", player[i].num);
		settextcolor(RGB(106, 90, 205));
		setfillcolor(RGB(200, 200, 200));
		settextstyle(30, 0, "微软雅黑");
		outtextxy(115, 60 + 30 * i, num[i]);

		//得分
		sprintf_s(grade[i], "%d", player[i].grade);
		settextcolor(RGB(106, 90, 205));
		setfillcolor(RGB(200, 200, 200));
		settextstyle(30, 0, "微软雅黑");
		outtextxy(265, 60 + 30 * i, grade[i]);

		//长度
		sprintf_s(len[i], "%d", player[i].len);
		settextcolor(RGB(106, 90, 205));
		setfillcolor(RGB(200, 200, 200));
		settextstyle(30, 0, "微软雅黑");
		outtextxy(415, 60 + 30 * i, len[i]);

		//难度
		if (player[i - 1].level == 1)
		{
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			settextstyle(30, 0, "微软雅黑");
			outtextxy(550, 60 + 30 * i, "简单");
		}
		if (player[i - 1].level == 2)
		{
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			settextstyle(30, 0, "微软雅黑");
			outtextxy(550, 60 + 30 * i, "普通");
		}
		if (player[i - 1].level == 3)
		{
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			settextstyle(30, 0, "微软雅黑");
			outtextxy(550, 60 + 30 * i, "困难");
		}
		//速度
		if (player[i].speed == 1)
		{
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			settextstyle(30, 0, "微软雅黑");
			outtextxy(700, 60 + 30 * i, "慢速");
		}
		if (player[i].speed == 2)
		{
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			settextstyle(30, 0, "微软雅黑");
			outtextxy(700, 60 + 30 * i, "中速");
		}
		if (player[i].speed == 3)
		{
			settextcolor(RGB(106, 90, 205));
			setfillcolor(RGB(200, 200, 200));
			settextstyle(30, 0, "微软雅黑");
			outtextxy(700, 60 + 30 * i, "快速");
		}
	}
}

//排行榜鼠标控制
void learderboardctrl() {
	MOUSEMSG leader;
	while (1)
	{
		leader = GetMouseMsg();
		//返回
		if (leader.x >= 370 && leader.x <= 510 && leader.y >= 550 && leader.y <= 600) {
			setlinecolor(BLACK);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 550, 510, 599);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 550, "返回");
			if (leader.mkLButton == true)
			{
				cleardevice();
				menu();
				mousectrl();
			}

		}
		if (leader.x <= 370 || leader.x >= 510 || leader.y <= 550 || leader.y >= 600) {
			setlinecolor(WHITE);
			setfillcolor(RGB(200, 200, 200));
			fillrectangle(370, 550, 510, 599);
			settextcolor(RGB(106, 90, 205));
			settextstyle(50, 0, "微软雅黑");
			outtextxy(400, 550, "返回");
		}
	}

}


//开始玩🐍
void play() {
	setbkmode(TRANSPARENT);			// 设置文字输出模式为透明
	if (snake.bkcolor == 1) setbkcolor(RGB(176, 196, 222));
	if (snake.bkcolor == 2) setbkcolor(RGB(255, 182, 193));
	if (snake.bkcolor == 3) setbkcolor(RGB(221, 160, 221));
	cleardevice();//清屏
	makesnake();
	drawsnake();
	setfood();
	setfence();
	while (1) {
		snakemove();
		drawsnake();
		if (fence.flag == 0) {
			setfence();
		}
		if (food.flag == 0) {
			setfood();
		}
		if (_kbhit())
		{
			Sleep(5);
			keydown();
		}
		if (snakedie())
		{
			cleardevice();
			printresult();
			_getch();
		}
		drawfence();
		drawfood();
		eatfood();
		//printgrade();
		if (100 - snake.speed - 0.51 * snake.socre == 100) player[n].speed = 1;
		if (100 - snake.speed - 0.51 * snake.socre == 80) player[n].speed = 2;
		if (100 - snake.speed - 0.51 * snake.socre == 60) player[n].speed = 3;
		Sleep(100 - snake.speed - 0.5 * snake.socre);
	}
	_getch();//防止闪屏，等待用户输入字符
}

int main() {
	menu();
	mousectrl();
	_getch();
	srand((unsigned int)time(NULL));  //随机数的使用
	return 0;
}
