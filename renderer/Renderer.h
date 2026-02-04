#pragma once

#include "Mesh.h"
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <cmath>

class Renderer 
{
public:
    Renderer();
    ~Renderer();
    void Initialise(SDL_Renderer* renderer, int width, int height);
public:
    void Clear();
    void AddToBatch(Mesh* mesh);
    void ClearBatchQueue();
    void RenderMeshes(glm::mat4& view, glm::mat4& projection, float rotation);
    void RenderMesh(Mesh* mesh, glm::mat4& view, glm::mat4& projection, float rotation);
    SDL_Renderer* GetRenderer() const { return m_renderer; };
private:
    static glm::mat4 CreateViewportMatrix(int x, int y, int width, int height);
    glm::vec2 GetVertexScreenPos(const glm::vec3& vertexPos, const glm::mat4& mvp);
    bool PointOnScreen(glm::vec2 point);
    void DrawLineDDA(int x1, int y1, int x2, int y2);
private:
    SDL_Renderer* m_renderer;
    glm::mat4 m_viewPort;
    std::vector<Mesh*> m_batchQueue;
    std::vector<float> m_depthBuffer;
    int m_screenWidth, m_screenHeight;
};