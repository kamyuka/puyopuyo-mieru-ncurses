/***** Sample Program for MieruEMB System v1.1                    *****/
/**********************************************************************/
/*
volatile char *e_vram = (char*)0x900000;
volatile int  *e_time = (int *)0x80010c;
volatile char *e_gp1  = (char*)0x8001f0;
volatile char *e_gp2  = (char*)0x8001f1;
volatile char *e_sw1  = (char*)0x8001fc;
volatile char *e_sw2  = (char*)0x8001fd;
volatile char *e_sw3  = (char*)0x8001fe;
volatile char *e_gin  = (char*)0x8001ff;
*/
#include "cfont.h"
/**********************************************************************/


#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


int sw_1 = 0, sw_2 = 0, sw_3 = 0;

unsigned int field[128][128] = {};

#define F_XPOS   1
#define F_YPOS   1
#define F_WIDTH  8
#define F_HEIGHT 15

unsigned int field_data[F_HEIGHT][F_WIDTH] = {
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,0,0,0,0,0,0,7},
{7,7,7,7,7,7,7,7}
};

unsigned int cpfield[F_HEIGHT][F_WIDTH];

#define C_NUM    8
#define C_WIDTH  8
#define C_HEIGHT 8

const unsigned int cell_data[C_NUM][C_HEIGHT][C_WIDTH] = {
{
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7},
{7,7,7,7,7,7,7,7}
},
{
{7,7,0,0,0,0,7,7}, //RED_puyo
{7,0,1,1,1,1,0,7},
{0,1,0,1,1,0,1,0},
{0,1,0,1,1,0,1,0},
{0,1,0,1,1,0,1,0},
{0,1,1,1,1,1,1,0},
{0,1,1,1,1,1,1,0},
{7,0,0,0,0,0,0,7}
},
{
{7,7,0,0,0,0,7,7}, //BLUE_puyo
{7,0,4,4,4,4,0,7},
{0,4,0,4,4,0,4,0},
{0,4,0,4,4,0,4,0},
{0,4,0,4,4,0,4,0},
{0,4,4,4,4,4,4,0},
{0,4,4,4,4,4,4,0},
{7,0,0,0,0,0,0,7}
},
{
{7,7,0,0,0,0,7,7}, //GREEN_puyo
{7,0,2,2,2,2,0,7},
{0,2,0,2,2,0,2,0},
{0,2,0,2,2,0,2,0},
{0,2,0,2,2,0,2,0},
{0,2,2,2,2,2,2,0},
{0,2,2,2,2,2,2,0},
{7,0,0,0,0,0,0,7}
},
{
{7,7,0,0,0,0,7,7}, //YELLOW_puyo
{7,0,3,3,3,3,0,7},
{0,3,0,3,3,0,3,0},
{0,3,0,3,3,0,3,0},
{0,3,0,3,3,0,3,0},
{0,3,3,3,3,3,3,0},
{0,3,3,3,3,3,3,0},
{7,0,0,0,0,0,0,7}
},
{
{7,7,0,0,0,0,7,7}, //PURPLE_puyo
{7,0,5,5,5,5,0,7},
{0,5,0,5,5,0,5,0},
{0,5,0,5,5,0,5,0},
{0,5,0,5,5,0,5,0},
{0,5,5,5,5,5,5,0},
{0,5,5,5,5,5,5,0},
{7,0,0,0,0,0,0,7}
},
{
{7,7,0,0,0,0,7,7}, //OJAMA_puyo
{7,0,7,7,7,7,0,7},
{0,7,0,7,7,0,7,0},
{0,7,0,7,7,0,7,0},
{0,7,0,7,7,0,7,0},
{0,7,7,7,7,7,7,0},
{0,7,7,7,7,7,7,0},
{7,0,0,0,0,0,0,7}
},
{
{4,4,4,4,4,4,4,4}, //WALL
{4,6,6,6,6,6,6,4},
{4,6,5,5,5,5,6,4},
{4,6,5,3,3,5,6,4},
{4,6,5,3,3,5,6,4},
{4,6,5,5,5,5,6,4},
{4,6,6,6,6,6,6,4},
{4,4,4,4,4,4,4,4}
}
};




