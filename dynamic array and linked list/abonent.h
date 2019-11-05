struct Abonent
{
    char name[21];
    char surname[21];
    char number[12];
};

struct Abonent_Arr
{
	int capacity;
	struct Abonent* arr;
	int length;
};

struct Abonent_Node
{
	struct Abonent_Node* prev;
	struct Abonent_Node* next;
	struct Abonent abonent;
};

struct Abonent_List
{
	struct Abonent_Node* first;
	struct Abonent_Node* last;
};

void add(struct Abonent_List* abon_list);
void show(struct Abonent_List abon_list);
void find(struct Abonent_List abon_list);
void free_list(struct Abonent_List* abon_list);


