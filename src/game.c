#include "raylib.h"
#include "game_process.h"
#include "game_ui.h"

Camera2D camera = { 0 };
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void){
//  MenuSetState(&ui.menus[MENU_HUD],MENU_ACTIVE);
  //camera.target = player.position;
  InitCamera(3.0f,0.0f,Vector2Scale(VECTOR2_CENTER_SCREEN,0.5f),VECTOR2_CENTER_SCREEN);

  InitGameEvents();
}

void PreUpdate(void){
  GameProcessStep();
  WorldPreUpdate();
}

void FixedUpdate(void){
  AudioStep();
  WorldFixedUpdate();
}

void PostUpdate(void){
  WorldPostUpdate();
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
  if(player)
    ScreenCameraSync( WorldPlayer()->pos );
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
  BeginDrawing();
  ClearBackground(BLACK);

  ScreenCameraToggle();

  WorldRender();

  ScreenRender();
  ScreenCameraToggle();
  //MapGenRender();
  //UISync(FETCH_UPDATE);
  //UIRender();
  DrawFPS(5,5);

  EndDrawing();


}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
  //MenuSetState(&ui.menus[MENU_HUD],MENU_CLOSED);
  //MenuSetState(&ui.menus[MENU_PLAY_AREA],MENU_INACTIVE);

  GameProcessEnd();
  // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
  return 0;
//  return finishScreen;
}
