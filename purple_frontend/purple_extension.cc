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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* config.h may define PURPLE_PLUGINS; protect the definition here so that we
 * don't get complaints about redefinition when it's not necessary. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>

/* This will prevent compiler errors in some instances and is better explained in the
 * how-to documents on the wiki */
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>

#include "macros.h"
#include "commands.h"

#include <string>
using namespace std;

void all_accounts_set_status(PurpleStatusPrimitive pstat,const string& msg)
{
  PurpleSavedStatus* nss = purple_savedstatus_new(NULL, pstat); 
  purple_savedstatus_set_message(nss, msg.c_str()); 
  purple_savedstatus_activate(nss);
}


string buddy_get_uname(PurpleAccount* account,string email)
{
  PurpleBuddy *b = purple_find_buddy(account, email.c_str());
  if (b == NULL)
    return email;
  const char* n = purple_buddy_get_server_alias(b);
  return n? n : email;
}
string buddy_get_alias(PurpleAccount* account,string email)
{
  PurpleBuddy *b = purple_find_buddy(account, email.c_str());
  const char *a = purple_buddy_get_contact_alias(b);
  return a? a : email;
}
string buddy_get_status(PurpleAccount* account,string email)
{
  return "<insert buddy's status here ^_^>";
}

string get_my_name(PurpleAccount* account) {
  return "LibPurple sucks.";
}
string get_my_email(PurpleAccount* account) {
  return "I mean, LibPurple REALLY sucks.";
}
string get_my_status(PurpleAccount* account) {
  return "I mean, LibPurple REALLY sucks.";
}


string get_my_ip()
{
  return "Oh, let me just look that up for you.";
}

//This will simplify the Purple-V8 bridge
int conv_to_print_to_type;
PurpleConversation* conv_to_print_to;
enum { pct_im = 0, pct_chat = 1 };

void set_receiving_window(PurpleConversation* conv, int window_type)
{
  conv_to_print_to = conv;
  conv_to_print_to_type = window_type;
}
void pidgin_printf(const char* message)
{
  switch (conv_to_print_to_type)
  {
    case pct_im:   purple_conv_im_send (PURPLE_CONV_IM(conv_to_print_to), message);     break;
    case pct_chat: purple_conv_chat_send (PURPLE_CONV_CHAT(conv_to_print_to), message); break;
  }
}

