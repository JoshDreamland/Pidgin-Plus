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
   @file js_functions.cc
   @summary This file defines built-in functions that are to be
     accessable through JS. It is also responsible for setting
     allotted resources for each script, such as the number of
     messages that can be sent in one event.
*/

#include "v8_shared.h"

int prints_remaining = 25;

void jso_init_msgplus();
void jso_free_msgplus();
void jso_init_chatwnd();
void jso_free_chatwnd();

void js_functions_initialize()
{
  jso_init_msgplus();
  jso_init_chatwnd();
}

void js_resources_renew()
{
  prints_remaining = 25;
}
