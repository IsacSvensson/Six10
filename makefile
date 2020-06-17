INCLUDES := Includes\\
BUILD := Build\\
BIN := BIN\\
EXECUTABLE := $(BIN)six10.exe
SOURCE := SOURCE\\
DELETE := del

CPP := g++
CPPFLAGS := -g -std=c++17 -Wall -Wextra -Wpedantic -Werror

$(EXECUTABLE): $(BUILD)lexer.o $(BUILD)main.o $(BUILD)typeCheckers.o $(BUILD)parser.o $(BUILD)nodes.o $(BUILD)error.o $(BUILD)position.o 
	$(CPP) $(CPPFLAGS) $(BUILD)lexer.o $(BUILD)main.o $(BUILD)typeCheckers.o $(BUILD)parser.o $(BUILD)nodes.o $(BUILD)error.o $(BUILD)position.o  -o $(EXECUTABLE)

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

$(BUILD)error.o: $(SOURCE)error.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)error.cpp -I$(INCLUDES) -o $(BUILD)error.o

$(BUILD)position.o: $(SOURCE)position.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)position.cpp -I$(INCLUDES) -o $(BUILD)position.o

clean:
	-$(DELETE) $(EXECUTABLE)
	-$(DELETE) $(BUILD)main.o
	-$(DELETE) $(BUILD)lexer.o
	-$(DELETE) $(BUILD)typeCheckers.o
	-$(DELETE) $(BUILD)parser.o
	-$(DELETE) $(BUILD)nodes.o
	-$(DELETE) $(BUILD)error.o
	-$(DELETE) $(BUILD)position.o