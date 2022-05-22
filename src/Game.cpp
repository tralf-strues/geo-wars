#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include <stdio.h>

#include "game_layer.hpp"

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  schedule_quit_game() - quit game after act()

using namespace gwars;

FrameBuffer g_FrameBuffer{reinterpret_cast<Color*>(buffer), SCREEN_WIDTH, SCREEN_HEIGHT};
Renderer g_Renderer(g_FrameBuffer);
GameLayer* g_GameLayer;

// initialize game data in this function
void initialize()
{
  g_GameLayer = new GameLayer();
  g_GameLayer->onInit();
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();

  g_GameLayer->onUpdate(dt);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  g_GameLayer->onRender(g_Renderer);
}

// free game data in this function
void finalize()
{
  delete g_GameLayer;
}

