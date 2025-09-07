# Compiler and flags
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -static -mconsole -Ilib

# All executables we want to build
TARGETS = bin/get-dword.exe bin/set-dword.exe  bin/set-string.exe bin/get-string.exe

# Default target: build everything
all: $(TARGETS)

# Rule to build each .exe from its .cpp and the registry library
bin/%.exe: %.cpp lib/registry.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGETS)
