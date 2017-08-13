#ifndef __RESTAURANT_H
#define __RESTAURANT_H

#define MAX_DISHNAME_LEN 128

int chef_cook(const char *order, char*dish, int dishsize);
int waiter_process(const char *order, char *dish, int dishsize);

#endif

