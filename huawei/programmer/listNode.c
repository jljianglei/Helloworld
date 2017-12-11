/*
 *2个链表的第一个公共节点
 *分析:如果2个单链表有公共的节点，那么这2个链表从某一节点开始，他们的next都指向同一个节点
 *之后所有节点都是重合的，不可能再出现分叉，所以2个有公共节点二部分重合的链表，拓扑形状看起来
 *像一个Y,而不是X
 *
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
typedef struct listNode {
    int elem;
	struct listNode* next;
}listNode;
void listInsert(listNode** head,listNode* newNode);
void listPrint(listNode* head);
void listCount(listNode* head,int * count);
void listCount(listNode* head,int * count) {
    if(head == NULL || count == NULL)
		return ;
	int tmpCount = 0;
	listNode* p = head;
	while(p != NULL) {
	  tmpCount++;
	  p = p->next;
	}
	*count = tmpCount;
}
void listPrint(listNode* head) {
    if(head == NULL)
		return;
	listNode* p = head;
	while(p != NULL) {
	   printf("%d ",p->elem);
	   p = p->next;
	}
	printf("\n");
}
void listInsert(listNode** head,listNode* newNode) {
    if(head == NULL || newNode == NULL)
		return;
	if(*head == NULL) {
	   *head = newNode;
	   return;
	}
	listNode * p = *head;
	while(p != NULL) {
	   if(p->elem == newNode->elem) {
	      printf("the %d is exist\n",newNode->elem);
		  return;
	   }
	   if(p->next != NULL)
		   p = p->next;
	   else {
	      break;
	   }
	}
    p->next = newNode;
}
listNode* findFirstCommonNode(listNode* head1,listNode* head2) {
    int length1 = 0;
	int length2 = 0;
	listCount(head1,&length1);
	listCount(head2,&length2);
	int lengthDif = length1 - length2;
	listNode* longList = head1;
	listNode* shortList = head2;
	if(length2 > length1) {
	   longList = head2;
	   shortList = head1;
	   lengthDif = length2 - length1;
	}
	int i;
	for(i = 0; i < lengthDif; ++i) {
	    longList = longList->next;
	}
	while(shortList != NULL && (longList != NULL) && (longList != shortList)) {
	    shortList = shortList->next;
		longList = longList->next;
	}
	listNode* common = longList;
	return common;
}
static listNode* head1,*head2;
int main() {
   listNode* node1 = (listNode*)malloc(sizeof(listNode));
   node1->elem = 1;
   node1->next = NULL;
   listNode* node2 = (listNode*)malloc(sizeof(listNode));
   node2->elem = 3;
   node2->next = NULL;
   listNode* node3 = (listNode*)malloc(sizeof(listNode));
   node3->elem = 6;
   node3->next = NULL;
   listNode* node4 = (listNode*)malloc(sizeof(listNode));
   node4->elem = 4;
   node4->next = NULL;
   listInsert(&head1,node1);
   listInsert(&head1,node2);
   listInsert(&head1,node3);
   int count = 0;
   listCount(head1,&count);
   printf("list1 length = %d\n",count);
   listInsert(&head2,node4);
   listCount(head2,&count);
   printf("list2 length = %d\n",count);
   listPrint(head1);
   listPrint(head2);
   listNode* common = findFirstCommonNode(head1,head2);
   if(common != NULL)
	   printf("the common elem is %d\n",common->elem);
   else {
       printf("no common elem\n");
   }
   return 0;
}