void drawfield()
{
  int i, j;
  int flx, fly;

  for(i = 0; i < 128; i++){
    for(j = 0; j < 128; j++){

      fly = i;
      flx = j;	

      if(field[i][j] == 1){
        attrset(COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 2){
        attrset(COLOR_PAIR(2) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 3){
        attrset(COLOR_PAIR(3) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 4){
        attrset(COLOR_PAIR(4) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 5){
        attrset(COLOR_PAIR(5) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 6){
        attrset(COLOR_PAIR(6) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 7){
        attrset(COLOR_PAIR(7) | A_REVERSE | A_BOLD);
        mvaddch(fly, flx, ' ');
      }
      if(field[i][j] == 0 ){
        attrset(COLOR_PAIR(8));
        mvaddch(fly, flx, ' ');
      }
    }
  }
}


void Control()
{
  int key;

  key = getch();
  sw_1 = 0;
  sw_2 = 0;
  sw_3 = 0;
  switch (key) {
  case KEY_LEFT :    sw_1 =  1.0; break;
  case KEY_DOWN :    sw_2 =  1.0; break;
  case KEY_RIGHT :   sw_3 =  1.0; break;
  default: break;
  }
}


/**********************************************************************/

int block_x, block_y, block_c1=1, block_c2=2, n=0;
int b2_x=1, b2_y=0, b2_point=0; //block2の座標補助

int block_color[4]={1,2,3,4}; //使う色を入れておく

/**********************************************************************/
void mylib_put_cell(int x, int y, int n){
    int i,j;

    for(i=0; i<C_HEIGHT; i++)
        for(j=0; j<C_WIDTH; j++)
            field[(C_WIDTH*x+i+F_YPOS)][(C_HEIGHT*y+j+F_XPOS)] =  cell_data[n][i][j];
}

/**********************************************************************/
void mylib_put_field(){
    int i,j;

    for(i=0; i<F_HEIGHT; i++)
        for(j=0; j<F_WIDTH; j++)
            mylib_put_cell(i,j,field_data[i][j]);
}

/**********************************************************************/
/*
void mylib_msleep(unsigned int tm){
    unsigned int end = (unsigned int) *e_time + tm;
    while (*e_time < end);
}
*/
/**********************************************************************/
void mylib_clear(int color){
    int i, j;
    for(i=0; i<128; i++)
	for(j=0; j<128; j++)
	    field[i][j] = color;
}

/**********************************************************************/
unsigned int rand_seed = 1;

int my_rand(void){
    rand_seed = rand_seed*1103515245+12345;
    return (unsigned int)(rand_seed / 65536) % 32768;
}

/**********************************************************************/
void mylib_putc(int x, int y, char c, int color){
    int i, j;

    for(i=0; i<16; i++){
        for(j=0; j<8; j++){
            if(e_char[(int)(c-'A')][i][j]) 
                field[y+i][x+j] = color;
        }
    }
}
/**********************************************************************/
void mylib_putnum(int x, int y, int num, int color){
    int i, j;

    for(i=0; i<16; i++){
        for(j=0; j<8; j++){
            if(e_number[num][i][j]) field[y+i][x+j] = color;
        }
    }
}
/**********************************************************************/
///走査フィールドの作成
void mylib_copy_field(){
    int i, j;

    for(i=0; i<F_HEIGHT; i++)
        for(j=0; j<F_WIDTH; j++)
            cpfield[i][j] = field_data[i][j];
}

/**********************************************************************/
//いくつ繋がっているか調べる
void mylib_connect(int x, int y, int c){
    n++;
    cpfield[x][y] = 0;
    if(cpfield[x+1][y]==c) mylib_connect(x+1, y, c);
    if(cpfield[x][y+1]==c) mylib_connect(x, y+1, c);
    if(cpfield[x-1][y]==c) mylib_connect(x-1, y, c);
    if(cpfield[x][y-1]==c) mylib_connect(x, y-1, c);
}

/**********************************************************************/
//ブロックを消す
void mylib_vanish(int x, int y, int c){
    field_data[x][y] = 0;
    if(field_data[x+1][y]==c) mylib_vanish(x+1, y, c);
    if(field_data[x][y+1]==c) mylib_vanish(x, y+1, c);
    if(field_data[x-1][y]==c) mylib_vanish(x-1, y, c);
    if(field_data[x][y-1]==c) mylib_vanish(x, y-1, c);
}

/**********************************************************************/
//繋がっているところを消す一連の動作
int mylib_delete(){
    int i, j;
    int c;
    int delete = 0;

    mylib_copy_field();

    for(i=0; i<F_HEIGHT; i++){
        for(j=0; j<F_WIDTH; j++){
            c = cpfield[i][j];
            if((c != 0 && c != 7) && c != 8){
                n = 0;
                mylib_connect(i,j,c);
                if(n>=4){
                    mylib_vanish(i,j,c);
                    delete = 1;
                }
            }
        }
    }
    return delete;
}

/**********************************************************************/
//浮いているブロックを落とす
void mylib_fallen(){
    int i, j, k;
    int fall = 1;

    while(fall){
        fall = 0;
        for(i=1; i<F_WIDTH-1; i++){
            for(j=F_HEIGHT-1; j>0; j--){
                if(field_data[j][i] == 0 && field_data[j-1][i] != 0){
                    for(k=j; k>0; k--){
                        field_data[k][i] = field_data[k-1][i];
                    }
                    fall = 1;
                    break;
                }
            }
        }

        if(fall != 0){
            usleep(50000*5);
            mylib_put_field();

               erase();
                drawfield();
                refresh();
        }
    }
}

/**********************************************************************/
//連鎖した時の一連の動作
int mylib_rensa(){
    int rensa = 0;
    int delete = 1;

    while(delete == 1){
        mylib_fallen();
        delete = 0;
        delete = mylib_delete();
        rensa +=  delete;
        if(delete != 0){
		usleep(500000);
		mylib_put_field();

		erase();
		drawfield();
		refresh();

	}
    }
    
    return rensa;
}

/**********************************************************************/
/**********************************************************************/
//ブロックの座標を初期位置へ&色を変更
void mylib_set_block(){
    int rand;

    block_x = 3;
    block_y = 2;

    rand = my_rand();
    block_c1 = block_color[rand % 4];
    rand = my_rand();
    block_c2 = block_color[rand % 4]; 
}

/**********************************************************************/
/**********************************************************************/
//ブロックの動作
int mylib_move_block(int mvx, int rotate, int drop_cnt){
    int land=0;

    field_data[block_y][block_x] = 0;
    field_data[block_y+b2_y][block_x+b2_x] = 0;

    if(field_data[block_y][block_x+mvx] == 0 && field_data[block_y+b2_y][block_x+b2_x+mvx] == 0) block_x = block_x+mvx;

    if(rotate == 1){
        b2_point++;
        if(b2_point==4) b2_point = 0; 
        if(b2_point==0 && field_data[block_y][block_x+1]==0) b2_x=1, b2_y=0; 
        if(b2_point==1 && field_data[block_y+1][block_x]==0) b2_x=0, b2_y=1;
        if(b2_point==2 && field_data[block_y][block_x-1]==0) b2_x=-1, b2_y=0;
        if(b2_point==3 && field_data[block_y-1][block_x]==0) b2_x=0, b2_y=-1;
    }

    if(drop_cnt == 0){
        if(field_data[block_y+1][block_x] != 0 || field_data[block_y+b2_y+1][block_x+b2_x] != 0){
            land = 1;
        }else{
            block_y++;
        }
    }
    
    field_data[block_y][block_x] = block_c1;
    field_data[block_y+b2_y][block_x+b2_x] = block_c2;

    return land;
}

/**********************************************************************/
void mylib_fever();
void mylib_show_menu();
void mylib_show_success();
void mylib_show_rensa();
/**********************************************************************/
void mylib_game(int fever_level){
    int mvx = 0, rotate = 0, drop_cnt=0, land=0, rensa=0, success=0;

    if(fever_level != -1) mylib_fever(fever_level, 0);

    mylib_clear(7);
    mylib_put_field();
    mylib_set_block();

    while(1){

        drop_cnt++; //落ちるタイミングをカウント
        if(drop_cnt==10) drop_cnt=0;

 	Control();
        if(sw_1==1) mvx = -1;
        if(sw_3==1) mvx = +1;
        if(sw_2==1) rotate = 1;

        land = mylib_move_block(mvx, rotate, drop_cnt);
        mvx = 0;
        rotate = 0;
 
        if(land == 1){
            rensa = mylib_rensa();
            if(rensa != 0) mylib_show_rensa(rensa);
            if(fever_level != -1 && rensa != 0){
                if(rensa >= fever_level+5){
                    success = 1;
                }else{
                    success = -1;
                }
                if(rensa != 0) mylib_show_success(success);
                fever_level++;
                if(fever_level>=10) fever_level = 0;
                mylib_fever(fever_level, rensa);
            }
            mylib_set_block();
        }

        mylib_put_field(); //フィールドの描画

        erase();
        drawfield();
        refresh();

        usleep(50000); // sleep ~ msec
    }
}

/**********************************************************************/


/*
int main(){
    int fever_level;
    mylib_clear(7);

    while(1){
        rand_seed++;
        if(rand_seed == 1001) rand_seed = 1;
        mylib_show_menu();
        //if(*e_sw1==0) mylib_game(-1);
        //if(*e_sw2==0) fever_level++;
        //if(*e_sw3==0) mylib_game(fever_level);

        if(fever_level>=10) fever_level = 0;
        usleep(50);

    }
}
*/

int main(void)
{
    int fever_level;
    mylib_clear(7);
  
  /* curses の設定 */
  initscr();
  curs_set(0);		// カーソルを表示しない
  noecho();		// 入力されたキーを表示しない
  cbreak();		// 入力バッファを使わない(Enter 不要の入力)
  keypad(stdscr, TRUE);	// カーソルキーを使用可能にする
  
  /* カラーの設定 */	
  start_color();
  
  init_pair(1, COLOR_RED, COLOR_RED);	// 水色の■
  init_pair(2, COLOR_GREEN, COLOR_GREEN);	// 黄色の■
  init_pair(3, COLOR_YELLOW, COLOR_YELLOW);    	// 緑色の■
  init_pair(4, COLOR_BLUE, COLOR_BLUE);        	// 赤色の■
  init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);	// 青色の■
  init_pair(6, COLOR_CYAN, COLOR_CYAN);      	// 白色の■
  init_pair(7, COLOR_WHITE, COLOR_WHITE);  	// 紫色の■
  init_pair(8, COLOR_BLACK, COLOR_BLACK);    	// 黒色の■
  init_pair(9, COLOR_BLACK, COLOR_WHITE);	// 黒字/白地
  
  init_pair(10, COLOR_WHITE, COLOR_BLACK);	// 白字／黒地
  bkgd(COLOR_PAIR(10));

  /* ゲーム本体 */

	int mvx = 0;
	int fall_cnt=0;

	mylib_clear(7);

	int w, h;
	int key;
	int i, j;

	getmaxyx(stdscr, h, w);
	srand(time(NULL));
	timeout(0);

    while(1){
        rand_seed++;
        if(rand_seed == 1001) rand_seed = 1;
        mylib_show_menu();

	Control();
        if(sw_1==1) mylib_game(-1);
        if(sw_2==1) fever_level++;
        if(sw_3==1) mylib_game(fever_level);

	erase();
        drawfield(h, w);
	refresh();

        if(fever_level>=10) fever_level = 0;
        usleep(50);

    }
   
  /* 終了 */
  endwin();
  
  return 0;
  
}

/**********************************************************************/
void mylib_show_menu(){
    char title[] = {'P','U','Y','O','P','U','Y','O'};
    char moji1[] = {'T','O','K','O','T','O','N'};
    char moji2[] = {'F','E','V','E','R'};
    int i;    

    for(i=0; i<8; i++)
        mylib_putc(i*8+28, 24, title[i], 1);

    for(i=0; i<7; i++)
        mylib_putc(i*8, 72, moji1[i], 0);

    for(i=0; i<5; i++)
        mylib_putc(i*8+80, 72, moji2[i], 0); 

}

/**********************************************************************/
void mylib_show_success(int success){
    char moji1[] = {'S','U','C','C','E','S','S'};
    char moji2[] = {'F','A','I','L','D'};
    int i;

    if(success == 1)
        for(i=0; i<7; i++)
            mylib_putc(i*8+64, 60, moji1[i], 5);

    if(success == -1)
        for(i=0; i<5; i++)
            mylib_putc(i*8+72, 60, moji2[i], 1);
}

/**********************************************************************/
void mylib_show_rensa(int rensa){
    mylib_clear(7);
    mylib_putnum(72, 0, rensa/10, 4);
    mylib_putnum(80, 0, rensa%10, 4);
    mylib_putc(88, 0, 'R', 4);
    mylib_putc(96, 0, 'E', 4);
    mylib_putc(104, 0, 'N', 4);
    mylib_putc(112, 0, 'S', 4);
    mylib_putc(120, 0, 'A', 4);

}
/**********************************************************************/
void mylib_fever(int fever_level, int rensa){
    int i, j, tmp;
    int color[4]={1,2,3,4};
    int R,G,B,Y;
    
    for(i=0; i<4; i++){
        j = my_rand()%4;
        tmp = color[i];
        color[i] = color[j];
        color[j] = tmp;
    } 

    R = color[0];
    G = color[1];
    B = color[2];
    Y = color[3];

    unsigned int fever_data[10][F_HEIGHT][F_WIDTH] = {
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,Y,G,B,0,7},
        {7,0,G,B,R,B,0,7},
        {7,0,Y,G,B,R,0,7},
        {7,0,Y,G,B,R,B,7},
        {7,0,Y,G,B,R,B,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,B,0,R,0,G,7},
        {7,0,G,Y,R,B,G,7},
        {7,0,G,B,Y,B,G,7},
        {7,R,R,G,Y,R,B,7},
        {7,R,G,R,Y,G,B,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,G,R,0,0,0,0,7},
        {7,Y,G,G,G,0,0,7},
        {7,R,R,R,Y,R,0,7},
        {7,Y,Y,B,G,R,0,7},
        {7,Y,G,G,B,G,G,7},
        {7,G,B,B,G,R,R,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,B,0,0,0,7},
        {7,0,0,Y,R,G,0,7},
        {7,0,0,Y,Y,G,0,7},
        {7,0,B,B,R,R,B,7},
        {7,0,B,Y,R,G,G,7},
        {7,0,R,R,G,Y,B,7},
        {7,0,R,G,Y,B,B,7},
        {7,0,G,G,R,Y,Y,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,B,0,0,0,0,0,7},
        {7,R,0,0,0,0,0,7},
        {7,Y,Y,0,0,0,0,7},
        {7,R,Y,0,0,0,0,7},
        {7,R,R,0,0,0,0,7},
        {7,B,G,0,0,R,0,7},
        {7,B,B,0,0,R,0,7},
        {7,R,G,0,B,B,G,7},
        {7,G,G,B,G,B,G,7},
        {7,R,Y,G,Y,G,R,7},
        {7,R,R,Y,Y,G,R,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,B,7},
        {7,0,0,0,0,0,B,7},
        {7,B,0,0,0,0,B,7},
        {7,R,0,0,0,B,R,7},
        {7,G,0,0,0,G,R,7},
        {7,R,0,0,0,G,R,7},
        {7,R,G,0,0,G,Y,7},
        {7,R,B,Y,B,R,Y,7},
        {7,G,R,G,Y,B,G,7},
        {7,G,R,G,Y,B,Y,7},
        {7,G,R,G,Y,B,Y,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,B,0,0,0,0,0,7},
        {7,R,Y,B,R,0,0,7},
        {7,R,Y,R,R,0,0,7},
        {7,Y,B,B,Y,Y,0,7},
        {7,R,Y,B,R,Y,G,7},
        {7,R,G,R,Y,R,G,7},
        {7,B,G,G,Y,R,R,7},
        {7,B,B,R,R,Y,G,7},
        {7,Y,G,R,Y,R,G,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,0,Y,G,0,0,0,7},
        {7,G,Y,B,B,0,0,7},
        {7,Y,G,G,B,0,0,7},
        {7,Y,R,G,Y,0,0,7},
        {7,G,G,R,Y,R,0,7},
        {7,G,B,R,R,Y,Y,7},
        {7,R,Y,B,G,R,B,7},
        {7,B,Y,G,B,R,R,7},
        {7,Y,G,Y,G,Y,B,7},
        {7,Y,G,Y,Y,B,B,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,0,0,7},
        {7,Y,0,0,0,0,Y,7},
        {7,B,0,0,0,0,B,7},
        {7,Y,B,R,G,Y,B,7},
        {7,Y,R,R,Y,Y,B,7},
        {7,B,B,G,G,B,G,7},
        {7,Y,B,R,G,R,G,7},
        {7,Y,R,G,B,R,G,7},
        {7,B,Y,R,G,B,R,7},
        {7,B,Y,R,G,B,R,7},
        {7,B,Y,R,G,B,G,7},
        {7,7,7,7,7,7,7,7}
    },
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {7,0,0,0,0,0,0,7},
        {7,0,0,0,0,R,0,7},
        {7,0,0,G,B,Y,0,7},
        {7,B,R,G,Y,Y,0,7},
        {7,R,G,B,B,R,R,7},
        {7,R,G,Y,B,Y,R,7},
        {7,R,B,B,G,G,B,7},
        {7,G,R,B,Y,Y,Y,7},
        {7,Y,R,G,B,G,G,7},
        {7,G,G,B,B,Y,B,7},
        {7,R,R,G,Y,B,B,7},
        {7,Y,Y,Y,B,Y,Y,7},
        {7,7,7,7,7,7,7,7}
    }
    };

    for(i=0; i<F_HEIGHT; i++)
        for(j=0; j<F_WIDTH; j++)
            field_data[i][j] = fever_data[fever_level][i][j];
}
/**********************************************************************/
