#
# Kutkov Oleg (kutkov.o@yandex.ru) (December 2016)
#

CC := gcc
PROGRAM = sclass
SRC := sclass.c curve_data.c date_utils.c
CFLAGS := -Wall -std=gnu99 -g -ggdb
LDFLAG := -lm

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAG) -o $(PROGRAM)

clean:
	rm -fr $(PROGRAM) $(PROGRAM).o
