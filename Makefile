CFLAGS = `pkg-config --cflags purple`
LIBS = `pkg-config --libs purple`

OBJECTS = basics/basics.o plugin_basics/plugin_template.o plugin_basics/plugin_events.o commands_and_macros/macros.o commands_and_macros/commands.o purple_frontend/purple_extension.o gtk_etc_frontend/date_time.o scripting/v8_implementation.o scripting/v8_shared.o scripting/js_functions.o scripting/js_objects/js_objects_basics.o scripting/js_objects/chatwnd.o scripting/js_objects/msgplus.o


%.so: $(OBJECTS)
	$(CXX) -fpic -shared -o $@ ./*.o $(LIBS) ./google_v8/v8-read-only/libv8.a

.SUFFIXES: .cc .c .o

.cc.o:
	$(CXX) -c $< $(CFLAGS)

.c.o:
	$(CC) -c $< $(CFLAGS)

