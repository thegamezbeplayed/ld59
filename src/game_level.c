#include "game_process.h"
#include "game_control.h"
#include "game_helpers.h"

solution_t* PuzzleGetEntry(stage_puzzle_t* p, game_object_uid_i gouid){
  for(int i = 0; i < p->count; i++){
    solution_t* s = &p->entries[i];
    if(s->slot->gouid == gouid)
      return s;
  }

  return NULL;
}

void PuzzleSetStatus(stage_puzzle_t* p, solution_t* s, SolveStatus status){
  switch(status){
    case SOLV_TRUE:
      LevelEvent(EVENT_LEVEL_SOLVED, WorldGetLevel(), s->slot->gouid);
  }
}

void OnSolutionStatus(stage_puzzle_t* p, solution_t* s, SolveStatus status){

  switch(status){
    case SOLV_TRUE:
      p->solved++;
      if(p->solved >= p->count)
        PuzzleSetStatus(p, s, SOLV_TRUE);
      break;
  }
}

bool SolutionSetStatus(solution_t* s, SolveStatus status){
  if(s->status == status)
    return false;

  SolveStatus old = s->status;
  s->status = status;
  return true;
}

void PuzzleUpdateSolution(stage_puzzle_t* p, map_cell_t* mc, SolveStatus status){
  solution_t* s = PuzzleGetEntry(p, mc->gouid);

  if(!s)
    return;

  if(SolutionSetStatus(s, status))
    OnSolutionStatus(p, s, status);
}

SolveStatus LevelCheckSolution(map_cell_t* mc, ent_t* e){
  SolveStatus status = SOLV_NO;

  Signals mcig = TILE_SIGNALS[mc->tile];
  Signals esig = e->signals;

  if((mcig & esig) > 0)
    status = SOLV_TRUE;

  return status;
}

void OnLevelEvent(event_t *e, void* user){
  level_t* l = user;

  switch(e->type){
    case EVENT_LEVEL_TURN_END:
      l->turn++;

      break;
    case EVENT_LEVEL_SHIFT:
      LevelEvent(EVENT_ENT_ACTION, player, player->gouid);
      InputSync(l->turn);
      break;
    case EVENT_LEVEL_CHECK:
      ent_t* p = e->data;
      if(p->type != ENT_TILE)
        return;

      map_cell_t* mc = WorldGetTile(e->iuid);
      if(mc->tile <= TILE_BLANK)
        return;

      SolveStatus status = LevelCheckSolution(mc, p);
      PuzzleUpdateSolution(l->puzzle, mc, status);
      break;
    case EVENT_LEVEL_SOLVED:
      TraceLog(LOG_INFO, "YOU DID IT");
      LevelEvent(EVENT_LEVEL_END, l, l->id);
      break;
    case EVENT_TILE_INSERT:
      map_cell_t* m = WorldGetTile(e->iuid);
      ent_t* slab = e->data;
      if(!CanSignalEvent(e->type, slab->signals)){
        if(MapForceOccupant(WorldGetMap(), slab, m->coords) < TILE_ISSUES){
          slab->old_pos = slab->pos;
          slab->pos = m->coords;
          SetState(slab, STATE_PLACED, NULL);
        }
        return;
      }
      if(MapSetTile(WorldGetMap(), TILE_BLANK, m->coords) < TILE_ISSUES){
        MapRemoveOccupant(WorldGetMap(), slab->pos);
        SetState(slab, STATE_DIE,NULL);
        
      }

      break;
  }
}

void OnPlayerAction(event_t* ev, void* user){
  level_t* l = user;
  ent_t* e = ev->data;

  LevelScheduleEvent(EVENT_LEVEL_TURN_END, l, l->id, TF_UPDATE, 15);
}

stage_puzzle_t* StartPuzzle(int cap){
  stage_puzzle_t* p = GameCalloc("StartPuzzle", 1, sizeof(stage_puzzle_t));
  p->cap = cap;
  p->entries = GameCalloc("StartPuzzle", cap, sizeof(solution_t));

  return p;
}

bool PuzzleRegisterSolution(stage_puzzle_t* p, map_cell_t* mc, Signals solve){

  solution_t* s = &p->entries[p->count++];

  s->slot = mc;
  s->solution = solve;

  return solve > 0;
}

