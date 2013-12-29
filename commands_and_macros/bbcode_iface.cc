/**
 * @file   bbcode_iface.cc
 * @brief  Source implementing formatting BBtags.
 * 
 * @section License
 * 
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
**/

#include "bbcode_iface.h"
#include <basics/basics.h>

bbcode_tag::bbcode_tag(string name, bool is_unary): unary(is_unary) { bbcode_tags[name] = this; }
bbcode_tag::~bbcode_tag() { }

string bbcode_tag::get_replacement(PurpleConversation*, PurpleAccount*, int, string, string, string) { return "Internal error: invalid tag."; }
string bbcode_tag::get_unary_replacement(PurpleConversation*, PurpleAccount*, int, string) { return "Internal error: invalid tag."; }

bbmap bbcode_tags;
map<string,double> bbcode_tag::vars; ///< Real-valued variables needed by the functions during BB parsing.
map<string,string> bbcode_tag::svars; ///< String-valued variables needed by the functions during BB parsing.

bool bbcode_tag::at(const char *msg, int pos, bbcode_tag::instance &inst) {
  if (msg[pos] != '[')
    return false;
  
  int ctsp = pos; // Complete tag starting position
  
  inst.closing = false; // Whether this is a closing tag (denoted by /).
  if (msg[++pos] == '/') {
    inst.closing = true;
    pos++;
  }
  
  if (!is_letter(msg[pos]))
    return false;
  
  const int ts = pos;
  while (is_letterd(msg[++pos]));
  
  inst.name = string(msg,ts,pos-ts);
  bbiter bit = bbcode_tags.find(inst.name);
  
  if (bit == bbcode_tags.end()) return false;
  inst.tag = bit->second;
  inst.unary = bit->second->unary;
  inst.arg = "";
  
  if (msg[pos] == '=') {
    const int as = ++pos;
    while (msg[pos] and msg[pos] != ']') pos++;
    if (msg[pos] != ']') { return false; }
    inst.arg = string(msg,as,pos-as);
  }
  
  if (msg[pos] != ']')
    return false;
  
  inst.length = pos - ctsp + 1;
  return true;
}
