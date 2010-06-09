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

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>

#include <string>
using namespace std;


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
