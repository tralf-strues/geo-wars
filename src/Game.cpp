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

void fireKeyPressedEvent(uint32_t platformKey, Key key)
{
    if (is_key_pressed(platformKey))
    {
        g_EventDispatcher.fireEvent<KeyPressedEvent>(key);
    }
}

void fireMouseMoveEvent()
{
    g_MousePos = g_Renderer.frameBufferToNdc(Vec2f(get_cursor_x(), get_cursor_y()));
    g_EventDispatcher.fireEvent<MouseMoveEvent>(g_MousePos.x, g_MousePos.y);
    printf("Mouse moved: %f, %f\n", g_MousePos.x, g_MousePos.y);
}

void processInputEvents()
{
    fireKeyPressedEvent(VK_ESCAPE, Key::Esc);
    fireKeyPressedEvent(VK_SPACE, Key::Space);
    fireKeyPressedEvent(VK_LEFT, Key::Left);
    fireKeyPressedEvent(VK_UP, Key::Up);
    fireKeyPressedEvent(VK_RIGHT, Key::Right);
    fireKeyPressedEvent(VK_DOWN, Key::Down);
    fireKeyPressedEvent(VK_RETURN, Key::Return);

    fireMouseMoveEvent();
}

class EventSystemTester
{
public:
    int x{0};

    void onKeyPressed(const KeyPressedEvent& event)
    {
        printf("Key pressed [%d], EventSystemTester.x = %d\n", event.key, x);
    }
};

void staticOnKeyPressed(const KeyPressedEvent& event)
{
    printf("Key pressed [%d] STATIC\n", event.key);
}

EventSystemTester g_Tester;
void initialize()
{
    g_GameLayer = new GameLayer(g_EventDispatcher);
    g_GameLayer->onInit();

    g_Tester.x = 22;
    g_EventDispatcher.getSink<KeyPressedEvent>().addHandler<&EventSystemTester::onKeyPressed>(g_Tester);
    g_EventDispatcher.getSink<KeyPressedEvent>().addHandler<&staticOnKeyPressed>();
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
