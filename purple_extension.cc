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
  return purple_buddy_get_server_alias(b);
}

string buddy_get_alias(PurpleAccount* account,string email)
{
  PurpleBuddy *b = purple_find_buddy(account, email.c_str());
  return purple_buddy_get_contact_alias(b);
}



string get_my_name(PurpleAccount* account)
{
  return "LibPurple sucks.";
}

string get_my_email(PurpleAccount* account)
{
  return "I mean, LibPurple REALLY sucks.";
}


string get_my_ip()
{
  return "Oh, let me just look that up for you.";
}

PurpleConversation* conv_to_print_to;
void set_receiving_window(PurpleConversation* conv)
{
  conv_to_print_to = conv;
}
void pidgin_printf(const char* message)
{
  purple_conv_im_send (PURPLE_CONV_IM(conv_to_print_to), message);
}

