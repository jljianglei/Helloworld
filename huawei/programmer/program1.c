/*
 *程序员面试宝典面试题例题：编写一个函数，实现把c/c++程序代码中的注释
 *去掉
 *
 */
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include<sys/types.h>
void remove_comment(char* buf,size_t size) {
    char * p,* end,c;/*p动态移动的字符指针，end指向文件末尾的字符指针，c代表p指向的字符*/
	char * sq_start,* dq_start;/*sq_start单引号开始位置指针，dq_start双引号开始位置指针*/
	char * lc_start,* bc_start;/*lc_start//的开始位置指针，bc_start'//'*的开始位置指针*/
    size_t len;/*记录某符号结束与开始的位置之差*/

	/*parameter init*/
	p = buf;
	end = p + size;
    sq_start = NULL;
	dq_start = NULL;
	lc_start = NULL;
	bc_start = NULL;


	while(p < end)/*还没有读到文件末尾*/
	{
	     c = *p;
		 switch(c) {
		    case '\'':/*单引号*/
				if(dq_start || lc_start || bc_start) {
				   /*当遇到过双引号，//或'\/'*的时候，忽略字符串与注释中的单引号*/
					p++;
					continue;
				}
				if(sq_start == NULL)/*之前未遇到过单引号*/
				{
				    sq_start = p++;/*sq_start指向单引号的开始位置指针，p指向下一个字符(p + 1)*/
				}
				/*如果之前遇到过单引号，sq_start已经保存了单引号的开始位置，p指向当前单引号*/
				else {
				    len = (p++) - sq_start;
					if(len == 2 && *(sq_start + 1) == '\\') {//处理'\''这种情况
					   continue;
					}
					sq_start = NULL;
				}
				break;
			case '\"':/*双引号*/
				if(sq_start || lc_start || bc_start) {
				   p++;
				   continue;
				}
				if(dq_start == NULL) {//如果之前未遇到过双引号
				   dq_start = p++;//dq_start指向双引号的开始位置，p指向下一个字符
				}
				else {
				   if(*((p++) - 1) == '\\') {
				      continue;
				   }
				   dq_start = NULL;
				}
				break;
			case '/':
				if(sq_start || dq_start || lc_start || bc_start) {
				  p++;
				  continue;
				}
				c = *(p + 1);
                if(c == '/')//2个//的情况
				{
				   lc_start = p;
				   p += 2;
				}
				else if(c == '*')///*的情况
				{
				   bc_start = p;
				   p += 2;
				}
				else {
				   p++;
				}
				break;
			case '*':
				if(sq_start || dq_start || lc_start || bc_start) {
				   p++;
				   continue;
				}
				if(*(p + 1) != '/') {
				  p++;
				  continue;
				}
				p += 2;
                memset(bc_start,' ',p - bc_start);//清除一对/*--------------------*/之间的内容
				bc_start = NULL;
                break;
			case '\n':
				if(lc_start == NULL) {
				   p++;
				   continue;
				}
				c = *(p - 1);
                memset(lc_start,' ',((c == '\r') ? (p++ - 1): (p++)) - lc_start);
				lc_start = NULL;
				break;
			default:
				p++;
				break;
		 
		 }
	
	}
	if(lc_start) {
	   memset(lc_start,' ',p - lc_start);
	}
}
int main() {
   int fd,n;
   static char buf[102400];
   fd = open("program1.c",O_RDONLY,0);
   if(fd == -1) {
      return -1;
   }
   n = read(fd,buf,sizeof(buf));
   if(n == -1 || n == 0) {
      close(fd);
	  return -1;
   }
   remove_comment(buf,n);
   *(buf + n) = '\0';
   printf(buf);
   close(fd);
   return 0;
}
