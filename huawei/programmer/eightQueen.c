/*
 *1850年由高斯提出，在8*8的国际象棋盘上摆放8个皇后，
 *使其不能互相攻击，即任意2个皇后不能在同一行，同一列或同一斜线，
 *有多少种摆法
 *
 *算法分析：a,b,c分别用来标记冲突，a数组代表列冲突，从a[0]到a[7]代表第0列到第7列
 *如果某列上已经有皇后，则为1，否则为0
 *数组b代表主对角线冲突，为b[0]~b[14].如果某条主对角线上已经有皇后，则为1，否则为0
 *数组c代表从对角线冲突，c[0]~c[14]，如果某条从对角线上已经有皇后，则为1，否则为0
 *
 */

#include<stdio.h>
static char Queen[8][8];
static int a[8];
static int b[15];
static int c[15];
static int queenNum = 0;//记录总的棋盘状态数
void qu(int i);//参数i代表行
int main() {
    int iLine,iColumn;
	for(iLine = 0; iLine < 8; ++iLine) {
	    a[iLine] = 0;//列标记初始化，表示无列冲突
		for(iColumn = 0; iColumn < 8; ++iColumn) {
		    Queen[iLine][iColumn] = '*';
		}
	}
	for(iLine = 0; iLine < 15; ++iLine) {
	    b[iLine] = c[iLine] = 0;
	}
	qu(0);
	return 0;
}
void qu(int i) {
    int iColumn;
	for(iColumn = 0; iColumn < 8; ++iColumn) {
	    if(a[iColumn] == 0 && b[i-iColumn+7] == 0 && c[i + iColumn] == 0) {
		   Queen[i][iColumn] = '@';
		   a[iColumn] = 1;
		   b[i - iColumn + 7] = 1;
		   c[i + iColumn] = 1;
		   if(i < 7)
			   qu(i + 1);
		   else {
		      int iLine,iColumn;
			  printf("第%d种状态\n",++queenNum);
			  for(iLine = 0; iLine < 8; ++iLine) {
			      for(iColumn = 0; iColumn < 8; ++iColumn)
					  printf("%c ",Queen[iLine][iColumn]);
				  printf("\n");
			  }
			  printf("\n\n");
		   }
		   /*如果前次的皇后放置导致后面的放置无论如何都不能满足要求，则回溯*/
		   Queen[i][iColumn] = '*';
		   a[iColumn] = 0;
		   b[i - iColumn + 7] = 0;
		   c[i + iColumn] = 0;
		}
	}
}



