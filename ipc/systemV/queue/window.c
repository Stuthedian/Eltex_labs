#include "window.h"

void init_window(Window* wnd)
{
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	wnd->users = NULL;

	wnd->height = 20;
	wnd->width = 70;
	wnd->whole_wnd = newwin(wnd->height, wnd->width, 0, 0);
	wbkgd(wnd->whole_wnd, COLOR_PAIR(1));
	
	int user_wnd_width = 15;
	
	wnd->user_wnd = derwin(wnd->whole_wnd, wnd->height, user_wnd_width, 0, wnd->width - user_wnd_width);
	box(wnd->user_wnd, '|', '-');
	wnd->users = malloc((wnd->height-2) * sizeof (WINDOW*));
	for (int i = 0; i < wnd->height-2; i += 1)
	{
		wnd->users[i] = NULL;
	}
	
	wnd->last_pos_user_wnd = 1;
	
	wnd->chat_wnd = derwin(wnd->whole_wnd, wnd->height-1, wnd->width - user_wnd_width, 0, 0);
	box(wnd->chat_wnd, '|', '-');
	wnd->messages_size = wnd->height-1-2;
	wnd->messages = malloc(wnd->messages_size * sizeof (MessageWnd));
	for (int i = 0; i < wnd->messages_size; i++)
	{
		wnd->messages[i].message_wnd = derwin(wnd->chat_wnd, 1, 70-15-2, i+1, 1);
		wnd->messages[i].message_text = malloc(70-15-2);
	}
		
	wnd->input_wnd = derwin(wnd->whole_wnd, 1, wnd->width - user_wnd_width, wnd->height-1, 0);
	wbkgd(wnd->input_wnd, COLOR_PAIR(2));
}

void destroy_window(Window* wnd)
{
	for (int i = 0; i < wnd->height-2; i += 1)
	{
		delwin(wnd->users[i]);
	}
	free(wnd->users);
	
	for (int i = 0; i < wnd->messages_size; i += 1)
	{
		free(wnd->messages[i].message_text);
		delwin(wnd->messages[i].message_wnd);
	}
	free(wnd->messages);

	delwin(wnd->input_wnd);
	delwin(wnd->user_wnd);
	delwin(wnd->whole_wnd);
}

char* login(Window* wnd)
{
	WINDOW* text;
	WINDOW* input;
	char* name = calloc(USERNAME_MAX, 1);
	text = newwin(1, wnd->width - 15, wnd->height-2, 0);
	input = newwin(1, wnd->width - 15, wnd->height-1, 0);
	
	wprintw(text, "Enter your name:");
	wrefresh(text);
	wrefresh(input);
	wgetnstr(input, name, USERNAME_MAX-1);
	delwin(text);
	delwin(input);
	
	wrefresh(wnd->whole_wnd);
	return name;
}

void register_at_server(int msg_que, Message* message, char* username)
{
	message->mtype = 1;
	message->mtext[0] = NEW_USER;
	long p = getpid();
	strncpy(message->mtext + 1, (char*)&p, sizeof(long));
	strcpy(message->mtext + 1 + sizeof(long), username);
	msgsnd(msg_que, message, MAXMSGSZ, 0);
}

void send_message_to_server(int msg_que, Message* message, char* buffer)
{
	int n = 1+sizeof(long);
	message->mtype = 1;
	message->mtext[0] = NEW_MESSAGE;
	strcpy(message->mtext + n + strlen(message->mtext+n) + 1, buffer);
	msgsnd(msg_que, message, MAXMSGSZ, 0);
}

void add_new_user(Window* wnd, char* username)
{
	if(wnd->last_pos_user_wnd > wnd->height-2)
		return;
	WINDOW* username_wnd = derwin(wnd->user_wnd, 1, 15-2, wnd->last_pos_user_wnd, 1);
	wnd->last_pos_user_wnd++;
	wprintw(username_wnd, username);
	wrefresh(username_wnd);
}

void add_new_message(Window* wnd, char* message)
{
	shift_array(wnd->messages, wnd->messages_size);
	strcpy(wnd->messages[wnd->messages_size-1].message_text, message);
	werase(wnd->messages[wnd->messages_size-1].message_wnd);
	wprintw(wnd->messages[wnd->messages_size-1].message_wnd, wnd->messages[wnd->messages_size-1].message_text);
	wrefresh(wnd->messages[wnd->messages_size-1].message_wnd);	
}

void shift_array(MessageWnd* messages, int size)
{
	free(messages[0].message_text);
	for (int i = 1; i < size; i++)
	{
		messages[i-1].message_text = messages[i].message_text;
		werase(messages[i-1].message_wnd);
		wprintw(messages[i-1].message_wnd, messages[i-1].message_text);
		wrefresh(messages[i-1].message_wnd);
	}
	messages[size-1].message_text = malloc(70-15-2);
}

void* wait_for_message(void* arg_struct)
{
	Arg_struct* args = arg_struct;
	Message message;
	while(1)
	{
		msgrcv(args->msg_que, &message, MAXMSGSZ, getpid(), 0);
		if(message.mtext[0] == NEW_USER)
		{
			add_new_user(args->wnd, message.mtext + 1 + sizeof(long));
		}
		else if(message.mtext[0] == NEW_MESSAGE)
		{
			int n = 1+sizeof(long);
			add_new_message(args->wnd, message.mtext + n + strlen(message.mtext+n) + 1);
		}
	}
}

