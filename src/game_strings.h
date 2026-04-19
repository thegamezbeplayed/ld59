#ifndef __GAME_STR__
#define __GAME_STR__
#include <stdio.h>
#include "game_ui.h"

void StringPrependPadding(char* s, size_t padding);
void StringAppendPadding(char* s, size_t padding);
void RepeatChar(char* out, size_t cap, char c, int times);

static void replace_char(char* str, char find, char replace) {
  if (!str) return;
  for (int i = 0; str[i]; i++) {
    if (str[i] == find)
      str[i] = replace;
  }
}

static char* StringSplit(char* str, char delimiter){
  char* token = strtok(str, &delimiter);

  return token;
}
void StringBounds(Rectangle *b, char* buff);

static const char* SIG_STR[TILE_DONE] = {
  "ALGIZ",
  "ANSUZ",
  "BERKANA",
  "DAGAZ",
  "EHWAZ",
  "FEHU",
  "GEBO",
  "HAGALAZ",
  "INGUZ",
  "INGWAZ",
  "ISA",
  "JERA",
  "KUANAZ",
  "LAGUZ",
  "MANNAZ",
  "NAUTHIZ",
  "OTHILA",
  "PEORTH",
  "RAIDO",
  "SOL",
  "TEIWAZ",
  "THURISAZ",
  "URUZ",
  "WUNJO",
  "YR",

};
#endif
