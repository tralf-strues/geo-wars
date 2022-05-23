#include "Engine.h"
#include <memory.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>

#include "game_layer.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

using namespace gwars;

constexpr size_t MOUSE_BUTTONS_COUNT = 2;

FrameBuffer     g_FrameBuffer{reinterpret_cast<Color*>(buffer), SCREEN_WIDTH, SCREEN_HEIGHT};
Renderer        g_Renderer(g_FrameBuffer);
EventDispatcher g_EventDispatcher;
GameLayer*      g_GameLayer;
Vec2f           g_MousePos{0, 0};
bool            g_KeyPressed[static_cast<uint32_t>(Key::Total)];
bool            g_MouseButtonPressed[MOUSE_BUTTONS_COUNT];

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

void processMouseButtonEvent(uint32_t button)
{
    if (is_mouse_button_pressed(button) && !g_MouseButtonPressed[button])
    {
        g_EventDispatcher.fireEvent<MouseButtonPressedEvent>(button);
        g_MouseButtonPressed[button] = true;
    }
    else if (!is_mouse_button_pressed(button) && g_MouseButtonPressed[button])
    {
        g_EventDispatcher.fireEvent<MouseButtonReleasedEvent>(button);
        g_MouseButtonPressed[button] = false;
    }
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

    for (uint32_t mouseButton = 0; mouseButton < MOUSE_BUTTONS_COUNT; ++mouseButton)
    {
        processMouseButtonEvent(mouseButton);
    }
}

void initialize()
{
    srand(time(nullptr));

    for (uint32_t i = 0; i < static_cast<uint32_t>(Key::Total); ++i)
    {
        g_KeyPressed[i] = false;
    }

    for (uint32_t i = 0; i < MOUSE_BUTTONS_COUNT; ++i)
    {
        g_MouseButtonPressed[i] = false;
    }

    g_GameLayer = new GameLayer(g_EventDispatcher);
    g_GameLayer->onInit();
}

void act(float dt)
{
    if (is_key_pressed(VK_ESCAPE) || g_GameLayer->isStopped())
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
