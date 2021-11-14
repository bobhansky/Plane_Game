#include<iostream>
#include<graphics.h>   //easyX
#include<mmsystem.h>   //	WIN32  multi media interface
#pragma comment(lib,"winmm.lib")     //加载库文件
//为了使用mciSendString()函数（这个函数可以播放音乐和视频） 这个函数在winmm.lib静态库里
using namespace std;


#define WIN_WIDTH 591  //591   864    the width and height  match the size of the background JPG
#define WIN_HEIGHT 864   // don't know why 591*864 is too big for my window
#define BULLET_NUM 15   //maximum bullet the plane can  shoot at a time
#define ENEMY_NUM 10


namespace image {
	IMAGE background;
	IMAGE player[2];
	IMAGE bullets[2];
	IMAGE enemy[4]; 
}

enum TYPE {
	BIG,
	SMALL
};

class Item {  // me   enemy    bullet
public:
	int x;
	int y;
	int width;
	int height;
	bool flag; // alive?

	union {
		//https://blog.csdn.net/u013066730/article/details/84638489  union stuff
		TYPE type;   // the enemy type   enemy unique attribute
		int score;  // player unique attribute
	}un;

	int HP; 
	
}player, bullets[BULLET_NUM], enemy[ENEMY_NUM];


DWORD t1, t2;  //unsigned long    this is used for time frequency counting    bullet
DWORD t3, t4;







//#####################################Bullet################################################
void CreateBullet() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullets[i].flag == false) {
			bullets[i].flag = true;
			bullets[i].x = player.x+42;  //about head position of the plane
			bullets[i].y = player.y;
			break;
		}
	}
}

void BulletMove(int speed) {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullets[i].flag == true) {
			bullets[i].y -= speed;
			if (bullets[i].y <= 0) bullets[i].flag = false;   //bullets out of screen
		}
	}
}
//#####################################Bullet END################################################


//#####################################Enemy################################################
//initialize the enemy type and hp
void enemyHP(int i) {
	if (rand() % 10 == 0) {   // 1/10 posibility  to produce a big enemy
		enemy[i].un.type = TYPE::BIG;
		enemy[i].HP = 3;
		enemy[i].width = 104;
		enemy[i].height = 148;
	}
	else {
		enemy[i].un.type = TYPE::SMALL;
		enemy[i].HP = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
}

// create enemy plane
void CreateEnemy() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].flag == false) {
			enemy[i].flag = true;
			enemy[i].x = rand() % (WIN_WIDTH - 100);    //100 : enemy image width
			enemy[i].y = 0;
			enemyHP(i);
			break; // if find a not appearing one then break;
		}
	}
}

void EnemyMove(int speed) {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].flag == true) {
			enemy[i].y += speed;

			if (enemy[i].y >= WIN_HEIGHT) {
				enemy[i].flag = false;
			}
		}

	}
}


void hitEnemy() {
	// loop over the enemies  check if it is alive
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (bullets[i].flag == true) {
			for (int j = 0; j < BULLET_NUM; j++) {
				if (bullets[j].flag == true) {
					if ((bullets[j].x >= enemy[i].x && bullets[j].x < enemy[i].x + enemy[i].width)
						&& (bullets[j].y > enemy[i].y && bullets[j].y <= enemy[i].y + enemy[i].height)) {

						bullets[j].flag = false;
						enemy[i].HP--;
						if (enemy[i].HP == 0) enemy[i].flag = false;

					}
				}
			}
		}
		
	}

	//loop over the bullets

	//check if bullet hits enemy
}

//#####################################Game################################################


// handle with different module
// 1. load the background jpg
void GameInit() {		//initialize
	//set random seed
	srand(GetTickCount());

	t1 = t2 = t3 = t4 = GetTickCount(); //get the ms since Windows system is open

	// Load music
	//Send String to multi media interface     media device interface
	mciSendString("open ./images/game_music.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);

	// load background  and player plane   and bullet   and enemy
	loadimage(&image::background, "./images/background.jpg");		//easyX func
	//player plane
	loadimage(&image::player[0], "./images/planeNormal_1.jpg");
	loadimage(&image::player[1], "./images/planeNormal_2.jpg");
	//bullets
	loadimage(&image::bullets[0], "./images/bullet1.jpg");
	loadimage(&image::bullets[1], "./images/bullet2.jpg");
	//enemy
	loadimage(&image::enemy[0], "./images/enemy_1.jpg");
	loadimage(&image::enemy[1], "./images/enemy_2.jpg");
	loadimage(&image::enemy[2], "./images/enemyPlane1.jpg");
	loadimage(&image::enemy[3], "./images/enemyPlane2.jpg");



	//initialize player
	player.x = WIN_WIDTH / 2;
	player.y = WIN_HEIGHT - 120;  //120: the height of the plane image
	player.flag = true;
	player.HP = 10;
	player.un.score = 0;

	//initialize bullet attribute
	for (int i = 0; i < BULLET_NUM; i++) {
		bullets[i].flag = false;   //at first the bullet is dead/not visible
	}

	//initialize enemy attribute
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].flag = false;   //at first the enemy is dead/not visible
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
	//enemy
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].flag == true) {
			if (enemy[i].un.type == TYPE::SMALL) {
				putimage(enemy[i].x, enemy[i].y, &image::enemy[0], NOTSRCERASE); //processing parameter
				putimage(enemy[i].x, enemy[i].y, &image::enemy[1], SRCINVERT);  //processing parameter
			}
			else if (enemy[i].un.type == TYPE::BIG) {
				putimage(enemy[i].x, enemy[i].y, &image::enemy[2], NOTSRCERASE); //processing parameter
				putimage(enemy[i].x, enemy[i].y, &image::enemy[3], SRCINVERT);  //processing parameter
			}
		}
	}
}


void GameControl(int speed) {
	//GetAsyncKeyState()    //WIN32 API
	if (GetAsyncKeyState(VK_UP) && player.y >= 0) {
		player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && player.y + 70 <= WIN_HEIGHT) {
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && player.x >= -40) {
		player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && player.x + 70 <= WIN_WIDTH) {
		player.x += speed;
	}
	//shoot
	if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 200) {
		CreateBullet();
		// shoot sound
		mciSendString("close shoot", 0, 0, 0);
		mciSendString("open ./images/f_gun.mp3 alias shoot", 0, 0, 0);
		mciSendString("play shoot", 0, 0, 0);

		t1 = t2;    //update time difference 
	}

	if (t4 - t3 > rand() % 500 + 500) {  //time gap is at least 500 ms
		CreateEnemy();
		t3 = t4;
	}


	t2 = GetTickCount();   //update time difference
	t4 = GetTickCount();
	hitEnemy();
	EnemyMove(1);
	BulletMove(1);

}
//#####################################Game END################################################




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