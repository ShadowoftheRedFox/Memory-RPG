# defines compiler
CC = gcc
# defines build directory 
BUILD_DIR = bin
# defines object directory
OBJ_DIR = obj
# defines the source directory
SRC_DIR = src
# defines the save directory
SAVE_DIR = save
# defines executable name 
EXEC_NAME = MemoryRPG.exe

# get all c files
SRC=$(wildcard $(SRC_DIR)/*.c)
# get all o files
OBJ=$(SRC:.c=.o)
#strip the source folder from the names
OBJ_STRIP=$(OBJ:$(SRC_DIR)/%=%)
# list of o files in the object directory
OBJ_IN_DIR=$(addprefix $(OBJ_DIR)/, $(OBJ_STRIP))

# build everything
all: savedir objdir buildir exec

# build the executable
exec: $(OBJ_IN_DIR)
	$(CC) $^ -o $(BUILD_DIR)/$(EXEC_NAME)

# create bin directory
buildir: 
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# create obj directory
objdir: 
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

# create save directory
savedir: 
	if not exist $(OBJ_DIR) mkdir $(SAVE_DIR)

# delete bin then obj directory, with all files in it
clean: 
	if exist $(BUILD_DIR)\$(EXECNAME) del $(BUILD_DIR)\$(EXECNAME)
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(OBJ_DIR) rmdir /s /q $(SAVE_DIR)

# build o files
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(OBJ_DIR) objdir # compile .c to .o object
	$(CC) -c $< -o $@
