
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

TARGET = inventory

SOURCES = main.cpp
HEADERS = HashTable.h Inventory.h


OBJECTS = $(SOURCES:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "BUILD SUCCESSFUL! EXECUTABLE: $(TARGET)"


%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)


run-csv: $(TARGET)
	./$(TARGET) "Amazon Marketing Sample Jan 2020.csv"

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "CLEAN COMPLETE"


rebuild: clean all

help:
	@echo "AVAILABLE TARGETS:"
	@echo "  all       - BUILD THE PROJECT (DEFAULT)"
	@echo "  run       - BUILD AND RUN THE PROGRAM"
	@echo "  run-csv   - BUILD AND RUN WITH SPECIFIC CSV FILE"
	@echo "  clean     - REMOVE BUILD ARTIFACTS"
	@echo "  rebuild   - CLEAN AND REBUILD"
	@echo "  help      - SHOW THIS HELP MESSAGE"

.PHONY: all run run-csv clean rebuild help