#pragma once

#include "Camera.h"
#include <renderer/Mesh.h>
#include <renderer/Renderer.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <imgui/imgui_impl_sdl2.h>
#include <SDL2/SDL_image.h>
#include <string>


struct GlobalState
{
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // World
    int loadingDistance = 4;

    // Rendering
    bool useZPrepass = false;

    // Lighting
    /*
    DirectionalLight sunLight;
    PointLight tempPlayerLight = PointLight{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(1.0f, 0.9f, 0.7f),
        glm::vec3(1.0f, 1.0f, 0.9f),
        1.0f,
        0.09f,
        0.032f
    };*/

    // ExtraFX
    float windSpeed = 0.0f;


    float rotation;

    // Performance
    float displayedFPS = 0.0f;
    float memoryMB = 0.0f;
    float memoryChangeMB = 0.0f;
};

class Engine
{
public:
    Engine();
    ~Engine();
public:
    void InitWindow(int width, int height);
    void MainLoop();
    void LoadModel(const std::string& filePath);
private:
    void RenderModels();
    void ProcessInput();
    void UpdateTiming();
    void RenderUI();
private:
    unsigned char* GetRGBPointer(int* outWidth = nullptr, int* outHeight = nullptr, int* outPitch = nullptr) 
    {
        int width, height;
        SDL_GetRendererOutputSize(m_renderer.GetRenderer(), &width, &height);
        
        if (outWidth) *outWidth = width;
        if (outHeight) *outHeight = height;

        if (!m_surface || m_surface->w != width || m_surface->h != height) {
            if (m_surface) SDL_FreeSurface(m_surface);
            m_surface = SDL_CreateRGBSurfaceWithFormat(
                0, width, height, 24, SDL_PIXELFORMAT_RGB24
            );
        }

        SDL_RenderReadPixels(m_renderer.GetRenderer(), NULL,
                             m_surface->format->format,
                             m_surface->pixels,
                             m_surface->pitch);
        
        if (outPitch) *outPitch = m_surface->pitch;

        return static_cast<unsigned char*>(m_surface->pixels);
    }
private:
    bool m_running = true;
    int m_width;
    int m_height;
    GlobalState m_state;
    SDL_Window* m_window;
    Renderer m_renderer;
    SDL_Surface* m_surface = nullptr;
    std::vector<unsigned char> m_rgbBuffer;
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    std::vector<Mesh> m_meshes;
};