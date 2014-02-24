COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror -Wno-unused-but-set-variable
LINKER = g++

all : testimage testscene

testimage : testimage.o rgbapixel.o png.o image.o
	$(LINKER) testimage.o rgbapixel.o png.o image.o -lpng -o testimage

testscene : rgbapixel.o png.o image.o testscene.o scene.o
	$(LINKER) rgbapixel.o png.o image.o testscene.o scene.o -lpng -o testscene

image.o : image.cpp image.h
	$(COMPILER) $(COMPILER_OPTS) image.cpp

testimage.o : testimage.cpp image.h
	$(COMPILER) $(COMPILER_OPTS) testimage.cpp

testscene.o : testscene.cpp
	$(COMPILER) $(COMPILER_OPTS) testscene.cpp

rgbapixel.o : rgbapixel.cpp rgbapixel.h
	$(COMPILER) $(COMPILER_OPTS) rgbapixel.cpp

png.o : png.cpp png.h rgbapixel.h
	$(COMPILER) $(COMPILER_OPTS) png.cpp

scene.o : scene.cpp scene.h
	$(COMPILER) $(COMPILER_OPTS) scene.cpp

clean :
	-rm -f *.o testimage testscene
