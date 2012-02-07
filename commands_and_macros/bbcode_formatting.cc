/**
 * @file   bbcode_formatting.cc
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
 *
**/

#include "bbcode_iface.h"

/// Struct defining the bold tag, [b].
struct boldtag: bbcode_tag {
  void init() { vars["bold"] = vars["boldstart"] = 0; }
  string get_replacement(PurpleConversation*, PurpleAccount*, string content, string) { return "<b>" + content + "</b>"; }
  boldtag(): bbcode_tag("b",false) {}
};

/// Struct defining the bold tag, [i].
struct italictag: bbcode_tag {
  void init() { vars["ital"] = 0; }
  string get_replacement(PurpleConversation*, PurpleAccount*, string content, string) { return "<i>" + content + "</i>"; }
  italictag(): bbcode_tag("i",false) {}
};

/// Struct defining the bold tag, [u].
struct underlinetag: bbcode_tag {
  void init() { vars["undr"] = 0; }
  string get_replacement(PurpleConversation*, PurpleAccount*, string content, string) { return "<u>" + content + "</u>"; }
  underlinetag(): bbcode_tag("u",false) {}
};

void load_bb_formatting();
void load_bb_formatting() {
  new boldtag;
  new italictag;
  new underlinetag;
}
