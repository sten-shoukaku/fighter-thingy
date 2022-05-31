#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <math.h>
#include <time.h>

int linearCongruentialGenerator(int customSeed = -1){
    static unsigned long long int newState;
    static unsigned long long int multiplier = 1664525;
    static unsigned long long int increment = 1013904223;
    static unsigned long long int modulus = pow(2,32);
    static unsigned long long int seed = time(NULL);
    if(customSeed != -1){
        seed = customSeed;
    }
    newState = ((multiplier * seed) + increment) % modulus;
    seed = newState;
    return newState;
}

void clearScreen(){
	cleardevice();
}

void drawText(int x, int y, int bgColor, int textColor, char *text, int center = 0, int fontSize = 1, int font = 0, int fontDirection = 0){
	setbkcolor(bgColor);
	setcolor(textColor);
	settextjustify(center, 1);
	settextstyle(font, fontDirection, fontSize);
	outtextxy(x, y, text);
}

void drawButton(int left, int top, int right, int bottom, int bgColor, int textColor, char buttonText[20]){
	/*
	BLACK			0
	BLUE			1
	GREEN			2
	CYAN			3
	RED				4
	MAGENTA			5
	BROWN			6
	LIGHTGRAY		7
	DARKGRAY		8
	LIGHTBLUE		9
	LIGHTGREEN		10
	LIGHTCYAN		11
	LIGHTRED		12
	LIGHTMAGENTA	13
	YELLOW			14
	WHITE			15
	*/
	setfillstyle(SOLID_FILL, bgColor);
	bar(left, top, right, bottom);
	drawText(left + 0.5 * (right - left), (getmaxy() - (getmaxy() - top)) + 0.5 * (bottom - top), bgColor, textColor, buttonText, 1);
}

int leftClick(int left, int top, int right, int bottom){
	int x, y;
	int delayTime = 1;
	while(!ismouseclick(WM_LBUTTONDOWN)){
		delay(delayTime);
	}
	getmouseclick(WM_LBUTTONDOWN, x, y);
	if(left < x && x < right && top < y && y < bottom){
		return 1;
	} else {
		return 0;
	}
}

void drawEnemy(int lanePosition, int y_pos = 0){
	int height = getmaxy(), width = getmaxx() - (0.2 * getmaxx());
	int centerx, centery;
	setfillstyle(SOLID_FILL, RED);
	centerx = 0.5 * width + (lanePosition * 0.2 * width);
	fillellipse(centerx, y_pos, 0.03 * width, 0.03 * height);
	bar(centerx - 0.05 * width, y_pos - 0.03 * height, centerx - 0.025 * width, y_pos + 0.05 * height);
	bar(centerx + 0.025 * width, y_pos - 0.03 * height, centerx + 0.05 * width, y_pos + 0.05 * height);
}

void drawBullet(int lanePosition, int y_pos = 0){
	int height = getmaxy(), width = getmaxx() - (0.2 * getmaxx());
	int centerx, centery;
	setfillstyle(SOLID_FILL, BLUE);
	centerx = 0.5 * width + (lanePosition * 0.2 * width);
	fillellipse(centerx, y_pos, 0.01 * width, 0.01 * height);
}

void drawStats(int killCount){
	int height = getmaxy(), width = getmaxx();
	char killOneText[5] = "Kill";
	char killMoreText[6]= "Kills";
	char killCountText[10];
	snprintf(killCountText, 10, "%d", killCount);
	drawText(0.9 * width, height/2, 8, 15, killCountText, 1, 3, 3);
	if(killCount > 1 || killCount == 0){
		drawText(0.9 * width, height/2 + 0.075 * height, 8, 15, killMoreText, 1, 1, 3);
	} else {
		drawText(0.9 * width, height/2 + 0.075 * height, 8, 15, killOneText, 1, 1, 3);
	}
}

void startScreen(){
	int height = getmaxy(), width = getmaxx();
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(0, 0, width * 2, height * 2);
	drawText(width/2, height/2 - 0.1 * height, DARKGRAY, WHITE, "You Only Have One Bullet", 1, 3, 3);
	drawText(width/2, height/2 + 0.1 * height, DARKGRAY, WHITE, "Move Left = A, Move Right = D, Shoot = L", 1, 1, 3);
	drawText(width/2, height/2 + 0.3 * height, DARKGRAY, WHITE, "Press Any Key", 1, 1, 3);
	getch();
}

