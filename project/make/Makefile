include paths.mak

CC := gcc
CFLAGS := -Wall -g -fPIC $(INCLUDED)

OBJS := $(subst .c,.o,$(SRC_LIST))

TARGET := $(SRC_PATH)/test.elf

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJS): %.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fr $(OBJS)
