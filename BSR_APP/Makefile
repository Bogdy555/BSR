BSR_LIBRARY_IDIR=../BSR/Headers
IDIR=Headers
SDIR=Sources
CXX=x86_64-w64-mingw32-g++

CFLAGS+=-I$(IDIR) -I$(BSR_LIBRARY_IDIR) -I/usr/x86_64-w64-mingw32/include -std=c++23

LDIR=../BSR
ODIR=Build

LIBS=-static -lm -L$(LDIR) -l:bsr.a -municode -lole32 -lgdi32 -lwinmm

SIMPLE_DEPS = Application.hpp Dependencies.hpp \
MainMenu.hpp Templates.hpp \
BSR_APP.hpp Macros.hpp \
Resources.hpp Window.hpp

DEPS = $(patsubst %,$(IDIR)/%,$(SIMPLE_DEPS))

SIMPLE_OBJ = Application.o EntryPoint.o MainMenu.o Window.o
OBJ = $(patsubst %,$(ODIR)/%,$(SIMPLE_OBJ))

BSR_APP: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	mkdir -p $(ODIR)
	$(CXX) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