void deadScreen(int killCount){
	int height = getmaxy(), width = getmaxx();
//	char killOneText[5] = "Kill";
//	char killMoreText[6]= "Kills";
	char killText[20];
	if(killCount > 1 || killCount == 0){
		snprintf(killText, 20, "%d Kills", killCount);
	} else {
		snprintf(killText, 20, "%d Kill", killCount);
	}
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(0, 0.4 * height, width, 0.6 * height);
	setcolor(WHITE);
	line(0, 0.4 * height, width, 0.4 * height);
	line(0, 0.6 * height, width, 0.6 * height);
	drawText(0.5 * width, 0.47 * height, DARKGRAY, 15, "You're Dead", 1, 1, 3);
	drawText(0.5 * width, 0.55 * height, DARKGRAY, 15, killText, 1, 1, 3);
}

void drawOverlay(int killCount){
	int height = getmaxy(), width = getmaxx();
	setfillstyle(SOLID_FILL, DARKGRAY);
	bar(0.8 * width, 0, width * 2, height * 2);
	setfillstyle(SOLID_FILL, BLACK);
	drawStats(killCount);
	bar(0, 0, 0.8 * width, height * 2);
	setcolor(DARKGRAY);
	for(int i = 0; i < 5; i++){
		line(0.16 * (i + 1) * width, 0, 0.16 * (i + 1) * width, height);
	}
}

void drawPlayer(int position){
	int height = getmaxy(), width = getmaxx() - (0.2 * getmaxx());
	int centerx, centery;
	int wingPosition[12], bodyPosition[18];
	setfillstyle(SOLID_FILL, MAGENTA);
	centerx = 0.5 * width + (position * 0.2 * width);
	centery = 0.8 * height;
	// wings
	wingPosition[0] = centerx - 0.07 * width;
	wingPosition[1] = centery;
	wingPosition[2] = centerx + 0.07 * width;
	wingPosition[3] = centery;
	wingPosition[4] = centerx + 0.07 * width;
	wingPosition[5] = centery + 0.02 * height;
	wingPosition[6] = centerx;
	wingPosition[7] = centery + 0.04 * height;
	wingPosition[8] = centerx - 0.07 * width;
	wingPosition[9] = centery + 0.02 * height;
	wingPosition[10] = centerx - 0.07 * width;
	wingPosition[11] = centery;
	// body
	bodyPosition[0] = centerx - 0.005 * width;
	bodyPosition[1] = centery - 0.05 * height;
	bodyPosition[2] = centerx + 0.005 * width;
	bodyPosition[3] = centery - 0.05 * height;
	bodyPosition[4] = centerx + 0.01 * width;
	bodyPosition[5] = centery - 0.02 * height;
	bodyPosition[6] = centerx + 0.01 * width;
	bodyPosition[7] = centery + 0.05 * height;
	bodyPosition[8] = centerx + 0.005 * width;
	bodyPosition[9] = centery + 0.1 * height;
	bodyPosition[10] = centerx - 0.005 * width;
	bodyPosition[11] = centery + 0.1 * height;
	bodyPosition[12] = centerx - 0.01 * width;
	bodyPosition[13] = centery + 0.05 * height;
	bodyPosition[14] = centerx - 0.01 * width;
	bodyPosition[15] = centery - 0.02 * height;
	bodyPosition[16] = centerx - 0.005 * width;
	bodyPosition[17] = centery - 0.05 * height;
	fillpoly(6, wingPosition);
	fillpoly(9, bodyPosition);
}

char playerController(){
	char c;
	if(kbhit()){
		c = (char)getch();
	}
	if(c == 'd'){
		return('d');
	} else if (c == 'a'){
		return('a');
	} else if (c == 'l'){
		return('l');
	}
}
int getEnemyLane(int getData = 0, int lane = -10){
	static int xlane = -10;
	if(lane != -10 && getData == 0){
		xlane = lane;
		return 0;
	} else if(getData == 1){
		return xlane;
	} else {
		return 1;
	}
}

