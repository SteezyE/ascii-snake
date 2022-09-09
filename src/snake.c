#include "snake.h"

node::node(int posx, int posy) : x(posx), y(posy) {}

inline bool snake::bound(int x, int y)
{
	return !x || !y || x==HEIGHT-1 || y==WIDTH-1;
}

void snake::clear()
{
#if BOUND
	for(int i=0; i<HEIGHT; i++)
		for(int j=0; j<WIDTH; j++)
			 screen[i][j] = bound(i,j);
#else 
	memset(screen, 0, sizeof(screen));
#endif
}

void snake::center()
{
	dx = -1, dy = 0;
	node mid(HEIGHT>>1, WIDTH>>1);
	snk.push_back(mid);
	screen[mid.x][mid.y] = SNAKE;
}

void snake::score()
{
	char format[WIDTH+1] = {0};
	wchar_t visual[WIDTH+1] = {0};
	sprintf(format, SCOREVIS, snk.size()-1); 		
	mbstowcs(visual, format, strlen(format));
	mvwaddwstr(win, HEIGHT>>2, (WIDTH>>1)-(strlen(SCOREVIS)>>1)+1, visual); 
}

void snake::spawn(int symbol)
{
	int x, y;
	do
	{
		x = rand() % (HEIGHT-2) + 1;
		y = rand() %  (WIDTH-2) + 1;
	} while(screen[x][y] != EMPTY);
	screen[x][y] = symbol; 
}

void snake::init()
{
#ifdef SNAKE_OPT
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);
#endif
	setlocale(LC_ALL, "");
	if(!(win = initscr())) exit(EXIT_FAILURE);  
	srand(time(NULL));
	curs_set(0);
	cbreak();
	noecho();
	nodelay(win, true);
	wresize(win, (HEIGHT>>1)+2, WIDTH+2);
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	clear();	
	center();	
	spawn(FOOD);
}

void snake::render()
{
	wchar_t line[WIDTH+1] = {0}, symbols[10];
	mbstowcs(symbols, SYMBOLS, 9);
	for(int i=0; i<HEIGHT-1; i+=2)
	{
		for(int j=0; j<WIDTH; j++)
			line[j] = symbols[screen[i+1][j]+(screen[i][j]<<2)-screen[i][j]];
		mvwaddwstr(win, (i>>1)+1, 1, line);
	}
	refresh();
}

uint64_t snake::micro()
{
	return (uint64_t) duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();	
}

#define DIRECT(X,Y)	dx = X; \
			dy = Y; \
			break 

void snake::keystroke()
{
	int key = 0;
	uint64_t start = micro();
	while(1)
	{
		key = 0;	
		switch((key = toupper(getch())))
		{
			case UP:	DIRECT(-1, 0); 
			case DOWN:	DIRECT( 1, 0);
			case LEFT: 	DIRECT( 0,-1);
			case RIGHT:	DIRECT( 0, 1);
			default: 	break;
		}
		if(micro() - start >= 1e6/FPS) return;			
	}
}

void snake::move()
{
	node head = snk.front();		
	head.x = (head.x + dx + HEIGHT) % HEIGHT;
	head.y = (head.y + dy + WIDTH)  % WIDTH;
	if(screen[head.x][head.y] == FOOD)
	{
		snk.push_front(head);
		screen[head.x][head.y] = SNAKE;
		spawn(FOOD);
#if OBSTACLES
		spawn(SNAKE);
#endif
	}
	else if(screen[head.x][head.y] == EMPTY)
	{
		snk.push_front(head);
		node tail = snk.back();	
		screen[head.x][head.y] = SNAKE;
		screen[tail.x][tail.y] = EMPTY;
		snk.pop_back();
	}
	else 
	{
		score();
		nodelay(win, false);
		getch(); 
		getch();
		nodelay(win, true);
		clear();	
		snk.clear();
		center();
		spawn(FOOD);
	}
}

void snake::clean()
{
	delwin(win);
	endwin();
	refresh();
}

void snake::run()
{
	init();	
	while(1)
	{
		render();
		keystroke(); 		
		move();
	}
	clean();
}

int main()
{
	snake game;
	game.run();
	return EXIT_SUCCESS;
}
