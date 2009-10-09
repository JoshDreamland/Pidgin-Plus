CFLAGS = `pkg-config --cflags purple`
LIBS = `pkg-config --libs purple`

OBJECTS = plugin_template.o purple_extension.o macros.o date_time.o commands.o

%.so: $(OBJECTS)
	$(CXX) -fpic -shared -o $@ $^ $(LIBS)

.SUFFIXES: .cc .c .o

.cc.o:
	$(CXX) -c $< $(CFLAGS)

.c.o:
	$(CC) -c $< $(CFLAGS)

