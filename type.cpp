#include <bits/stdc++.h>
#include <ncurses.h>
using namespace std;
int height, width, corner_y, corner_x, num_words;
WINDOW* main_win;
string target;
int curr_pos, curr_x, curr_y;
vector<string> words;
int err_flag, new_round;
double wpm;
std::chrono::steady_clock::time_point start_time;
std::chrono::steady_clock::time_point end_time;

// #include "tileset.cpp"

string to_string1(int x)
{
	if(x == 0) return "0";
	string ans;
	while(x)
	{
		ans += '0' + (x%10);
		x = x/10;
	}
	reverse(ans.begin(), ans.end());
	return ans;
}

void show_wpm()
{
	int a,b,c;
	c = (int)(wpm*100);
	a = c/100;  b = c%100;
	string s = "WPM: " + to_string1(a) + "." + to_string1(b);
	mvprintw(15, 117, s.c_str());
	refresh();
}

void init_display()
{
	height = 7, width = 68, corner_y = 10;
	num_words = 10;

	corner_x = (COLS - width)/2;
	main_win = newwin(height, width, corner_y, corner_x);
	wattron(main_win, COLOR_PAIR(1));
	attron(COLOR_PAIR(1));

	box(main_win, 0 , 0);
	mvwprintw(main_win, 1, 2, "aaaaaaaaaaaa");
	wrefresh(main_win);

	wpm = 0;
	show_wpm();
}

string get_next_word(int i)
{
	string ans = "";
	while(i < target.size())
	{
		if(target[i] == ' ') break;
		ans += target[i];
		i++;
	}
	return ans;
}

void show()
{
	werase(main_win);
	box(main_win, 0 , 0);

	//target = "abcdefghijklmnopqustuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";

	int x = 2, y = 1;
	int idx = 0;
	while(idx < target.size())
	{
		while(idx < target.size() && target[idx] == ' ') // fill space
		{
			mvwprintw(main_win, y, x, " ");
			x++;
			if(x > width - 3) { x = 2; y++; }
			idx++;
		}

		string w = get_next_word(idx);
		if(w.size() > width - 4) // word would not fit in single line
		{
			for(int i=0; i<w.size(); i++)
			{
				// cerr << "x : " << x << " ch: " << w[i] << endl;
				mvwaddch(main_win, y, x, w[i]); 
				x++; if(x > width - 3) { x = 2; y++; } 
			}
			idx += w.size();
			continue;
		}
		else if(x + w.size() > width - 2) { x = 2; y++; } // wrap (word would fit in new line)

		mvwprintw(main_win, y, x, w.c_str());
		x += w.size();
		if(x > width - 3) { x = 2; y++; };
		idx += w.size();

	}
	wrefresh(main_win);
}

void load_words()
{
	ifstream infile("data.txt");
	string word;
	while(getline(infile, word))
	{
		words.push_back(word);
	}
}

void generate_target()
{
	target = "";
	int n = words.size();
	int w = num_words; // no of words
	while(w--)
	{
		int x = ((rand()%n)*(rand()%n))%n;
		target += words[x];
		if(w != 0)
		target += " ";
	}
}

void accept(int x, int y)
{
	char tmp = mvwinch(main_win, y, x);
	wattron(main_win, COLOR_PAIR(2));
	mvwaddch(main_win, y, x, tmp);
	wattron(main_win, COLOR_PAIR(1));
	wrefresh(main_win);
}
void reject(int x, int y)
{
	char tmp = mvwinch(main_win, y, x);
	wattron(main_win, COLOR_PAIR(3));
	mvwaddch(main_win, y, x, tmp);
	wattron(main_win, COLOR_PAIR(1));
	wrefresh(main_win);
}

void put_cursor(int err, int x, int y)
{
	//cursor
	char tmp = mvwinch(main_win, y, x);
	if(err) wattron(main_win, COLOR_PAIR(3));
	wattron(main_win, A_REVERSE);
	mvwaddch(main_win, y, x, tmp);
	if(err) wattron(main_win, COLOR_PAIR(1));
	wattroff(main_win, A_REVERSE);
	wrefresh(main_win);
}

void reset()
{
	generate_target();
	show();
	curr_pos = 0;
	curr_x = 2; curr_y = 1;
	err_flag = 0;

	new_round = 1;

	put_cursor(0, curr_x, curr_y);
}




void end_round()
{
	end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff = end_time - start_time;
	// double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	double elapsed = diff.count();

	wpm = (double)(target.size()*12)/elapsed;

	cerr << "diff: " << elapsed << " wpm: " << wpm << endl;

	show_wpm();
}


void move_cursor()
{
	// cerr << "move_cursor: " << (int)target[curr_pos] << endl;
	if(err_flag) reject(curr_x, curr_y);
	else accept(curr_x, curr_y);

	if(curr_pos == target.size()-1) // end
	{
		end_round();
		reset();
		return;
	}
	else if(target[curr_pos] != ' ' || target[curr_pos+1] == ' ') // middle of word
	{
		if(curr_x == width - 3) { curr_x = 2; curr_y++; }
		else curr_x++;
		curr_pos++;
	}
	else // on the space before next word
	{
		string w = get_next_word(curr_pos + 1);
		int m = w.size();
		if(m == 0) // not possible because target[curr_pos+1] = ' ' case is handled
		{
			if(curr_x == width - 3) { curr_x = 2; curr_y++; }
			else curr_x++;
		}
		else if(curr_x + m > width - 3) // wrapped
		{
			curr_x = 2; curr_y++;
		}
		else // same line
		{
			curr_x++;
		}
		curr_pos++;
	}
	put_cursor(0, curr_x, curr_y);
}

void error()
{
	put_cursor(1, curr_x, curr_y);
}

void game_loop()
{
	reset();
	char ch;
	while(1)
	{
		ch = getch();

		if(new_round)
		{
			start_time = std::chrono::steady_clock::now(); // round start time
			new_round = 0;
		}

		if(ch == target[curr_pos])
		{
			move_cursor();
			err_flag = 0;
		}
		else
		{
			error();
			err_flag = 1;
		}
		// reset();
		if(ch == 10) break; // exit on enter
	}
}

int main()
{
	srand(time(0)); // seed
	freopen("log.txt", "w", stderr); // debug file

	// start---------------
	initscr();
	noecho();
	refresh();
	curs_set(0);
	// ---------------

	// color
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);

	load_words();
	init_display();
	game_loop();

	// end---------------
	// getch();
	endwin();
	return 0;
	//---------------
}



https://github.com/narutouzumaki798/tttttt.git
