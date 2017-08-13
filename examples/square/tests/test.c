#include "../src/square.h"
#include <assert.h>

// 测试正int
void test_square_with_normal_int() {
    assert(square(0) == 0);
    assert(square(1) == 1);
    assert(square(100) == 10000);
    assert(square(-100) == 10000);

    // 非法值
    square(0xffffffff);
}

// 测试正float
void test_square_with_normal_float() {
    float a = 0.1;
    assert(square(a) == 0.01); // ?
    //...
}

// 测试作为表达式
void test_square_as_expression() {
    assert(2*square(3) == 18);
    assert(square(3)*2 == 18);
    assert((int)square(1.5) == 2);
}

// 测试传入表达式
void test_square_with_expression() {
    assert(square(3+2) == 25);
    assert(square(3*2) == 36);
}

// 测试传入有副作用表达式
void test_square_with_side_effect_expression() {
    int a = 2;
    assert(square(a++) == 4);
}

int main() {
    test_square_with_normal_int();
    test_square_with_normal_float();
    test_square_as_expression();
    test_square_with_expression();
    test_square_with_side_effect_expression();
}

