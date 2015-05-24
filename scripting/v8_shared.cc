/**
 * @file v8_shared.cc
 * @brief Defines wrapper calls to simple V8 functionality.
 * 
 * This file defines some calls and variables that are shared
 * by files in the /scripting/ directory. Some declarations here do
 * more than they would suggest; see individual comments for details
 * 
 * @section License
 * Copyright (C) 2009, 2013 Josh Ventura
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
 
#include "v8_shared.h"

PlusV8Instance::PlusV8Instance():
    platform(makePlatform()),         // Step one:    Create platform
    isolate(makeIsolate()),           // Step two:    Get isolate
    isoscope(isolate),                // Step what:   Create isolate scope
    scope(isolate),                   // Step three:  Create handleScope
    global(makeObjTemplate(isolate)), // Step four:   Create global scope
    context(),                  // Step five:   Create context; done in begin().
    begun(false)
  {}

Platform* PlusV8Instance::makePlatform() {
  puts("- Initialize ICU");
  v8::V8::InitializeICU();
  
  puts("- Request LibPlatform Platform");
  Platform* const platform = platform::CreateDefaultPlatform();

  puts("- Initialize V8 Platform");
  V8::InitializePlatform(platform);
  
  puts("- Initialize V8");
  V8::Initialize();
  
  return platform;
}

#include <string.h>
class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
} main_allocator;

Isolate* PlusV8Instance::makeIsolate() {
  puts("- Create Main V8 Isolate");
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = &main_allocator;
  Isolate* isolate = Isolate::New(create_params);
  return isolate;
}

PlusV8Instance::HObjTemplate PlusV8Instance::makeObjTemplate(Isolate* iso) {
  puts("- Create Main Object Template");
  return ObjectTemplate::New(iso);
}

void PlusV8Instance::begin() {
  if (!begun) {
    begun = 1;
    puts("- Creating main context");
    context = Context::New(isolate, nullptr, global);
    puts("- Entering...");
    context->Enter();
  }
}

void PlusV8Instance::end() {
  if (begun) {
    begun = 0;
    context->Exit();
  }
}

PlusV8Instance *plus_v8_global;

Local<String> PlusV8Instance::string(std::string str) {
  return String::NewFromUtf8(isolate, str.c_str());
}
Local<String> PlusV8Instance::string(const char* str) {
  return String::NewFromUtf8(isolate, str);
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
