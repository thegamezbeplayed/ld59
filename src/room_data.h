#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 774
#define ROOM_HEIGHT 774

#define ROOM_LEVEL_COUNT 3

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
