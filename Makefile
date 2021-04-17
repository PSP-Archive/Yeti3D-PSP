TARGET = yeti3d_psp
OBJS = main.o yeti.o data.o draw.o extra.o font.o game.o maps.o sprites.o model.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS=-lSDL_mixer -lSDL_image -lSDL -lpspaudiolib -lpspaudio -lpspgum -lpspgu -lpsphprm -lpng -ljpeg -lz -lc -lm -lgcc
LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Template

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
