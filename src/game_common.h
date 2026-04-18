#ifndef __GAME_COMMON__
#define __GAME_COMMON__

#include "raylib.h"
#include "game_tools.h"
#include "game_enum.h"
#include "game_lists.h"


#define CELL_WIDTH 16
#define CELL_HEIGHT 16

static inline bool LESS_THAN(int a, int b){
  return a<b;
}

static inline bool GREATER_THAN(int a, int b){
  return a>b;
}

static inline bool NOT_EQUAL_TO(int a, int b){
  return a!=b;
}


static inline bool EQUAL_TO(int a, int b){
  return a==b;
}

static inline bool ALWAYS(int a, int b){
  return true;
}

static inline bool NEVER(int a, int b){
  return false;
}
struct ent_s;   // forward declaration

//====EVENTS===>
typedef void (*CooldownCallback)(void* params);


typedef struct{
  uint64_t          uid;
  EventType         type;
  int               duration;
  int               elapsed;
  bool              is_complete;
  bool              is_recycled;
  void*             on_end_params;
  void*             on_step_params;
  CooldownCallback  on_end;
  CooldownCallback  on_step;
}cooldown_t;
cooldown_t* InitCooldown(int dur, EventType type, CooldownCallback on_end_callback, void* params);
void UnloadCooldown(cooldown_t* cd);

typedef struct{
  cooldown_t  cooldowns[MAX_EVENTS];
  bool        cooldown_used[MAX_EVENTS];
}events_t;

events_t* InitEvents();
void UnloadEvents(events_t* ev);
int AddEvent(events_t* pool, cooldown_t* cd);
void StepEvents(events_t* pool);
void StartEvent(events_t* pool, EventType type);
void ResetEvent(events_t* pool, EventType type);
bool CheckEvent(events_t* pool, EventType type);
typedef enum{
  STAT_POINTS,
  STAT_TURNS,
  STAT_ENT,
  STAT_MAX_COLOR,
  STAT_MAX_TYPE,
  STAT_COLOR_MUL,
  STAT_TYPE_MUL,
  STAT_COMBO_MUL,
  STAT_BLANK
}StatType;

struct stat_s;
typedef bool (*StatOwnerCallback)(struct ent_s* owner);
typedef void (*StatCallback)(struct ent_s* owner, float old, float cur);
typedef float (*StatGetter)(struct stat_s* self);
typedef struct stat_s{
  StatType  attribute;
  float     min;
  float     max;
  float     current;
  float     increment;
  StatGetter ratio;
  StatCallback on_stat_change;
  StatOwnerCallback on_stat_empty;
} stat_t;

stat_t* InitStatOnMin(StatType attr, float min, float max);
stat_t* InitStatOnMax(StatType attr, float val);
stat_t* InitStatEmpty(void);
stat_t* InitStat(StatType attr,float min, float max, float amount);
bool StatIncrementValue(stat_t* attr,bool increase);
bool StatChangeValue(struct ent_s* owner, stat_t* attr, float val);
void StatMaxOut(stat_t* s);
void StatEmpty(stat_t* s);
float StatGetRatio(stat_t *self);
//<====STATS

typedef enum{
  STATE_NONE,//if ent_t is properly initalized to {0} this is already set
  STATE_SPAWN,//Should only be set after NONE
  STATE_IDLE, //should be able to move freely between these ==>
  STATE_PLACED,
  STATE_DIE,//<===== In MOST cases. Should not be able to go down from DIE
  STATE_END//sentinel entity state should never be this or greater
}EntityState;

typedef bool (*StateComparator)(int a, int b);

typedef struct{
  int             state;
  StateComparator can;
  int             required;
}state_change_requirement_t;

#endif
