#include <string.h>
#include "restaurant.h"

/* 厨师做菜
 * 返回值
 * 0  - 成功
 * -1 - 失败
 */
int chef_cook(const char *order, char*dish, int size)
{
    if (strcmp(order, "Toufu") == 0) {
        strncpy(dish, "[Toufu]", size);
        return 0;
    }
    return -1;
}

