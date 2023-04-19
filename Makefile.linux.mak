# defines compiler
CC = gcc
# defines build directory 
BUILD_DIR = bin
# defines object directory
OBJ_DIR = obj
# defines executable name 
EXEC_NAME = MemoryRPG
# get all c files
SRC=$(wildcard *.c)
# get all o files
OBJ=$(SRC:.c=.o)
# list of o files in the object directory
OBJ_IN_DIR=$(addprefix $(OBJ_DIR)/, $(OBJ))

# build everything
all: objdir buildir exec

# build the executable
exec: $(OBJ_IN_DIR)
	$(CC) $^ -o $(BUILD_DIR)/$(EXEC_NAME)

# create bin directory
buildir: 
	mkdir -p $(BUILD_DIR)

# create obj directory
objdir: 
	mkdir -p $(OBJ_DIR)

# delete bin then obj dirrectory, with all files in it
clean: 
	if exist $(BUILD_DIR)\$(EXECNAME) del $(BUILD_DIR)\$(EXECNAME)
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)

# build o files
$(OBJ_DIR)/%.o : %.c $(H_FILES) $(OBJ_DIR) # compile .c to .o object
	$(CC) -c $< -o $@
