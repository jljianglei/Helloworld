/*统计数组中每个元素出现的次数*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct elemMap {
   int elem;
   int count;
};
static int a[] = {1,2,3,3,3,3,4,5,6,6,7,8,8,8};
static int mapCount = 0;
int main() {
    int length = sizeof(a)/sizeof(a[0]);
	struct elemMap map[length];
    int i,j;
	/*仅作用于排好序的数组*/
	for(i = 0; i < length;) {
		map[mapCount].elem = a[i];
		map[mapCount].count = 1;
	    for(j = i + 1; j < length; ++j) {
		    if(a[j] == a[i]) {
			   map[mapCount].count++;
			}
		}
		i += map[mapCount].count;
		mapCount++;
	}
	for(i = 0; i < mapCount; ++i) {
	    printf("elem %d,count %d\n",map[i].elem,map[i].count);
	}
	return 0;
}
