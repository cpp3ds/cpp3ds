PROJECT = gateway
CXX = arm-none-eabi-g++
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CPP3DS = ../..

INCLUDES = -I$(CPP3DS)/include
CXXFLAGS = -g -Wall -pedantic $(INCLUDES) -std=c++11 -march=armv5te -fno-rtti -fno-exceptions
LDFLAGS = -T ../cpp3ds.ld -L/usr/arm-none-eabi/lib -L$(CPP3DS)/lib -lcpp3ds -lc -lgcc -lcpp3ds

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

.PHONY: all dir clean

all: dir $(PROJECT).bin

dir:
	@mkdir -p build

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	@rm $<

$(PROJECT).elf: $(OBJECTS)
	$(LD) $< -o $@ $(LDFLAGS)

clean:
	@rm -rf build
	@rm -f $(PROJECT).bin
	@echo "Successfully cleaned."

build/%.o: src/%.cpp
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)
	$(CXX) -MM $< > build/$*.d
