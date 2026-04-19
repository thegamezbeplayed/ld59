#include "game_process.h"
#include "game_types.h"

void OnSignalEvent(event_t* ev, void* user){
  signal_interaction_d* sigint = user;
  signal_condition_t sigcon = SIGCON[sigint->condition];

  if(ev->type != sigcon.state)
    return;

  param_t p = EMPTY_PARAM;
  param_t other = EMPTY_PARAM;

  switch(ev->type){
    case EVENT_ENTER_CELL:
      p = WorldGetParam(DATA_ENT, ev->iuid);
      break;
    default:
      return;
      break;
  }
  if(sigint->wait == 0)    
  sigint->action(p, other);

  ent_t* e = p.data;
  e->action = sigint->action;
  e->action_param = other;
  LevelScheduleEvent(EVENT_SIGNAL_ACTION, e, ev->iuid, TF_UPDATE, 9 ); 
}

void InitSignalEvent(game_object_uid_i gouid, signal_interaction_d* sint,
    signal_condition_t* scond){

  LevelTargetSubscribe(scond->state, OnSignalEvent, sint, gouid);
}

void InitSignals(game_object_uid_i gouid, Signals signals){
  while(signals){
    Signal sig = signals & -signals;
    signals &= signals -1;

    signal_interaction_d* sigint = SignalsGetEntry(sig);

    if(!sigint || sigint->signal != sig)
      continue;

    signal_condition_t sigcon = SIGCON[sigint->condition];
    switch(sigcon.type){
      case DATA_EVENT:
        InitSignalEvent(gouid, sigint, &sigcon);
      default:
        continue;
        break;
    }
  }
}

bool ActionGlide(param_t user, param_t other){
  if(user.type != DATA_ENT)
   return false;

  ent_t* e = user.data;

  int x = e->facing.x + e->pos.x;
  int y = e->facing.y + e->pos.y;

  if(y==10)
    DO_NOTHING();
  TraceLog(LOG_INFO, "GLIDE TO [%i, %i]",x, y);
  return EntGridStep(e, e->facing) < TILE_ISSUES;
}
