/**
 * @file   bbcode_unicode.cc
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
#include "compatibility/codes.h"

/// Utility function to make alphabetic replacements
static void realphabet(string& str, const char* alphabet) {
  for (size_t i = 0; i < str.length(); i++)
  {
    if (str[i] == '[') {
      bbcode_tag::instance tag;
      if (bbcode_tag::at(str.c_str(),i,tag))
        i += tag.length-1;
      continue;
    }
    if (str[i] >= 'A' && str[i] <= 'Z') {
      char *r = g_utf8_substring(alphabet, str[i]-'A', str[i]-'A'+1);
      str.replace(i,1,r);
      g_free(r);
      continue;
    }
    if (str[i] >= 'a' && str[i] <= 'z') {
      char *r = g_utf8_substring(alphabet, 26+str[i]-'a', 27+str[i]-'a');
      str.replace(i,1,r);
      g_free(r);
    }
  }
}

/// Struct defining the unicode sans-serif bold tag, [ub].
struct unicode_bold_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_sansbold);
    return content;
  }
  unicode_bold_tag(): bbcode_tag("ub",false) {}
};

/// Struct defining the unicode sans-serif italics tag, [ui].
struct unicode_italic_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_sansitalic);
    return content;
  }
  unicode_italic_tag(): bbcode_tag("ui",false) {}
};

/// Struct defining the unicode sans-serif bold italics tag, [ubi].
struct unicode_bold_italic_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_sansbolditalic);
    return content;
  }
  unicode_bold_italic_tag(): bbcode_tag("ubi",false) {}
};

/// Struct defining the unicode serif bold tag, [usb].
struct unicode_serif_bold_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_serifbold);
    return content;
  }
  unicode_serif_bold_tag(): bbcode_tag("usb",false) {}
};

/// Struct defining the unicode serif italics tag, [usi].
struct unicode_serif_italic_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_serifitalic);
    return content;
  }
  unicode_serif_italic_tag(): bbcode_tag("usi",false) {}
};

/// Struct defining the unicode serif bold-italics tag, [usbi].
struct unicode_serif_bold_italic_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_serifbolditalic);
    return content;
  }
  unicode_serif_bold_italic_tag(): bbcode_tag("usbi",false) {}
};

/// Struct defining the unicode double-strike tag, [ds].
struct unicode_doublestrike_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_doublestrike);
    return content;
  }
  unicode_doublestrike_tag(): bbcode_tag("ds",false) {}
};

/// Struct defining the unicode frakture tag, [frak].
struct unicode_fraktur_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_fraktur);
    return content;
  }
  unicode_fraktur_tag(): bbcode_tag("frak",false) {}
};

/// Struct defining the unicode script tag, [script].
struct unicode_script_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_script);
    return content;
  }
  unicode_script_tag(): bbcode_tag("script",false) {}
};

/// Struct defining the unicode monospace tag, [mono].
struct unicode_mono_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_monospace);
    return content;
  }
  unicode_mono_tag(): bbcode_tag("mono",false) {}
};

/// Struct defining the unicode braille tag, [braille].
struct unicode_braille_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_braille);
    return content;
  }
  unicode_braille_tag(string name = "braille"): bbcode_tag(name,false) {}
};

/// Struct defining the unicode circled letters tag, [circle].
struct unicode_circle_tag: bbcode_tag {
  void init() { }
  string get_replacement(PurpleConversation*, PurpleAccount*, int, string content, string, string) {
    realphabet(content, unicode_circled);
    return content;
  }
  unicode_circle_tag(string name = "circle"): bbcode_tag(name,false) {}
};


void load_bb_unicode();
void load_bb_unicode() {
  new unicode_bold_tag;
  new unicode_italic_tag;
  new unicode_bold_italic_tag;
  new unicode_serif_bold_tag;
  new unicode_serif_italic_tag;
  new unicode_serif_bold_italic_tag;
  new unicode_doublestrike_tag;
  new unicode_fraktur_tag;
  new unicode_script_tag;
  new unicode_mono_tag;
  new unicode_braille_tag;
  new unicode_braille_tag("brail");
  new unicode_circle_tag;
  new unicode_circle_tag("circ");
}
