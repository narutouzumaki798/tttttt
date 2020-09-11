#include <bits/stdc++.h>
#include <ncurses.h>
#include <unistd.h>
using namespace std;


class board{
public:
	int startx;
	int starty;
	WINDOW* main_win;
	WINDOW* inp_win;
	WINDOW* ver_win;

	board()
	{
		// main win
		startx = 10;
		starty = 10;
		int height = 16 + 4;
		int width  = 32 + 4;
		main_win = newwin(height, width, starty, startx);
		wrefresh(main_win);

		// input win
		inp_win = newwin(3, 6, starty + 4, startx + 38);
		wattron(inp_win, COLOR_PAIR(4));
		box(inp_win, 0, 0);
		mvwprintw(inp_win, 1, 1, "____");
		wrefresh(inp_win);

		// verdict win
		ver_win = newwin(3, 10, starty + 8, startx + 38);
		wattron(ver_win, COLOR_PAIR(2));
		box(ver_win, 0, 0);
		mvwprintw(ver_win, 1, 1, "        ");
		wrefresh(ver_win);
	}

	void cell(int y, int x, int f)
	{
		wattron(main_win, COLOR_PAIR(f));
		int r =  2 + (y-1)*2;
		int c =  2 + (x-1)*4;

		int sc = c;
		mvwprintw(main_win, r, c, " "); c++;
		mvwprintw(main_win, r, c, " "); c++;
		mvwprintw(main_win, r, c, " "); c++;
		mvwprintw(main_win, r, c, " "); c = sc; r++;
		mvwprintw(main_win, r, c, " "); c++;
		mvwprintw(main_win, r, c, " "); c++;
		mvwprintw(main_win, r, c, " "); c++;
		mvwprintw(main_win, r, c, " "); c = sc; r++;

		// if(f == 0) wattron(main_win, COLOR_PAIR(1));
	}

	void draw()
	{
		int x = 10, y = 10;
		int idx = 0;
		for(int i=1; i<=8; i++)
		{
			for(int j=1; j<=8; j++)
			{
				cell(i, j, 1+(idx%2));
				idx++;
				x+=2; 
			}
			idx++;
			x = 10; y++;
		}
		wrefresh(main_win);
	}

	bool alphanum1(char ch)
	{
		if(ch >= '0' && ch <= '9') return true;
		if(ch >= 'a' && ch <= 'z') return true;
		return false;
	}
	void check(char t_ch, char t_x, char ch, char x)
	{
		if(t_ch == ch && t_x == x)
			mvwprintw(ver_win, 1, 1, "correct");
		else
			mvwprintw(ver_win, 1, 1, "na: %c%c", t_ch, t_x);
		wrefresh(ver_win);
	}

	void start()
	{
		srand(time(0));
		while(1)
		{
			// select new target cell
			int i = rand()%8+1;
			int j = rand()%8+1;
			char t_ch = 'a' + (j-1);
			char  t_x = '0' + (8 - i + 1);
			cell(i, j, 3); wrefresh(main_win);

			// erase input box and verdict
			mvwprintw(inp_win, 1, 1, "____"); wrefresh(inp_win);
			mvwprintw(ver_win, 1, 1, "        "); wrefresh(ver_win);

			//inputs
			char ch = getch();
			if(alphanum1(ch))
			mvwprintw(inp_win, 1, 1, "%c", ch);
			else 
			mvwprintw(inp_win, 1, 1, "#");  wrefresh(inp_win);

			char x = getch();
			if(alphanum1(x))
			mvwprintw(inp_win, 1, 2, "%c", x);
			else
			mvwprintw(inp_win, 1, 2, "#");  wrefresh(inp_win);

			// check
			check(t_ch, t_x, ch, x);

			usleep(1500000);  // introspect about life choices

			// reset target cell
			cell(i, j, 1+((i+j)%2)); wrefresh(main_win);
		}
	}
};



int main()
{
	// start
	initscr();
	noecho();
	curs_set(0);
	refresh();
	setlocale(LC_ALL, "");

	// color
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_BLACK, COLOR_YELLOW);
	init_pair(4, COLOR_BLACK, COLOR_BLUE);


	board b;
	b.draw();
	b.start();

	// end
	getch();
	endwin();
	return 0;
}
