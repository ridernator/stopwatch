BIN_NAME = stopwatch
BIN_INSTALL_FOLDER = /usr/bin

$(BIN_NAME) : src/stopwatch.c
	@$(CC) -o $(BIN_NAME) -Wall -Wextra -Werror src/stopwatch.c -pthread

all : $(BIN_NAME)

clean :
	@$(RM) $(BIN_NAME)

install : $(BIN_NAME)
	@cp -f $(BIN_NAME) $(BIN_INSTALL_FOLDER)/

uninstall : 
	@$(RM) $(BIN_INSTALL_FOLDER)/$(BIN_NAME)