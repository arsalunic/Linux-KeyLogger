CC 			:= gcc
CFLAGS 		:= -g -Wall
BIN_NAME 	:= keyLogger
OBJS 		:= keyLogger.o opt.o configuration.o

all: $(BIN_NAME)

$(BIN_NAME): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $(BIN_NAME)

clean:
	rm -f *.o $(BIN_NAME)