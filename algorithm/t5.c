#include<stdio.h>
#define MSGID_MAX_LENGTH 10
static const int msgId[MSGID_MAX_LENGTH] = {0,1,2,3,4,5,6,7,8,9};
bool isExist(int id) {
  int i;
  for(i = 0; i < MSGID_MAX_LENGTH; ++i) {
      if(msgId[i] == id)
		  return true;
  }
  return false;
}
int main() {
    return 0;
}
