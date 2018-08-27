EXE = can_encrypt
CC = gcc

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

AES = tiny-AES-c
VIRTUALCAR = virtualCar

CPPFLAGS += -Iinclude -I$(AES) -I$(VIRTUALCAR)/src
CFLAGS += -Wall -fPIC
LDFLAGS += -Llib -L$(VIRTUALCAR)
LDLIBS += -lm $(VIRTUALCAR)/virtualcar.a -lpthread

.PHONY: all clean

subsystem:
	$(MAKE) -C tiny-AES-c
	$(MAKE) -C virtualCar

all: subsystem $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(AES)/aes.o $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -pthread $(CPPFLAGS) $(CFLAGS) -c $< -o $@



clean:
	$(RM) $(OBJ)

