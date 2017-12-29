#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>
//#define USE_SQLITE_TABLE
static int resultCount = 0;
static int sqlite3_cb(void *param,int column_num,char** column_value,char** column_name) {
       resultCount++;
       return 0;
}
int main() {
    sqlite3* db;
    int ret;
    ret = sqlite3_open("test.db",&db);
    if(ret != SQLITE_OK) {
       printf("sqlite3_open faild\n");
       return -1;
    }
    char * errMsg = 0;
    char sql[128];
    char buff[1024];
    int len;
    memset(sql,0,sizeof(sql));
    strcpy(sql,"select * from sqlite_master where type = 'table' and name = 'student'");
    ret = sqlite3_exec(db,sql,&sqlite3_cb,0,&errMsg);
    if(ret != SQLITE_OK) {
       printf("select faild,errMsg %s\n",errMsg);
       return -1;
    }
    if(resultCount != 0) {
       printf("the table student already exist\n");
       resultCount = 0;
    }
    else {
        memset(sql,0,sizeof(sql));
        /*创建表*/
        strcpy(sql,"create table student(id integer,name varchar(10))");
        ret = sqlite3_exec(db,sql,0,0,&errMsg);
        if(ret != SQLITE_OK) {
            printf("create table student faild,errMsg %s\n",errMsg);
            return -1;
        }
    }
    memset(sql,0,sizeof(sql));
    /*插入数据*/
    strcpy(sql,"insert into student values(1,'jiang')");
    ret = sqlite3_exec(db,sql,0,0,&errMsg);
    if(ret != SQLITE_OK) {
      printf("insert data faild,errMsg %s\n",errMsg);
      return -1;
    }
    memset(sql,0,sizeof(sql));
    /*插入数据*/
    strcpy(sql,"insert into student values(2,'zhao')");
    ret = sqlite3_exec(db,sql,0,0,&errMsg);
    if(ret != SQLITE_OK) {
      printf("insert data faild,errMsg %s\n",errMsg);
      return -1;
    }
    memset(sql,0,sizeof(sql));
    /*插入数据*/
    strcpy(sql,"insert into student values(1,'xiao')");
    ret = sqlite3_exec(db,sql,0,0,&errMsg);
    if(ret != SQLITE_OK) {
      printf("insert data faild,errMsg %s\n",errMsg);
      return -1;
    }
#ifdef USE_SQLITE_TABLE
    /*查询结果*/
    int nrow = 0,ncolumn = 0;
    char ** result;
    memset(sql,0,sizeof(sql));
    strcpy(sql,"select * from student");
    ret = sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errMsg);
    if(ret != SQLITE_OK) {
      printf("sqlite3_get_table faild,errMsg %s\n",errMsg);
      return -1;
    }
    int i,j;
    printf("nrow = %d,ncolumn = %d\n",nrow,ncolumn);
    for(j = 0; j < ncolumn; ++j) {
       printf("%-20s",result[j]);
    }
    printf("\n");
    j = ncolumn;
    for(i = 0; i < nrow; ++i) {
       printf("%-20d%-20s\n",atoi(result[j]),result[j + 1]);
       j += ncolumn;
    }
    sqlite3_free_table(result);
#else
    sqlite3_stmt * stmt;
    int ncolumn;
    memset(sql,0,sizeof(sql));
    strcpy(sql,"select * from student");
    ret = sqlite3_prepare(db,sql,strlen(sql),&stmt,NULL);
    if(ret != SQLITE_OK) {
       printf("sqlite3_prepare faild,errMsg %s\n",errMsg);
       return -1;
    }
    while(sqlite3_step(stmt) == SQLITE_ROW) {
       printf("%-20d%-20s\n",sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1));
    }
    sqlite3_finalize(stmt);
#endif
    /*删除所有数据*/
    memset(sql,0,sizeof(sql));
    strcpy(sql,"delete from student");
    ret = sqlite3_exec(db,sql,NULL,NULL,&errMsg);
    if(ret != SQLITE_OK) {
      printf("delete data faild,errMsg %s\n",errMsg);
      return -1;
    }
    sqlite3_free(errMsg);
    sqlite3_close(db);
    return 0;
}
