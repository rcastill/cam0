PKC=\
	opencv4

CXXFLAGS=\
	-std=c++14\
	-Wall\
	-Iinclude\
	`pkg-config --cflags $(PKC)`

ifeq ($(RELEASE),1)
	CXXFLAGS+=-Ofast
else
	CXXFLAGS+=\
		-DDEBUG\
		-g
endif

LDFLAGS=\
	-ldl\
	`pkg-config --libs $(PKC)`

	S=$(shell ls ./src/cam0/*.cpp)
O=$(S:.cpp=.o)

EXE=cam0

default: $(EXE) filters

$(EXE): $(O)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%-fpic.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -fPIC -o $@ $^

cam0f_%.so: src/cam0/filters/%-fpic.o
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^ $(LDFLAGS)

FILTERS=$(shell cd src/cam0/filters; ls *.cpp)

filters: $(FILTERS:%.cpp=cam0f_%.so)

clean:
	find . -name '*.o' -delete
	find . -name '*.so' -delete
	rm -f $(EXE)