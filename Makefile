ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
APP     := game.exe
LIB_LOC := -Llib
WIN_FL := -Iinclude 
else
APP     := game.out
LIB_LOC := -L/usr/lib/x86_64-linux-gnu
endif

CC 		:= g++
ENTRY 	:= MainWin.cpp

SRCDIR  := ./include
SRCDIRS := ./ ./Core ./Net ./Arth
OBJDIR  := ./bin

# Files and folders
SRCS    := $(shell find $(SRCDIR) -name '*.cpp')
OBJS    := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Flags
CFLAGS  := -Wall -pedantic -ansi -g -pthread -std=c++17 -g $(WIN_FL)
LDFLAGS := -std=c++17

# Libraries
LIBS := $(LIB_LOC) -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window -lSFGUI

# Targets
help:
	@echo Usage: make \<option\>
	@echo Options:
	@echo -b Builds app
	@echo -r Runs app
	@echo -br Builds and runs app
os:
	@echo $(detected_OS)
b: $(APP)
r: $(ENTRY)
	./$(APP)
br:	
	make b && make r

$(APP): buildrepo $(OBJS)
	$(CC) $(ENTRY) $(OBJS) $(LIBS) $(CFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(LDFLAGS) -c $< -o $@
	
clean:
	rm $(OBJDIR) -Rf
	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef