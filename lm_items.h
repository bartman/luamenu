#ifndef LM_ITEMS_H
#define LM_ITEMS_H

struct Item {
	char *text;
	struct Item *next;		/* traverses all items */
	struct Item *left, *right;	/* traverses items matching current search pattern */
};
typedef struct Item Item;

struct List {
	struct Item *head;
	struct Item *tail;
};
typedef struct List List;

// TODO: get rid of globals, make this into a state structure
extern Item *allitems;	/* first of all items */
extern Item *item;	/* first of pattern matching items */
extern Item *sel;
extern Item *next;
extern Item *prev;
extern Item *curr;

#define INIT_LIST \
	{ NULL, NULL }
#define DECLARE_LIST(name) \
	struct List name = INIT_LIST

extern void lm_list_append(List *list, Item *i);
extern void lm_build_matches(char *pattern);

#endif // LM_ITEMS_H
