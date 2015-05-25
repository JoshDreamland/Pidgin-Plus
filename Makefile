# V8 Build/Link options
V8ARCH   := x64
V8MODE   := $(V8ARCH).release
V8OUTDIR := v8/out/$(V8MODE)/obj.target
V8FLAGS  := -Iv8
V8LIBS   := $(V8OUTDIR)/tools/gyp/libv8_base.a $(V8OUTDIR)/tools/gyp/libv8_libbase.a $(V8OUTDIR)/tools/gyp/libv8_snapshot.a $(V8OUTDIR)/tools/gyp/libv8_libplatform.a
V8DEPS   := -Wl,--start-group $(V8LIBS) $(V8OUTDIR)/third_party/icu/libicudata.a $(V8OUTDIR)/third_party/icu/libicui18n.a $(V8OUTDIR)/third_party/icu/libicuuc.a -lpthread -ldl -Wl,--end-group
# `icu-config --ldflags`

# NOTE: -Wundef has been disabled because Chrome team has most warnings disabled
# and preprocessor warnings cannot be deactivated using #pragma.

# This is where we make up for a lack of -weverything in GCC. No -werror, because libpurple does not compile with -pedantic.
WARNINGS := -Wall -Wextra -Wswitch-default -pedantic -Wmain -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wfloat-equal -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum
CXXFLAGS := `pkg-config --cflags glib-2.0` `pkg-config --cflags purple` -I. $(V8FLAGS) $(WARNINGS)
CPPFLAGS := -std=c++11 -fPIC -g
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

export PATH := `pwd`depot_tools:$(PATH)

default: Release

$(OBJDIR): $(OBJDIRS)
	mkdir -p $@
$(OBJDIRS):
	mkdir -p $@

depot_tools:
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
v8: depot_tools
	fetch v8
$(V8LIBS): v8
	cd v8 && make $(V8MODE) library=static CFLAGS="-fPIC" CXXFLAGS="-fPIC" -j 4

$(OBJDIR)/%.o $(OBJDIR)/%.d: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $(OBJDIR)/$*.o

pidgin_plus.so: $(OBJECTS) $(V8LIBS)
	$(CXX) -o pidgin_plus.so -z defs -shared -fPIC $(OBJECTS) $(LIBS) $(LINKS)

Release: pidgin_plus.so

clean cleanRelease:
	 rm -rf .objs

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJECTS:.o=.d)
endif
