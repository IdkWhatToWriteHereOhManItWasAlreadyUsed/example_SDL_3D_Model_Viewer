#pragma once

#include <my_glm/glm.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>
#include <array>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    static void Initialize(SDL_Window* window);
    static void Update();

    static bool ProcessEvent(const SDL_Event& event);

    static bool IsKeyPressed(SDL_Keycode key);
    static bool IsKeyReleased(SDL_Keycode key);
    static bool IsKeyDown(SDL_Keycode key);
    static bool IsKeyUp(SDL_Keycode key);

    static glm::vec2 GetMousePosition();
    static glm::vec2 GetMouseDelta();
    static float GetMouseWheelDelta();

    static bool IsMouseButtonDown(Uint8 button);
    static bool IsMouseButtonPressed(Uint8 button);
    static bool IsMouseButtonReleased(Uint8 button);

    static void SetMouseCursorVisible(bool visible);
    static void SetMouseCursorMode(SDL_bool mode);
    static void SetRelativeMouseMode(bool enabled);

    static struct InputState GetInputState();

    static void UpdateCursorLock();
    static bool lockCursor;

protected:
    static std::unordered_map<SDL_Keycode, bool> keys;
    static std::unordered_map<SDL_Keycode, bool> prevKeys;

    static std::array<bool, 5> mouseButtons;
    static std::array<bool, 5> prevMouseButtons;

    static glm::vec2 lastMousePos;
    static glm::vec2 currentMousePos;
    static glm::vec2 mouseDelta;
    static bool firstMouse;

    static float mouseWheelDelta;
    static float mouseWheelCurrent;

    static SDL_Window* window;
};

struct InputState
{
    std::unordered_map<SDL_Keycode, bool> keys;
    std::unordered_map<SDL_Keycode, bool> keysPressed;
    std::unordered_map<SDL_Keycode, bool> keysReleased;

    std::array<bool, 5> mouseButtons;
    std::array<bool, 5> mouseButtonsPressed;
    std::array<bool, 5> mouseButtonsReleased;

    glm::vec2 mousePosition;
    glm::vec2 mouseDelta;
    float mouseWheelDelta;
};