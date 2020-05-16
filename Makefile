
EXECUTABLE= brick

CC= gcc

CFLAGS= -c  

all:Bricks

INCLUDE = ./include

OUTPUT = ./build

ROOT_DIR = .
ROOT_DIR_OBJ = $(patsubst $(ROOT_DIR)/%.c,$(ROOT_DIR)/%.o,$(wildcard $(ROOT_DIR)/*.c))

SYS_DIR = ./system
SYS_DIR_OBJ = $(patsubst $(SYS_DIR)/%.c,$(SYS_DIR)/%.o,$(wildcard $(SYS_DIR)/*.c))

CORE_DIR = ./core
CORE_DIR_OBJ = $(patsubst $(CORE_DIR)/%.c,$(CORE_DIR)/%.o,$(wildcard $(CORE_DIR)/*.c))

CORE_FILE_DIR = ./file
CORE_FILE_DIR_OBJ = $(patsubst $(CORE_FILE_DIR)/%.c,$(CORE_FILE_DIR)/%.o,$(wildcard $(CORE_FILE_DIR)/*.c))


Bricks:MAKEDIR $(ROOT_DIR_OBJ) $(CORE_DIR_OBJ) $(CORE_FILE_DIR_OBJ) $(SYS_DIR_OBJ) 
	$(CC) $(OUTPUT)/*.o $(OUTPUT)/core/*.o $(OUTPUT)/file/*.o $(OUTPUT)/system/*.o -lpthread -o $(OUTPUT)/$(EXECUTABLE)

	
MAKEDIR:
	mkdir -p $(OUTPUT)/core
	mkdir -p $(OUTPUT)/system
	mkdir -p $(OUTPUT)/file

$(CORE_DIR)/%.o: $(CORE_DIR)/%.c
	$(CC) -g -I $(INCLUDE) -c $< -o $(OUTPUT)/$@  
	
$(CORE_FILE_DIR)/%.o: $(CORE_FILE_DIR)/%.c
	$(CC) -g -I $(INCLUDE) -c $< -o $(OUTPUT)/$@  

$(SYS_DIR)/%.o: $(SYS_DIR)/%.c
	$(CC) -g -I $(INCLUDE) -c $< -o $(OUTPUT)/$@  

$(ROOT_DIR)/%.o: $(ROOT_DIR)/%.c
	$(CC) -g -I $(INCLUDE) -c $< -o $(OUTPUT)/$@ 

clean:
	rm -rf $(OUTPUT)



