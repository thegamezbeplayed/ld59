#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 900
#define ROOM_HEIGHT 900

#define ROOM_LEVEL_COUNT 3

typedef struct{
  Tiles   tiles[12][12];
  Tiles   slabs[12][12];
}level_layout_t;

static level_layout_t test = {
  .tiles = {
    [0][0]    = TILE_URUZ,
    [10][10]  = TILE_FEHU
  },
  .slabs = {
    [5][5]  = SLAB_FEHU,
    [10][4] = SLAB_URUZ, 
  }
};
typedef struct {
  BehaviorID           id;
  bool                 is_root;
  BehaviorTreeType     bt_type;
  behavior_tree_node_t *(*func)(behavior_params_t *);
  bool          param_overide;
  EntityState   state;
  int           num_children;
  BehaviorID   children[5];
} BehaviorData;

static const BehaviorData room_behaviors[BEHAVIOR_COUNT] = {
  {BEHAVIOR_NONE} ,
  {BEHAVIOR_CHANGE_STATE  ,false,BT_LEAF,LeafChangeState,false, STATE_IDLE,0,{}},
};
#endif
