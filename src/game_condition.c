#include "game_utils.h"
#include "game_types.h"

void InitSignalEvent(game_object_uid_i gouid, signal_interaction_d* sint,
    signal_condition_t* scond){

}

void InitSignals(game_object_uid_i gouid, Signals signals){
  while(signals){
    Signal sig = signals & -signals;
    signals &= signals -1;

    signal_interaction_d* sigint = SignalsGetEntry(sig);

    if(!sigint)
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
  return EntGridStep(e, e->facing) < TILE_ISSUES;
}
