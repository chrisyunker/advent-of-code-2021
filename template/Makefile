CXX := -c++
CXXFLAGS := -Wall -std=c++17
INCLUDE :=
LDFLAGS :=

SRD_DIR := src
OBJ_DIR := objects
TARGET := aoc
SOURCES := $(wildcard $(SRD_DIR)/*.cpp)

OBJECTS      := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -o $(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all clean debug

debug: CXXFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
