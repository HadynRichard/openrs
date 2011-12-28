CC := gcc
LD := gcc

MODULES := main net world script net/io world/player net/codec net/util
SRC_DIR := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

PYTHON_DIR := /usr/include/python2.7
PYTHON_LIB := python2.7

PYAPI_SRC := src/script/api/libpyapi.c
PYAPI_DIR := data/scripts
PYAPI_LIB := pyapi

SRC := $(foreach sdir,$(SRC_DIR), $(wildcard $(sdir)/*.c))
OBJ := $(patsubst src/%.c,build/%.o, $(SRC))
INCLUDES := $(addprefix -I, $(SRC_DIR)) -I$(PYTHON_DIR)

vpath %.c $(SRC_DIR)

define make-goal
$1/%.o: %.c
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: clean checkdirs pyapi openrs

openrs: $(OBJ)
	$(LD) $^ -o $@ -l$(PYTHON_LIB) -L$(PYAPI_DIR) -l$(PYAPI_LIB)

checkdirs: $(BUILD_DIR)

pyapi:
	$(CC) -shared -fPIC $(INCLUDES) -I$(PYTHON_DIR) -l$(PYTHON_LIB) -o $(PYAPI_DIR)/$(PYAPI_LIB).so $(PYAPI_SRC)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR), $(eval $(call make-goal,$(bdir))))