int enemyController(int override = 0){
	int height = getmaxy();
	long long unsigned int rn = linearCongruentialGenerator();
	long long unsigned int rlane = (rn % 5);
	static int xlane = 10;
	int lane = rlane - 2;
	static int alive = 0;
	static int y_pos = 0;
	if (xlane == 10){
		xlane = lane;
	}
	if (alive == 0){
		y_pos = 0;
		drawEnemy(xlane, y_pos);
		alive = 1;
	} else {
		y_pos += 0.01 * height;
		drawEnemy(xlane, y_pos);
		if (y_pos > 0.75 * height || override == 1){
			xlane = 10;
			alive = 0;
		}
	}
	if(xlane != 10){
		getEnemyLane(0, xlane);
	}
	return y_pos;
}

int getBulletPosition(int getData = 0, int position = -1, int alive = 0){
	static int bulletPosition = -10;
	if(position != -1 && getData == 0 && alive == 1){
		bulletPosition = position;
		return 0;
	} else if(getData == 0 && alive == 0){
		bulletPosition = -10;
		return 0;
	} else if(getData == 1){
		return bulletPosition;
	} else {
		bulletPosition = -10;
		return 1;
	}
}

int shoot(int lane, int isShooting = 0, int override = 0){
	int height = getmaxy();
	static int alive = 0;
	static int y_pos = 0.8 * height;
	static int xlane = 10;
	if (xlane == 10){
		xlane = lane;
	}
	if (alive == 0 && isShooting == 1){
		y_pos = 0.8 * height;
		drawBullet(xlane, y_pos);
		alive = 1;
	} else if (alive == 1){
		y_pos -= 0.01 * height;
		drawBullet(xlane, y_pos);
		if (y_pos < 0 || override == 1){
			alive = 0;
			xlane = 10;
		}
	}
	getBulletPosition(0, y_pos, alive);
	return alive;
}

void update(){
	int height = getmaxy(), width = getmaxx();
	int playerInput = 0;
	int isShooting = 0;
	int temp, temp1;
	static int alive = 1;
	static int bulletCondition = 0;
	static int killCount = 0;
	static int position = 0;
	
	static int enemyLane = -10;
	static int enemyPosition = -10;
	static int bulletPosition = -10;
	static int bulletLane = -10;
	static int override = 0;
	int bulletTouchEnemy = 0;
	
	if(alive == 1){
		drawOverlay(killCount);
		drawPlayer(position);
		enemyPosition = enemyController(override);
		override = 0;
		temp = getBulletPosition(1);
		if(temp != -10){
			bulletPosition = temp;
		} else {
			bulletPosition = -10;
		}
		temp1 = getEnemyLane(1);
		if(temp1 != -10){
			enemyLane = temp1;
		} else {
			enemyLane = -10;
		}
		if(bulletPosition <= (enemyPosition + (0.03 * height)) && bulletPosition >= (enemyPosition - (0.03 * height))){
			bulletTouchEnemy = 1;
		}
		if(bulletTouchEnemy == 1 && bulletLane == enemyLane && enemyPosition != -10 && enemyLane != -10 && bulletPosition != -10 && bulletLane != -10){
			override = 1;
			killCount += 1;
		}
		playerInput = playerController();
		switch(playerInput){
			case 'a':
				if(position > -2){
					position -= 1;
				}
				break;
			case 'd':
				if(position < 2){
					position += 1;
				}
				break;
			case 'l':
				isShooting = 1;
				break;
		}
		if(isShooting == 1 && bulletCondition == 0){
			bulletCondition = shoot(position, 1, override);
			isShooting = 0;
			bulletLane = position;
		} else if (bulletCondition == 1){
			bulletCondition = shoot(position, 0, override);
		}
		if(enemyPosition > (0.75 * height)){
			alive = 0;
		}
		delay(15); //should be about 66.6-ish fps
		update();
	} else {
		deadScreen(killCount);
	}
}

void start(){
	int height = 360, width = 640;
	initwindow(width, height, "Fighter Things");
	startScreen();
	update();
}

int main(){
	start();
	getch();
	return 0;
}
