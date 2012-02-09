/**
 * @file plugin_template.cc
 * 
 * This file is the catch-it-allfor BBCode tag initializers. Its sole
 * purpose is to make up for C++'s lack of static code.
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
*/

extern void load_bb_formatting();
void load_bb_unicode();
static void load_bbcode() {
  load_bb_formatting();
  load_bb_unicode();
}
