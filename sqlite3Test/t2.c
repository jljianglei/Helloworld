#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>
static int resultCount = 0;
int sqlite3_cb(void * param,int num,char** values,char**name) {
    resultCount++;
    FILE* tmp = (FILE*)param;
    if(tmp) {
      char record[128] = {0};
      snprintf(record,128,"%-20d%-20s\n",atoi(values[0]),values[1]);
      int ret = fwrite(record,sizeof(char),strlen(record),tmp);
      if(ret != strlen(record)) {
         printf("fwrite faild\n");
         return -1;
      }
    }
    return 0;
}
int main() {
    sqlite3* db;
    char sql[128];
    int ret;
    char * errMsg;
    ret = sqlite3_open("test.db",&db);
    if(ret != SQLITE_OK) {
       printf("sqlite3_open faild\n");
       return -1;
    }
    snprintf(sql,128,"select * from sqlite_master where type = 'table' and name = 'test'");
    ret = sqlite3_exec(db,sql,&sqlite3_cb,NULL,&errMsg);
    if(ret != SQLITE_OK) {
       printf("select faild,errMsg %s\n",errMsg);
       return -1;
    }
    if(resultCount != 0) {
       printf("the table test already exist\n");
       resultCount = 0;
    }
    else {
        /*创建表*/
        snprintf(sql,128,"create table test(id integer,data text)");
        ret = sqlite3_exec(db,sql,NULL,NULL,&errMsg);
        if(ret != SQLITE_OK) {
            printf("create table faild,errMsg %s\n",errMsg);
            return -1;
        }
    }
    /*插入数据*/
    snprintf(sql,128,"insert into test values(1,'jiang')");
    ret = sqlite3_exec(db,sql,NULL,NULL,&errMsg);
    if(ret != SQLITE_OK) {
       printf("insert data faild,errMsg %s\n",errMsg);
       return -1;
    }
    snprintf(sql,128,"insert into test values(2,'zhao')");
    ret = sqlite3_exec(db,sql,NULL,NULL,&errMsg);
    if(ret != SQLITE_OK) {
       printf("insert data faild,errMsg %s\n",errMsg);
       return -1;
    }
    /*查询数据库，把查询结果保存至文件*/
    FILE* fp = NULL;
    fp = fopen("db_record","wb+");
    if(fp == NULL) {
       printf("fopen faild\n");
       return -1;
    }
    snprintf(sql,128,"select * from test");
    ret = sqlite3_exec(db,sql,&sqlite3_cb,(void*)fp,&errMsg);
    if(ret != SQLITE_OK) {
       printf("select data faild,errMsg %s\n",errMsg);
       return -1;
    }
    snprintf(sql,128,"delete from test");
    ret = sqlite3_exec(db,sql,NULL,NULL,&errMsg);
    if(ret != SQLITE_OK) {
       printf("delete data faild,errMsg %s\n",errMsg);
       return -1;
    }
    sqlite3_close(db);
    int fileSize = 0;
    fseek(fp,0,SEEK_END);
    fileSize = ftell(fp);
    printf("fileSize = %d\n",fileSize);
    fseek(fp,0,SEEK_SET);
    char * buf = calloc(1,fileSize + 1);
    ret = fread(buf,1,fileSize,fp);
    if(ret != fileSize) {
       printf("fread faild\n");
       return -1;
    }
    printf("%s\n",buf);
    free(buf);
    if(fp) {
       fclose(fp);
       fp = NULL;
    }
    return 0;
}
