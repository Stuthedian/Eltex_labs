#include "panel.h"

void init_panel(int active, Panel* panel, int row, int column)
{
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	init_pair(2, COLOR_CYAN, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	panel->is_active = active;
	panel->height = 20;
	panel->width = 40;
	panel->wnd = newwin(panel->height, panel->width, row, column);
	panel->subwnd = derwin(panel->wnd, panel->height-2, panel->width-2, 1, 1);
	panel->path_wnd = derwin(panel->wnd, 1, panel->width-2, 0, 1);
	panel->progress_bar = derwin(panel->wnd, 1, panel->width-2, panel->height-1, 1);
	
	box(panel->wnd, '|', '-');
	wbkgd(panel->wnd, COLOR_PAIR(1));
	wrefresh(panel->wnd);
	wbkgd(panel->subwnd, COLOR_PAIR(1));
	wbkgd(panel->path_wnd, COLOR_PAIR(2));
	wbkgd(panel->progress_bar, COLOR_PAIR(3));
	panel->entries = NULL;
	panel->dir_info = NULL;
	panel->current_entry = NULL;
	panel->num_showed_entries = 0;
	panel->page = -1;
	panel->current_path = NULL;
	
	fill_dir_info(panel);
	populate(panel);
}

void close_panel(Panel* panel)
{
	free_dir_info(panel);
	free_entries(panel);
	delwin(panel->wnd);
	delwin(panel->subwnd);
	delwin(panel->progress_bar);
	delwin(panel->path_wnd);
	free(panel->current_path);
}

void switch_active_panel(Panel** active_panel, Panel* left, Panel* right)
{
	if(*active_panel == left)
	{
		left->is_active = FALSE;
		right->is_active = TRUE;
		*active_panel = right;
	}
	else
	{
		left->is_active = TRUE;
		right->is_active = FALSE;
		*active_panel = left;
	}
	refresh_selection(left);
	refresh_selection(right);
}

Panel* get_non_active_panel(Panel* active_panel, Panel* left, Panel* right)
{
	if(active_panel == left)
		return right;
	else
		return left;
}

void process_mouse(Panel** active_panel, Panel* left, Panel* right)
{
	MEVENT event;
	getmouse(&event);
	int row = event.y, column = event.x;
	
	if(row == 0)
		return;
	if(column == 0 ||  column == left->width - 1 || column == left->width)
		return;
	int on_left = 0 < column && column < left->width - 1;
	int on_right = left->width - 1 < column && column < left->width*2-1;
	if((on_left && *active_panel == right) || (on_right && *active_panel == left))
		switch_active_panel(active_panel, left, right);
			
	if(row > (*active_panel)->num_showed_entries)
	{
		return;
	}
		
	if(event.bstate & BUTTON1_CLICKED)
	{
		Panel* panel = *active_panel;
		int i = panel->current_entry_idx;
		int idx = panel->page * (panel->height-2);
		
		werase(panel->current_entry);
		print_name(panel->current_entry, panel->dir_info[idx+i], A_NORMAL);
		wrefresh(panel->current_entry);
		
		panel->current_entry_idx = row-1;
		i = panel->current_entry_idx;
		panel->current_entry = panel->entries[i];
		
		werase(panel->current_entry);
		print_name(panel->current_entry, panel->dir_info[idx+i], A_REVERSE);
		wrefresh(panel->current_entry);	
	}
	if(event.bstate & BUTTON1_DOUBLE_CLICKED)
	{
		enter_dir(*active_panel);
	}
}
char* make_path(char* dir_path, char* file_name)
{
	int size = strlen(dir_path) + 1 + strlen(file_name) + 1;
	char* path = malloc(size);
	
	char* offset = path;
	strcpy(offset, dir_path);
	offset += strlen(dir_path);
	strcpy(offset, "/");
	offset += 1;
	strcpy(offset, file_name);
	path[size-1] = '\0';
	
	return path;
}

void* update_progress_bar(void* args)
{
	WINDOW* progress_bar = args;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while(1)
	{
		werase(progress_bar);
		wprintw(progress_bar, "Copying");
		wrefresh(progress_bar);
		sleep(1);
		for (int i = 0; i < 5; i += 1)
		{
			wprintw(progress_bar, "#");
			wrefresh(progress_bar);
			sleep(1);
		}
	}
}

void* copy_file(void* args)
{
	struct Arg_struct* arg_struct = args;
	Panel* active_panel = arg_struct->active_panel;
	Panel* left = arg_struct->left;
	Panel* right = arg_struct->right;
	pthread_t* thread = arg_struct->thread;
	int idx = active_panel->page * (active_panel->height-2);
	struct dirent* current_entry_dir_info = active_panel->dir_info[idx + active_panel->current_entry_idx];
	if(current_entry_dir_info->d_type == DT_REG)
	{
		char* file_to_copy_path = make_path(active_panel->current_path, current_entry_dir_info->d_name);
		FILE* file_to_copy = fopen(file_to_copy_path, "r");
		free(file_to_copy_path);
		long file_size;
		fseek(file_to_copy , 0 , SEEK_END);
		file_size = ftell(file_to_copy);
		rewind(file_to_copy);
		
		char* buffer = malloc (file_size);
		fread(buffer, 1, file_size, file_to_copy);
		fclose(file_to_copy);
		
		Panel* not_active_panel = get_non_active_panel(active_panel, left, right);
		char* target_file_path = make_path(not_active_panel->current_path, current_entry_dir_info->d_name);		
		FILE* target_file = fopen(target_file_path, "w");
		free(target_file_path);
		fwrite(buffer, 1, file_size, target_file);
		fclose(target_file);		
		
		free(buffer);
		pthread_cancel(*thread);
		mvwhline(active_panel->wnd, active_panel->height-1, 1, '-', active_panel->width-2);
		wrefresh(active_panel->wnd);
	}
}

void fill_dir_info(Panel* panel)
{
	free_dir_info(panel);
	if(panel->current_path != NULL)
		free(panel->current_path);
	panel->current_path = getcwd(NULL, 0);
	werase(panel->path_wnd);
	wprintw(panel->path_wnd, panel->current_path);
	wrefresh(panel->path_wnd);
	int n = scandir (panel->current_path, &panel->dir_info, NULL, alphasort);
	panel->max_entries = n;
	panel->num_entries_to_show = panel->max_entries;
	panel->dir_info_idx = 0;
	if(n < 0)
		perror ("Couldn't open the directory");
}

void populate(Panel* panel)
{
	free_entries(panel);
	werase(panel->subwnd);
	int to_show = panel->num_entries_to_show - (panel->height-2);

	panel->num_showed_entries = to_show >= 0?(panel->height-2):panel->num_entries_to_show;
	panel->num_entries_to_show -= panel->num_showed_entries;
	panel->entries = malloc(panel->num_showed_entries * sizeof(WINDOW *));
	
	
	for (int i = 0; i < panel->num_showed_entries; i++)
	{
		panel->entries[i] = derwin(panel->subwnd, 1, panel->width-2, i, 0);

		if(i == 0 && panel->is_active)
			print_name(panel->entries[i], panel->dir_info[panel->dir_info_idx], A_REVERSE);
		else
			print_name(panel->entries[i], panel->dir_info[panel->dir_info_idx], A_NORMAL);
		panel->dir_info_idx++;
	}
	
	panel->current_entry = panel->entries[0];
	panel->current_entry_idx = 0;
	panel->page++;
	wrefresh(panel->subwnd);
}

void populate_upwards(Panel* panel)
{
	free_entries(panel);
	werase(panel->subwnd);
	panel->num_entries_to_show += panel->num_showed_entries;
	panel->num_showed_entries = (panel->height-2);
	panel->entries = malloc(panel->num_showed_entries * sizeof(WINDOW *));
	
	panel->dir_info_idx = (panel->page-1) * (panel->height-2);
	for (int i = 0; i < panel->num_showed_entries; i++)
	{
		panel->entries[i] = derwin(panel->subwnd, 1, panel->width-2, i, 0);
		if(i == panel->num_showed_entries-1)
			print_name(panel->entries[i], panel->dir_info[panel->dir_info_idx], A_REVERSE);
		else
			print_name(panel->entries[i], panel->dir_info[panel->dir_info_idx], A_NORMAL);
		panel->dir_info_idx++;
	}
	
	panel->current_entry = panel->entries[panel->num_showed_entries-1];
	panel->current_entry_idx = panel->num_showed_entries-1;
	panel->page--;
	wrefresh(panel->subwnd);
}

void refresh_selection(Panel* panel)
{
	int i = panel->current_entry_idx;
	int idx = panel->page * (panel->height-2);
	werase(panel->current_entry);
	
	int attr = panel->is_active ? A_REVERSE : A_NORMAL;
	print_name(panel->current_entry, panel->dir_info[idx+i], attr);
	
	wrefresh(panel->current_entry);
}

void enter_dir(Panel* panel)
{
	int idx = panel->page * (panel->height-2);
	struct dirent* current_entry_dir_info = panel->dir_info[idx + panel->current_entry_idx];
	if(current_entry_dir_info->d_type == DT_DIR)
	{
		char* path = make_path(panel->current_path, current_entry_dir_info->d_name);
		
		if(chdir(path) == 0)
		{
			fill_dir_info(panel);
			panel->num_showed_entries = 0;
			panel->page = -1;
			populate(panel);
		}
		free(path);
	}
}

void go(Panel* panel, int direction)
{
	int i = panel->current_entry_idx;
	int idx = panel->page * (panel->height-2);
	WINDOW * entry = panel->current_entry;
	
	if(direction == TRUE)//downwards
	{
		if(idx+i+1 == panel->max_entries)
			return;
		if(panel->current_entry_idx+1 == panel->num_showed_entries)
		{
			populate(panel);
			return;
		}
	}
	else//upwards
	{
		if(idx+i-1 < 0)
			return;
		if(panel->current_entry_idx-1 < 0)
		{
			populate_upwards(panel);
			return;
		}
	}
	
	werase(entry);
	print_name(entry, panel->dir_info[idx+i], A_NORMAL);
	wrefresh(panel->current_entry);
	
	if(direction == TRUE)
		panel->current_entry_idx++;
	else
		panel->current_entry_idx--;	
	i = panel->current_entry_idx;
	panel->current_entry = panel->entries[i];
	entry = panel->current_entry;
	
	werase(entry);
	print_name(entry, panel->dir_info[idx+i], A_REVERSE);
	wrefresh(panel->current_entry);
}

void print_name(WINDOW* entry, struct dirent* dir_entry, int attrs)
{
	if(is_dir(dir_entry))
		wprintw(entry, "/");
	wattron(entry, attrs);
	wprintw(entry, dir_entry->d_name);
	wattroff(entry, attrs);
}

bool is_dir(struct dirent* dir_entry)
{
	return dir_entry->d_type == DT_DIR ? true : false;
}

void free_dir_info(Panel* panel)
{
	if(panel->dir_info != NULL)
	{
		for (int i = 0; i < panel->max_entries; i++)
		{
			free(panel->dir_info[i]);
		}
		free(panel->dir_info);
		panel->dir_info = NULL;
	}
}

void free_entries(Panel* panel)
{
	if(panel->entries != NULL)
	{
		for (int i = 0; i < panel->num_showed_entries; i++)
		{
			werase(panel->entries[i]);
			wrefresh(panel->entries[i]);
			delwin(panel->entries[i]);
		}	
		free(panel->entries);
		panel->entries = NULL;
	}
}
m_showed_entries; i++)
		{
			werase(panel->entries[i]);
			wrefresh(panel->entries[i]);
			delwin(panel->entries[i]);
		}	
		free(panel->entries);
		panel->entries = NULL;
	}
}
