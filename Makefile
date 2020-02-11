CXX = g++
CXXFLAGS = -Wall -std=c++17 `sdl2-config --cflags` -Isrc/headers
LDFLAGS = `sdl2-config --static-libs` -lSDL2_ttf

SRCDIR = src
OBJDIR = obj

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all

all: find_path

find_path: $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

analyze: 
	$(CXX) $(CXXFLAGS) --analyze $(SRC)

debug: CXXFLAGS += -ggdb
debug: find_path

pre: CXXFLAGS += -E
pre: find_path

.PHONY: clean
clean:
	rm -rf $(OBJDIR) *.plist find_path