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
 
#ifndef _V8_SHARED_H
#define _V8_SHARED_H

#include <string>
using namespace std;

//Assuming those who want to compile this have 
//checked out V8 as per my well-hidden instruction
#include "../google_v8/v8-read-only/include/v8.h"

using namespace v8;

string ValueToStr(Handle<Value>);
const char* ToCString(const String::Utf8Value&, const char* = "<string conversion failed>");

extern struct plus_v8_instance {
  // This block of declarations CANNOT BE REORDERED. 
  // Each one has a complicated-ASS private constructor.
  // Rearranging or removing these declarations could cause V8 to not work.
  HandleScope               scope; //Global scope
  Handle<ObjectTemplate>    object_template; //Containing global "this"
  Handle<Context>           context; //Global Context
  plus_v8_instance();
} *plus_v8_global;

#endif
