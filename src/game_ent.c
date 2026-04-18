#include "game_types.h"
#include "game_tools.h"
#include "game_math.h"
#include "game_process.h"
#include "game_helpers.h"

MAKE_ADAPTER(StepState, ent_t*);

ent_t* InitEnt(EntityType t){
  ent_t* e = malloc(sizeof(ent_t));
  *e = (ent_t){0};  // zero initialize if needed

  ent_d def = DEF_ENT[t];
  if(def.id != t)
    return NULL;

  e->sprite = InitAnimationByID(def.anims, def.base, SHEET_CHAR);
  e->control = InitController();
  SetState(e,STATE_SPAWN,NULL);
  return e;
}

void EntDestroy(ent_t* e){
  if(!e || !SetState(e, STATE_END,NULL))
    return;

  if(e->sprite!=NULL){
    e->sprite->owner = NULL;
    e->sprite->is_visible = false;
  }

  e->control = NULL;
}

bool FreeEnt(ent_t* e){
  if(!e)
    return false;

  free(e);
  return true;
}

controller_t* InitController(){
  controller_t* ctrl = malloc(sizeof(controller_t));
  *ctrl = (controller_t){0};

  return ctrl;
}

void EntSync(ent_t* e){
  if(e->control)  
    EntControlStep(e);

  SpriteSync(e, e->sprite);
}

void EntControlStep(ent_t *e){
  if(!e->control || !e->control->bt || !e->control->bt[e->state])
    return;

  behavior_tree_node_t* current = e->control->bt[e->state];

  current->tick(current, e);
}

bool SetState(ent_t *e, EntityState s,StateChangeCallback callback){
  if(CanChangeState(e->state,s)){
    EntityState old = e->state;
    e->state = s;

    if(callback!=NULL)
      callback(e,old,s);

    OnStateChange(e,old,s);
    return true;
  }

  return false;
}

void StepState(ent_t *e){
  SetState(e, e->state+1,NULL);
}

bool CanChangeState(EntityState old, EntityState s){
  return true;
} 

void OnStateChange(ent_t *e, EntityState old, EntityState s){
}

bool CheckEntPosition(ent_t* e, Vector2 pos){
}

bool CheckEntAvailable(ent_t* e){
  if(!e)
    return false;

  return (e->state < STATE_DIE);
}


TileStatus EntGridStep(ent_t *e, Cell step){
  Cell newPos = CellInc(e->pos,step);

  e->pos = newPos;
  /*
  TileStatus status = MapSetOccupant(e->map,e,newPos);

  if(status < TILE_ISSUES){
    //map_cell_t* mc = &e->map->tiles[newPos.x][newPos.y];
    Cell oldPos = e->pos;
    //WorldDebugCell(e->pos, YELLOW);
    e->pos = newPos;
    e->old_pos = oldPos;
    e->facing = CellInc(e->pos,step);
  }
  else
    e->facing = newPos;
*/
  return TILE_SUCCESS;
}

