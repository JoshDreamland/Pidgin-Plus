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
   @file js_functions.cc
   @summary This file defines built-in functions that are to be
     accessable through JS. It is also responsible for setting
     allotted resources for each script, such as the number of
     messages that can be sent in one event.
*/

#include "v8_shared.h"
#include "js_functions.h"
#include "js_objects/js_objects_basics.h"
#include "js_objects/js_objects_manifest.h"

#include <stdio.h>

int prints_remaining = 5;

jsObject_basic::jsObject_basic(const char* n, jsFuncDesc* fs, jsClassDesc* cs): name(n), myFunctions(fs), myClasses(cs) { }
void jsSubClass_basic::construct() { }


static void object_add_to_global(Handle<ObjectTemplate> obj, const char* name) {
  plus_v8_global->object_template->Set(String::New(name), obj);
}


void js_functions_initialize()
{
  // For every object that has registered itself
  for (jsObject_basic **i = all_js_objects; *i; i++)
  { // for each object as obj
    jsObject_basic* obj = *i;
    
    // Create the object in V8's environment
    obj->me = ObjectTemplate::New();
    
    // Add all its functions in
    for (jsFuncDesc* f = obj->myFunctions; f->name and f->jsFunc; f++)
      obj->me->Set(String::New(f->name), FunctionTemplate::New(f->jsFunc));
    
    // Add all its classes in
    for (jsClassDesc* c = obj->myClasses; c->name and c->jsSubClass; c++)
    {
      c->jsSubClass->me = ObjectTemplate::New();
      obj->me->Set(String::New(c->name), c->jsSubClass->me);
      c->jsSubClass->construct();
    }
    object_add_to_global(obj->me, obj->name);
  }
}

void js_resources_renew()
{
  prints_remaining = 5;
}
