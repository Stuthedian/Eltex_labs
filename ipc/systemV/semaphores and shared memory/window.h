#include <pthread.h>
#include<curses.h>
#include<stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "server/message_type.h"

typedef struct
{
	char* message_text;
	WINDOW* message_wnd;
} MessageWnd;

typedef struct
{
	WINDOW* whole_wnd;
	WINDOW* input_wnd;
	WINDOW* chat_wnd;
	WINDOW* user_wnd;
	int last_pos_user_wnd;
	
	WINDOW** users;
	MessageWnd* messages;
	int messages_size;
	
	int width;
	int height;
} Window;

typedef struct
{
	int client_semaphore_id;
	Window* wnd;
	Message* message;
} Arg_struct;



void init_window(Window* wnd);
void destroy_window(Window* wnd);
char* login(Window* wnd);
void add_new_user(Window* wnd, char* username);
void add_new_message(Window* wnd, char* message);
void shift_array(MessageWnd* messages, int size);
void register_at_server(int server_sem, int client_semaphore_id, Message* message, char* username);
void send_message_to_server(int msg_que, Message* message, char* buffer);
void* wait_for_message(void* arg_struct);
