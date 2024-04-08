CC		:= gcc
SRCS	:= src/yukon/card_llist/card_llist.c src/yukon/yukon.c src/main.c 
OBJS 	:= $(patsubst %.c, %.o, $(SRCS))

all: $(OBJS)
	$(CC) $^ -o yukon.exe

%.o: %.c
	$(CC) -c $< -o $@

clean:
	-rm $(OBJS)
	-rm $(wildcard yukon.exe)