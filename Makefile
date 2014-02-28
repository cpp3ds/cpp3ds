PROJECT = libcpp3ds

INCLUDES = -Iinclude/ -I../tween-engine/include

CXX = arm-none-eabi-g++
AR = arm-none-eabi-ar
CXXFLAGS = -g -Wall -pedantic -std=c++11 -march=armv5te -fno-rtti -fno-exceptions
SOURCES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.S)
OBJECTS = $(SOURCES:src/%.cpp=build/arm/%.o) $(SOURCES:src/%.S=build/arm/%.o)

TEST_CXX = g++
TEST_AR = ar
TEST_CXXFLAGS = -g -Wall -pedantic -std=c++11 -fno-rtti -fno-exceptions -DTESTING
TEST_SOURCES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:src/%.cpp=build/test/%.o)

SIM_CXX = g++
SIM_AR = ar
SIM_CXXFLAGS = -g -Wall -pedantic -std=c++11 -DSIMULATION `pkg-config gtkmm-3.0 gmodule-2.0 --cflags --libs`
SIM_SOURCES = $(wildcard sim/*.cpp) $(wildcard sim/*/*.cpp) $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) sim/res.cpp
SIM_OBJECTS = $(SIM_SOURCES:sim/%.cpp=build/sim/%.o) $(SIM_SOURCES:src/%.cpp=build/sim/%.o)

GLIB_COMPILE_RESOURCES = `pkg-config --variable glib_compile_resources gio-2.0`

.PHONY: all dir clean lib test sim

all: lib test sim

lib: dir $(PROJECT).a
test: dir $(PROJECT)-test.a
sim: dir include/sim3ds/res.h $(PROJECT)-sim.a

dir:
	@mkdir -p build/arm/actors
	@mkdir -p build/test/actors
	@mkdir -p build/sim/actors
	@mkdir -p lib

clean:
	@rm -rf build
	@rm -rf lib
	@rm -f sim/res.cpp
	@rm -f include/sim3ds/res.h
	@rm -f $(PROJECT)
	@echo "Successfully cleaned."

$(PROJECT).a: $(OBJECTS)
	$(AR) rvs lib/$@ $^
$(PROJECT)-test.a: $(TEST_OBJECTS)
	$(TEST_AR) rvs lib/$@ $^
$(PROJECT)-sim.a: $(SIM_OBJECTS)
	$(SIM_AR) rvs lib/$@ $^

build/arm/%.o: src/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $< > build/arm/$*.d
build/arm/%.o: src/%.S
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $< > build/arm/$*.d

build/test/%.o: src/%.cpp
	$(TEST_CXX) $(INCLUDES) $(TEST_CXXFLAGS) -c $< -o $@
	$(TEST_CXX) -MM $< > build/test/$*.d

build/sim/%.o: sim/%.cpp
	$(SIM_CXX) $(INCLUDES) $(SIM_CXXFLAGS) -c $< -o $@
	$(SIM_CXX) -MM $< > build/sim/$*.d
build/sim/%.o: src/%.cpp
	$(SIM_CXX) $(INCLUDES) $(SIM_CXXFLAGS) -c $< -o $@
	$(SIM_CXX) -MM $< > build/sim/$*.d

# Resource building
include/sim3ds/res.h: res/sim/simulator.gresource.xml
	$(GLIB_COMPILE_RESOURCES) --sourcedir res/sim --generate-header $< --target=$@
sim/res.cpp: res/sim/simulator.gresource.xml
	$(GLIB_COMPILE_RESOURCES) --sourcedir res/sim --generate-source $< --target=$@
sim/res.cpp include/sim3ds/res.h: \
		res/sim/simulator.gresource.xml \
		$(shell $(GLIB_COMPILE_RESOURCES) --sourcedir res/sim --generate-dependencies res/sim/simulator.gresource.xml)