#include<iostream>
const int a = 10;
extern int b;
void func() {
    std::cout << "b = " << b << std::endl;
}
