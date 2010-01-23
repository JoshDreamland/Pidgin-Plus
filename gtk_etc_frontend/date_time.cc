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

#include <string>
using namespace std;


string get_weekday()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[24];
  strftime (wd,24,"%A",lt);
  return wd;
}
string get_date_numeric()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[12];
  strftime (wd,12,"%m/%d/%Y",lt);
  string wds = wd;
  
  int i = 0;
  if (wds[i] == '0') wds.erase(i,1); else i++;
  i += 2;
  if (wds[i] == '0') wds.erase(i,1); else i++;
  
  return wds;
}
string get_month()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[12];
  strftime (wd,12,"%B",lt);
  return wd;
}
string get_year()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[8];
  strftime (wd,8,"%Y",lt);
  return wd;
}
string get_time()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[12];
  strftime (wd,12,"%I:%M:%S %p",lt);
  string wds = wd;
  
  if (wds[0] == '0') wds.erase(0,1);
  return wds;
}
string get_hour()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[3];
  strftime (wd,3,"%H",lt);
  string wds = wd;
  
  //if (wds[0] == '0') wds.erase(0,1);
  return wds;
}
string get_minute()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[3];
  strftime (wd,3,"%M",lt);
  string wds = wd;
  return wds;
}
string get_second()
{
  time_t tt; time(&tt);
  tm *lt = localtime(&tt);
  
  char wd[3];
  strftime (wd,3,"%S",lt);
  string wds = wd;
  return wds;
}
