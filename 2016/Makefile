OBJS=      \
      01-a \
      02-a \
      02-b \
      03-a \
      03-b \
      04-a \
      05-a \
      05-b \
      06-a \
      07-a \
      07-b \
      08-a \
      09-a \
      09-b \
      10-a \
      11-a \
      12-a \
      14-a \
      14-b \
      15-a \
      16-a \
      18-a \
      19-a \
      19-b \
      20-a \
      20-c \
      21-a \
      21-b \
      21-a2 \
      22-a \
      23-a \
      24-a \
      25-a \


CFLAGS += -Werror -Wall -Wextra

all: $(OBJS)

# Needs crypto
04-a 05-a 05-b 14-a 14-b : LDLIBS += -lcrypto

# Needs list
11-a 14-a 14-b 19-a 19-b 20-c 22-a 24-a : list.h

# Needs ncurses
24-a : LDLIBS += -lncurses

clean:
	-rm $(OBJS)
