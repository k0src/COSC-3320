CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall

all: runTests tests/test tests/test.js

runTests: runTests.cpp
	$(CXX) $(CXXFLAGS) -o runTests runTests.cpp

tests/test: tests/test.cpp
	$(CXX) $(CXXFLAGS) -o tests/test tests/test.cpp

tests/test.js: tests/test.ts
	tsc tests/test.ts

clean:
	rm -f runTests tests/test tests/test.js

.PHONY: all clean