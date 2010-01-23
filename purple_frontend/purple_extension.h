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

void all_accounts_set_status(PurpleStatusPrimitive pstat,const string& msg);

string buddy_get_uname(PurpleAccount* account,string email);
string buddy_get_alias(PurpleAccount* account,string email);
string buddy_get_status(PurpleAccount* account,string email);

string get_my_name(PurpleAccount* account);
string get_my_email(PurpleAccount* account);
string get_my_status(PurpleAccount* account);
string get_my_ip();

void set_receiving_window(PurpleConversation* conv,int window_type); //Set the window that will receive script output
void pidgin_printf(const char* message); //Print to the window in question.
enum { pct_im = 0, pct_chat = 1 }; //Type of the window in question, or any window in question.
