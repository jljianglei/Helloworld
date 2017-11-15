#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main() {
   char * p;
   p =  malloc(20);
   printf("p = %p\n",p);
   char * filepath;
   filepath = malloc(20);
   printf("filepath %p\n",filepath);
   strcpy(filepath,"a.txt");
   char * q;
   /*strcpy的长度超过了20，那么由于filepath和p的内存空间是紧挨的，那么filepath的内存就会被p的内容覆盖掉，导致文件打开失败
	*如果说越界多写的内存是系统内存，那么会造成系统的崩溃，所以内存越界是非常严重的问题，造成的后果也是不可预知的，也许
	*它一点影响也没有，也许它会造成灾难性的后果
	*/
   //q = strcpy(p,"please answer the question carefully");
   q = strcpy(p,"please");
   printf("filepath %s\n",filepath);
   q += 7;
   printf("p %s,q %s\n",p,q);
   int fd = -1;
   fd = open(filepath,O_RDONLY);
   if(fd < 0) {
      perror("open faild");
   }
   return 0;
}
