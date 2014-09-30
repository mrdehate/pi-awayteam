CFLAGS+= -std=c++0x

LDFLAGS+=-lwiringPi

CC=g++

INCLUDES+=

OBJS=button.o

all: button 

%.o:  %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

me:
	vim Makefile

edit:
	vim button.c

clean:
	rm *.o

button: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
