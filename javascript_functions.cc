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


// Print things, limit to 25 messages a time, reset each parse call
int prints_remaining = 25;
Handle<Value> Print(const Arguments& args)
{
  bool first = true;
  for (int i = 0; i < args.Length(); i++)
  {
    HandleScope handle_scope;
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    pidgin_printf(cstr);
    if (prints_remaining-- <= 0)
      return ThrowException(String::New("Too many prints called in one script event"));
  }
  return Undefined();
}

void js_functions_initialize()
{
  global_object_template->Set(String::New("print"), FunctionTemplate::New(Print)); //This explains a lot
  
}

void js_resources_renew()
{
  prints_remaining = 25;
}
