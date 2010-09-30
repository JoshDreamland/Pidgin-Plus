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
const char* ToCString(const String::Utf8Value&,const char* = "<string conversion failed>");

extern HandleScope               handle_to_global_scope; //Nothingness scope
extern Handle<ObjectTemplate>    global_object_template; //Containing global "this"
extern Handle<Context>           global_context; //Global Context

#endif
