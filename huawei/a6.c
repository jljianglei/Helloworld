#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PRINT_LIST \
	p = head->next;\
    while(p != NULL) {\
	    printf("%d ",p->id);\
        p = p->next;\
	}\
    printf("\n")
struct listNode{
   int id;
   struct listNode * next;
};
struct listNode * head = NULL;
struct listNode* listInsert(struct listNode* head,struct listNode* newNode) {
     if(head == NULL)/*if head == NULL,show this is a empty list*/
		 return NULL;
	 struct listNode * p = head->next;
	 if(p == NULL) {/*if head->next == NULL,show the newNode is first data node*/
	    head->next = newNode;
		return head;
	 }
	 struct listNode * rear;
	 while(p != NULL) {
	    if(p->id == newNode->id)/*if the newNode already exist,return NULL*/
			return NULL;
		else {
		  rear = p;
		  p = p->next;
		}
	 }
	 /*when p == NULL,rear pointer list last node*/
	 rear->next = newNode;
	 return rear;
}
struct listNode* listDel(struct listNode* head,struct listNode* theNode) {
	   /*if head == NULL,show this is a empty list,head->next == NULL,show list no data node*/
       if(head == NULL || head->next == NULL)
		   return NULL;
	   if(theNode == NULL)
		   return NULL;
	   struct listNode* p = head->next;
	   struct listNode* rear = head;
	   while(p != NULL) {
	     if(p->id == theNode->id) {
		    rear->next = p->next;
//			free(p);
			return rear;
		 }
		 else {
		  rear = p;
		  p = p->next;
		 }
	   }
	   return NULL;
}
struct listNode* listInit(struct listNode** head) {
   *head = (struct listNode*)malloc(sizeof(struct listNode));
   if(*head == NULL)
	   return NULL;
   (*head)->id = -1;
   (*head)->next = NULL;
   return *head;
}
int main() {
    listInit(&head);
	if(head == NULL)
		return 0;
	struct listNode * p = NULL;
	struct listNode node1,node2,node3;
	node1.id = 1;
	node1.next = NULL;
	node2.id = 2;
	node2.next = NULL;
	node3.id = 3;
	node3.next = NULL;
	listInsert(head,&node1);
	listInsert(head,&node2);
	listInsert(head,&node3);
    PRINT_LIST;
	if(listDel(head,&node3) == NULL) {
	   printf("the node not exist\n");
	}
	struct listNode node4;
	node4.id = 4;
	node4.next = NULL;
	if(listDel(head,&node3) == NULL) {
	   printf("the node not exist\n");
	}
	PRINT_LIST;
	return 0;
}
