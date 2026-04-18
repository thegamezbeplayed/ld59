#include "game_gen.h"
#include "game_types.h"
#include "game_process.h"

void InitMap(void){
  for(int i = 0; i < TILE_DONE; i++){
    sprite_t* s = InitSpriteByID(i, SHEET_TILE);
  }
}

map_grid_t* InitMapGrid(void){
  map_grid_t* m = GameMalloc("InitMapGrid", sizeof(map_grid_t));

  *m = (map_grid_t){0};

  m->step_size = CELL_WIDTH;

  m->x = 0;
  m->y = 0;
  m->width = 24;
  m->height = 24;
  m->floor = DARKBROWN;

  m->tiles = GameMalloc("InitMapGrid", m->width * sizeof(map_cell_t*));
  for(int x = 0; x < m->width; x++){
    m->tiles[x] = GameCalloc("InitMapGrid", m->height, sizeof(map_cell_t)); 
    for(int y = 0; y < m->height; y++){
      Cell pos = CELL_NEW(x,y);
      map_cell_t* mc = InitMapCell(m, TILE_EMPTY, pos);

      mc->gouid = RegisterMapCell(mc);
    }
  }
  
  return m;
}

map_cell_t* InitMapCell(map_grid_t* m, Tiles t, Cell pos){
  map_cell_t* mc = &m->tiles[pos.x][pos.y];

  mc->coords = pos;
  mc->tile = t;

  return mc;
}

TileStatus MapChangeOccupant(map_grid_t* map,ent_t* e, Cell old, Cell c){
  Cell bounds = CELL_NEW(map->width,map->height);
  if(!cell_in_bounds(c,bounds))
    return TILE_OUT_OF_BOUNDS;

  if(map->tiles[c.x][c.y].occupant)
    return TILE_OCCUPIED;

  if(map->tiles[old.x][old.y].occupant)
    MapRemoveOccupant(map,old);

  return MapSetOccupant(map,e,c);
}


TileStatus MapSetTile(map_grid_t* m, Tiles t, Cell c){
  Cell bounds = CELL_NEW(m->width,m->height);
  if(!cell_in_bounds(c,bounds))
    return TILE_OUT_OF_BOUNDS;

  map_cell_t* mc = &m->tiles[c.x][c.y];

  mc->tile = t;
  if(TileHasFlag(t, TILEFLAG_SOLID))
    mc->status = TILE_COLLISION;
  else if(TileHasFlag(t, TILEFLAG_BORDER))
    mc->status = TILE_BORDER;
  else
    mc->status = TILE_EMPTY;

  
  return TILE_SUCCESS;
}

TileStatus MapSetOccupant(map_grid_t* m, ent_t* e, Cell c){
  Cell bounds = CELL_NEW(m->width,m->height);
  if(!cell_in_bounds(c,bounds))
    return TILE_OUT_OF_BOUNDS;

  map_cell_t* mc = &m->tiles[c.x][c.y];
  if(mc->status > TILE_ISSUES)
    return TILE_OCCUPIED;

  MapRemoveOccupant(m,e->pos);
  mc->occupant =e;
  mc->status = TILE_OCCUPIED;

  return TILE_SUCCESS;
}

map_cell_t* MapGetTile(map_grid_t* map,Cell tile){
  Cell bounds = CELL_NEW(map->width,map->height);

  if(!cell_in_bounds(tile,bounds))
    return NULL;

  return &map->tiles[tile.x][tile.y];

}

TileStatus MapTileAvailable(map_grid_t* m, Cell c){
  Cell bounds = CELL_NEW(m->width,m->height);

  if(!cell_in_bounds(c,bounds))
    return TILE_OUT_OF_BOUNDS;
  map_cell_t* mc = &m->tiles[c.x][c.y];

  return mc->status;
}

TileStatus MapRemoveOccupant(map_grid_t* m, Cell c){
  Cell bounds = CELL_NEW(m->width,m->height);
  if(!cell_in_bounds(c,bounds))
    return TILE_OUT_OF_BOUNDS;

  m->tiles[c.x][c.y].occupant = NULL;

  m->tiles[c.x][c.y].status = TILE_EMPTY;

  return TILE_SUCCESS;
}

ent_t* MapGetOccupant(map_grid_t* m, Cell c, TileStatus* status){
  map_cell_t tile = m->tiles[c.x][c.y];

  *status = tile.status;

  return tile.occupant;
}

