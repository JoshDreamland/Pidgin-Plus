/**
 * @file   codes.cc
 * @brief  Source filling in special unicode strings.
 * 
 * @section License
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
 *
**/

#include "codes.h"

const char* pplus_unicode_doublestrike    = "í´“í¸½Ô¹â„‚í´“í»½Ô¼í´“í½½Ô¾â„í´”í°½Õí´”í²½Õƒí´”í´¢Ä•í´”í¶¢Ä™â„šâ„í´”íº½Õ‹í´”í¼½Õí´”í¾½Õí´•í°¢Ä¤í´•í²½Õ“í´•í´½Õ•í´•í¶½Õ—í´•í¸½Õ™í´•íº½Õ›í´•í¼½Õí´•í¾½ÕŸí´–í°½Õ¡í´–í²½Õ£í´–í´½Õ¥í´–í¶½Õ§í´–í¸½Õ©í´–íº½Õ«";
const char* pplus_unicode_fraktur         = "í´–í¼½Õ­í´–í¾½Õ¯í´—í°½Õ±í´—í²½Õ³í´—í´½Õµí´—í¶½Õ·í´—í¸½Õ¹í´—íº½Õ»í´—í¼½Õ½í´—í¾½Õ¿í´˜í°½Öí´˜í²½Öƒí´˜í´½Ö…í´˜í¶½Ö‡í´˜í¸½Ö‰í´˜íº½Ö‹í´˜í¼½Öí´˜í¾½Öí´™í°½Ö‘í´™í²½Ö“í´™í´½Ö•í´™í¶½Ö—í´™í¸½Ö™í´™íº½Ö›í´™í¼½Öí´™í¾½ÖŸ";
const char* pplus_unicode_sansserif       = "í´ší°½Ö¡í´ší²½Ö£í´ší´½Ö¥í´ší¶½Ö§í´ší¸½Ö©í´šíº½Ö«í´ší¼½Ö­í´ší¾½Ö¯í´›í°½Ö±í´›í²½Ö³í´›í´½Öµí´›í¶½Ö·í´›í¸½Ö¹í´›íº½Ö»í´›í¼½Ö½í´›í¾½Ö¿í´œí°½×í´œí²½×ƒí´œí´½×…í´œí¶½×‡í´œí¸½×‰í´œíº½×‹í´œí¼½×í´œí¾½×í´í°½×‘í´í²½×“";
const char* pplus_unicode_sansbold        = "í´í´½×•í´í¶½×—í´í¸½×™í´íº½×›í´í¼½×í´í¾½×Ÿí´ží°½×¡í´ží²½×£í´ží´½×¥í´ží¶½×§í´ží¸½×©í´žíº½×«í´ží¼½×­í´ží¾½×¯í´Ÿí°½×±í´Ÿí²½×³í´Ÿí´½×µí´Ÿí¶½×·í´Ÿí¸½×¹í´Ÿíº½×»í´Ÿí¼½×½í´Ÿí¾½×¿í´ í°½Øí´ í²½Øƒí´ í´½Ø…í´ í¶½Ø‡";
const char* pplus_unicode_sansitalic      = "í´ í¸½Ø‰í´ íº½Ø‹í´ í¼½Øí´ í¾½Øí´¡í°½Ø‘í´¡í²½Ø“í´¡í´½Ø•í´¡í¶½Ø—í´¡í¸½Ø™í´¡íº½Ø›í´¡í¼½Øí´¡í¾½ØŸí´¢í°½Ø¡í´¢í²½Ø£í´¢í´½Ø¥í´¢í¶½Ø§í´¢í¸½Ø©í´¢íº½Ø«í´¢í¼½Ø­í´¢í¾½Ø¯í´£í°½Ø±í´£í²½Ø³í´£í´½Øµí´£í¶½Ø·í´£í¸½Ø¹í´£íº½Ø»";
const char* pplus_unicode_sansbolditalic  = "í´£í¼½Ø½í´£í¾½Ø¿í´¤í°½Ùí´¤í²½Ùƒí´¤í´½Ù…í´¤í¶½Ù‡í´¤í¸½Ù‰í´¤íº½Ù‹í´¤í¼½Ùí´¤í¾½Ùí´¥í°½Ù‘í´¥í²½Ù“í´¥í´½Ù•í´¥í¶½Ù—í´¥í¸½Ù™í´¥íº½Ù›í´¥í¼½Ùí´¥í¾½ÙŸí´¦í°½Ù¡í´¦í²½Ù£í´¦í´½Ù¥í´¦í¶½Ù§í´¦í¸½Ù©í´¦íº½Ù«í´¦í¼½Ù­í´¦í¾½Ù¯";
const char* pplus_unicode_monospace       = "í´§í°½Ù±í´§í²½Ù³í´§í´½Ùµí´§í¶½Ù·í´§í¸½Ù¹í´§íº½Ù»í´§í¼½Ù½í´§í¾½Ù¿í´¨í°½Úí´¨í²½Úƒí´¨í´½Ú…í´¨í¶½Ú‡í´¨í¸½Ú‰í´¨íº½Ú‹í´¨í¼½Úí´¨í¾½Úí´©í°½Ú‘í´©í²½Ú“í´©í´½Ú•í´©í¶½Ú—í´©í¸½Ú™í´©íº½Ú›í´©í¼½Úí´©í¾½ÚŸí´ªí°½Ú¡í´ªí²½Ú£";
const char* pplus_unicode_script          = "í´í°½Ó‘í´í²½Ó“í´í´½Ó•í´í¶½Ó—í´í¸½Ó™í´íº½Ó›í´í¼½Óí´í¾½ÓŸí´Ží°½Ó¡í´Ží²½Ó£í´Ží´½Ó¥í´Ží¶½Ó§í´Ží¸½Ó©í´Žíº½Ó«í´Ží¼½Ó­í´Ží¾½Ó¯í´í°½Ó±í´í²½Ó³í´í´½Óµí´í¶½Ó·í´í¸½Ó¹í´íº½Ó»í´í¼½Ó½í´í¾½Ó¿í´í°½Ôí´í²½Ôƒ";
const char* pplus_unicode_serifbold       = "í´€í°½Ðí´€í²½Ðƒí´€í´½Ð…í´€í¶½Ð‡í´€í¸½Ð‰í´€íº½Ð‹í´€í¼½Ðí´€í¾½Ðí´í°½Ð‘í´í²½Ð“í´í´½Ð•í´í¶½Ð—í´í¸½Ð™í´íº½Ð›í´í¼½Ðí´í¾½ÐŸí´‚í°½Ð¡í´‚í²½Ð£í´‚í´½Ð¥í´‚í¶½Ð§í´‚í¸½Ð©í´‚íº½Ð«í´‚í¼½Ð­í´‚í¾½Ð¯í´ƒí°½Ð±í´ƒí²½Ð³";
const char* pplus_unicode_serifitalic     = "í´ƒí´½Ðµí´ƒí¶½Ð·í´ƒí¸½Ð¹í´ƒíº½Ð»í´ƒí¼½Ð½í´ƒí¾½Ð¿í´„í°½Ñí´„í²½Ñƒí´„í´½Ñ…í´„í¶½Ñ‡í´„í¸½Ñ‰í´„íº½Ñ‹í´„í¼½Ñí´„í¾½Ñí´…í°½Ñ‘í´…í²½Ñ“í´…í´¢ÄŽí´…í¶½Ñ—í´…í¸½Ñ™í´…íº½Ñ›í´…í¼½Ñí´…í¾½ÑŸí´†í°½Ñ¡í´†í²½Ñ£í´†í´½Ñ¥í´†í¶½Ñ§";
const char* pplus_unicode_serifbolditalic = "í´†í¸½Ñ©í´†íº½Ñ«í´†í¼½Ñ­í´†í¾½Ñ¯í´‡í°½Ñ±í´‡í²½Ñ³í´‡í´½Ñµí´‡í¶½Ñ·í´‡í¸½Ñ¹í´‡íº½Ñ»í´‡í¼½Ñ½í´‡í¾½Ñ¿í´ˆí°½Òí´ˆí²½Òƒí´ˆí´½Ò…í´ˆí¶½Ò‡í´ˆí¸½Ò‰í´ˆíº½Ò‹í´ˆí¼½Òí´ˆí¾½Òí´‰í°½Ò‘í´‰í²½Ò“í´‰í´½Ò•í´‰í¶½Ò—í´‰í¸½Ò™í´‰íº½Ò›";
const char* pplus_unicode_braille         = "â â ƒâ ‰â ™â ‘â ‹â ›â “â Šâ šâ …â ‡â â â •â â Ÿâ —â Žâ žâ ¥â §â ºâ ­â ½â µâ â ƒâ ‰â ™â ‘â ‹â ›â “â Šâ šâ …â ‡â â â •â â Ÿâ —â Žâ žâ ¥â §â ºâ ­â ½â µ";
const char* pplus_unicode_circled         = "â’¶â’·â’¸â’¹â’ºâ’»â’¼â’½â’¾â’¿â“€â“â“‚â“ƒâ“„â“…â“†â“‡â“ˆâ“‰â“Šâ“‹â“Œâ“â“Žâ“â“â“‘â“’â““â“”â“•â“–â“—â“˜â“™â“šâ“›â“œâ“â“žâ“Ÿâ“ â“¡â“¢â“£â“¤â“¥â“¦â“§â“¨â“©";
const char* pplus_smilies = "í¼ í±¿Ø‚í¼ í³¿Ø„í¼ íµ¿Ø†í¼ í·¿Øˆí¼ í¹¿ØŠí¼ í»¿ØŒí¼ í½¿ØŽí¼ í¿¿Øí¼¡í²¿Ø“í¼¡í´¿Ø–í¼¡í¸¿Øší¼¡í¼¿Øí¼¡í¾¿Ø í¼¢í±¿Ø¢í¼¢í³¿Ø¤í¼¢íµ¿Ø¨í¼¢í¹¿Øªí¼¢í»¿Ø­í¼£í°¿Ø±í¼£í²¿Ø³í¼£íµ¿Ø¶í¼£í·¿Ø¸í¼£í¹¿Øºí¼£í»¿Ø¼í¼£í½¿Ø¾í¼£í¿¿Ù€";

