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
  @file  basics.cc
  @brief Source implementing basic utility functions.
  
  This is a general-purpose file to be used throughout multiple headers.
  These are, again, a method of simplifying code. The functions are, for
  the most part, self-explanatory, with the exception of tostring_time(),
  which was designed specifically for the ping command.
*/

#include <string>
#include <stdio.h>
using namespace std;

#include "basics.h"

/// Return a string representation of a floating point number.
string tostring(long double x)
{
  static char retc[24];
  sprintf(retc,"%Lf",x);
  return retc;
}
/// Return a string representation of an integer.
string tostring(int x)
{
  static char retc[24];
  sprintf(retc,"%d",x);
  return retc;
}
/// Return a string representation of a time, in seconds.
/// @param x  The time to represent.
/// @return A string representation of \p x, or five question marks ("?????") if x is less than zero.
string tostring_time(long double x)
{
  if (x < 0) return "?????";
  static char retc[24];
  sprintf(retc,"%Lf sec",x);
  return retc;
}
