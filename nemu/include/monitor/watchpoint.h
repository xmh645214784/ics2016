#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char expr[50];
	int oldvalue;
	bool isfirstfigure;//是首次求值
	char enable;
} WP;
WP* new_wp();
void  free_wp(int id);
WP *returnhead();
#endif
