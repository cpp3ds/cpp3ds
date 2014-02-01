PROJECT = libcpp3ds.a
CXX = arm-none-eabi-g++
AR = arm-none-eabi-ar
INCLUDES = -Iinclude/
CXXFLAGS = -g -Wall -pedantic -std=c++11 -march=armv5te -fno-rtti -fno-exceptions

SOURCES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.s)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o) $(SOURCES:src/%.s=build/%.o)

.PHONY: all dir clean

all: dir $(PROJECT)

dir:
	@mkdir -p build/actors
	@mkdir -p lib

$(PROJECT): $(OBJECTS)
	$(AR) rvs lib/$@ $^

clean:
	@rm -rf build
	@rm -rf lib
	@echo "Successfully cleaned."

build/%.o: src/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $< > build/$*.d

build/%.o: src/%.s
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $< > build/$*.d
