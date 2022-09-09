#ifndef SNAKE_H
#define SNAKE_H

#define _XOPEN_SOURCE_EXTENDED

#define SNAKE_OPT
#undef  SNAKE_OPT

#ifdef _WIN32
#define _WIN32_WINNT 0x0600
#include <conio.h>
#include <Windows.h>
#else
#include <ncurses.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <locale.h>
#include <list>
#include <chrono>
#include <utility>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define FPS    11 
#define HEIGHT 50 
#define WIDTH  50
#define SYMBOLS	 " oπ°8Ω˅ƔΧ"
#define SCOREVIS "GAME OVER YOU REACHED SNAKELVL=%06lu"
#define BOUND 	  false 
#define OBSTACLES false 

using namespace std;
using namespace chrono;
enum symbol 
{
	EMPTY, SNAKE, FOOD 
};

enum input 
{
	UP = 'W', DOWN = 'S', LEFT = 'A', RIGHT = 'D' 
};

struct node
{
	int x, y;
	node(int posx, int posy);
};

class snake
{
	private:
		WINDOW * win;
		list<node> snk;
		int dx, dy;
		int screen[HEIGHT][WIDTH];
		inline bool bound(int x, int y);
		void spawn(int symbol);
		void init();
		void move();
		void clear();
		void clean();
		void score();
		void render();
		void center();
		void keystroke();
		uint64_t micro();

	public:
		void run();
};
#endif
