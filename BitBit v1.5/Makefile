ifeq ($(OS),Windows_NT)
	EXE := BitBit.exe
else
	EXE := BitBit
endif

DEFAULT_NET = net70.bin

EVALFILE ?= $(DEFAULT_NET)

MARCH    = -march=native
OPTIMIZE = -O3 -funroll-loops -flto -s -DNDEBUG -std=c++17
FILES    = $(wildcard src/*.cpp) src/fathom/src/tbprobe.c
OBJS     = $(FILES:.cpp=.o)

FLAGS = $(OPTIMIZE) $(MARCH) -DEvalFile=\"$(EVALFILE)\"

%.o: %.cpp
	g++ $(FLAGS) -c $< -o $@

%.o: %.c
	gcc $(FLAGS) -c $< -o $@

make: download-net $(FILES)
	g++ $(FLAGS) $(FILES) -o $(EXE)

clean:
	rm -f $(OBJS)

download-net:
	@if test -f "$(EVALFILE)"; then \
		echo "File $(EVALFILE) exists, skipping download."; \
	else \
		echo "Downloading net..."; \
		curl -sOL https://github.com/ExpiredGuy/BitBit/releases/download/nets/$(EVALFILE); \
	fi
