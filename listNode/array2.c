/*统计数组中每个元素的个数*/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
struct elemMap {
    int elem;
	int count;
};
static int a[] =  {1,2,3,3,3,3,5,5,10,3,7,17,1};
int main() {
    int length = sizeof(a)/sizeof(a[0]);
	printf("array length = %d\n",length);
	int i;
	int j;
	struct elemMap map[length];
	static int mapCount = 0;
	printf("arr elem is :\n");
	for(i = 0; i < length; ++i) {
	    printf("%d ",a[i]);
	}
	printf("\n");
	for(i = 0; i < length; ++i) {
	    map[i].elem = 0xffffffff;
	}
	/*遍历数组a的每一个元素*/
	for(i = 0; i < length; ++i) {
		/*遍历已有的map数组，如果当前元素在map数组中已经找到，则对应的count自增，如果没有找到，则是新的map数据，加入到map数组中*/
        /*把个数赋值为1，mapCount指向下一个位置*/
	    for(j = 0; j < mapCount; ++j) {
		    if(a[i] == map[j].elem) {
			   map[j].count++;
			   break;
			}
		}
		if(j >= mapCount) {
		  map[mapCount].elem = a[i];
		  map[mapCount].count = 1;
		  mapCount++;
		}
	}
	int result = 0;
	for(i = 0; i < mapCount; ++i) {
	    printf("elem %d,count %d\n",map[i].elem,map[i].count);
		result += map[i].count;
	}
    printf("result = %d\n",result);
	return 0;
}
