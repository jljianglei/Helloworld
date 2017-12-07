/*
 *翻转链表，需要注意的点是
 *1注意指针为空的情况，就是说空链表，或者链表中只有一个节点，这都是比较特殊的情况
 *2完成翻转需要3个指针，一个指向当前需要翻转的节点，一个指向它的前一个节点，一个指向它的下一个节点
 *因为调整了next后，下一个节点会丢，所以在赋值前，需要先保存.
 *
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*链表数据结构定义*/
typedef struct listNode {
	int elem;
	struct listNode * next;
}listNode;
typedef enum listresult {
  list_result_success,
  list_result_faild
}listresult;
listresult listInsert(listNode**head,int elem);
listresult listReverve(listNode**head);
void listPrint(listNode* head);
/*当插入的节点是链表的第一个节点，则需要修改head的值，所以需要传head的地址，所以是二级指针*/
/*插入节点的算法是如果是第一个节点，则直接赋值给head，如果不是，则找到链表的最后一个节点，添加到链表的最后一个节点后面*/
listresult listInsert(listNode**head,int elem) {
    if(head == NULL) {
	   return list_result_faild;
	}
	listNode* tmp = (listNode*)calloc(1,sizeof(listNode));
	if(tmp == NULL) {
	   printf("calloc faild\n");
	   return list_result_faild;
	}
	tmp->elem = elem;
	tmp->next = NULL;
	if(*head == NULL) {
	   *head = tmp;
	   return list_result_success;
	}
    listNode * p = *head;
	while(p->next != NULL) p = p->next;
    p->next = tmp;
}

void listPrint(listNode* head) {
   if(head == NULL) {
      printf("list is empty\n");
	  return;
   }
   listNode * p = head;
   while(p != NULL) {
     printf("%d ",p->elem);
	 p = p->next;
   }
   printf("\n");
}
/*如果是空链表或者链表只有一个元素，不需要翻转*/
/*如果不是，则定义三个指针变量，分别指向当前，当前的前一个，当前的后一个，然后以此修改指针的指向*/
/*如果是最后一个节点，则赋值给head，成为翻转链表的头结点*/
listresult listReverve(listNode**head) {
   if(head == NULL) {
      return list_result_faild;
   }
   if(*head == NULL || (*head)->next == NULL) {
      return list_result_success;
   }
   listNode* currNode = *head;
   listNode * prev = NULL;
   while(currNode != NULL) {
        listNode* next = currNode->next;
		if(next == NULL) {
		   *head = currNode;
		}
		currNode->next = prev;
		prev = currNode;
		currNode = next;
   }
   return list_result_success;
}
listNode* head;
int main() {
    listInsert(&head,1);
    listInsert(&head,22);
    listInsert(&head,30);
    listInsert(&head,-4);
    listInsert(&head,100);
	listPrint(head);
	listReverve(&head);
	listPrint(head);
    return 0;
}
