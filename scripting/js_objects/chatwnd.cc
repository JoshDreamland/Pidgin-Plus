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
#include "../../purple_frontend/purple_extension.h"

extern int prints_remaining;

struct ChatWnd
{
  Handle<ObjectTemplate> me;
  
  JavaScript:
  
      static Handle<Value> SendMessage(const Arguments& args)
      {
        for (int i = 0; i < args.Length(); i++)
        {
          HandleScope handle_scope;
          String::Utf8Value str(args[i]);
          string cstr = ToCString(str);
          if (cstr.length() > 512)
          {
            cstr.erase(511);
            cstr[508] = cstr[509] = cstr[510] = '.';
          }
          pidgin_printf(cstr.c_str());
          if (prints_remaining-- <= 0)
            return ThrowException(String::New("Too many messages sent in one script event"));
        }
        return Undefined();
      }
  
  Foundational:
  
  ChatWnd(): me(ObjectTemplate::New()) {
    me->Set(String::New("SendMessage"), FunctionTemplate::New(SendMessage));
  }
} *inst_ChatWnd;

void jso_init_chatwnd()
{
  delete inst_ChatWnd;
  inst_ChatWnd = new ChatWnd;
  object_add_to_global(inst_ChatWnd->me,"ChatWnd");
}
void jso_free_chatwnd()
{
  delete inst_ChatWnd;
}
