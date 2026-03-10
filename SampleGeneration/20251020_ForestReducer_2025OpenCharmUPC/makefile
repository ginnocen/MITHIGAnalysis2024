CXX = g++
BUILD = build

CXXFLAGS = -I$(ProjectBase)/CommonCode/include `root-config --cflags`
LDFLAGS  = `root-config --glibs` -lTMVA -lTMVAGui -lXMLIO -lstdc++fs -lEG \
           $(ProjectBase)/CommonCode/library/Messenger.o

TARGET = Execute_Dzero
SRC = ReduceForest_Dzero.cpp
OBJ = $(BUILD)/ReduceForest_Dzero.o
DEP = $(BUILD)/ReduceForest_Dzero.d

default: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

-include $(DEP)
