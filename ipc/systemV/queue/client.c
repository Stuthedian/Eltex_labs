#include<termios.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<stdlib.h>
#include<curses.h>
#include<pthread.h>
#include "window.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main(int argc, char ** argv)
{
	char* username;
	Message message;
	pthread_t thread1;
	int msg_que = -1;
	FILE* msg_que_id = fopen("server/msg_que_id", "r");
	fscanf(msg_que_id, "%d", &msg_que);
	fclose(msg_que_id);
	
	initscr();
	signal(SIGWINCH, sig_winch);
	Window window;
	start_color();
	refresh();
	init_window(&window);
	username = login(&window);
	
	register_at_server(msg_que, &message, username);

	Arg_struct arg_struct;
	arg_struct.msg_que = msg_que;
	arg_struct.wnd = &window;
	pthread_create(&thread1, NULL, wait_for_message, (void*)&arg_struct);
	
	
	char buffer[TEXT_MAX];
	int a;
	int n = 1+sizeof(long);
	while(1)
	{
		wgetnstr(window.input_wnd, message.mtext + n + strlen(message.mtext+n) + 1, TEXT_MAX-1);
		werase(window.input_wnd);
		message.mtype = SERVER;
		message.mtext[0] = NEW_MESSAGE;
		msgsnd(msg_que, &message, MAXMSGSZ, 0);
	}
	
	free(username);
	destroy_window(&window);
	endwin();
	exit(EXIT_SUCCESS);
}
