#include "game_ui.h"
#include "game_process.h"

ui_menu_d MENU_DATA[MENU_DONE] = {
  [MENU_MAIN] = {MENU_MAIN, "TITLE_MENU_DOM", ELEMENT_NONE, false,
    {[MENU_ACTIVE] = MenuActivateChildren}
  },
  [MENU_HUD] = {MENU_HUD, "HUD_MENU_DOM", ELEMENT_NONE, false,
    {[MENU_LOAD] = MenuActivateChildren,
    }
  }
};

ui_element_d ELEM_DATA[ELE_COUNT] = {
  {"TITLE_MENU_DOM", VECTOR2_ZERO, FIXED_MENU_FULL, UI_CONTAINER,
    ELEMENT_NONE, LAYOUT_STACK, ALIGN_MID | ALIGN_CENTER,
    .cb = {
      [ELEMENT_IDLE] = ElementActivateChildren,
      [ELEMENT_SHOW] = ElementShowChildren,
    },
    {
      [UI_MARGIN_TOP] = 8, [UI_MARGIN_LEFT] = 6,
      [UI_PADDING_TOP] = 24, [UI_PADDING_LEFT] = 8 
    },
    1, {
    //  "TITLE_BACKGROUND"
      "TITLE_PANEL",
    },
    .text = "Signals Crossed"
  },
  {"TITLE_PANEL", VECTOR2_ZERO, VECTOR2_ZERO, UI_GROUP, ELEMENT_NONE,
    LAYOUT_VERTICAL, ALIGN_MID | ALIGN_CENTER,
    .cb = {
      [ELEMENT_IDLE] = ElementActivateChildren,
      [ELEMENT_SHOW] = ElementShowChildren,
    },
    {
      [UI_MARGIN_TOP] = -36, [UI_PADDING_BOT] = 36, [UI_PADDING_TOP] = -24
    },
    5,{
      "TITLE_HEADER",
      "TEXT_HEADER",
      "TEXT_HEADER",
      "TEXT_HEADER",
      "FLASHY_TEXT"
    },
    .text = "SIGNALS CROSSED, A puzzle and memory game made in 72 hours, Made by August Karbowski, Music by Dan Jarosz, Press Enter to Play!",
    .delimiter = ','
  },
  {"TITLE_HEADER", VECTOR2_ZERO, VECTOR2_ZERO, UI_TITLE, ELEMENT_NONE,
    LAYOUT_HORIZONTAL, ALIGN_MID | ALIGN_CENTER,
    GetOwnerTextAt, ElementPresetContext,
    .cb = {
      [ELEMENT_LOAD] = ElementSetContext,
    }
  },
  {"TEXT_HEADER", VECTOR2_ZERO, VECTOR2_ZERO, UI_TEXT, ELEMENT_NONE,
    LAYOUT_HORIZONTAL, ALIGN_MID | ALIGN_CENTER,
    GetOwnerTextAt, ElementPresetContext,
    .cb = {
      [ELEMENT_LOAD] = ElementSetContext,
    }
  },
  {"FLASHY_TEXT", VECTOR2_ZERO, VECTOR2_ZERO, UI_TEXT, ELEMENT_NONE,
    LAYOUT_HORIZONTAL, ALIGN_MID | ALIGN_CENTER,
    GetOwnerTextAt, ElementPresetContext,
    .cb = {
      [ELEMENT_LOAD] = ElementSetContext,
    },
    {
      [UI_MARGIN_BOT] = 16, [UI_MARGIN_TOP] = 32
    }
  },
  {"HUD_MENU_DOM", VECTOR2_ZERO, VECTOR2_ZERO, UI_CONTAINER, ELEMENT_NONE, 
    LAYOUT_FREE, ALIGN_LEFT,
    .spacing = {
      [UI_MARGIN_TOP] = 12, [UI_MARGIN_LEFT] = 6,
      [UI_PADDING_TOP] = 12, [UI_PADDING_LEFT] = 8
    },
    .cb = {
      [ELEMENT_IDLE] = ElementActivateChildren,
      [ELEMENT_SHOW] = ElementShowChildren,
    },
  },
  {"PLAYER_PANEL_HEADERS", VECTOR2_ZERO, VECTOR2_ZERO, UI_TAB_PANEL,
    ELEMENT_NONE, LAYOUT_HORIZONTAL, ALIGN_LEFT, NULL, ElementNiblings,
    {
      [ELEMENT_LOAD] = ElementSetContext,
      [ELEMENT_IDLE] = ElementLoadChildren,
      [ELEMENT_SHOW] = ElementActivateChildren,

    },
    .kids = {
      "TAB_BUTTON"
    },
    .spacing = {[UI_MARGIN_LEFT] = 36, [UI_PADDING_RIGHT] = 4}
  },
  {"TAB_BUTTON", VECTOR2_ZERO, FIXED_BUTTON_SIZE, UI_BUTTON,
    ELEMENT_NONE, LAYOUT_HORIZONTAL, ALIGN_CENTER | ALIGN_MID, GetElementName, ElementPresetContext,
    {
      //[ELEMENT_LOAD] = ElementSetContext,
      [ELEMENT_IDLE] = ElementSetContext,
      [ELEMENT_ACTIVATE] = ElementTabToggle,
    },
    .delimiter = ' '
  },
  {"HEADER_BAR", VECTOR2_ZERO, FIXED_HEADER_SIZE, UI_HEADER,
    ELEMENT_NONE, LAYOUT_HORIZONTAL, ALIGN_LEFT | ALIGN_TOP,
    GetOwnerText, ElementGetOwnerContext, 
    {
      [ELEMENT_IDLE] = ElementSetContext,
    },
  },
  {"HEADER_BAR_VAL", VECTOR2_ZERO, FIXED_HEADER_SIZE, UI_HEADER,
    ELEMENT_NONE, LAYOUT_HORIZONTAL, ALIGN_LEFT, GetOwnerValue, ElementGetOwnerContext,
    {
      [ELEMENT_IDLE] = ElementSetContext,
    },
  }, 
};

