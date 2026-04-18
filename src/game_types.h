#ifndef __GAME_TYPES__
#define __GAME_TYPES__

#include "game_common.h"
#include "game_assets.h"
#include "game_utils.h"
#include "raylib.h"
#include "game_math.h"
#include "game_tools.h"

#define GRID_SIZE 128
#define GRID_STEP 32

#define MAX_ENTS 128  

typedef struct ent_s ent_t;
typedef struct bounds_s {
  Vector2     pos,offset;
  float       radius;
  float       width,height;
  //BoundsCheckCall col_check;
} bounds_t;

typedef struct{
  struct ent_s*           target;
  int                     moves;
  bool                    moved;
  behavior_tree_node_t*   bt[STATE_END];
}controller_t;
controller_t* InitController();

//===ENT_T===>
struct ent_s{
  int                   uid;
  game_object_uid_i     gouid;
  EntityType            type;
  Cell                  pos;
  EntityState           state;
  controller_t          *control;
  sprite_t              *sprite;
};

ent_t* InitEnt(EntityType id);

void EntToggleTooltip(ent_t* e);

void EntSync(ent_t* e);
bool EntKill(ent_t* e);
void EntDestroy(ent_t* e);
bool FreeEnt(ent_t* e);
void EntAddPoints(ent_t* e,EntityState old, EntityState s);
void EntPrepStep(ent_t *e);
void EntSetPos(ent_t *e, Vector2 pos);
void EntControlStep(ent_t *e);
typedef void (*StateChangeCallback)(ent_t *e, EntityState old, EntityState s);
bool CheckEntAvailable(ent_t* e);
bool CheckEntPosition(ent_t* e, Vector2 pos);
bool SetState(ent_t *e, EntityState s,StateChangeCallback callback);
void StepState(ent_t *e);
void OnStateChange(ent_t *e, EntityState old, EntityState s);
bool CanChangeState(EntityState old, EntityState s);

TileStatus EntGridStep(ent_t *e, Cell step);
#endif
