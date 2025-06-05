EXEC=park
FLAGS=-g -o

all: pseudoshell

park: park.o string_parser.o 
	$(CC) $(FLAGS) $@ $^

park.o: main.c
	$(CC) -c $^    

# %< grabs left dependency

%.o: %.c %.h
	$(CC) -c $<    

# does string_parser and command at once (line above)

clean:
	rm -f *.o $(EXEC)

# More about makefiles on canvas