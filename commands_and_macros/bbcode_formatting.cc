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
**/

#include "bbcode_iface.h"
#include "bbcode_formatting.h"

/// Struct defining the bold tag, [b].
struct boldtag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) { return "<b>" + content + "</b>"; }
  boldtag(): bbcode_tag("b",false) {}
};

/// Struct defining the bold tag, [i].
struct italictag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) { return "<i>" + content + "</i>"; }
  italictag(): bbcode_tag("i",false) {}
};

/// Struct defining the bold tag, [u].
struct underlinetag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) { return "<u>" + content + "</u>"; }
  underlinetag(): bbcode_tag("u",false) {}
};

/// Struct defining a rainbow tag, [rb].
struct rainbowtag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    char colors[10] = { '4', '7', '8', '3', '2', '6' };
    string res = "\00304"; int color = 0;
    for (const char* uc = content.c_str(); *uc; ) {
      const char *ucf = uc; uc = g_utf8_next_char(uc);
      res += string(ucf,uc);
      gunichar c = g_utf8_get_char(ucf);
      if (!g_unichar_isspace(c)) {
        res += "\0030", res += colors[++color % 6];
      }
    }
    return res;
  }
  rainbowtag(): bbcode_tag("rb",false) {}
};

/// Struct defining a rainbow word tag, [rbw].
struct rainbowwordtag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    char colors[10] = { '4', '7', '8', '3', '2', '6' };
    string res = "\00304"; int color = 0;
    for (const char* uc = content.c_str(); *uc; ) {
      const char *ucf = uc;
      uc = g_utf8_next_char(uc);
      res += string(ucf,uc);
      gunichar c = g_utf8_get_char(ucf);
      if (g_unichar_isspace(c)) {
        res += "\0030", res += colors[++color % 6];
      }
    }
    return res;
  }
  rainbowwordtag(): bbcode_tag("rbw",false) {}
};

void load_bb_formatting() {
  new boldtag;
  new italictag;
  new underlinetag;
  new rainbowtag;
  new rainbowwordtag;
}
