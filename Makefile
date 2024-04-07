CC		:= gcc
SRCS	:= src/yukon/yukon.c src/main.c 
OBJS 	:= $(patsubst %.c, %.o, $(SRCS))

all: $(OBJS)
	$(CC) $^ -o yukon.exe

%.o: %.c
	$(CC) -c $< -o $@

clean:
	-rm $(wildcard **/*.o)
	-rm $(wildcard *.exe)