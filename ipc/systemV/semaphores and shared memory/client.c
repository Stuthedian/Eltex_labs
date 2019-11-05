#include <sys/stat.h>
#include<termios.h>
#include<sys/ioctl.h>
#include <sys/shm.h>
#include <sys/sem.h>
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
	Message* message = NULL;
	pthread_t thread1;
	int shared_memory_id = -1;
	int server_semaphore_id = -1;
	int client_semaphore_id = semget(IPC_PRIVATE, 1, IPC_CREAT | S_IRWXU);
	
	FILE* id_file = fopen("server/id_file", "r");
	fscanf(id_file, "%d %d", &shared_memory_id, &server_semaphore_id);
	fclose(id_file);

	message = shmat(shared_memory_id, NULL, 0);
	
	initscr();
	signal(SIGWINCH, sig_winch);
	Window window;
	start_color();
	refresh();
	init_window(&window);
	username = login(&window);
	
	register_at_server(server_semaphore_id, client_semaphore_id, message, username);

	Arg_struct arg_struct;
	arg_struct.client_semaphore_id = client_semaphore_id;
	arg_struct.wnd = &window;
	arg_struct.message = message;
	pthread_create(&thread1, NULL, wait_for_message, (void*)&arg_struct);
	
	
	int a;

	while(1)
	{
		wgetnstr(window.input_wnd, message->text, TEXT_MAX-1);
		werase(window.input_wnd);
		message->type = NEW_MESSAGE;
		message->sem_id = client_semaphore_id;
		strcpy(message->username, username);
		
		//tell server that we are done
		increment_sem(server_semaphore_id);
	}
	
	free(username);
	destroy_window(&window);
	endwin();
	exit(EXIT_SUCCESS);
}
