#include "lm_items.h"

#include <stdlib.h>
#include <string.h>

#include "lm_util.h"

Item *allitems = NULL;	/* first of all items */
Item *item = NULL;	/* first of pattern matching items */
Item *sel = NULL;
Item *next = NULL;
Item *prev = NULL;
Item *curr = NULL;

void
lm_list_append(List *list, Item *i) {
	if(!list->tail)
		list->head = i;
	else
		list->tail->right = i;
	i->left = list->tail;
	i->right = NULL;
	list->tail = i;
}

void
lm_build_matches(char *pattern) {
	unsigned int plen;
	Item *i;
	DECLARE_LIST(lall);
	DECLARE_LIST(lexact);
	DECLARE_LIST(lprefix);
	DECLARE_LIST(lsubstr);

	if(!pattern)
		return;
	plen = strlen(pattern);
	i = NULL;
	for(i = allitems; i; i = i->next)
		if(!fstrncmp(pattern, i->text, plen + 1))
			lm_list_append(&lexact, i);
		else if(!fstrncmp(pattern, i->text, plen))
			lm_list_append(&lprefix, i);
		else if(fstrstr(i->text, pattern))
			lm_list_append(&lsubstr, i);
	if(lexact.head) {
		lall.head = lexact.head;
		lall.tail = lexact.tail;
	}
	if(lprefix.head) {
		if(lall.tail) {
			lall.tail->right = lprefix.head;
			lprefix.head->left = lall.tail;
		}
		else
			item = lprefix.head;
		lall.tail = lprefix.tail;
	}
	if(lsubstr.head) {
		if(lall.tail) {
			lall.tail->right = lsubstr.head;
			lsubstr.head->left = lall.tail;
		}
		else
			item = lsubstr.head;
	}
	curr = prev = next = sel = item = lall.head;
}

