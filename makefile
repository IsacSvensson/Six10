INCLUDES := Includes\\
BUILD := Build\\
BIN := BIN\\
EXECUTABLE := $(BIN)six10.exe
SOURCE := SOURCE\\
DELETE := del

CPP := g++
CPPFLAGS := -g -std=c++17

$(EXECUTABLE): $(BUILD)main.o $(BUILD)parser.o $(BUILD)nodes.o $(BUILD)helpers.o $(BUILD)interpreter.o $(BUILD)test.o
	$(CPP) $(CPPFLAGS) $(BUILD)main.o $(BUILD)parser.o $(BUILD)nodes.o $(BUILD)helpers.o $(BUILD)interpreter.o $(BUILD)test.o -o $(EXECUTABLE)

$(BUILD)main.o: $(SOURCE)main.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)main.cpp -I$(INCLUDES) -o $(BUILD)main.o

$(BUILD)nodes.o: $(SOURCE)nodes.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)nodes.cpp -I$(INCLUDES) -o $(BUILD)nodes.o

$(BUILD)parser.o: $(SOURCE)parser.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)parser.cpp -I$(INCLUDES) -o $(BUILD)parser.o

$(BUILD)helpers.o: $(SOURCE)helpers.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)helpers.cpp -I$(INCLUDES) -o $(BUILD)helpers.o

$(BUILD)interpreter.o: $(SOURCE)interpreter.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)interpreter.cpp -I$(INCLUDES) -o $(BUILD)interpreter.o

$(BUILD)test.o: $(SOURCE)test.cpp
	$(CPP) $(CPPFLAGS) -c $(SOURCE)test.cpp -I$(INCLUDES) -o $(BUILD)test.o

clean:
	-$(DELETE) $(EXECUTABLE)
	-$(DELETE) $(BUILD)main.o
	-$(DELETE) $(BUILD)parser.o
	-$(DELETE) $(BUILD)nodes.o
	-$(DELETE) $(BUILD)helpers.o
	-$(DELETE) $(BUILD)interpreter.o
	