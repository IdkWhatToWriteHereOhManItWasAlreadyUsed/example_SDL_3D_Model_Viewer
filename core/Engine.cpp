#include "Engine.h"
#include <core/InputManager.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <imgui/imgui_impl_sdl2.h>
#include <SDL2/SDL_image.h>
#include <Global.h>

Engine::Engine()
{

}

Engine::~Engine()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Engine::InitWindow(int width, int height)
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return;
    }
    m_window = SDL_CreateWindow("sdfghoiulk67e5hgw!",SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_UNDEFINED, width, height, 
    SDL_WINDOW_SHOWN);

    if (m_window == NULL) 
    {
        return;
    }

    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags)) 
    {
        return;
    }

    m_width = width;
    m_height = height;
    m_renderer.Initialise(SDL_CreateRenderer(m_window, -1, 
         SDL_RENDERER_PRESENTVSYNC), width, height);

    /*
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer.GetRenderer());
    ImGui_ImplSDLRenderer2_Init(m_renderer.GetRenderer());*/
}

void Engine::MainLoop()
{
    auto renderer = m_renderer.GetRenderer();

    SDL_Surface* captureSurface = SDL_CreateRGBSurfaceWithFormat(
        0, m_width, m_height, 24, SDL_PIXELFORMAT_RGB24);

    while (m_running)
    {
        UpdateTiming();
        ProcessInput();
        RenderModels();
        //RenderUI();
        
        SDL_RenderPresent(renderer);

        SDL_Rect captureRect = {0, 0, m_width, m_height};
        int result = SDL_RenderReadPixels(renderer, &captureRect,
                                         captureSurface->format->format,
                                         captureSurface->pixels, 
                                         captureSurface->pitch);
        
        if (result == 0) {
            uint8_t* pixels = static_cast<uint8_t*>(captureSurface->pixels);
            Global::frameDisplayer->DisplayFrame(&pixels);
        } else {
            std::cerr << "Failed to read pixels: " << SDL_GetError() << std::endl;
        }
    }
    
    SDL_FreeSurface(captureSurface);
}

void Engine::UpdateTiming()
{
    float currentFrame = SDL_GetTicks() / 1000.0f;
    m_state.deltaTime = currentFrame - m_state.lastFrame;
    m_state.lastFrame = currentFrame;
}

void Engine::LoadModel(const std::string& filePath)
{
    Mesh mesh;
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texcoords;
    
    struct PackedVertex 
    {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 tex;
        
        bool operator<(const PackedVertex& other) const 
        {
            return memcmp(this, &other, sizeof(PackedVertex)) < 0;
        }
    };
    
    std::map<PackedVertex, unsigned int> vertexMap;
    
    std::ifstream file(filePath);
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (type == "vn") {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        }
        else if (type == "vt") {
            glm::vec2 tex;
            iss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);
        }
        else if (type == "f") {
            unsigned int faceIndices[3];
            std::string vertexTokens[3];
            iss >> vertexTokens[0] >> vertexTokens[1] >> vertexTokens[2];
            
            for (int i = 0; i < 3; i++) {
                PackedVertex packed;
                std::istringstream vss(vertexTokens[i]);
                std::string indices[3];
                
                for (int j = 0; j < 3; j++) {
                    std::getline(vss, indices[j], '/');
                }
                
                int posIdx = std::stoi(indices[0]) - 1;
                packed.pos = temp_positions[posIdx];
                
                if (!indices[1].empty()) {
                    int texIdx = std::stoi(indices[1]) - 1;
                    packed.tex = temp_texcoords[texIdx];
                }
                
                if (!indices[2].empty()) {
                    int normIdx = std::stoi(indices[2]) - 1;
                    packed.norm = temp_normals[normIdx];
                }
                
                auto it = vertexMap.find(packed);
                if (it != vertexMap.end()) {
                    faceIndices[i] = it->second;
                }
                else {
                    Vertex vertex;
                    vertex.Position = packed.pos;
                    vertex.Normal = packed.norm;
                    vertex.TexCoords = packed.tex;
                    vertex.Color = glm::vec3(1.0f);
                    
                    unsigned int newIndex = mesh.vertices.size();
                    mesh.vertices.push_back(vertex);
                    vertexMap[packed] = newIndex;
                    faceIndices[i] = newIndex;
                }
            }
            
            mesh.indices.push_back(faceIndices[0]);
            mesh.indices.push_back(faceIndices[1]);
            mesh.indices.push_back(faceIndices[2]);
        }
    }
    
    m_meshes.push_back(std::move(mesh));
}

void Engine::RenderModels()
{
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
        static_cast<GLfloat>(m_width) /
        static_cast<GLfloat>(m_height),
        0.1f, 1500.0f
    );
    m_renderer.Clear();
    for (auto& mesh: m_meshes)
    {
        m_renderer.AddToBatch(&mesh);
    }
    m_renderer.RenderMeshes(view, projection, m_state.rotation);
    m_renderer.ClearBatchQueue();
}

void Engine::RenderUI()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", 
                camera.Position.x, camera.Position.y, camera.Position.z);
    ImGui::Text("Yaw: %.2f", camera.Yaw);
    ImGui::Text("Pitch: %.2f", camera.Pitch);

    if (ImGui::SliderInt("Loading Distance", &m_state.loadingDistance, 1, 52))
    {
    }
    
    ImGui::SliderFloat("Speed", &camera.MovementSpeed, 10.0f, 520.0f);
    ImGui::SliderFloat("FOV", &camera.Zoom, 1.0f, 100.0f);
    
    ImGui::Checkbox("Z-Prepass", &m_state.useZPrepass);
    
    ImGui::SliderFloat("Wind Speed", &m_state.windSpeed, 0.0f, 40.0f);
    ImGui::SliderFloat("Model rotation", &m_state.rotation, -90.0f, 90.0f);
    
    ImGui::Separator();
    
    if (ImGui::Button("Reset Camera"))
    {
        camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    
 
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer.GetRenderer());
}

void Engine::ProcessInput() 
{
    SDL_Event e;
    
    while (SDL_PollEvent(&e)) 
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
        
        InputManager::ProcessEvent(e);
        
        if (e.type == SDL_QUIT) 
            m_running = false;
    }
    
    InputManager::UpdateCursorLock();
    
    float delta = m_state.deltaTime;
    if (InputManager::IsKeyDown(SDLK_w)) 
        camera.ProcessKeyboard(FORWARD, delta);
    if (InputManager::IsKeyDown(SDLK_s)) 
        camera.ProcessKeyboard(BACKWARD, delta);
    if (InputManager::IsKeyDown(SDLK_a)) 
        camera.ProcessKeyboard(LEFT, delta);
    if (InputManager::IsKeyDown(SDLK_d)) 
        camera.ProcessKeyboard(RIGHT, delta);
    if (InputManager::IsKeyDown(SDLK_ESCAPE)) 
        m_running = false;
    
    if (InputManager::lockCursor) 
    {
        glm::vec2 mouseDelta = InputManager::GetMouseDelta();
        camera.ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
    }
    
    InputManager::Update();
}