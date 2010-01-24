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
 
 
inline bool is_letter(char a) {
  return (a >= 'A' and a <= 'Z') or (a >= 'a' and a <= 'z');
}
inline bool is_letterd(char c) { 
  return (c >= 'A' and c <= 'Z') or (c >= 'a' and c <= 'z') or (c >= '0' and c <= '9') or c == '_';
}
inline bool is_white(char c) {
  return c == ' ' or c == '\t' or c == '\r' or c == '\n';
}
inline string lpos(string s,unsigned int p)
{
  if (s[p] >= 'A' and s[p] <= 'Z') 
    s[p] -= 'A'-'a'; 
  return s;
}

string tostring(int x);
string tostring(long double x);
string tostring_time(long double x);
