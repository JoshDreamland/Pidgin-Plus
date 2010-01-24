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
   @file basics.cc
   @summary This is a general-purpose file to be used
     throughout multiple headers. These are, again, a
     method of simplifying code. Again, most functions
     are self-explanatory. The exception is tostring_
     time(), which is specifically designed for ping,
     returning ????? when the input is negative.
*/

#include <string>
using namespace std;

string tostring(long double x)
{
  static char retc[24];
  sprintf(retc,"%Lf",x);
  return retc;
}
string tostring(int x)
{
  static char retc[24];
  sprintf(retc,"%d",x);
  return retc;
}
string tostring_time(long double x)
{
  if (x < 0) return "?????";
  static char retc[24];
  sprintf(retc,"%Lf sec",x);
  return retc;
}
