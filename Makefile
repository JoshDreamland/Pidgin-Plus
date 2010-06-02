# Awesome Makefile generated by ./genmake.sh
# File consists of a lot of gobbledy followed by a target

CFLAGS = `pkg-config --cflags purple`
LIBS = `pkg-config --libs purple`
LINKS = './google_v8/v8-read-only/libv8.a'

.objs/basics.o: ./basics/basics.cc
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./basics/basics.cc		-o .objs/basics.o
.objs/commands.o: ./commands_and_macros/commands.cc commands_and_macros/commands.h purple_frontend/purple_extension.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./commands_and_macros/commands.cc		-o .objs/commands.o
.objs/macros.o: ./commands_and_macros/macros.cc gtk_etc_frontend/date_time.h gtk_etc_frontend/gtk_timer_struct.h purple_frontend/purple_extension.h basics/basics.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./commands_and_macros/macros.cc		-o .objs/macros.o
.objs/date_time.o: ./gtk_etc_frontend/date_time.cc
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./gtk_etc_frontend/date_time.cc		-o .objs/date_time.o
.objs/plugin_events.o: ./plugin_basics/plugin_events.cc commands_and_macros/macros.h commands_and_macros/commands.h purple_frontend/purple_extension.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./plugin_basics/plugin_events.cc		-o .objs/plugin_events.o
.objs/plugin_template.o: ./plugin_basics/plugin_template.cc plugin_basics/plugin_events.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./plugin_basics/plugin_template.cc		-o .objs/plugin_template.o
.objs/purple_extension.o: ./purple_frontend/purple_extension.cc
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./purple_frontend/purple_extension.cc		-o .objs/purple_extension.o
.objs/js_functions.o: ./scripting/js_functions.cc scripting/v8_shared.h google_v8/v8-read-only/include/v8.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./scripting/js_functions.cc		-o .objs/js_functions.o
.objs/v8_implementation.o: ./scripting/v8_implementation.cc scripting/v8_shared.h google_v8/v8-read-only/include/v8.h scripting/js_functions.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./scripting/v8_implementation.cc		-o .objs/v8_implementation.o
.objs/v8_shared.o: ./scripting/v8_shared.cc scripting/v8_shared.h google_v8/v8-read-only/include/v8.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./scripting/v8_shared.cc		-o .objs/v8_shared.o
.objs/chatwnd.o: ./scripting/js_objects/chatwnd.cc scripting/js_objects/js_objects_basics.h scripting/v8_shared.h google_v8/v8-read-only/include/v8.h purple_frontend/purple_extension.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./scripting/js_objects/chatwnd.cc		-o .objs/chatwnd.o
.objs/js_objects_basics.o: ./scripting/js_objects/js_objects_basics.cc scripting/v8_shared.h google_v8/v8-read-only/include/v8.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./scripting/js_objects/js_objects_basics.cc		-o .objs/js_objects_basics.o
.objs/msgplus.o: ./scripting/js_objects/msgplus.cc scripting/js_objects/js_objects_basics.h scripting/v8_shared.h google_v8/v8-read-only/include/v8.h
	-mkdir .objs
	$(CXX) -Wall -fPIC $(CFLAGS)   -c  ./scripting/js_objects/msgplus.cc		-o .objs/msgplus.o

# Nobody knows the trouble I've seen, no... no... nooo...
link: .objs/basics.o .objs/commands.o .objs/macros.o .objs/date_time.o .objs/plugin_events.o .objs/plugin_template.o .objs/purple_extension.o .objs/js_functions.o .objs/v8_implementation.o .objs/v8_shared.o .objs/chatwnd.o .objs/js_objects_basics.o .objs/msgplus.o
	$(CXX) -shared -fPIC .objs/*.o $(LIBS) $(LINKS) -o pidgin_plus.so

Release: link
	 # Nothing to do here.
cleanRelease:
	 rm ./.objs/*.o

