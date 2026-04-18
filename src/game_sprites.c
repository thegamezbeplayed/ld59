#include "game_assets.h"
#include "game_utils.h"
#include "game_math.h"
#include "game_tools.h"
#include "screens.h"
#include "character_asset.h"
#include "tile_assets.h"

void InitResources(){
  Image spritesImg = LoadImage(TextFormat("resources/%s",CHAR_IMAGE_PATH)); 
  SHEETS[SHEET_CHAR].sprite_sheet = GameMalloc("",sizeof(Texture2D));
  SpriteLoadSubTextures(CHAR_SPRITES,SHEET_CHAR,CHAR_DONE);
  *SHEETS[SHEET_CHAR].sprite_sheet = LoadTextureFromImage(spritesImg);


  Image tilesImg = LoadImage(TextFormat("resources/%s",TILE_IMAGE_PATH)); 
  SHEETS[SHEET_TILE].sprite_sheet = GameMalloc("",sizeof(Texture2D));
  SpriteLoadSubTextures(TILE_SPRITES,SHEET_TILE,TILE_DONE);
  *SHEETS[SHEET_TILE].sprite_sheet = LoadTextureFromImage(tilesImg);


}

sprite_t* InitSpriteByID(int id, SheetID s){
  sprite_t* spr = GameMalloc("InitSprite", sizeof(sprite_t));
  memset(spr,0,sizeof(sprite_t));
  spr->anim = GameMalloc("InitSprite", sizeof(anim_t));
  sprite_sheet_data_t* data = &SHEETS[s];

  spr->state = ANIM_IDLE;
  spr->anim->duration = 67;
  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->id != id)
      continue;

    spr->slice = data->sprites[i];
    spr->sheet = data->sprite_sheet;

    spr->offset = spr->slice->offset;
    //spr->slice->scale = SPRITE_SCALE;
  }

  return spr;

}

sprite_t* InitAnimationByID(CharacterSprite set[12], int base, SheetID s){
  sprite_t* spr = GameMalloc("InitAnim", sizeof(sprite_t));
  memset(spr,0,sizeof(sprite_t));
  spr->anim = GameMalloc("InitAnim", sizeof(anim_t));

  sprite_sheet_data_t* data = &SHEETS[s];
  spr->state = ANIM_IDLE;
  spr->anim->duration = 12;
  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->id != base)
      continue;

    spr->slice = data->sprites[i];
    spr->sheet = data->sprite_sheet;

    spr->offset = spr->slice->offset;
    //spr->slice->scale = SPRITE_SCALE;
  }

  return spr;

}

void SpriteSync(ent_t* e, sprite_t *spr){
  if(!spr)
    return;

  spr->pos = CellToVector2(e->pos, CELL_WIDTH);
  if(!spr->anim)
    return;

  switch(COMBO_KEY(spr->owner->state,spr->state)){
    case COMBO_KEY(STATE_IDLE,ANIM_IDLE):
      if(spr->owner->control->moves>0)
        SpriteSetAnimState(spr, ANIM_BOUNCE);
      else
        SpriteSetAnimState(spr, ANIM_IDLE);
      break;
    default:
      break;
  }

  SpriteAnimate(spr);
}

void SpriteOnAnimChange(sprite_t* spr, AnimState old, AnimState s){
  spr->anim->elapsed = 0;

  switch(COMBO_KEY(old,s)){
    case COMBO_KEY(ANIM_BOUNCE,ANIM_DONE):
      SpriteSetAnimState(spr, ANIM_RETURN);
      break;
    case COMBO_KEY(ANIM_RETURN,ANIM_DONE):
      SpriteSetAnimState(spr, ANIM_BOUNCE);
      break;
    default:
      break;
  }
}

bool SpriteCanChangeState(AnimState old, AnimState s){
  if(old == s)
    return false;

  switch(COMBO_KEY(old,s)){
    case COMBO_KEY(ANIM_DONE,ANIM_BOUNCE):
    case COMBO_KEY(ANIM_DONE,ANIM_RETURN):
    case COMBO_KEY(ANIM_IDLE,ANIM_BOUNCE):
    case COMBO_KEY(ANIM_BOUNCE,ANIM_DONE):
    case COMBO_KEY(ANIM_RETURN,ANIM_DONE):
      return true;
      break;
    default:
      return false;
      break;
  }
}

