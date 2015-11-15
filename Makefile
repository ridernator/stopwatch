stopwatch : src/stopwatch.c
	@$(CC) -o stopwatch -Wall -Wextra -Werror src/stopwatch.c -pthread

clean :
	@$(RM) stopwatch

all : stopwatch