/*
 * Pidgin Plus! Plugin
 *
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

#include "js_objects_basics.h"
#include "../../purple_frontend/purple_extension.h"
#include "../v8_shared.h"

#define NAME ChatWnd

extern int prints_remaining;

Begin_JavaScript_Functions
//{
    static v8_funcresult SendMessage(v8_funcargs args)
    {
      Isolate *iso = args.GetIsolate();
      HandleScope handle_scope(iso);
      for (int i = 0; i < args.Length(); i++)
      {
        String::Utf8Value str(args[i]);
        string cstr = ToCString(str);
        if (cstr.length() > 512)
        {
          cstr.erase(511);
          cstr[508] = cstr[509] = cstr[510] = '.';
        }
      int linesprinted = pidgin_printf(cstr.c_str());
        prints_remaining -= linesprinted ? linesprinted : 1;
        if (prints_remaining <= 0)
          return void(iso->ThrowException(
              String::NewFromUtf8(iso, "Too many messages sent in one script event")));
      }
    }
//}
End_JavaScript_Functions

Begin_Function_List
   Add_Function(SendMessage, ChatWnd::SendMessage)
End_Function_List

Begin_Class_List
End_Class_List

Finalize_JavaScript_Class();
