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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <include/v8.h>
#include <include/libplatform/libplatform.h>
#pragma GCC diagnostic pop

using namespace v8;

/// Get an std::string representation of any V8 Value.
string ValueToStr(Handle<Value>);
/// Get a C string representation of a V8 UTF-8 string, or if null, return the second parameter.
const char* ToCString(const String::Utf8Value&, const char* = "<string conversion failed>");

/// A structure representing an instance of Google V8, packed with our extension functions.
/// We'll only be using one of these, but the ctor does a lot, so we'll be allocating it dynamically.
extern class PlusV8Instance {
  
  typedef Handle<ObjectTemplate> HObjTemplate;
  
  static Platform    *makePlatform();
  static Isolate     *makeIsolate();
  static HObjTemplate makeObjTemplate(Isolate*);

 public:
  Platform* const      platform; ///< The main V8 platform. I never really use this.
  Isolate* const       isolate;  ///< Big-scale isolate. I think it's one per process.
  const Isolate::Scope isoscope; ///< Is this really necessary?
  const HandleScope    scope;    ///< Cannot create a handle without a HandleScope
  const HObjTemplate   global;   ///< This is the thing we add objects into, then pass to Context::New.
  Handle<Context>      context;  ///< Finally, the context, which depends on everything above. Initialized in begin().
  
  /// Creates and enters the context; must be called AFTER defining all functions, etc.
  void begin();
  /// Ends whatever begin() begins.
  void end();
  
  // Prevent getting bitten by changes to the V8 API.
  /// Construct a V8 String from an std::string.
  Local<String> string(std::string str);
  /// Construct a V8 String from a C string.
  Local<String> string(const char* str);
  
  /// Construct, creating a bunch of V8 stuff.
  PlusV8Instance();
  
  private:
    bool begun;
  
} *plus_v8_global;

/// The return type required of all native (C++) JavaScript functions. Used in case the V8 API changes again.
typedef void v8_funcresult;
/// The arguments type required of all native (C++) JavaScript functions. Used in case the V8 API changes again.
typedef const v8::FunctionCallbackInfo<Value>& v8_funcargs;

/// Global V8 functions; functions which can be called assuming they are to operate inside our own isolate/context.
namespace GV8 {
  v8_funcresult Return(v8_funcargs args, Handle<Value> val); ///< Return values through this function from your JS functions, eg, return GV8::Return(args, value). Offered in case the V8 API changes again.
}

#endif
