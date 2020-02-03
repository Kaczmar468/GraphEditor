CXX ?= g++
PROGRAM = GraphEditor
CXXFILES = main.cpp mainwindow.cpp area.cpp algorithms.cpp
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`

$(PROGRAM):
	$(CXX) $(CXXFILES) -o $(PROGRAM) $(LIBS)

clean:
	rm -f $(PROGRAM)
