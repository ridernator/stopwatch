BIN_NAME = stopwatch
BIN_INSTALL_FOLDER = /usr/local/bin
CFLAGS = -Wall -Wextra -Werror -pthread

$(BIN_NAME) : src/stopwatch.c
	@$(CC) -o $@ $(CFLAGS) $^

all : $(BIN_NAME)

clean :
	@$(RM) $(BIN_NAME)

install : $(BIN_NAME)
	@cp -f $(BIN_NAME) $(BIN_INSTALL_FOLDER)
	@echo "Installed $(BIN_NAME) into $(BIN_INSTALL_FOLDER)"

uninstall : 
	@$(RM) $(BIN_INSTALL_FOLDER)/$(BIN_NAME)
	@echo "Removed $(BIN_NAME) from $(BIN_INSTALL_FOLDER)"
