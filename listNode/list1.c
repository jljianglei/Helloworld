/*实现一个循环链表*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define bool int
#define true 1
#define false 0
typedef struct listNode {
     int elem;
	 struct listNode* next;
}listNode;
void listInsert(listNode ** head,int elem);
void listPrint(listNode* head);
void listInsert(listNode** head,int elem) {
    if(head == NULL)
		return;
	listNode* tmp = malloc(sizeof(listNode));
	tmp->elem = elem;
	if(*head == NULL) {
	   *head = tmp;
	   tmp->next = *head;
	   return;
	}
	listNode * p = *head;
	while(p != NULL) {
         if(p->elem == elem) {
		    printf("the elem is exist\n");
			if(tmp) {
			   free(tmp);
			   tmp = NULL;
			}
			return;
		 }
		 if(p->next != (*head))
			 p = p->next;
		 else 
			 break;
	}
	p->next = tmp;
	tmp->next = *head;
}
void listPrint(listNode* head) {
     if(head == NULL)
		 return;
	 listNode* p = head;
	 while(p != NULL) {
	   printf("%d ",p->elem);
	   if(p->next != (head))
		   p = p->next;
	   else
		   break;
	 }
	 printf("\n");
}
bool isRingList(listNode* head) {
     if(head == NULL)
		 return false;
	 listNode* p = head;
	 while(p != NULL) {
	    if(p->next == head)
			return true;
		p = p->next;
	 }
	 return false;
}
static listNode* head;
int main() {
    listInsert(&head,3);
	listInsert(&head,6);
	listPrint(head);
	if(isRingList(head)) {
	   printf("is ring list\n");
	}
	return 0;
}
