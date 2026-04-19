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
    {3,6},
    .tiles = {
      [5][0] = TILE_VOID,
      [5][1] = TILE_VOID,
      [5][2] = TILE_VOID,
      [5][3] = TILE_VOID,
      [5][4] = TILE_VOID,
      [5][5] = TILE_VOID,
      [5][6] = TILE_VOID,
      [5][7] = TILE_VOID,
      [5][8] = TILE_VOID,
      [5][9] = TILE_VOID,
      [1][1] = TILE_LAGUZ,
      [9][9] = TILE_SOL,
    },
    .slabs = {
      [4][3] = SLAB_SOL,
      [2][2] = SLAB_LAGUZ,
      [7][8] = SLAB_LAGUZ,
    }
  },
  {
    10, 10,
    {5,4},
    .tiles = {

    },
    .slabs = {
      [3][2] = SLAB_ISA
    }
  },
  {
    12, 10,
    {3, 1},
    .tiles = {
      [7][0] = TILE_VOID,
      [7][1] = TILE_VOID,
      [7][2] = TILE_VOID,
      [7][3] = TILE_VOID,
      [7][4] = TILE_VOID,
      [7][5] = TILE_VOID,
      [7][6] = TILE_VOID,
      [7][7] = TILE_VOID,
      [7][8] = TILE_VOID,
      [7][9] = TILE_VOID,
      [0][4] = TILE_VOID,
      [1][4] = TILE_VOID,
      [2][4] = TILE_VOID,
      [3][4] = TILE_VOID,
      [4][4] = TILE_VOID,
      [5][4] = TILE_VOID,
      [6][4] = TILE_VOID,
      [8][4] = TILE_VOID,
      [9][4] = TILE_VOID,
      [10][4] = TILE_VOID,
      [11][4] = TILE_VOID,
    },
    .slabs = {
      [2][2] = SLAB_ANSUZ,
      [3][2] = SLAB_GEBO
    }
  }  
};

static const BehaviorData room_behaviors[BN_COUNT] = {
  {BN_NONE} ,
  {BN_CHANGE_STATE, false, BT_LEAF,LeafChangeState},
  {BN_CHECK_SIGNAL, false, BT_LEAF, LeafCheckSignal},
  {BN_GET_COND,     false, BT_LEAF, LeafGetConditions},
};
#endif
