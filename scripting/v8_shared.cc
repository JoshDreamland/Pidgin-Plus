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
   @summary This file defines some calls and variables that are shared
     by files in the ^/scripting/ directory. Some declarations here do
     more than they would suggest; see individual comments for details
 */
 
#include "v8_shared.h"
plus_v8_instance::plus_v8_instance(): scope(), object_template(ObjectTemplate::New()), context() {}
plus_v8_instance *plus_v8_global;


// These are just convenient
const char* ToCString(const String::Utf8Value& value,const char* ifnull) {
  return *value ? *value : ifnull;
}
string ValueToStr(Handle<Value> val)
{
  const String::Utf8Value str(val);
  return *str ? *str : "ERROR: Cannot convert output to string";
}
