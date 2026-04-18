#ifndef __GAME_PROCESS__
#define __GAME_PROCESS__

#include "game_types.h"
#include "game_common.h"
#include "game_systems.h"
#include "room_data.h"
#include "screens.h"
#include "game_gen.h"

#define MAX_INTERACTIONS 256
#define DEBUG false
static int CURRENT_ENT_IDENTIFIER = 0;
extern ent_t* player;

extern Font font;
static int fixedFPS = 60;

typedef void (*UpdateFn)(void);
typedef bool EntFilterFn(ent_t* e, ent_t* other); 

//INTERACTIONS_T===>
typedef struct {
  int             source_uid; //uid of who interacted (body, ent)
  int             target_uid; //uid of interactee (body, ent)
  cooldown_t*     timer;
} interaction_t;

int InitInteractions();
interaction_t* EntInteraction(unsigned int source, unsigned int target, int duration);
bool AddInteraction(interaction_t* inter);
bool CanInteract(int source, int target);
int GetInteractions(int source);
void FreeInteraction(interaction_t* item);
void FreeInteractionByIndex(int i);
void FreeInteractions();
void InteractionStep();
//==INTERACTIONS_T==>
//EVENTS==>

typedef enum{
  PROCESS_NONE,
  PROCESS_LEVEL,
  PROCESS_DONE
}ProcessType;

typedef enum{
  UPDATE_FRAME,//update running at full fps
  UPDATE_DRAW,
  UPDATE_PRE,
  UPDATE_FIXED,
  UPDATE_POST,
  UPDATE_DONE
}UpdateType;

typedef enum{
  GAME_NONE,
  GAME_LOADING,
  GAME_READY,
  GAME_PAUSE,
  GAME_FINISHED,
  GAME_OVER
}GameState;

void GameSetState(GameState state);

typedef struct{
  ProcessType process;
  GameState   state[PROCESS_DONE];
  events_t    *events;
  UpdateFn    init[PROCESS_DONE];
  UpdateFn    update_steps[PROCESS_DONE][UPDATE_DONE];
  UpdateFn    finish[PROCESS_DONE];
}child_process_t;

typedef struct{
  GameScreen     screen;
  int            game_frames;
  child_process_t children[SCREEN_DONE];
  GameScreen     next[SCREEN_DONE];
  GameState      state[SCREEN_DONE];//TODO each screen needs a state
  events_t       *events;
  int            album_id[SCREEN_DONE];
  UpdateFn       init[SCREEN_DONE];
  UpdateFn       update_steps[SCREEN_DONE][UPDATE_DONE];
  UpdateFn       finish[SCREEN_DONE];
}game_process_t;
extern game_process_t game_process;
void InitGameEvents();
void InitGameProcess();
void GameProcessStep();
void GameProcessSync(bool wait);
bool GameTransitionScreen();
void GameProcessEnd();
void GameProcessAddEvent(ProcessType, cooldown_t*);
//===WORLD_T===>
typedef struct{
  map_cell_t*   slot;
  Signals       solution;
  SolveStatus   status;
}solution_t;

typedef struct{
  int         cap, count, solved;
  solution_t  *entries;
  SolveStatus completion;
}stage_puzzle_t;

struct level_s{
  Levels          id;
  int             turn;
  Cell            start_pos;
  event_bus_t     *events;
  stage_puzzle_t* puzzle;
  map_grid_t*     map;
};
level_t* InitLevel(Levels id);
void LevelReady(level_t* l);
void LevelEventOnce(EventType type, void* data, uint64_t uid);
void LevelEvent(EventType type, void* data, uint64_t uid);
void LevelTargetSubscribe(EventType, EventCallback, void*, uint64_t);
void LevelSubscribe(EventType event, EventCallback cb, void* data);
void LevelStartCooldown(cooldown_t* cd);
bool PuzzleRegisterSolution(stage_puzzle_t*, map_cell_t*, Signals);

typedef struct{
  unsigned int    num_ents;
}world_data_t;

typedef struct world_s{
  ent_t*        ents[MAX_ENTS];
  unsigned int  num_ent;
  hash_map_t    ent_map, tile_map;
  render_text_t *texts[MAX_EVENTS];
  bool          floatytext_used[MAX_EVENTS];
  level_t       *levels[LVL_ALL];
  Levels        stage;
} world_t;
extern world_t world;
void OnWorldEvent(event_t *e, void* user);
ent_t* WorldPlayer(void);
int WorldGetEnts(ent_t** results,EntFilterFn fn, void* params);
bool RegisterBehaviorTree(BehaviorData data);
game_object_uid_i RegisterEnt( ent_t *e, Cell pos);
game_object_uid_i RegisterMapCell(map_cell_t*);
void WorldInitOnce();
void WorldPreUpdate();
void WorldFixedUpdate();
void WorldPostUpdate();
void InitWorld(world_data_t data);
void WorldRender();
Rectangle WorldRoomBounds();
static int WorldGetTime(){
  return game_process.game_frames;
}

static level_t* WorldGetLevel(void){
  return world.levels[world.stage];
}

const char* GetLevelString(void);
static map_grid_t* WorldGetMap(void){

  level_t* l = WorldGetLevel();

  if(l)
    return l->map;

  return NULL;
}
static int WorldGetTurn(){
  if(world.levels[world.stage])
    return world.levels[world.stage]->turn;

  return 0;
}

static void CooldownEmit(void* params){
  cooldown_t* cd = params;
  LevelEvent(cd->type, cd, cd->uid);
}

static map_cell_t* WorldGetTile(hash_key_t key){
  return HashGet(&world.tile_map, key);
}

#endif

