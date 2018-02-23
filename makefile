# TODO: Make stuff work.
 
CC = clang++

SRCDIR = src
PLUGDIR = plugins

# Compile for shared library.
CFLAGS = -fPIC -Wall $(shell pkg-config --cflags gazebo)
LFLAGS = -shared $(shell pkg-config --libs gazebo)

# Find required files.
SRCEXT = cc
SOURCES = $(wildcard src/*)
PLUGINS = $(patsubst $(SRCDIR)/%,$(PLUGDIR)/%,$(SOURCES:.$(SRCEXT)=.so))

all : $(PLUGINS)

$(PLUGDIR)/%.so : $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(PLUGDIR)
	@echo -n "Building $@ ... "; $(CC) $(CFLAGS) $< -o $@ $(LFLAGS)
	@echo "done.";

clean :
	@echo -n "Cleaning $(PLUGDIR) ..."; $(RM) -r $(PLUGDIR)
	@echo "done.";

check :
	@echo "Sources: $(SOURCES)"
	@echo "Plugins: $(PLUGINS)"

.PHONY : all clean check