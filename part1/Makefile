EXEC=park
FLAGS=-g -pthread -lpthread -o

all: park

park: park.o
	$(CC) $(FLAGS) $@ $^
 

# %< grabs left dependency

%.o: %.c %.h
	$(CC) -c $<    

# does string_parser and command at once (line above)

clean:
	rm -f *.o $(EXEC)

# More about makefiles on canvas