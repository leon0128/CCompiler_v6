CXX      = g++-9
CXXFLAGS = -std=c++17 $(DEBFLAGS)
DEBFLAGS = -g3 -Wall

APPNAME = compiler

APPDIR = app/
SRCDIR = src/
TP1DIR = tp1/

ALLOBJS = $(OBJS) $(TP1OBJS)

OBJS    = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)*.cpp))
TP1OBJS = $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)$(TP1DIR)*.cpp))

all: $(APPNAME)

$(APPNAME): $(ALLOBJS)
	$(CXX) $(ALLOBJS) $(CXXFLAGS) -o $(APPDIR)$(APPNAME)

clean:; rm -f $(APPDIR)$(APPNAME) $(SRCDIR)*.o $(SRCDIR)*/*.o