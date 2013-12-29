# V8 Build/Link options
V8ARCH   := x64
V8MODE   := native
V8OUTDIR := google_v8/v8-read-only/out/$(V8MODE)/obj.target
V8LIBS   := $(V8OUTDIR)/tools/gyp/libv8_base.$(V8ARCH).a $(V8OUTDIR)/tools/gyp/libv8_snapshot.a
V8DEPS   := -Wl,--start-group $(V8LIBS) `icu-config --ldflags` -Wl,--end-group -lrt -lpthread

# This is where we make up for a lack of -weverything in GCC. No -werror, because libpurple does not compile with -pedantic.
WARNINGS := -Wall -Wextra -Wswitch-default -pedantic -Wmain -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum
CXXFLAGS := `pkg-config --cflags glib-2.0` `pkg-config --cflags purple` -I. $(WARNINGS)
CPPFLAGS := -fPIC -g
FLAGS := 
LIBS := `pkg-config --libs purple glib-2.0`
LINKS :=  $(V8DEPS)
OBJDIR := .objs

SOURCES = $(wildcard basics/*.cc)
SOURCES += $(wildcard commands_and_macros/*.cc)
SOURCES += $(wildcard compatibility/*.cc)
SOURCES += $(wildcard gtk_etc_frontend/*.cc)
SOURCES += $(wildcard purple_frontend/*.cc)
SOURCES += $(wildcard plugin_basics/*.cc)
SOURCES += $(wildcard scripting/*.cc)
SOURCES += $(wildcard scripting/js_objects/*.cc)
OBJECTS = $(addprefix $(OBJDIR)/,$(patsubst %.cc, %.o, $(SOURCES)))
OBJDIRS = $(sort $(dir $(OBJECTS)))

default: Release

$(OBJDIR): $(OBJDIRS)
	mkdir -p $@
$(OBJDIRS):
	mkdir -p $@

google_v8/v8-read-only:
	cd google_v8 && ./getnew.sh
	cd google_v8/v8-read-only && make dependencies

$(V8LIBS): google_v8/v8-read-only
	cd google_v8/v8-read-only && make CXXFLAGS=-fPIC CPPFLAGS=-fPIC $(V8MODE) GYPFLAGS="-Duse_system_icu=1"

$(OBJDIR)/%.o $(OBJDIR)/%.d: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c -o $(OBJDIR)/$*.o $<

Release: $(OBJECTS) $(V8LIBS)
	echo hi
	$(CXX) -o pidgin_plus.so -z defs -shared -fPIC $(OBJECTS) $(LIBS) $(LINKS)

clean cleanRelease:
	 rm -rf .objs

