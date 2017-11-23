/*
 *问题1：求链表中倒数第k个节点
 *算法思路：实现只遍历链表一次就能找到倒数第k个节点，定义2个指针，第一个指针先走k-1步，
 *然后第二个指针指向头结点，从第k步开始，2个指针一起走，当走到最后一个节点的时候，
 *第二个节点刚好走到倒数第k个节点，根据这个思路，先写代码
 *问题2:反转链表
 *由于链表中存在大量的“->”对指针进行解引用操作，必须特别关注"->"左边的指针是否可能会出现NULL值，如果可能，则对空指针解引用
 *程序就会崩溃，要特别注意边界值，异常值是否会引起指针为空的case
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct listnode {
	int id;
	struct listnode * next;
}listNode;
typedef enum resultCode{
  RESULT_SUCCESS,
  RESULT_FAILD
}resultCode;
resultCode listInit(listNode** head) {
   (*head) = (listNode*)malloc(sizeof(listNode));
   if(*head == NULL) {
      printf("malloc listnode faild\n");
	  return RESULT_FAILD;
   }
   (*head)->id = -1;
   (*head)->next = NULL;
}
resultCode listInsert(listNode* head,int id) {
   if(head == NULL) {
      printf("please init list first\n");
	  return RESULT_FAILD;
   }
   if(head->next == NULL) {
      listNode * tmp = (listNode*)malloc(sizeof(listNode));
	  if(tmp == NULL) {
	     printf("malloc listNode faild\n");
		 return RESULT_FAILD;
	  }
	  tmp->id = id;
	  tmp->next = NULL;
	  head->next = tmp;
	  return RESULT_SUCCESS;
   }
   listNode* rear = head;
   listNode* p = head->next;
   while(p != NULL) {
     if(p->id == id) {
	    printf("the num is exist\n");
		return RESULT_FAILD;
	 }
	 rear = p;
	 p = p->next;
   }
   listNode* tmp = (listNode*)malloc(sizeof(listNode));
   if(tmp == NULL) {
      printf("malloc listNode faild\n");
	  return RESULT_FAILD;
   }
   tmp->id = id;
   tmp->next = NULL;
   rear->next = tmp;
}
void listPrint(listNode *head) {
     if(head == NULL || (head->next == NULL))
		 return;
	 listNode* p = head->next;
	 while(p != NULL) {
	    printf("%d ",p->id);
		p = p->next;
	 }
	 printf("\n");
}
listNode* findKToTail(listNode* head,unsigned int k) {
    if(head == NULL || k == 0)
		return NULL;
	int i;
	listNode* p = head->next;
	for(i = 0; i < k - 1; ++i) {
	    if(p->next != NULL) {
		   p = p->next;
		}else {
		   return NULL;
		}
	}
	listNode* q = head->next;
	while(p->next != NULL) {
	  p = p->next;
	  q = q->next;
	}
	return q;
}
resultCode listReverse(listNode* head) {
    if(head == NULL || (head->next == NULL))
		return RESULT_FAILD;
	listNode* front = NULL;
	listNode * currNode = head->next;
	while(currNode != NULL) {
		listNode * mnext = currNode->next;
		if(mnext == NULL)
			head->next = currNode;
		currNode->next = front;
		front = currNode;
		currNode = mnext;
	}
}
static listNode* head;
int main() {
    if(listInit(&head) == RESULT_FAILD) {
	   printf("list init faild\n");
	   return 1;
	}
	listInsert(head,1);
	listInsert(head,1);
	listInsert(head,2);
	listInsert(head,3);
	listInsert(head,4);
	listInsert(head,5);
	listInsert(head,6);
	listInsert(head,7);
	listPrint(head);
	listNode* kNode = findKToTail(head,7);
	if(kNode != NULL) {
		printf("kNode id %d\n",kNode->id);
	}
	listReverse(head);
	listPrint(head);
	listReverse(NULL);
	return 0;
}

