
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>
#include <restaurant.h>

// 厨师替身
int __wrap_chef_cook(const char *order, char *dish, int size)
{
    check_expected(order);

    char* mock_dish = (char *)(int)mock();
    if (mock_dish) {
        strncpy(dish, mock_dish, size);
    }
    return (int)mock();
}

// 正常测试
void test_waiter_normal_order(void **state)
{
    // 正常输入输出
    char dish[10];
    int rv;

    // 配置厨师替身
    expect_string(__wrap_chef_cook, order, "Toufu");
    will_return(__wrap_chef_cook, "[Toufu]");
    will_return(__wrap_chef_cook, 0);

    rv = waiter_process("Toufu", dish, sizeof(dish));
    assert_int_equal(rv, 0);
    assert_string_equal(dish, "[Toufu]");
}

//// 异常测试
// 顾客输入数据边界:
// 输入菜品名称过长  -- 期望服务员直接拒绝，不用传到厨房
// 输入菜品名称为空  -- 期望服务员直接拒绝，不用传到厨房
// 没带容器 -- 编程错误, 调用者负责
// 容器太小 -- 体现为chef 输出被截断

// 厨师反馈数据边界:
// 菜品不存在或材料不足无法做菜 -- chef return -1
// 做错菜了 -- chef return 0


// 菜品不存在或材料不足无法做菜 -- chef return -1
void test_waiter_ordering_with_unknown_dishname(void **state)
{
    char dish[10];
    int rv;

    // 配置厨师替身
    expect_string(__wrap_chef_cook, order, "Toufu");
    will_return(__wrap_chef_cook, NULL);
    will_return(__wrap_chef_cook, -1);

    strcpy(dish, "notouch");
    rv = waiter_process("Toufu", dish, sizeof(dish));
    assert_int_equal(rv, -1);
    assert_string_equal(dish, "notouch");
}

// 输入菜品名称过长  -- 期望服务员直接拒绝，不用传到厨房
void test_waiter_ordering_with_toolong_dishname(void **state)
{
    int rv;
    char dish[256];
    char mock_dish[256];
    char dishname[256];

    // 构造一个超长菜名
    memset(dishname, 'x', MAX_DISHNAME_LEN+1);
    dishname[MAX_DISHNAME_LEN+1] = 0;

    strcpy(dish, "notouch");
    rv = waiter_process(dishname, dish, sizeof(dish));
    assert_int_equal(rv, -1);
    assert_string_equal(dish, "notouch");

    // 构造一个刚好最大值的名字
    memset(dishname, 'x', MAX_DISHNAME_LEN);
    dishname[MAX_DISHNAME_LEN] = 0;

    sprintf(mock_dish, "[%s]", dishname);
    expect_string(__wrap_chef_cook, order, dishname);
    will_return(__wrap_chef_cook, mock_dish);
    will_return(__wrap_chef_cook, 0);

    rv = waiter_process(dishname, dish, sizeof(dish));
    assert_int_equal(rv, 0);
    assert_string_equal(dish, mock_dish);
}

// 输入菜品名称为空  -- 期望服务员直接拒绝，不用传到厨房
void test_waiter_ordering_with_empty_dishname(void **state)
{
    char dish[10];
    int rv;

    strcpy(dish, "notouch");
    rv = waiter_process("", dish, sizeof(dish));
    assert_int_equal(rv, -1);
    assert_string_equal(dish, "notouch");
}

// 容器太小 -- 体现为chef 输出被截断
void test_waiter_ordering_with_toosmall_dish(void **state)
{
    char dish[1];
    int rv;

    expect_string(__wrap_chef_cook, order, "Toufu");
    will_return(__wrap_chef_cook, "[Toufu]");
    will_return(__wrap_chef_cook, 0);

    rv = waiter_process("Toufu", dish, sizeof(dish));
    assert_int_equal(rv, -2);


    expect_string(__wrap_chef_cook, order, "Toufu");
    will_return(__wrap_chef_cook, "[Toufu]");
    will_return(__wrap_chef_cook, 0);

    rv = waiter_process("Toufu", NULL, 0);
    assert_int_equal(rv, -2);
}

// 厨师做错菜了 -- chef return 0
void test_waiter_ordering_with_chef_mistake(void **state)
{
    char dish[1];
    int rv;

    expect_string(__wrap_chef_cook, order, "Toufu");
    will_return(__wrap_chef_cook, "[Tudou]");
    will_return(__wrap_chef_cook, 0);

    rv = waiter_process("Toufu", dish, sizeof(dish));
    assert_int_equal(rv, -2);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_waiter_normal_order),
        cmocka_unit_test(test_waiter_ordering_with_toolong_dishname),
        cmocka_unit_test(test_waiter_ordering_with_unknown_dishname),
        cmocka_unit_test(test_waiter_ordering_with_empty_dishname),
        cmocka_unit_test(test_waiter_ordering_with_toosmall_dish),
        cmocka_unit_test(test_waiter_ordering_with_chef_mistake),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

