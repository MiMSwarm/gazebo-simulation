# TODO: Make stuff work.
 
CC = clang++

PLUGSRCDIR = src/plugins
PLUGLIBDIR = plugins

# Compile for shared library.
CFLAGS = -fPIC -Wall $(shell pkg-config --cflags gazebo)
LFLAGS = -shared $(shell pkg-config --libs gazebo)

# Find required files.
PLUGSRC = $(wildcard $(PLUGSRCDIR)/*)
PLUGLIB = $(patsubst $(PLUGSRCDIR)/%,$(PLUGLIBDIR)/%,$(PLUGSRC:.cc=.so))

all : $(PLUGLIB)

$(PLUGLIBDIR)/%.so : $(PLUGSRCDIR)/%.cc
	@mkdir -p $(PLUGLIBDIR)
	@echo -n "Building $@ ... "; $(CC) $(CFLAGS) $< -o $@ $(LFLAGS)
	@echo "done.";

clean :
	@echo -n "Cleaning $(PLUGLIBDIR) ..."; $(RM) -r $(PLUGLIBDIR)
	@echo "done.";

check :
	@echo "Plugin src dir: $(PLUGSRCDIR)"
	@echo "Plugin sources: $(PLUGSRC)"
	@echo "Plugin lib dir: $(PLUGLIBDIR)"
	@echo "Plugin library: $(PLUGLIB)"

.PHONY : all clean check