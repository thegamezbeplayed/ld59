#include "game_types.h"
#include "game_tools.h"
#include "game_math.h"
#include "game_process.h"
#include "game_helpers.h"
#include "game_control.h"

MAKE_ADAPTER(StepState, ent_t*);

ent_t* InitEnt(EntityType t){
  ent_t* e = GameCalloc("InitEnt", 1, sizeof(ent_t));

  ent_d def = DEF_ENT[t];
  if(def.id != t)
    return NULL;

  e->type = t;
  e->sprite = InitAnimationByID(def.anims, def.base, SHEET_CHAR);
  e->sprite->owner = e;
  e->control = InitController();
  SetState(e,STATE_SPAWN,NULL);
  return e;
}

ent_t* InitEntStatic(EntityType t, Tiles r){
  ent_t* e = GameCalloc("InitEnt", 1, sizeof(ent_t));

  e->type = t;
  e->sprite = InitSpriteByID(r, SHEET_TILE);
  e->sprite->owner = e;
  e->control = InitController();
  e->signals = TILE_SIGNALS[r];
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

  GameFree("FreeEnt",e);
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
/*
void EntRender(ent_t* e){
  if(!e->sprite)
    DrawSprite(e->base);
  else
    DrawSprite(e->sprite);
}
*/
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
  state_change_requirement_t req = CAN_CHANGE[s];
  if(req.can)
  return req.can(old, req.required);

  TraceLog(LOG_WARNING, "IMPOSSIBLE STATE %i from %i", s, old);
  return false;
} 

void OnStateChange(ent_t *e, EntityState old, EntityState s){
  switch(s){
    case STATE_DIE:
      EntDestroy(e);
      LevelEvent(EVENT_ENT_DIE, e, e->gouid);
      break;
    default:
      break;
  }
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

  e->facing = step;
  TileStatus status = MapSetOccupant(WorldGetMap(),e,newPos);

  if(status < TILE_ISSUES){
    Cell oldPos = e->pos;


    //SpriteAnimateTo(e->base, oldPos, newPos);
    //WorldDebugCell(e->pos, YELLOW);
    e->pos = newPos;
    e->old_pos = oldPos;
    LevelEvent(EVENT_ENTER_CELL, e, e->gouid);
  }

  return status;
}


void OnShiftEvent(event_t* e, void* user){
  ent_t* p = user;
  ent_t* slab = e->data;

  if(p->state != STATE_PUSHING || slab->state != STATE_SHIFTING)
    return;

  SetState(slab, STATE_IDLE, NULL);
  SetState(p, STATE_IDLE, NULL);
  if(EntGridStep(slab, p->facing) >= TILE_ISSUES)
    return;

  if(EntGridStep( p, p->facing) < TILE_ISSUES)
    LevelEvent(EVENT_LEVEL_SHIFT, slab, p->gouid);
}

void OnStaticCollide( event_t* e, void* user){
  ent_t* slab = user;
  ent_t* other = e->data;

  if(!SetState(other, STATE_PUSHING, NULL))
    return;

  if(SetState(slab, STATE_SHIFTING, NULL))
  LevelEvent(EVENT_SHIFT_SLAB, slab, other->gouid);
}

void EntSignalAction(event_t* ev, void* user){
  ent_t* e = user;

  if(!e->action)
    return;
  e->action(ParamMakeObj(DATA_ENT, e->gouid, e), e->action_param);

  e->action = NULL;
  e->action_param = EMPTY_PARAM;

}
