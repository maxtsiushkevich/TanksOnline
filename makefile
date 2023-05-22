APP_NAME := Tanks

CXX := g++

CXXFLAGS := -std=c++17 -lboost_serialization

SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system -pthread -lboost_serialization

SRC_DIR := sources
OBJ_DIR := objects
DEP_DIR := headers

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEPS := $(wildcard $(SRC_DIR)/*.h)

$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(APP_NAME) $(OBJS)

.PHONY: clean