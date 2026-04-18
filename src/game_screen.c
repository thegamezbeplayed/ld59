#include "raylib.h"
#include "game_tools.h"
#include "game_process.h"
#if defined(PLATFORM_ANDROID)
#include <jni.h>
#endif
camera_t* cam;

void InitCamera(float zoom, float rot, Vector2 offset, Vector2 target){
  cam = GameCalloc("InitCamera", 1,sizeof(camera_t));
  Camera2D* raycam = GameCalloc("InitCamera", 1,sizeof(Camera2D));

  raycam->offset = offset;
  raycam->offset.y*=2;
  raycam->rotation = rot;
  raycam->zoom = zoom;

  raycam->target = target;

  float hei_disp = 0;
  cam->bounds = Rect(offset.x, hei_disp, ROOM_WIDTH, ROOM_HEIGHT);
  cam->size = vec_to_cell(RectSize(cam->bounds), CELL_WIDTH);
  cam->view = Rect(0, 0, ROOM_WIDTH, -ROOM_HEIGHT);
  cam->render = LoadRenderTexture(cam->bounds.width, cam->bounds.height);
  cam->target = CELL_UNSET;
  cam->camera = raycam;
}

void ScreenCameraToggle(void){
  cam->mode = !cam->mode;

  if(cam->mode){
    BeginTextureMode(cam->render);
    ClearBackground(BLACK);
    BeginMode2D(*cam->camera);

  }
  else{
    EndMode2D();
    EndTextureMode();
    DrawTexturePro(cam->render.texture, cam->view, cam->bounds, VECTOR2_ZERO, 0, WHITE);
  }
}

void ScreenCameraSync(Cell target){
  Vector2 vpos =  CellToVector2(target,CELL_WIDTH);

    
  cam->camera->target = Vector2Lerp(cam->camera->target,vpos,0.1);

  cam->target = target;

  cam->pos.x = target.x - cam->size.x/2;
  cam->pos.y = target.y - cam->size.y/2;

}

void ScreenRender(void){

}


