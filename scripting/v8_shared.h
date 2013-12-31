/**
 * @file v8_shared.h
 * Header declaring V8 objects and functions for use in all JS sources.
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
 
#ifndef _V8_SHARED__H
#define _V8_SHARED__H

#include <string>
using namespace std;

// It is assumed those who want to compile this have 
// checked out V8 as per my well-hidden instructions,
// or that the makefile's process to do so actually works.
#include "google_v8/v8-read-only/include/v8.h"

using namespace v8;

/// Get an std::string representation of any V8 Value.
string ValueToStr(Handle<Value>);
/// Get a C string representation of a V8 UTF-8 string, or if null, return the second parameter.
const char* ToCString(const String::Utf8Value&, const char* = "<string conversion failed>");

/// A structure representing an instance of Google V8, packed with our extension functions.
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

/// The return type required of all native (C++) JavaScript functions. Used in case the V8 API changes again.
typedef void v8_funcresult;
/// The arguments type required of all native (C++) JavaScript functions. Used in case the V8 API changes again.
typedef const v8::FunctionCallbackInfo<Value>& v8_funcargs;

/// Global V8 functions; functions which can be called assuming they are to operate inside our own isolate/context.
namespace GV8 {
  v8::Local<v8::String> String(std::string x); ///< Construct a V8 String in our global scope from only a given std::string.
  v8::Local<v8::String> String(const char* x); ///< Construct a V8 String in our global scope from only a given C string.
  v8::Local<v8::String> String(Isolate *iso, std::string x); ///< Construct a V8 String in our global scope from a given std::string and isolate from anywhere. Offered in case the V8 API changes again.
  v8::Local<v8::String> String(Isolate *iso, const char* x); ///< Construct a V8 String in our global scope from a given C string and isolate from anywhere. Offered in case the V8 API changes again.
  v8_funcresult Return(v8_funcargs args, Handle<Value> val); ///< Return values through this function from your JS functions, eg, return GV8::Return(args, value). Offered in case the V8 API changes again.
}

#endif
