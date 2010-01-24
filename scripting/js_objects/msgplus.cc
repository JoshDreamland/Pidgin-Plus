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

#include "js_objects_basics.h"

struct MsgPlus
{
  Handle<ObjectTemplate> me;
  
  JavaScript:
    struct _version
    {
      Handle<ObjectTemplate> me;
      
      static Handle<Value> toString(const Arguments& args) {
        return String::New("Pidgin Plus! Version 0.00.01");
      }
      static Handle<Value> valueOf (const Arguments& args) {
        return Integer::New(5);
      }
      
      _version(): me(ObjectTemplate::New()) {
        me->Set(String::New("toString"), FunctionTemplate::New(toString) );
        me->Set(String::New("valueOf"),  FunctionTemplate::New(valueOf)  );
      }
    } version;
    
  Foundational:
  
  MsgPlus(): me(ObjectTemplate::New()) {
    me->Set(String::New("version"), version.me);
  }
} *inst_MsgPlus = NULL;

void jso_init_msgplus()
{
  delete inst_MsgPlus;
  inst_MsgPlus = new MsgPlus;
  object_add_to_global(inst_MsgPlus->me,"MsgPlus");
}
void jso_free_msgplus()
{
  delete inst_MsgPlus;
}
