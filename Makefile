# AI Generated & Corrected!

CXX = g++ # The compiler we use
# Flags: C++20 standard, show warnings, look in include/ for headers, and auto-generate .d dependency files
CXXFLAGS = -std=c++20 -Wall -Iinclude -MMD -MP 

# Find every .cpp under src/, automatically
SRCS = $(shell find src -name '*.cpp')

# Turn each src/foo.cpp into build/foo.o
OBJS = $(SRCS:src/%.cpp=build/%.o)

# The .d (dependency) files Make will auto-generate
DEPS = $(OBJS:.o=.d)

LIBS = -lncurses

TARGET = app

# --- Link phase: all .o files into the program ---
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

# --- Compile phase: one pattern rule for ALL files ---
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pull in the auto-generated header dependencies
-include $(DEPS)

clean:
	rm -rf build $(TARGET)

.PHONY: clean
