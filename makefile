INCLUDES = -Iincludes
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,  $(OBJ_DIR)/%.o, $(SRCS))
# generate list of depenency files (for header files)
DEPS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))

EXEC = 6502

CC = g++
CFLAGS = $(INCLUDES)
LDFLAGS =

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

#-MMD tells compiler to generate .d files, -MF $(OBJ_DIR)/$*.d specifies where to write the file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) -MMD -MF $(OBJ_DIR)/$*.d -c -o $@ $< $(CFLAGS)

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

