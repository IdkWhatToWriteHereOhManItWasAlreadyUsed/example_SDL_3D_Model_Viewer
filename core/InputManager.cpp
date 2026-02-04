#include "InputManager.h"
#include <iostream>

std::unordered_map<SDL_Keycode, bool> InputManager::keys;
std::unordered_map<SDL_Keycode, bool> InputManager::prevKeys;
std::array<bool, 5> InputManager::mouseButtons;
std::array<bool, 5> InputManager::prevMouseButtons;
glm::vec2 InputManager::lastMousePos;
glm::vec2 InputManager::currentMousePos;
glm::vec2 InputManager::mouseDelta;
bool InputManager::firstMouse = true;
float InputManager::mouseWheelDelta = 0.0f;
float InputManager::mouseWheelCurrent = 0.0f;
SDL_Window* InputManager::window = nullptr;
bool InputManager::lockCursor = false;

InputManager::InputManager() = default;
InputManager::~InputManager() = default;

void InputManager::Initialize(SDL_Window* win) {
    window = win;
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    lastMousePos = glm::vec2(width / 2.0f, height / 2.0f);
    currentMousePos = lastMousePos;
    keys.clear();
    prevKeys.clear();
}

void InputManager::Update() {
    prevKeys = keys;
    prevMouseButtons = mouseButtons;
    mouseDelta = glm::vec2(0.0f);
    mouseWheelDelta = 0.0f;
}

bool InputManager::ProcessEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            keys[event.key.keysym.sym] = true;
            return true;
        
        case SDL_KEYUP:
            keys[event.key.keysym.sym] = false;
            return true;
        
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button >= 1 && event.button.button <= 5) {
                mouseButtons[event.button.button - 1] = true;
            }
            return true;
        
        case SDL_MOUSEBUTTONUP:
            if (event.button.button >= 1 && event.button.button <= 5) {
                mouseButtons[event.button.button - 1] = false;
            }
            return true;
        
        case SDL_MOUSEMOTION:
            currentMousePos = glm::vec2(event.motion.x, event.motion.y);
            
            if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
                mouseDelta = glm::vec2(event.motion.xrel, event.motion.yrel);
            } else {
                if (firstMouse) {
                    lastMousePos = currentMousePos;
                    firstMouse = false;
                }
                mouseDelta = currentMousePos - lastMousePos;
                lastMousePos = currentMousePos;
            }
            return true;
        
        case SDL_MOUSEWHEEL:
            mouseWheelDelta = static_cast<float>(event.wheel.y);
            mouseWheelCurrent += mouseWheelDelta;
            return true;
    }
    
    return false;
}

bool InputManager::IsKeyPressed(SDL_Keycode key) {
    return keys[key] && !prevKeys[key];
}

bool InputManager::IsKeyReleased(SDL_Keycode key) {
    return !keys[key] && prevKeys[key];
}

bool InputManager::IsKeyDown(SDL_Keycode key) {
    return keys[key];
}

bool InputManager::IsKeyUp(SDL_Keycode key) {
    return !keys[key];
}

glm::vec2 InputManager::GetMousePosition() {
    return currentMousePos;
}

glm::vec2 InputManager::GetMouseDelta() {
    return mouseDelta;
}

float InputManager::GetMouseWheelDelta() {
    return mouseWheelDelta;
}

bool InputManager::IsMouseButtonDown(Uint8 button) {
    if (button >= 1 && button <= 5) return mouseButtons[button - 1];
    return false;
}

bool InputManager::IsMouseButtonPressed(Uint8 button) {
    if (button >= 1 && button <= 5) 
        return mouseButtons[button - 1] && !prevMouseButtons[button - 1];
    return false;
}

bool InputManager::IsMouseButtonReleased(Uint8 button) {
    if (button >= 1 && button <= 5) 
        return !mouseButtons[button - 1] && prevMouseButtons[button - 1];
    return false;
}

void InputManager::SetMouseCursorVisible(bool visible) {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
    SDL_SetWindowGrab(window, visible ? SDL_FALSE : SDL_TRUE);
    SDL_SetRelativeMouseMode(visible ? SDL_FALSE : SDL_TRUE);
    lockCursor = !visible;
}

void InputManager::SetMouseCursorMode(SDL_bool mode) {
    SDL_SetRelativeMouseMode(mode);
}

void InputManager::SetRelativeMouseMode(bool enabled) {
    SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
    lockCursor = enabled;
}

InputState InputManager::GetInputState() {
    InputState state;
    state.keys = keys;
    state.mouseButtons = mouseButtons;
    
    for (const auto& [key, value] : keys) {
        state.keysPressed[key] = value && !prevKeys[key];
        state.keysReleased[key] = !value && prevKeys[key];
    }
    
    for (size_t i = 0; i < mouseButtons.size(); i++) {
        state.mouseButtonsPressed[i] = mouseButtons[i] && !prevMouseButtons[i];
        state.mouseButtonsReleased[i] = !mouseButtons[i] && prevMouseButtons[i];
    }
    
    state.mousePosition = currentMousePos;
    state.mouseDelta = mouseDelta;
    state.mouseWheelDelta = mouseWheelDelta;
    
    return state;
}

void InputManager::UpdateCursorLock() {
    static bool prevCtrlState = false;
    bool currentCtrlState = IsKeyDown(SDLK_LCTRL) || IsKeyDown(SDLK_RCTRL);
    
    if (currentCtrlState && !prevCtrlState) {
        lockCursor = !lockCursor;
        SetRelativeMouseMode(lockCursor);
        SetMouseCursorVisible(!lockCursor);
    }
    
    prevCtrlState = currentCtrlState;
}