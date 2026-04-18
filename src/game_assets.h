#ifndef __GAME_ASSETS__
#define __GAME_ASSETS__

//#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "game_common.h"

#define MAX_SONGS 4
#define MAX_SPRITES 64

struct ent_s;
typedef struct sub_texture_s {
    int tag;
    int originX, originY;
    int positionX, positionY;
    int sourceWidth, sourceHeight;
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;
    int colliderType;
    int colliderPosX, colliderPosY, colliderSizeX, colliderSizeY;
} sub_texture_t;

typedef struct{
  const char* text;
  Vector2     pos;
  int         size;
  Color       color;
  int         duration;
}render_text_t;

void AddFloatingText(render_text_t *rt);

void InitResources();

typedef enum{
  ANIM_NONE,
  ANIM_IDLE,
  ANIM_BOUNCE,
  ANIM_RETURN,
  ANIM_DONE,
  ANIM_COUNT
}AnimState;

typedef struct{
  int duration;
  int elapsed;

}anim_t;

typedef struct{
  int     duration;
  int     elapsed;
  float   vol;
  bool    increase;
}audio_fade_t;

typedef struct{
  Music        music;
  events_t     *events;
  audio_fade_t *fade;
}music_track_t;

typedef struct{
  unsigned int  index;
  int           num_songs;
  unsigned int  current_index;
  float         volume;
  music_track_t track[MAX_SONGS];
}music_group_t;

typedef enum{
  SFX_ALL,
  SFX_UI,
  SFX_ACTION,
  SFX_IMPORTANT,
  SFX_DONE
}SfxGroup;

typedef enum {
  ACTION_HOVER,
  ACTION_PLACE,
  END_SFX
}SfxType;

typedef struct{
  SfxType     type;
  SfxGroup    group;
  int         num_sounds;
  const char* paths[5];
}sfx_info_t;

typedef struct{
  SfxType   type;
  int       num_sounds;
  Sound     *sounds;
}sfx_sound_t;

static sfx_info_t sfx_catalog[]={
  {ACTION_PLACE,SFX_ACTION,4,{"switch_001.ogg","switch_002.ogg","switch_003.ogg","switch_004.ogg"}},
  {ACTION_HOVER,SFX_ACTION,5,{"click_001.ogg","click_002.ogg","click_003.ogg","click_004.ogg","click_005.ogg"}},
};

typedef struct{
  int         num_types;
  float       volume;
  sfx_sound_t items[END_SFX];
}sfx_group_t;

typedef struct{
  music_group_t   tracks[6];
  float           volume;
  int             current_track;
  int             concurrent_track;
  int             num_tracks;
  sfx_group_t     sfx[SFX_DONE];
  events_t        *timers[SFX_DONE];
}audio_manager_t;

void InitAudio();
int AudioBuildMusicTracks(const char* subdir);
void AudioStep();
void AudioPlaySfx(SfxGroup group, SfxType type, int index);
void AudioPlayRandomSfx(SfxGroup group, SfxType type);
void AudioPlayMusic(int index);
bool AudioPlayNext(music_group_t* t);
void AudioMusicFade(music_track_t* t);

typedef enum{
  LAYER_ROOT = -1,
  LAYER_BG,
  LAYER_MAIN,
  LAYER_TOP,
  LAYER_DONE
}RenderLayer;

typedef struct {
  int       id;
  int       sequence_index;
  Vector2   center;
  Rectangle bounds;
  Vector2   offset;
  float     scale;
  Color     color;
} sprite_slice_t;

typedef struct{
  int             num_sprites;
  sprite_slice_t  *sprites[128];
  Texture2D       *sprite_sheet;
}sprite_sheet_data_t;

typedef enum{
  SHEET_UI,
  SHEET_ICON,
  SHEET_TILE,
  SHEET_CHAR,
  SHEET_ALL
}SheetID;

static sprite_sheet_data_t SHEETS[SHEET_ALL];
void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap);

void SpriteLoadSlicedTextures();
//SPRITE_T===>
typedef struct {
  int             suid;
  AnimState       state;
  anim_t          *anim;
  Texture2D       *sheet;
  sprite_slice_t* slice;
  bool            is_visible;
  float           rot;
  Vector2         offset;
  Vector2         pos;
  RenderLayer     layer;
  struct ent_s    *owner;
} sprite_t;

void DrawSlice(sprite_t *spr, Vector2 position,float rot);
sprite_t* InitSpriteByID(int id, SheetID);
sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* spritesheet);
sprite_t* InitAnimationByID(CharacterSprite[12], int, SheetID);
bool FreeSprite(sprite_t* s);
void DrawSprite(sprite_t* s);
void DrawSpriteAtPos(sprite_t*s , Vector2 pos);
void SpriteSync(struct ent_s* e, sprite_t *spr);
bool SpriteSetAnimState(sprite_t* spr, AnimState s);
bool SpriteCanChangeState(AnimState old, AnimState s);
void SpriteAnimate(sprite_t *spr);
void SpritePreprocessImg(Image *img, Texture2D* out);


//====SPRITE_T>>
#endif
