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
#include <stdlib.h>

#define NAME MsgPlus

static double vs_to_double(const char* x)
{
  bool rdot = 0;
  string s = x;
  while (*(x++))
  {
    if (*x == '.')
    {
      if (!rdot)
        s += *x;
      rdot = true;
    }
    else
      s += *x;
  }
  return atof(s.c_str());
}

Begin_JavaScript_Functions
//{
    Begin_SubClass(version)
        static v8_funcresult toString(v8_funcargs args)  {
          return GV8::Return(args, String::NewFromUtf8(
              args.GetIsolate(), "Pidgin Plus! Version 0.00.01"));
        }
        static v8_funcresult valueOf (v8_funcargs args)  {
          return GV8::Return(args, Integer::New(plus_v8_global->isolate, 5));
        }
        
        void construct () override {
          addFunction("toString", toString);
          addFunction("valueOf",  valueOf);
        }
    End_SubClass(version)
    
    Begin_SubClass(v8version)
        static v8_funcresult toString(v8_funcargs args)  {
          return GV8::Return(args, plus_v8_global->string(string("Google V8 version ") + V8::GetVersion()));
        }
        static v8_funcresult valueOf (v8_funcargs args)  {
          return GV8::Return(args, Number::New(plus_v8_global->isolate, vs_to_double(V8::GetVersion())));
        }
        
        void construct () override  {
          addFunction("toString", toString);
          addFunction("valueOf",  valueOf);
        }
    End_SubClass(v8version)
//}
End_JavaScript_Functions

Link_Class(MsgPlus, version)
Link_Class(MsgPlus, v8version)

Begin_Class_List
  Add_Class(version,    MsgPlus::version)
  Add_Class(v8version,  MsgPlus::v8version)
  Add_Class(js_version, MsgPlus::v8version)
End_Class_List

Begin_Function_List
End_Function_List

Finalize_JavaScript_Class();
