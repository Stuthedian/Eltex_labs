struct Linked_List_Node
{
	struct Linked_List_Node* prev;
	struct Linked_List_Node* next;
	long num;
};

struct Linked_List
{
	struct Linked_List_Node* first;
	struct Linked_List_Node* last;
};

void add(struct Linked_List* ll_list, long num);
void free_list(struct Linked_List* ll_list);


