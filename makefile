INCLUDES := Includes\\
BUILD := Build\\
BIN := BIN\\
EXECUTABLE := $(BIN)main.exe
SOURCE := SOURCE\\
DELETE := del

CPP := g++
CPPFLAGS := -g -std=c++17 -Wall -Wextra -Wpedantic -Werror

$(EXECUTABLE): $(BUILD)lexer.o $(BUILD)main.o $(BUILD)typeCheckers.o $(BUILD)parser.o $(BUILD)nodes.o 
	$(CPP) $(CPPFLAGS) $(BUILD)lexer.o $(BUILD)main.o $(BUILD)typeCheckers.o $(BUILD)parser.o $(BUILD)nodes.o -o $(EXECUTABLE)

$(BUILD)main.o: $(SOURCE)main.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)main.cpp -I$(INCLUDES) -o $(BUILD)main.o

$(BUILD)lexer.o: $(SOURCE)lexer.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)lexer.cpp -I$(INCLUDES) -o $(BUILD)lexer.o

$(BUILD)typeCheckers.o: $(SOURCE)typeCheckers.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)typeCheckers.cpp -I$(INCLUDES) -o $(BUILD)typeCheckers.o

$(BUILD)nodes.o: $(SOURCE)nodes.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)nodes.cpp -I$(INCLUDES) -o $(BUILD)nodes.o

$(BUILD)parser.o: $(SOURCE)parser.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)parser.cpp -I$(INCLUDES) -o $(BUILD)parser.o

clean:
	-$(DELETE) $(EXECUTABLE)
	-$(DELETE) $(BUILD)main.o
	-$(DELETE) $(BUILD)lexer.o
	-$(DELETE) $(BUILD)typeCheckers.o
	-$(DELETE) $(BUILD)parser.o
	-$(DELETE) $(BUILD)nodes.o