/*
 *问题：实现一个不带头结点的单链表
 *因为链表操作涉及大量的指针操作，在考虑不全的情况，很有可能会出现指针为NULL的情况，那么这个时候对NULL的解引用
 *必然导致段错误，而且很难定位，所以对于每个解引用，你都应该想下是否这个指针会出现空的情况，如果不确定，把它的地址用%p打出来
 *，这样当问题发生的时候，你就能定位到问题所在，而不是惊慌失措，六神无主.事实证明链表的段错误绝大部分是对空指针解引用
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct listNode {
    int id;
	struct listNode* next;
}listNode;
typedef enum resultCode{
  RESULT_SUCCESS,
  RESULT_FAILD
}resultCode;
resultCode listInsert(listNode**head,int id) {
    if(head == NULL) {
	   return RESULT_FAILD;
	}
	printf("%p\n",*head);
	/*空链表，所以这是链表的第一个节点*/
	if(*head == NULL) {
	   listNode* tmp = (listNode*)malloc(sizeof(listNode));
	   tmp->id = id;
	   tmp->next = NULL;
	   *head = tmp;
	   return RESULT_SUCCESS;
	}
    listNode* p = (*head);
	while(p->next != NULL) {
	   if(p->id == id) {
	      printf("the id %d is exist\n",id);
		  return RESULT_FAILD;
	   }
	   p = p->next;
	}
	listNode* tmp = (listNode*)malloc(sizeof(listNode));
	tmp->id = id;
	tmp->next = NULL;
	p->next = tmp;
	return RESULT_SUCCESS;
}
resultCode listInsetInPosition(listNode**head,int id,int pos) {
   if(head == NULL)
	   return RESULT_FAILD;
   if(*head == NULL)
	   return RESULT_FAILD;
   int i;
   listNode* p = *head;
   listNode * rear = NULL;
   for(i = 0; i < pos - 1; ++i) {
	   if(p->next != NULL) {
		   rear = p;
		   p = p->next;
	   }
	   else {
	      break;
	   }
   }
   /*pos > list count*/
   if(p->next == NULL) {
	   printf("pos is too long\n");
	   return RESULT_FAILD;
   }
   listNode* tmp = (listNode*)malloc(sizeof(listNode));
   if(tmp == NULL) {
      printf("tmp malloc faild\n");
	  return RESULT_FAILD;
   }
   tmp->id = id;
   printf("rear %p,p %p\n",rear,p);
   /*rear == NULL,show pos equal 1*/
#if 1
   if(rear == NULL) {
      *head = tmp;
	  tmp->next = p;
	  return RESULT_SUCCESS;
   }
#endif
   rear->next  =tmp;
   tmp->next = p;
   return RESULT_SUCCESS;
}
resultCode listDel(listNode**head,int id) {
   if(head == NULL)
	   return RESULT_FAILD;
   if(*head == NULL)
	   return RESULT_FAILD;
   /*the node is first node*/
   if((*head)->id == id) {
       listNode* tmp = *head;
	   *head = (*head)->next;
	   free(tmp);
	   return RESULT_SUCCESS;
   }
   listNode* rear = *head;
   listNode* p = (*head)->next;
   while(p) {
      if(p->id == id) {
	     rear->next = p->next;
		 free(p);
		 return RESULT_SUCCESS;
	  }
	  rear = p;
	  p = p->next;
   }
   printf("the node is not exist\n");
   return RESULT_FAILD;
}
void listPrint(listNode* head) {
	if(head == NULL) {
		printf("this is a empty list\n");
		return;
	}
   listNode* p = head;
   do {
	   printf("%d ",p->id);
	   p = p->next;
   }while(p != NULL);
   printf("\n");
}
resultCode listReverse(listNode**head) {
   if(head == NULL)
	   return RESULT_FAILD;
   if(*head == NULL)
	   return RESULT_FAILD;
   listNode* rear = NULL;
   listNode* curr = *head;
   while(curr != NULL) {
	   listNode* mnext = curr->next;/*修改curr节点的next前，保存它，否则赋值后，下一个指针就找不到了*/
	   if(curr->next == NULL) {/*如果是最后一个指针,则让头指针指向它*/
		   *head = curr;
	   }
	   curr->next = rear;/*reserve ope*/
	   rear = curr;/*向前走，准备逆序下一个节点*/
	   curr = mnext;
   }
   return RESULT_SUCCESS;
}
static listNode* head;
int main() {
    listInsert(&head,1);
	listInsert(&head,2);
	listInsert(&head,3);
	listInsert(&head,4);
	listInsert(&head,5);
	listInsert(&head,6);
	listInsetInPosition(&head,7,1);
	listInsetInPosition(&head,8,-1);
	listInsetInPosition(&head,11,0);
	listInsetInPosition(&head,9,2);
	listInsetInPosition(&head,10,100);
	listPrint(head);
	listReverse(&head);
	listPrint(head);
	listDel(&head,1);
	listPrint(head);
	listDel(&head,4);
	listPrint(head);
	listDel(&head,2);
	listPrint(head);
	listDel(&head,3);
	listPrint(head);
	return 0;
}

