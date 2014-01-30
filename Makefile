PROJECT = cpp3ds.a
CXX = arm-none-eabi-g++
AR = arm-none-eabi-ar
INCLUDES = -Iinclude/
CXXFLAGS = -g -Wall -pedantic -std=c++11 -march=armv5te -fno-rtti -fno-exceptions

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

.PHONY: all dir clean

all: dir $(PROJECT)

dir:
	@mkdir -p build
	@mkdir -p lib

$(PROJECT): $(OBJECTS)
	$(AR) rvs lib/$@ $^

clean:
	@rm -f build/*.[od]
	@rm -f lib/$(PROJECT)
	@echo "Successfully cleaned."

build/%.o: src/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $< > build/$*.d
