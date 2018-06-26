OBJS=      \
      01-a \
      01-b \
      02-a \
      03-a \
      03-b \
      04-a \
      05-a \
      05-b \
      06-a \
      06-b \
      07-a \
      08-a \
       

CFLAGS+=-Werror -Wall -Wextra

all: $(OBJS)

04-a: 04-a.c
	$(CC) $(CFLAGS) -o $@ $^ -lcrypto

clean:
	-rm $(OBJS)
