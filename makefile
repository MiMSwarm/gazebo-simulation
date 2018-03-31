# TODO: Make stuff work.
 
# CC = clang++
CC = g++

SRCDIR = src
INCDIR = include
LIBDIR = lib

# Compile for shared library.
CFLAGS = -c -fPIC -Wall -Iinclude/ $(shell pkg-config --cflags gazebo)
LFLAGS = -shared -Lsrc/ $(shell pkg-config --libs gazebo)

# Find required files.
SRCFILES = $(wildcard $(SRCDIR)/*)
INCFILES = $(wildcard $(INCDIR)/*)
LIBFILES = $(LIBDIR)/sonar.so

all : $(LIBFILES)

$(LIBDIR)/sonar.so : $(SRCDIR)/sonar.o
	@mkdir -p $(LIBDIR)
	@echo -n "Building $@ ... "; $(CC) $< -o $@ $(LFLAGS)
	@echo "done.";

# Compile all .cpp files into .o files.
$(SRCDIR)/%.o : $(SRCDIR)/%.cpp $(INCFILES)
	@mkdir -p $(LIBDIR)
	@echo -n "Compiling $@ ... "; $(CC) $(CFLAGS) $< -o $@
	@echo "done.";

clean :
	@echo -n "Cleaning $(LIBDIR) ..."; $(RM) -r $(LIBDIR)
	@echo " done.";
	@echo -n "Cleaning object files ..."; $(RM) -r $(SRCDIR)/*.o
	@echo " done.";

check :
	@echo "Plugin src dir: $(SRCDIR)"
	@echo "Plugin inc dir: $(INCDIR)"
	@echo "Plugin lib dir: $(LIBDIR)"

	@echo "Plugin src files: $(SRCFILES)"
	@echo "Plugin inc files: $(INCFILES)"
	@echo "Plugin lib files: $(LIBFILES)"

.PHONY : all clean check