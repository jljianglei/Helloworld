#include<stdio.h>
//#define print(...) printf(__VA_ARGS__)
//#define print(fmt...) printf(fmt) 
//#define print(fmt,arg...) printf(fmt,##arg)
//#define print(fmt,...) printf(fmt,##__VA_ARGS__)
#define print(arg...) printf(arg)
//#define print(fmt,arg...) printf(fmt,##arg)
int main() {
   print("11 %d\n",2);
   return 0;
}
