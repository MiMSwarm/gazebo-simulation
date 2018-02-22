#
# TODO: Move `libmongoclient.a` to /usr/local/lib so this can work on production servers
#
 
CC := g++					# Compiler

SRCDIR := src
BUILDDIR := build

TARGET := plugins.so

CFLAGS := -Wall $(shell pkg-config --cflags gazebo)
LFLAGS := $(shell pkg-config --libs)

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	@echo "Linking to shared library $@."
	$(CC) $^ -shared -o $(TARGET) $(LFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "Compiling $@."
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean