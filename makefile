INCLUDES := Includes\\
BUILD := Build\\
BIN := BIN\\
EXECUTABLE := $(BIN)main.exe
SOURCE := SOURCE\\
DELETE := del

CPP := g++
CPPFLAGS := -g -std=c++17 -Wall -Wextra -Wpedantic -Werror

$(EXECUTABLE): $(BUILD)lexer.o $(BUILD)main.o $(BUILD)typeCheckers.o
	$(CPP) $(CPPFLAGS) $(BUILD)lexer.o $(BUILD)main.o $(BUILD)typeCheckers.o -o $(EXECUTABLE)

$(BUILD)main.o: $(SOURCE)main.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)main.cpp -I$(INCLUDES) -o $(BUILD)main.o

$(BUILD)lexer.o: $(SOURCE)lexer.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)lexer.cpp -I$(INCLUDES) -o $(BUILD)lexer.o

$(BUILD)typeCheckers.o: $(SOURCE)typeCheckers.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)typeCheckers.cpp -I$(INCLUDES) -o $(BUILD)typeCheckers.o

clean:
	-$(DELETE) $(EXECUTABLE)
	-$(DELETE) $(BUILD)lexer.o
	-$(DELETE) $(BUILD)typeCheckers.o