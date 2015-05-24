/**
 * @file js_functions.cc
 *
 * This file defines built-in functions that are to be accessable through JS.
 * It is also responsible for setting allotted resources for each script, such
 * as the number of messages that can be sent in one event.
 *
 * @section License
 * Copyright (C) 2009 Josh Ventura
 *
 * Pidgin Plus! is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <www.gnu.org/licenses>
**/

#include "v8_shared.h"
#include "js_functions.h"
#include "js_objects/js_objects_basics.h"
#include "js_objects/js_objects_manifest.h"

#include <cstdio>

int prints_remaining = 5;

jsObject_basic::jsObject_basic(const char* n, jsFuncDesc* fs, jsClassDesc* cs):
    name(n), myFunctions(fs), myClasses(cs) { }


static void object_add_to_global(Handle<ObjectTemplate> obj, const char* name) {
  plus_v8_global->global->Set(plus_v8_global->string(name), obj);
}

void js_functions_initialize()
{
  // For every object that has registered itself
  for (jsObject_basic *obj : all_js_objects)
  {
    printf(" - %s\n", obj->name);
    puts("   * Generate object template");
    // Create the object in V8's environment
    obj->me = ObjectTemplate::New(plus_v8_global->isolate);
    
    // Add all its functions in
    puts("   * Populate with functions");
    for (jsFuncDesc* f = obj->myFunctions; f->name and f->jsFunc; f++) {
      printf("     - %s\n", f->name);
      obj->me->Set(
          plus_v8_global->string(f->name),
          FunctionTemplate::New(plus_v8_global->isolate, f->jsFunc));
    }
    
    // Add all its classes in
    puts("   * Populate with classes");
    for (jsClassDesc* c = obj->myClasses; c->name and c->jsSubClass; c++) {
      printf("     - %s\n", c->name);
      c->jsSubClass->me = ObjectTemplate::New(plus_v8_global->isolate);
      obj->me->Set(plus_v8_global->string(c->name), c->jsSubClass->me);
      c->jsSubClass->construct();
    }
    
    puts("   * Add to global");
    object_add_to_global(obj->me, obj->name);
  }
}

void js_resources_renew()
{
  prints_remaining = 5;
}
