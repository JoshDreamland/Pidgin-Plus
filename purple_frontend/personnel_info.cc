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

#include <notify.h>
#include <plugin.h>
#include <version.h>
#include <account.h>
#include <status.h>
#include <blist.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>

#include <string>
using namespace std;

#include "personnel_info.h"

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

static const char* purple_status_get_text_or_message_or_whatever_the_purple_developers_think_it_should_be_called(PurpleStatus *ps) {
  if (!ps) return "";
  const char* stat = purple_status_get_attr_string(ps, "message");
  if (stat) return stat;
  stat = purple_status_get_name(ps);
  if (stat) return stat;
  return "";
}
string buddy_get_status(PurpleAccount* account,string email)
{
  PurpleBuddy *pb = purple_find_buddy(account, email.c_str());
  if (!pb) return ""; // Phony email, or super-fail.
  PurplePresence *useless_fucking_shit = purple_presence_new_for_buddy(pb);
  if (!useless_fucking_shit) return ""; // =O! No useless fucking shit?! This is a logical impossibility for libpurple! BAIL!
  PurpleStatus *ps = purple_presence_get_active_status(useless_fucking_shit);
  if (!ps) return ""; // OH, SNAP! There was literally nothing useful in our pile of useless_fucking_shit!
  return purple_status_get_text_or_message_or_whatever_the_purple_developers_think_it_should_be_called(ps);
}

string get_my_name(PurpleAccount* account) {
  return purple_account_get_alias(account);
}
string get_my_email(PurpleAccount* account) {
  return purple_account_get_username(account);
}
string get_my_status(PurpleAccount* account) {
  return purple_status_get_text_or_message_or_whatever_the_purple_developers_think_it_should_be_called(purple_account_get_active_status(account));
}
