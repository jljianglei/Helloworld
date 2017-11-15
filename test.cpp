#include<iostream>
#include<vector>
#include<stdio.h>
typedef unsigned char uint8_t;
using namespace std;
int main() {
    vector<int> v1;
	v1.push_back(1);
	printf("v1.size %d\n",v1.size());
	int len = v1.size();
	int a = v1[len - 1];
	printf("a = %d\n",a);
	v1.pop_back();
	printf("v1 size %d\n",v1.size());
	v1.push_back(2);
	printf("v1 size %d\n",v1.size());
	vector<int>::iterator iter = v1.begin();
	v1.erase(iter);
	printf("v1 size %d\n",v1.size());
	uint8_t c = 10;
	int b = c;
	if(b == 10) {
	  printf("yes\n");
	}
	return 0;
}
