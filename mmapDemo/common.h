#ifndef _COMMON_H
#define _COMMON_H
typedef enum msgId {
    MSG_0,
    MSG_1,
    MSG_2,
    MSG_3,
    MSG_MAX
}msgId;
typedef struct {
    msgId id;
    char msgValue[10];
}Message;
#endif/*_COMMON_H*/
