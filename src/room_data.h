#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 900
#define ROOM_HEIGHT 900

#define ROOM_LEVEL_COUNT 3

typedef struct{
  int     wid, hei;
  Cell    start;
  Tiles   tiles[12][12];
  Tiles   slabs[12][12];
}level_layout_t;

static level_layout_t LVLS[LVL_ALL] = {
  {
      10, 10,
      {4,3},
    .tiles = {
      [8][3]    = TILE_URUZ,
      [6][7]  = TILE_FEHU
    },
    .slabs = {
      [6][5]  = SLAB_FEHU,
      [7][6] = SLAB_URUZ, 
    }
  },
  {
    10,10,
    {5,6},
    .tiles = {
      [1][1] = TILE_LAGUZ,
      [9][9] = TILE_SOL,
    },
    .slabs = {
      [4][3] = SLAB_SOL,
      [7][8] = SLAB_LAGUZ,
    }
  
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
