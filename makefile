TARGET_EXEC ?= SuffixTree

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Directories within SRC_DIRS are "includes"/dependencies
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++11 $(INC_FLAGS) -MMD -MP

# Link object code into executable with same name as target $(BUILD_DIR)/$(TARGET_EXEC)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# default files for dev usage
FASTAFILE ?= src/data/s1.fasta
ALPHABETFILE ?= src/data/English_alphabet.txt

# run suffix tree construction and output
run: $(BUILD_DIR)/$(TARGET_EXEC)
	$< $(FASTAFILE) $(ALPHABETFILE)

# Clean up build directory
.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

ZIP=zip

# Zip up the project directory
.PHONY: zip
zip:
	$(ZIP) -r Program2-KylerLittle-StacySchauls .
