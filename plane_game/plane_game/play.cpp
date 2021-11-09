#include<iostream>
#include<graphics.h>   //easyX
#include<mmsystem.h>   //	WIN32  multi media interface
#pragma comment(lib,"winmm.lib")     //加载库文件
//为了使用mciSendString()函数（这个函数可以播放音乐和视频） 这个函数在winmm.lib静态库里
using namespace std;


#define WIN_WIDTH 500  //591   864    the width and height  match the size of the background JPG
#define WIN_HEIGHT 700   // don't know why 591*864 is too big for my window
#define BULLET_NUM 15   //maximum bullet the plane can  shoot at a time


namespace image {
	IMAGE background;
	IMAGE player[2];
	IMAGE bullets[2];
}


class Item {  // me   enemy    bullet
public:
	int x;
	int y;
	bool flag; // alive?
}player,bullets[BULLET_NUM];


// handle with different module
// 1. load the background jpg
void GameInit() {		//initialize

	// Load music
	//Send String to multi media interface     media device interface
	mciSendString("open ./images/game_music.mp3 alias BGM",0,0,0);	
	mciSendString("play BGM repeat", 0, 0, 0);

	// load background  and player plane   and bullet
	loadimage(&image::background, "./images/background.jpg");   //easyX
	loadimage(&image::player[0], "./images/planeNormal_1.jpg");
	loadimage(&image::player[1], "./images/planeNormal_2.jpg");
	loadimage(&image::bullets[0], "./images/bullet1.jpg");
	loadimage(&image::bullets[1], "./images/bullet2 .jpg");



	//initialize player
	player.x = WIN_WIDTH / 2;
	player.y = WIN_HEIGHT - 120;  //120: the height of the plane image
	player.flag = true;

	//initialize bullet attribute
	for (int i = 0; i < BULLET_NUM; i++) {
		bullets[i].flag = false;   //at first the bullet is dead/not visible
	}

}

void GameDraw() {
	//background
	putimage(0, 0, &image::background);  //put this outside will not refresh the back ground
	// player plane
	putimage(player.x, player.y, &image::player[0], NOTSRCERASE); //processing parameter
	putimage(player.x, player.y, &image::player[1], SRCINVERT);  //processing parameter
	//bullets
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullets[i].flag == true) {
			putimage(bullets[i].x, bullets[i].y, &image::bullets[0], NOTSRCERASE); //processing parameter
			putimage(bullets[i].x, bullets[i].y, &image::bullets[1], SRCINVERT);  //processing parameter
		}
	}
	

}

void CreateBullet() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullets[i].flag == false) {
			bullets[i].flag = true;
			bullets[i].x = player.x+50;  //about head position of the plane
			bullets[i].y = player.y;
			break;
		}
	}
}

void BulletMove(int speed) {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullets[i].flag == true) {
			bullets[i].y -= speed;
		}
	}
}

void GameControl(int speed) {
	//GetAsyncKeyState()    //WIN32 API
	if (GetAsyncKeyState(VK_UP)) {
		player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		player.x += speed;
	}
	//shoot
	if (GetAsyncKeyState(VK_SPACE)) {
		CreateBullet();
	}
	BulletMove(2);

}


int main() {
	initgraph(WIN_WIDTH, WIN_HEIGHT/*,1*/);  // width  height     1: show Console when showing window
	GameInit();

	BeginBatchDraw(); //双缓冲绘图  先在内存里画好，再显示
	while (true) {
		GameDraw();
		FlushBatchDraw();  //绘画 
		GameControl(1);
	}
	EndBatchDraw();  //结束
	return 0;
}