bool SpriteSetAnimState(sprite_t* spr, AnimState s){
  if(!SpriteCanChangeState(spr->state, s))
    return false;

  AnimState old = spr->state;
  spr->state = s;

  SpriteOnAnimChange(spr,old,s);
}

void SpriteAnimate(sprite_t *spr){
  if(spr->state <= ANIM_IDLE)
    return;

  spr->anim->elapsed++;
  float height = (spr->owner->control->moves) *6.9f;
  switch(spr->state){
    case ANIM_BOUNCE:
      spr->offset.y=EaseLinearOut(spr->anim->elapsed, 0.0f,-height,spr->anim->duration);
      break;
    case ANIM_RETURN:
      spr->offset.y=EaseLinearIn(spr->anim->elapsed,-height, height, spr->anim->duration);
      break;
  }

  if(spr->anim->elapsed >= spr->anim->duration)
    SpriteSetAnimState(spr, ANIM_DONE);

}

void DrawSlice(sprite_t *spr, Vector2 position,float rot){
  sprite_slice_t* slice = spr->slice; 
  Rectangle src = slice->bounds;

  position = Vector2Add(position,spr->offset);
  Rectangle dst = {
    position.x,
    position.y,
    slice->bounds.width * slice->scale,
    slice->bounds.height * slice->scale
  };

  Vector2 origin = (Vector2){
    slice->center.x * slice->scale,//offset.x,
      slice->center.y * slice->scale//offset.y
  };

  DrawTexturePro(*spr->sheet,src,dst, origin, rot, WHITE);

  return;
}

bool FreeSprite(sprite_t* s){
  if(!s) return false;

  if(s->anim)
    free(s->anim);

  //for(int i = 0; i<SHADER_DONE;i++)
  //free(s->gls[i]);

  free(s);
  return true;
}
void DrawSpriteAtPos(sprite_t*s , Vector2 pos){
  if(s->is_visible)
    DrawSlice(s, pos, s->rot);

}
void DrawSprite(sprite_t* s){
  if(s->is_visible)
    DrawSlice(s, s->pos,s->rot);
}

void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap){
  for(int i = 0; i < sheet_cap;i++){
    sub_texture_t sprData = data[i];

    Vector2 center = Vector2FromXY(sprData.originX,sprData.originY);
    Rectangle bounds = RecFromCoords(sprData.positionX,sprData.positionY,sprData.sourceWidth,sprData.sourceHeight);

    sprite_slice_t *spr = GameMalloc("SpriteLoadSubTextures",sizeof(sprite_slice_t));
    memset(spr,0,sizeof(sprite_slice_t));

    spr->scale = 1;//ScreenSized(SIZE_SCALE);
    spr->id = sprData.tag;
    spr->center = center;// Vector2Scale(offset,spr->scale);
    spr->offset = VECTOR2_ZERO;//offset;//center;//Vector2Scale(center,spr->scale);
    spr->bounds = bounds;
    SHEETS[id].sprites[SHEETS[id].num_sprites++] = spr;
  }
}

void SpritePreprocessImg(Image *img, Texture2D *out){
  ImageFormat(img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  // Access pixels
  Color *pixels = LoadImageColors(*img);

  int pixelCount = img->width * img->height;
  for (int i = 0; i < pixelCount; i++)
  {
    // If pixel is white (tolerate near-white)
    if (pixels[i].r > 240 && pixels[i].g > 240 && pixels[i].b > 240)
    {
      pixels[i].a = 0; // Make transparent
    }
  }

  // Apply modified pixels back to image
  Image newImg = {
    .data = pixels,
    .width = img->width,
    .height = img->height,
    .format = img->format,
    .mipmaps = 1
  };

  // Create a texture from modified image
  *out = LoadTextureFromImage(newImg);
  UnloadImageColors(pixels);
}