level_t* InitLevel(Levels id){
  level_t* l = GameCalloc("InitLevel", 1, sizeof(level_t));

  l->id = id;

  level_layout_t ld = LVLS[l->id];
  l->start_pos = ld.start;

  l->events = InitEventBus(128);
  
  player = InitEnt(ENT_PLAYER);
  if(!player)
    return NULL;

  player->gouid = RegisterEnt(player, l->start_pos);
  player->sprite->owner = player;

  l->puzzle = StartPuzzle(16);
  return l;
}

void LevelReady(level_t* l){
  l->map = InitMapGrid(l);

  level_layout_t ld = LVLS[l->id];
  LevelTargetSubscribe(EVENT_SHIFT_SLAB, OnShiftEvent, player, player->gouid);
  for(int x = 0; x < l->map->width; x++){
    for(int y = 0; y < l->map->height; y++){
      Tiles t =ld.slabs[x][y];
      if(t == TILE_BLANK)
        continue;
      Cell pos = CELL_NEW(x,y);
      ent_t* e = InitEntStatic(ENT_TILE, t);
      if(!e)
        continue;

      e->gouid = RegisterEnt(e, pos);

      if(MapSetOccupant(l->map, e, pos) > TILE_ISSUES)
        continue;

     LevelTargetSubscribe(EVENT_TILE_COLLISION, OnStaticCollide, e, e->gouid);

    }
  }
  l->turn++;

  LevelTargetSubscribe(EVENT_ENT_ACTION, OnPlayerAction, l, player->gouid);
  LevelSubscribe(EVENT_LEVEL_TURN_END, OnLevelEvent, l);
  LevelSubscribe(EVENT_LEVEL_SHIFT, OnLevelEvent, l);
  LevelSubscribe(EVENT_LEVEL_CHECK, OnLevelEvent, l);
  LevelSubscribe(EVENT_LEVEL_SOLVED, OnLevelEvent, l);
  LevelSubscribe(EVENT_TILE_INSERT, OnLevelEvent, l);
}

void LevelFixedUpdate(void){
  level_t* l = WorldGetLevel();
  if(!l)
    return;

  EventBusStep(l->events);
}

void LevelEventPayload(EventType type, void* data, uint64_t uid, param_t p){
  level_t* l = WorldGetLevel();
  if(!l)
    return;

  event_t event = {
    .type = type,
    .data = data,
    .iuid = uid,
    .max  = -1,
    .payload = p
  };

  EventEmit(l->events, &event);
}

void LevelEventOnce(EventType type, void* data, uint64_t uid){
 event_t event = {
    .type = type,
    .data = data,
    .iuid = uid,
    .max  = 1
  };

  EventEmit(WorldGetLevel()->events, &event);

}
void LevelScheduleEvent(EventType type, void* data, uint64_t uid, TimeFrame tf, int step){
  level_t* l = WorldGetLevel();
  if(!l)
    return;

  switch(tf){
    case TF_TURN:
      step += WorldGetTurn();
      break;
    case TF_UPDATE:
      step += WorldGetTime();
      break;
    default:
      return;
      break;
  }

  event_t* event = GameCalloc("LevelScheduleEvent",1, sizeof(event_t));

  *event = (event_t){
    .type = type,
    .data = data,
    .iuid = uid,
    .max  = -1,
    .timing = tf,
    .scheduled = step,
  };

  EventSchedule(l->events, event);
}

void LevelEvent(EventType type, void* data, uint64_t uid){
  level_t* l = WorldGetLevel();
  if(!l)
    return;

  event_t event = {
    .type = type,
    .data = data,
    .iuid = uid,
    .max  = -1
  };

  EventEmit(l->events, &event);
}

void LevelTargetSubscribe(EventType event, EventCallback cb, void* data, uint64_t iid){

  event_sub_t* sub = EventSubscribe(WorldGetLevel()->events, event, cb, data);
  sub->uid = iid;

}
void LevelSubscribe(EventType event, EventCallback cb, void* data){
  event_sub_t* sub = EventSubscribe(WorldGetLevel()->events, event, cb, data);
  sub->uid = -1;
}

void LevelStartCooldown(cooldown_t* cd){
  GameProcessAddEvent(PROCESS_LEVEL, cd);
 
}
