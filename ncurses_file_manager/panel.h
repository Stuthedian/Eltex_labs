#include <pthread.h>
#include<curses.h>
#include<stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>


typedef struct
{
	int is_active;
	
	WINDOW * wnd;
	WINDOW * subwnd;
	WINDOW * progress_bar;
	WINDOW ** entries;
	struct dirent ** dir_info;
	int dir_info_idx;
	
	int num_showed_entries;
	int num_entries_to_show;
	int max_entries;
	
	int width;
	int height;
	
	WINDOW * current_entry;
	int current_entry_idx;
	
	char* current_path;
	WINDOW* path_wnd;
	int page;
} Panel;
struct Arg_struct
{
	Panel* left;
	Panel* right;
	Panel* active_panel;
	pthread_t* thread;
};


void init_panel(int active, Panel* panel, int row, int column);
void close_panel(Panel* panel);
void switch_active_panel(Panel** active_panel, Panel* left, Panel* right);
void process_mouse(Panel** active_panel, Panel* left, Panel* right);
void* copy_file(void* args);
void* update_progress_bar(void* args);
Panel* get_non_active_panel(Panel* active_panel, Panel* left, Panel* right);
char* make_path(char* dir_path, char* file_name);
bool is_dir(struct dirent* dir_entry);
void go(Panel* panel, int direction);
void print_name(WINDOW* entry, struct dirent* dir_entry, int attrs);
void enter_dir(Panel* panel);
void fill_dir_info(Panel* panel);
void populate(Panel* panel);
void populate_upwards(Panel* panel);
void refresh_selection(Panel* panel);
void free_entries(Panel* panel);
void free_dir_info(Panel* panel);
free_entries(Panel* panel);
void free_dir_info(Panel* panel);
