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
 
#ifndef _V8_SHARED__H
#define _V8_SHARED__H

#include <string>
using namespace std;

//Assuming those who want to compile this have 
//checked out V8 as per my well-hidden instruction
#include "google_v8/v8-read-only/include/v8.h"

using namespace v8;

string ValueToStr(Handle<Value>);
const char* ToCString(const String::Utf8Value&, const char* = "<string conversion failed>");

typedef void v8_funcresult;
typedef const v8::FunctionCallbackInfo<Value>& v8_funcargs;

/// We'll only be using one of these, but the ctor does a lot, so we'll be allocating it dynamically.
extern struct plus_v8_instance {
  Isolate*               isolate;  ///< Big-scale isolate. I think it's one per process.
  HandleScope            scope;    ///< I don't know why this is required here, but it's important.
  Handle<ObjectTemplate> global;   ///< This is the thing we add objects into, then pass to Context::New.
  Handle<Context>        context;  ///< Finally, the context, which depends on everything above. Initialized in begin().
  
  /// Creates and enters the context; must be called AFTER defining all functions, etc.
  void begin();
  /// Ends whatever begin() begins.
  void end();
  
  // Prevent getting bitten by changes to the V8 API.
  /// Construct a V8 String from an std::string.
  Local<String> NewString(std::string str);
  /// Construct a V8 String from a C string.
  Local<String> NewString(const char* str);
  
  /// Construct, creating a bunch of V8 stuff.
  plus_v8_instance();
  
  private:
    bool begun;
  
} *plus_v8_global;

namespace GV8 {
  v8::Local<v8::String> String(std::string x);
  v8::Local<v8::String> String(const char* x);
  v8::Local<v8::String> String(Isolate *iso, std::string x);
  v8::Local<v8::String> String(Isolate *iso, const char* x);
  v8_funcresult Return(v8_funcargs args, Handle<Value> val);
}

#endif
