CC := gcc -fPIC
LD := gcc -fPIC

MODULES := main net world script net/io world/player net/codec net/util
SRC_DIR := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

PYTHON_DIR := /usr/include/python2.7
PYTHON_LIB := python2.7

SRC := $(foreach sdir,$(SRC_DIR), $(wildcard $(sdir)/*.c))
OBJ := $(patsubst src/%.c,build/%.o, $(SRC))
INCLUDES := $(addprefix -I, $(SRC_DIR)) -I$(PYTHON_DIR)

vpath %.c $(SRC_DIR)

define make-goal
$1/%.o: %.c
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: clean checkdirs openrs

openrs: $(OBJ)
	$(LD) $^ -o $@ -l$(PYTHON_LIB)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR), $(eval $(call make-goal,$(bdir))))
