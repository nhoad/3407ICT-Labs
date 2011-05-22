# Makefile for Graphics on Linux and generally make.
#
# You should be able to create a makefile project (also known as unmanaged
# makefile) and compile it this way.
#
# Remember to add new object files as you create them!

CC = g++
TARGET = Core
VPATH = Source

_OBJS = HPTime.o Pacman.o Terrain.o Mat4.o Vec2.o Vec3.o Vec4.o Primitives.o Loader.o StringFunctions.o GameEntity.o Game.o PacmanGame.o Camera.o
OBJDIR = Intermediates

# Optional to build examples
# VPATH += Examples/Simple
# _OBJS += Simple.o

OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
#INCLUDE = -I./Headers -I./Headers/Linux
#use my own header files
INCLUDE = -I/usr/include/ -I./Headers -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/SDL -I/usr/include/GL

FLAGS = -g -Wall -O0 -DDEBUG -fmessage-length=0 -pedantic
#LIBS = -ldl -lXft -lpthread -lSDL -lSDLmain -lSDL_ttf -lSDL_image -lm -lGLEW -lGL
LIBS = -ldl -lXft -lpthread -lSDL -lSDLmain -lSDL_ttf -lSDL_image -lm -lGLEW -lGL -L/usr/lib -L.
#LIB_PATH = -L./Libraries/
# use my own libraries
LIB_PATH =

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
