#include "Engine.h"
#include <memory.h>
#include <stdlib.h>

#include <stdio.h>

#include "game_layer.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

using namespace gwars;

FrameBuffer     g_FrameBuffer{reinterpret_cast<Color*>(buffer), SCREEN_WIDTH, SCREEN_HEIGHT};
Renderer        g_Renderer(g_FrameBuffer);
EventDispatcher g_EventDispatcher;
GameLayer*      g_GameLayer;
Vec2f           g_MousePos{0, 0};
bool            g_KeyPressed[static_cast<uint32_t>(Key::Total)];

void processKeyEvent(uint32_t platformKey, Key key)
{
    uint32_t keyNumber = static_cast<uint32_t>(key);
    if (is_key_pressed(platformKey) && !g_KeyPressed[keyNumber])
    {
        g_EventDispatcher.fireEvent<KeyPressedEvent>(key);
        g_KeyPressed[keyNumber] = true;
    }
    else if (!is_key_pressed(platformKey) && g_KeyPressed[keyNumber])
    {
        g_EventDispatcher.fireEvent<KeyReleasedEvent>(key);
        g_KeyPressed[keyNumber] = false;
    }
}

void processMouseMoveEvent()
{
    g_MousePos = g_Renderer.frameBufferToNdc(Vec2f(get_cursor_x(), get_cursor_y()));
    g_EventDispatcher.fireEvent<MouseMoveEvent>(g_MousePos.x, g_MousePos.y);
}

void processInputEvents()
{
    processKeyEvent(VK_ESCAPE, Key::Esc);
    processKeyEvent(VK_SPACE, Key::Space);
    processKeyEvent(VK_LEFT, Key::Left);
    processKeyEvent(VK_UP, Key::Up);
    processKeyEvent(VK_RIGHT, Key::Right);
    processKeyEvent(VK_DOWN, Key::Down);
    processKeyEvent(VK_RETURN, Key::Return);

    processMouseMoveEvent();
}

void initialize()
{
    for (uint32_t i = 0; i < static_cast<uint32_t>(Key::Total); ++i)
    {
        g_KeyPressed[i] = false;
    }

    g_GameLayer = new GameLayer(g_EventDispatcher);
    g_GameLayer->onInit();
}

void act(float dt)
{
    if (is_key_pressed(VK_ESCAPE))
    {
        schedule_quit_game();
    }

    processInputEvents();

    g_GameLayer->onUpdate(dt);
}

// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    g_GameLayer->onRender(g_Renderer);
}

void finalize() { delete g_GameLayer; }
