#include "a1.h"
#include<stdio.h>
void a1_func1() {
   printf("%s\n",__func__);
}
static void a1_func2() {
   printf("%s\n",__func__);
}
