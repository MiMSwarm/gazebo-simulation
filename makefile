# TODO: Make stuff work.

# CC = clang++
CC = g++

SRCDIR = src
INCDIR = include
LIBDIR = lib

# Compile for shared library.
CFLAGS = -c -fPIC -Wall -I$(INCDIR)/ $(shell pkg-config --cflags gazebo)
LFLAGS = -shared -L$(SRCDIR)/ $(shell pkg-config --libs gazebo)

# Include files
INCFILES = $(wildcard $(INCDIR)/*)

# Files to consider when building vision.
ifeq ($(MAKECMDGOALS), model_check)
	VISSRC = vision_debug
else
	VISSRC = vision
endif

PLUGINS = $(LIBDIR)/vision.so
# PLUGINS = $(LIBDIR)/vision.so $(LIBDIR)/minimapper.so

# Make targets
all : $(PLUGINS)

# Uses vision_debug instead of vision for the plugin.
model_check : $(LIBDIR)/vision.so

# Builds the vision plugin.
$(LIBDIR)/vision.so : $(SRCDIR)/$(VISSRC).o
	@mkdir -p $(LIBDIR)
	@echo -n "Building $@ ... "; $(CC) $< -o $@ $(LFLAGS)
	@echo "done.";

# Compile all .cpp files into .o files.
$(SRCDIR)/%.o : $(SRCDIR)/%.cpp $(INCFILES)
	@echo -n "Compiling $@ ... "; $(CC) $(CFLAGS) $< -o $@
	@echo "done.";

clean :
	@echo -n "Cleaning $(LIBDIR) ..."; $(RM) -r $(LIBDIR)
	@echo " done.";
	@echo -n "Cleaning object files ..."; $(RM) -r $(SRCDIR)/*.o
	@echo " done.";

.PHONY : all model_check clean
