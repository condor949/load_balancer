TARGET1		= balancer
TARGET2     = receiver
TARGET3     = sender

D ?= 
CROSS_COMPILE	=
INC		= -I$(shell pwd)/ -I$(shell pwd)/server -I$(shell pwd)/conf_processor -I$(shell pwd)/client

CXXFLAGS	= -O3 -g -fPIC -pthread -fmessage-length=0 -std=c++14
LDFLAGS		= -ldl -lm -lrt -lpthread
COMPILE_DIR	= ./objs

$(COMPILE_DIR)/%.o : ./client/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(D) $(INC) -c  $< -o  $@
$(COMPILE_DIR)/%.o : ./server/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(D) $(INC) -c  $< -o  $@
$(COMPILE_DIR)/%.o : ./conf_processor/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(D) $(INC) -c  $< -o  $@
$(COMPILE_DIR)/%.o : ./%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(D) $(INC) -c  $< -o  $@

SRC1  = $(notdir $(wildcard ./server/*.cpp))
OBJS1 = $(patsubst %.cpp,$(COMPILE_DIR)/%.o,$(SRC1))

SRC2  = $(notdir $(wildcard ./conf_processor/*.cpp))
OBJS2 = $(patsubst %.cpp,$(COMPILE_DIR)/%.o,$(SRC2))

SRC3  = $(notdir $(wildcard ./main.cpp))
OBJS3 = $(patsubst %.cpp,$(COMPILE_DIR)/%.o,$(SRC3))

SRC4  = $(notdir $(wildcard ./client/receiver.cpp))
OBJS4 = $(patsubst %.cpp,$(COMPILE_DIR)/%.o,$(SRC4))

SRC5  = $(notdir $(wildcard ./client/sender.cpp))
OBJS5 = $(patsubst %.cpp,$(COMPILE_DIR)/%.o,$(SRC5))

$(TARGET1) : $(OBJS1) $(OBJS2) $(OBJS3)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET2) : $(OBJS4)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET3) : $(OBJS5)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

all: $(TARGET1)	$(TARGET2) $(TARGET3)

receiver: $(TARGET2)

sender: $(TARGET3)

rebuild:
	rm -rf $(COMPILE_DIR) $(TARGET1)
	rm -rf $(COMPILE_DIR) $(TARGET2)
	rm -rf $(COMPILE_DIR) $(TARGET3)
	make all

clean:
	rm -rf $(COMPILE_DIR) $(TARGET1)
	rm -rf $(COMPILE_DIR) $(TARGET3)
	rm -rf $(COMPILE_DIR) $(TARGET2)