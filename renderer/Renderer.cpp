#include "Renderer.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Initialise(SDL_Renderer* renderer, int width, int height)
{
    m_renderer = renderer;
    m_batchQueue.reserve(10);
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    m_screenWidth = width;
    m_screenHeight = height;
    m_viewPort = CreateViewportMatrix(0, 0, m_screenWidth , m_screenHeight );
    m_depthBuffer.resize(m_screenWidth * m_screenHeight);
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);
}

void Renderer::AddToBatch(Mesh* mesh)
{
    m_batchQueue.push_back(mesh);
}

void Renderer::ClearBatchQueue()
{
    m_batchQueue.clear();
}

void Renderer::RenderMeshes(glm::mat4& view, glm::mat4& projection,float rotation) 
{
    for (auto mesh: m_batchQueue)
    {
        RenderMesh(mesh, view, projection, rotation);
    }
}

bool Renderer::PointOnScreen(glm::vec2 point)
{
    return point.x > 0 && point.x < m_screenWidth && point.y > 0 && point.y < m_screenHeight;
}

void Renderer::RenderMesh(Mesh* mesh, glm::mat4& view, glm::mat4& projection,float rotation) 
{  
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), mesh->position) * 
                  glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 mvp = projection * view * model;
     
    for (size_t i = 0; i < mesh->indices.size(); i += 3) 
    {
        auto p1 = GetVertexScreenPos(mesh->vertices[mesh->indices[i]].Position, mvp);
        auto p2 = GetVertexScreenPos(mesh->vertices[mesh->indices[i + 1]].Position, mvp);
        auto p3 = GetVertexScreenPos(mesh->vertices[mesh->indices[i + 2]].Position, mvp);

        if (PointOnScreen(p1))
        {
            if (PointOnScreen(p2))
            {
                if (PointOnScreen(p3))
                {             
                    DrawLineDDA(p1.x, p1.y, p2.x, p2.y);
                    DrawLineDDA(p1.x, p1.y, p3.x, p3.y);
                    DrawLineDDA(p2.x, p2.y, p3.x, p3.y);
                }
            }
        }

    }
}

void Renderer::DrawLineDDA(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    
    if (steps == 0)
    {
        if (x1 >= 0 && x1 < m_screenWidth && y1 >= 0 && y1 < m_screenHeight)
        {
            SDL_RenderDrawPoint(m_renderer, x1, y1);
        }
        return;
    }
    
    int x = x1 << 16;
    int y = y1 << 16;
    int xInc = (dx << 16) / steps;
    int yInc = (dy << 16) / steps;
    
    for (int i = 0; i <= steps; i++)
    {
        int ix = x >> 16;
        int iy = y >> 16;
        
        if (ix >= 0 && ix < m_screenWidth && iy >= 0 && iy < m_screenHeight)
        {
            SDL_RenderDrawPoint(m_renderer, ix, iy);
        }
        
        x += xInc;
        y += yInc;
    }
}

glm::vec2 Renderer::GetVertexScreenPos(const glm::vec3& vertexPos, const glm::mat4& mvp)
{
    glm::vec4 clipPos = mvp * glm::vec4(vertexPos, 1.0f);

    if (fabs(clipPos.w) > 1e-6f) 
    {
        clipPos /= clipPos.w;
    }

    float screenX = (clipPos.x + 1.0f) * 0.5f * m_screenWidth;
    float screenY = (1.0f - clipPos.y) * 0.5f * m_screenHeight;

    return glm::vec2(screenX, screenY);
}


glm::mat4 Renderer::CreateViewportMatrix(int x, int y, int width, int height) 
{
    glm::mat4 viewport = glm::mat4(0.0f);
    
    viewport[0][0] = width / 2.0f; 
    viewport[0][3] = x + width / 2.0f; 
    
    viewport[1][1] = -height / 2.0f;
    viewport[1][3] = y + height / 2.0f;
    
    viewport[2][2] = 1.0f;
    viewport[3][3] = 1.0f;
    
    return viewport;
}
    