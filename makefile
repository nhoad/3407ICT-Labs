# Makefile for Graphics on Linux and generally make.
#
# You should be able to create a makefile project (also known as unmanaged
# makefile) and compile it this way.
#
# Remember to add new object files as you create them!

CC = g++
TARGET = Core
VPATH = Source

_OBJS = HPTime.o RGBCube.o ObjectLoader.o StringFunctions.o Transformation.o Primitives.o
OBJDIR = Intermediates

# Optional to build examples
# VPATH += Examples/Simple
# _OBJS += Simple.o

OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
INCLUDE = -I./Headers -I./Headers/Linux

FLAGS = -g3 -Wall -O0 -DDEBUG -fmessage-length=0 -pedantic
LIBS = -ldl -lXft -lpthread -lSDL -lSDLmain -lSDL_ttf -lSDL_image -lm -lGLEW -lGL
LIB_PATH = -L./Libraries/

all: $(TARGET)

.SUFFIXES:
.SUFFIXES: .cpp .o

$(OBJDIR)/%.o: %.cpp
	$(CC) $< -c -o $@ $(FLAGS) $(INCLUDE)
	@echo

$(TARGET): $(OBJS)
	$(CC) -o $@ $(FLAGS) $(OBJS) $(INCLUDE) $(LIB_PATH) $(LIBS) $(XLIBS)
	@echo

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)
	@echo

run: $(TARGET)
	./Core