const string msgplus_bold = "\x02";
const string msgplus_italic = "\x05";
const string msgplus_underline = "\x1f";

#define MISSING 0
int msgplus_colors[128] = {
  /*00: */ 0xffffff, /*01: */ 0x000000, /*02: */ 0x00007f, /*03: */ 0x009300, /*04: */ 0xff0000, 
  /*05: */ 0x7f0000, /*06: */ 0x9c009c, /*07: */ 0xfc7f00, /*08: */ 0xffff00, /*09: */ 0x00fc00,
  /*10: */ 0x009393, /*11: */ 0x00ffff, /*12: */ 0x2020fc, /*13: */ 0xff00ff, /*14: */ 0x7f7f7f, 
  /*15: */ 0xd2d2d2, /*16: */ 0xe7e6e4, /*17: */ MISSING,  /*18: */ 0xffdea4, /*19: */ 0xffaeb9,
  /*20: */ 0xffa8ff, /*21: */ 0xb4b4fc, /*22: */ 0xbafbe5, /*23: */ 0xc1ffa3, /*24: */ 0xfafda2,
  /*26: */ MISSING,  /*27: */ MISSING,  /*27: */ 0xf9c152, /*28: */ 0xff6d66, /*29: */ 0xff62ff,
  /*30: */ 0x6c6cff, /*31: */ 0x68ffc3, /*32: */ 0x8eff67, /*33: */ 0xf9ff57, /*34: */ MISSING,
  /*35: */ MISSING,  /*36: */ 0xffa022, /*37: */ 0xf92611, /*38: */ 0xff20ff, /*39: */ 0x202bff,
  /*40: */ 0x1effa5, /*41: */ 0x60f913, /*42: */ MISSING,  /*43: */ MISSING,  /*44: */ MISSING,
  /*45: */ 0xd98812, /*46: */ 0xeb0505, /*47: */ 0xde00de, /*48: */ 0x0000d3, /*49: */ 0x03cc88, 
  /*50: */ 0x59d80d, /*51: */ 0xd4c804, /*52: */ MISSING,  /*49: */ MISSING,  /*54: */ MISSING,
  /*55: */ 0x9b0008, /*56: */ MISSING,  /*57: */ 0x01038c, /*58: */ 0x01885f, /*59: */ MISSING,
  /*60: */ MISSING,  /*61: */ 0x473400, /*62: */ 0x4d0000, /*63: */ 0x5f0162, /*64: */ 0x000047,
  /*65: */ 0x06502f, /*66: */ 0x1c5300, /*67: */ 0x544d05,
  
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
