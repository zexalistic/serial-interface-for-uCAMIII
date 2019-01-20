CC	= gcc
SRC_DIR		:= $(shell pwd)
OBJ_ROOT	:= $(SRC_DIR)
SRCS		:=  $(shell find $(SRC_DIR) -name '*.c')
SRCS		:= $(sort $(SRCS))

OBJS		:= $(SRCS:$(SRC_DIR)%.c=$(OBJ_ROOT)%.o)
OBJ_DIR		:= $(sort $(dir $(OBJS)))

TRAGET 		:= $(SRC_DIR)/tool

COMPLIE	= $(CC) $(CFLAGS) -c "$<" -o "$@"

.PHONY : all clean
all:$(TRAGET)
$(OBJS)	:$(OBJ_ROOT)/%.o :$(SRC_DIR)/%.c
		@echo "compling $@ ..."
		$(COMPLIE)

$(TRAGET):$(OBJS)
		$(CC) $(CFLAGS) -o $@ $^
		@rm -f $(OBJS)

clean:
		@rm -f $(TRAGET)

	
