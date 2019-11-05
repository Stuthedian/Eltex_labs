#include<termios.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<stdlib.h>
#include<curses.h>
#include <dirent.h>
#include "panel.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main(int argc, char ** argv)
{
	Panel left, right;
	Panel* active_panel;
	pthread_t thread1,thread2;
	initscr();
	
	noecho();
	signal(SIGWINCH, sig_winch);
	cbreak();
	curs_set(FALSE);
	start_color();
	refresh();
	
	init_panel(TRUE, &left, 0, 0);
	init_panel(FALSE, &right, 0, left.width);
	active_panel = &left;
	mousemask(BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED, NULL);
	int flag = 1;
	
	struct Arg_struct arg_struct;
	arg_struct.left  = &left;
	arg_struct.right  = &right;

	while(flag)
	{
		keypad(active_panel->current_entry, TRUE);
		int ch = wgetch(active_panel->current_entry);
		switch(ch)
		{
		case 'q': flag = 0; break;
		case '\t': switch_active_panel(&active_panel, &left, &right); break;
		case '\n': enter_dir(active_panel); break;
		case KEY_UP: go(active_panel, FALSE); break;
		case KEY_DOWN: go(active_panel, TRUE); break;
		case KEY_MOUSE: process_mouse(&active_panel, &left, &right); break;
		case KEY_F(2): 
		pthread_create(&thread1, NULL, update_progress_bar, (void*)active_panel->progress_bar);
		arg_struct.active_panel  = active_panel;
		arg_struct.thread = &thread1;
		pthread_create(&thread2, NULL, copy_file, (void *)&arg_struct);
		break;
		default:break;
		}
		
	}

	close_panel(&left);
	close_panel(&right);	
	endwin();
	exit(EXIT_SUCCESS);
}

}
