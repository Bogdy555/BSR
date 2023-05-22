IDIR=Headers
SDIR=Sources
CXX=x86_64-w64-mingw32-g++

CFLAGS+=-I$(IDIR) -I/usr/x86_64-w64-mingw32/include -std=c++23

LDIR=""
ODIR=Build

LIBS=-lm -municode

SIMPLE_DEPS = AssetManager.hpp Macros.hpp\
Renderer.hpp Templates.hpp\
WinTargetVer.hpp BSR.hpp\
Math.hpp Resources.hpp\
Time.hpp Dependencies.hpp\
MultiProcessing.hpp RunTime.hpp\
Vector.hpp Image.hpp\
Rasterizer.hpp String.hpp\
Window.hpp

DEPS = $(patsubst %,$(IDIR)/%,$(SIMPLE_DEPS))

SIMPLE_OBJ = AssetManager.o Math_Vector.o\
Renderer.o Time.o\
Image.o MultiProcessing.o\
Renderer_Instance.o Window.o\
Math.o Rasterizer.o\
RunTime.o Math_Matrix.o\
Rasterizer_Texture.o  String.o
OBJ = $(patsubst %,$(ODIR)/%,$(SIMPLE_OBJ))

bsr.a: $(OBJ)
	ar rcs $@ $^

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	mkdir -p $(ODIR)
	$(CXX) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
