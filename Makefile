# Defaults

# This is where we make up for a lack of -weverything in GCC. No -werror, because libpurple does not compile with -pedantic.
CXXFLAGS := `pkg-config --cflags purple glib-2.0` -I. -Wall -Wextra -Wswitch-default -pedantic -Wmain -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum
CPPFLAGS := -fPIC
FLAGS := 
LIBS := `pkg-config --libs purple glib-2.0`
LINKS := './google_v8/v8-read-only/libv8.a'
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

$(OBJDIR)/%.o $(OBJDIR)/%.d: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c -o $(OBJDIR)/$*.o $<

Release: $(OBJECTS)
	$(CXX) -shared -fPIC $(OBJECTS) $(LIBS) $(LINKS) -o pidgin_plus.so

clean cleanRelease:
	 rm -rf .objs

