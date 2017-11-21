/*写一个单链表，能实现链表的创建，添加，删除，顺序逆序打印，计算链表节点个数*/
#include<stdio.h>
typedef struct listNode {
    int id;
	struct listNode * next;
}listNode;
enum RESULT_CODE {
    SUCCESS,
	FAILD
};
RESULT_CODE listInit(listNode** head) {
     if(head == NULL) {
	    return FAILD; 
	 }
	 *head = (listNode*)malloc(sizeof(listNode));
	 if(*head == NULL)
		 return FAILD;
	 /*create a head node*/
	 (*head)->id = -1;
	 (*head)->next = NULL;
	 return SUCCESS;
}
RESULT_CODE listInsert(listNode* head,int num) {
	 /*if head == NULL,need init head first*/
     if(head == NULL)
		 return FAILD;
	 listNode* newNode = (listNode*)malloc(sizeof(listNode));
	 if(newNode == NULL) {
	    return FAILD;
	 }
	 newNode->id = num;
	 newNode->next = NULL;
	 /*if head->next == NULL,show the list is empty,the newNode is first node*/
     if(head->next == NULL) {
	    head->next = newNode;
		return SUCCESS;
	 }
	 listNode* p = head->next;
	 listNode * rear = head;
	 while(p != NULL) {
	    if(p->id == num) {
		   free(newNode);
		   return FAILD;
		}
		p = p->next;
		rear = p;
	 }

}
