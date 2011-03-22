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
/*
#include "../v8_shared.h"
#include "js_objects_basics.h"

void object_add_to_global(Handle<ObjectTemplate> obj, const char* name)
{
  global_object_template->Set(String::New(name), obj);
}

vector<jsObject_basic*> jsObjects;
void register_js_object(jsObject_basic* whom)
{
  jsObjects.push_back(whom);
}

void init_js_objects()
{
  for (size_t i = 0; i < jsObjects.size(); i++)
  {
    jsObjects[i]->me = ObjectTemplate::New()
    for (jsFuncDesc *i = jsObjects[i]->myFunctions; i->name; i++)
      jsObjects[i]->me->Set(String::New(i->name), FunctionTemplate::New(i->jsFunc));
    object_add_to_global(inst_ChatWnd->me,jsObjects[i].name);
  }
}
*/
