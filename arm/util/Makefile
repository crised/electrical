

#CC=arm-linux-gcc
#PKGCONFIG=arm-linux-pkg-config

PKGCONFIG=pkg-config

CFLAGS += -O2 -Werror -Wall -Wextra -fno-builtin -ggdb3 -fPIC -D_GNU_SOURCE
INCLUDES=-Iabt/Headers
LIBS=-Labt/Libraries/ABT21/ -labCAN -labDIO -labGPS -labGSM -labMSR -labPWR -labSYS

#CFLAGS += $(shell $(PKGCONFIG) --cflags libmodbus)
#LIBS += $(shell $(PKGCONFIG) --libs libmodbus)

# clear out all suffixes
.SUFFIXES:
#
# # list only those we use
.SUFFIXES: .o .c

SRCS    = $(wildcard *.c)
HDRS    = $(wildcard *.h)
OBJS    = ${SRCS:.c=.o}

$(OBJS) : ${HDRS}

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $<

all: utils

utils: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
