#include <stdio.h>
#include <string.h>
#include "restaurant.h"

/* 服务员
 * 职责：将顾客点菜点菜命令传递给厨师，厨师做好后检查是否正确
 * 参数:
 *   order, 菜名，不能为空, 最大 MAX_DISHNAME_LEN
 *   dish, 用于接收输出，不能为空
 *   dishsize, dish buffer大小
 * 返回值
 * 0  - 成功
 * -1 - 做不了这道菜
 * -2 - 厨师把菜做错了
 */
int waiter_process(const char *order, char *dish, int dishsize)
{
    int rv;
    char expected_output[MAX_DISHNAME_LEN+3];

    if (!order || order[0] == 0 || strlen(order) > MAX_DISHNAME_LEN) {
        return -1;
    }

    rv = chef_cook(order, dish, dishsize);
    if (rv != 0) {
        // 厨师不会做, 或原料不足
        return -1;
    }

    // 厨师做好了，检查是否正确
    snprintf(expected_output, sizeof(expected_output), "[%s]", order);
    if (strcmp(expected_output, dish) != 0) {
        return -2;
    }

    return 0;
}

