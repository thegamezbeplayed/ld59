#include "game_process.h"

void OnLevelEvent(EventType event, void* data, void* user){
  level_t* l = user;

  switch(event){
    case EVENT_LEVEL_TURN_END:
      l->turn++;
      break;
  }
}

void OnPlayerAction(EventType event, void* data, void* user){
  level_t* l = user;
  ent_t* e = data;

  cooldown_t* cd = InitCooldown(24, EVENT_LEVEL_TURN_END, CooldownEmit, cd);
  cd->on_end_params = cd;
  LevelStartCooldown(cd);
}

level_t* InitLevel(Levels id){
  level_t* l = GameCalloc("InitLevel", 1, sizeof(level_t));

  l->start_pos = CELL_EMPTY;

  l->events = InitEventBus(128);
  player = InitEnt(ENT_PLAYER);
  if(player)
    if(RegisterEnt(player, l->start_pos))
      player->sprite->owner = player;

  return l;
}

void LevelReady(level_t* l){
  l->turn++;

  LevelTargetSubscribe(EVENT_ENT_ACTION, OnPlayerAction, l, player->gouid);
  LevelSubscribe(EVENT_LEVEL_TURN_END, OnLevelEvent, l);
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

void LevelEvent(EventType type, void* data, uint64_t uid){
  event_t event = {
    .type = type,
    .data = data,
    .iuid = uid,
    .max  = -1
  };

  EventEmit(WorldGetLevel()->events, &event);
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
