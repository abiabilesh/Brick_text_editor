
EXECUTABLE= Brick

CC= gcc

CFLAGS= -c  

all:Bricks

INCLUDE = ./include

OUTPUT = ./Build

ROOT_DIR = .
ROOT_DIR_OBJ = $(patsubst $(ROOT_DIR)/%.c,$(ROOT_DIR)/%.o,$(wildcard $(ROOT_DIR)/*.c))

Bricks:MAKEDIR $(ROOT_DIR_OBJ) Brick_core.o Brick_sys.o
	$(CC) $(OUTPUT)/*.o -o $(OUTPUT)/$(EXECUTABLE)

MAKEDIR:
	mkdir -p Build 

Brick_core.o: $(wildcard ./core/*.c)
	$(CC) -I $(INCLUDE) -c $^ -o $(OUTPUT)/$@  

Brick_sys.o: $(wildcard ./system/*.c)
	$(CC) -I $(INCLUDE) -c $^ -o $(OUTPUT)/$@

$(ROOT_DIR)/%.o: $(ROOT_DIR)/%.c
	$(CC) -I $(INCLUDE) -c $< -o $(OUTPUT)/$@ 

clean:
	rm -rf $(OUTPUT)



