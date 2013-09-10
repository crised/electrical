

CFLAGS += -O2 -Werror -Wall -Wextra -fno-builtin -ggdb3 -fPIC -D_GNU_SOURCE
CFLAGS += $(shell pkg-config --cflags libmodbus)
LIBS += $(shell pkg-config --libs libmodbus)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: electric_meter_reader_daemon

CLIENT_OBJECTS = main.o

electric_meter_reader_daemon: $(CLIENT_OBJECTS)
	$(CC) -o $@ $(CLIENT_OBJECTS) $(LIBS)


