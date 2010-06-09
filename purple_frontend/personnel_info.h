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
 
//These commands fetch info for a buddy by "email," where "email" may actually be any of the things you are requesting.
string buddy_get_uname(PurpleAccount* account,string email);
string buddy_get_alias(PurpleAccount* account,string email);
string buddy_get_status(PurpleAccount* account,string email);

//None of these work. I think get_my_email returns one of its own parameters. Somehow.
string get_my_name(PurpleAccount* account);
string get_my_email(PurpleAccount* account);
string get_my_status(PurpleAccount* account);
string get_my_ip();
