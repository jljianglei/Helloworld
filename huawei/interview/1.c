/*
 *从尾到头打印链表,第一种思路是从头到尾遍历链表，以此把遍历的元素放到堆栈中
 *放完后，在把元素出栈，就是从尾到头打印里链表了，也可以通过递归来实现，
 *如果当前元素的后面还有元素，则先打印后面的元素，再打印当前的元素
 *它的一个问题在于，如果链表长度过长，则函数调用栈可能会溢出
 *
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
/*栈的定义*/
#define STACK_ELEMENT_LENGTH 10
typedef enum stackResult {
   STACK_RESULT_SUCCESS,
   STACK_RESULT_FAILD
}stackResult;
typedef struct stack {
   int elem[STACK_ELEMENT_LENGTH];
   int count;
   int top;
}stack;

/*链表的定义*/
typedef struct listNode {
   int elem;
   struct listNode * next;
}listNode;

typedef enum listNodeResult {
   LISTNODE_RESULT_SUCCESS,
   LISTNODE_RESULT_FAILD
}listNodeResult;
/*栈变量的定义*/
static stack s1;

/*链表变量的定义*/
static listNode * head;

/*栈接口的定义,pop,push,print,getTopElem,empty*/
stackResult stackPop(stack *s,int * elem) {
   if(elem == NULL) {
      return STACK_RESULT_FAILD;
   }
   if(s->count == 0) {
      printf("stack is empty\n");
	  *elem = 0;
	  return STACK_RESULT_FAILD;
   }
   *elem = s->elem[--s->top];
   s->count--;
   return STACK_RESULT_SUCCESS;
}

stackResult stackPush(stack *s,int elem) {
   if(s->top == STACK_ELEMENT_LENGTH) {
      printf("stack is full");
	  return STACK_RESULT_FAILD;
   }
   s->elem[s->top++] = elem;
   s->count++;
   return STACK_RESULT_SUCCESS;
}
int stackIsempty(stack *s) {
   return s->count ? 0:1;
}

stackResult StackGetTop(stack *s,int * elem) {
   if(elem == NULL) {
      return STACK_RESULT_FAILD;
   }
   if(s->count == 0) {
      printf("stack is empty\n");
	  return STACK_RESULT_FAILD;
   }
   *elem = s->elem[s->top - 1];
}
void stackPrint(stack *s) {
     if(s->count == 0) {
	    printf("stack is empty\n");
	 }
	 else {
	   printf("stack elem\n");
	   int tmp =  s->top;
	   do {
	       int elem = s->elem[--tmp];
		   printf("%d ",elem);
	   }while(tmp);
	   printf("\n");
	 }
}
/*链表接口的定义*/
listNodeResult listInsert(listNode ** head,int elem) {
    if(head == NULL) {
	   return LISTNODE_RESULT_FAILD;
	}
	listNode * newNode = (listNode*)calloc(1,sizeof(listNode));
	newNode->elem = elem;
	newNode->next = NULL;
	if(*head == NULL) {
	   *head = newNode;
	   return LISTNODE_RESULT_SUCCESS;
	}
	listNode* p = *head;
    while(p->next != NULL) {
	   p = p->next;
	}
    p->next = newNode;
	return LISTNODE_RESULT_SUCCESS;
}
void listPrint(listNode* head) {
   if(head == NULL) {
      return;
   }
   printf("list elem\n");
   listNode* p = head;
   do {
	   printf("%d ",p->elem);
	   p = p->next;
   }while(p != NULL);
   printf("\n");
}
void listPrintReverse(listNode * head) {
   if(head == NULL) {
      return;
   }
   if(head->next != NULL) {
     listPrintReverse(head->next);
   }
   printf("%d ",head->elem);
}
int main() {
#if 0
    stackPush(&s1,1);
    stackPush(&s1,2);
    stackPush(&s1,3);
	stackPrint(&s1);
#endif
	printf("s1.count = %d,s1.top = %d\n",s1.count,s1.top);
	listInsert(&head,1);
	listInsert(&head,3);
	listInsert(&head,6);
	listPrint(head);
	listNode * p = head;
	while(p != NULL) {
	  stackPush(&s1,p->elem);
	  p = p->next;
	}
	printf("s1.count = %d\n",s1.count);
    while(!stackIsempty(&s1)) {
	  int elem;
	  stackPop(&s1,&elem);
	  printf("%d ",elem);
	}
	printf("\n");
	listPrintReverse(head);
	printf("\n");
	return 0;
}
