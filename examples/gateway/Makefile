PROJECT = gateway
CXX = arm-none-eabi-g++
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CPP3DS = ../..

SOURCES = $(wildcard src/*.cpp)
INCLUDES = -I$(CPP3DS)/include -I$(CPP3DS)/../tween-engine/include -L$(CPP3DS)/lib -L$(CPP3DS)/../tween-engine/lib

CXXFLAGS = -g -Wall -pedantic $(INCLUDES) -std=c++11 -march=armv5te -fno-rtti -fno-exceptions
LIBS = --start-group -ltween -lc -lgcc -lcpp3ds --end-group
LDFLAGS = -T ../cpp3ds.ld -L/usr/arm-none-eabi/lib $(INCLUDES)
OBJECTS = $(SOURCES:src/%.cpp=build/arm/%.o)

SIM_CXX = g++
SIM_CXXFLAGS = -g -Wall -pedantic -std=c++11 -DSIMULATION `pkg-config gtkmm-3.0 gmodule-2.0 --cflags --libs`
SIM_LIBS = -Wl,--start-group -lc -lgcc -lcpp3ds-sim -ltween-test -lsfml-graphics -lsfml-window -lsfml-system -Wl,--end-group
SIM_LDFLAGS = -DSIMULATION -Wl,--wrap,main `pkg-config gtkmm-3.0 gmodule-2.0 --cflags --libs`
SIM_OBJECTS = $(SOURCES:src/%.cpp=build/sim/%.o)

.PHONY: all dir clean sim

all: dir $(PROJECT).bin $(PROJECT)

sim: dir $(PROJECT)

dir:
	@mkdir -p build/arm
	@mkdir -p build/sim

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	@rm $<

$(PROJECT).elf: $(OBJECTS)
	$(LD) $< -o $@ $(LDFLAGS) $(LIBS)

clean:
	@rm -rf build
	@rm -f $(PROJECT).bin
	@rm -f $(PROJECT)
	@echo "Successfully cleaned."

build/arm/%.o: src/%.cpp
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)
	$(CXX) -MM $< > build/arm/$*.d

build/sim/%.o: src/%.cpp
	$(SIM_CXX) $(INCLUDES) -c $< -o $@ $(SIM_CXXFLAGS)
	$(SIM_CXX) -MM $< > build/sim/$*.d

$(PROJECT): $(SIM_OBJECTS)
	$(SIM_CXX) $< -o $@ $(INCLUDES) $(SIM_LIBS) $(SIM_LDFLAGS)
