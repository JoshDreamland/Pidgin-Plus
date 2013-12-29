/*
 * Pidgin Plus! Plugin
 *
 * Copyright (C) 2009 Josh Ventura
 *
 * Pidgin Plus! is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <www.gnu.org/licenses>
 *
 */
 
 /**
   @file v8_shared.cc
   @brief Defines wrapper calls to simple V8 functionality.
   
   This file defines some calls and variables that are shared
   by files in the /scripting/ directory. Some declarations here do
   more than they would suggest; see individual comments for details
 */
 
#include "v8_shared.h"

plus_v8_instance::plus_v8_instance():
    isolate(Isolate::GetCurrent()),        // Step one:   Get isolate
    scope(isolate),                        // Step two:   Create handleScope
    global(ObjectTemplate::New(isolate)),  // Step three: Create global scope
    context(),                             // Step four:  Create context; this is done in begin()
    begun(0)                               // Which has not yet been invoked.
  {}

void plus_v8_instance::begin() {
  if (!begun) {
    begun = 1;
    context = Context::New(isolate, NULL, global);
    context->Enter();
  }
}

void plus_v8_instance::end() {
  if (begun) {
    begun = 0;
    context->Exit();
  }
}

plus_v8_instance *plus_v8_global;

Local<String> plus_v8_instance::NewString(std::string str) {
  return String::NewFromUtf8(isolate, str.c_str());
}
Local<String> plus_v8_instance::NewString(const char* str) {
  return String::NewFromUtf8(isolate, str);
}

// Functions to work with strings, in case something new and fantastic happens to the V8 API
Local<String> GV8::String(const char* str) {
  return plus_v8_global->NewString(str);
}
Local<String> GV8::String(std::string str) {
  return plus_v8_global->NewString(str);
}
Local<String> GV8::String(Isolate *iso, const char* str) {
  return String::NewFromUtf8(iso, str);
}
Local<String> GV8::String(Isolate *iso, std::string str) {
  return String::NewFromUtf8(iso, str.c_str());
}

v8_funcresult GV8::Return(v8_funcargs args, Handle<Value> val) {
  args.GetReturnValue().Set(val);
}

// These are just convenient
const char* ToCString(const String::Utf8Value& value, const char* ifnull) {
  return *value ? *value : ifnull;
}
string ValueToStr(Handle<Value> val)
{
  const String::Utf8Value str(val);
  return *str ? *str : "ERROR: Cannot convert output to string";
}
