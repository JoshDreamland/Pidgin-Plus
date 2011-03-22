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

/**
   @file purple_extension.cc
   @summary Contrary to the implications of the filename, this header
     is merely intended to function as a wrapper to sets of libpurple
     calls, in order to shrink code and disambiguate calls. Frankly,
     it becomes too big a pain in the ass to constantly anticipate
     every single scenario that different protocols can bring to an
     otherwise simple situation. If I had to code with such vague and
     often ambiguous cross-protocol terms as the Pidgin developers, I
     would go bat-shit insane. Props to them.
*/

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>

#include <string>
using namespace std;

void all_accounts_set_status(PurpleStatusPrimitive pstat,const string& msg)
{
  PurpleSavedStatus* nss = purple_savedstatus_new(NULL, pstat); 
  purple_savedstatus_set_message(nss, msg.c_str()); 
  purple_savedstatus_activate(nss);
}

int generic_protocol_invite_contact(PurpleConversation* conv)
{
  return false;
}


// Test the protocol used by a given conversation for a match to the given string
bool conversation_protocol_match(PurpleConversation* conv,const char* str) {
  const char* pn = purple_plugin_get_name(purple_find_prpl(conv->account->protocol_id));
  string pname = pn; return (pname.find(str) != string::npos);
}


string get_my_ip()
{
  return "Oh, let me just look that up for you.";
}

//This will simplify the Purple-V8 bridge
int conv_to_print_to_type;
PurpleConversation* conv_to_print_to;
string conv_protocol;
enum { pct_im = 0, pct_chat = 1 };

void set_receiving_window(PurpleConversation* conv, int window_type)
{
  conv_to_print_to = conv;
  conv_to_print_to_type = window_type;
  conv_protocol = conv->account->protocol_id;
}
inline int scountc(const char* s, char c)
{
  int cnt = 0;
  for (const char *i = s; *i; i++)
    cnt += *i == c;
  return cnt;
}

void pidgin_printf(const char* message)
{
  static int oncallstack = false;
  if (oncallstack) return;
  oncallstack = true;
  int lnc;
  if ((conv_protocol != "prpl-irc" and conv_protocol != "irc") or (lnc = scountc(message, '\n')) < 5)
    switch (conv_to_print_to_type)
    {
      case pct_im:   purple_conv_im_send (PURPLE_CONV_IM(conv_to_print_to), message);     break;
      case pct_chat: purple_conv_chat_send (PURPLE_CONV_CHAT(conv_to_print_to), message); break;
    }
  else
  {
    string msg = message; int sc = 1;
    const float pkat_base = lnc / 4.0;
    float pkat = pkat_base;
    
    for (size_t i = 0; i < msg.length(); i++) {
      if (sc >= pkat and pkat+.1 < lnc) { if (msg[i] == '\n') pkat += pkat_base, sc++; continue; }
      if (msg[i] == '\r') msg[i] = ' ';
      if (msg[i] == '\n') msg.replace(i,1," /  "), sc++;
    }
    
    switch (conv_to_print_to_type)
    {
      case pct_im:   purple_conv_im_send (PURPLE_CONV_IM(conv_to_print_to),     msg.c_str()); break;
      case pct_chat: purple_conv_chat_send (PURPLE_CONV_CHAT(conv_to_print_to), msg.c_str()); break;
    }
  }
  oncallstack = false;
}

