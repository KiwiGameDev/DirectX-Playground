#include "MeshFactory.h"
#include "GraphicsEngine.h"
#include "Mesh.h"
#include "Vector3.h"
#include "VertexPositionColor.h"
#include <vector>

#include "VertexPositionUV.h"

void MeshFactory::initializeAllMeshes()
{
    GraphicsEngine::get().getMeshManager().addResource(std::reinterpret_pointer_cast<Resource>(createPlaneMesh()));
    GraphicsEngine::get().getMeshManager().addResource(std::reinterpret_pointer_cast<Resource>(createCubeMesh()));
    GraphicsEngine::get().getMeshManager().addResource(std::reinterpret_pointer_cast<Resource>(createTexturedCubeMesh()));
    GraphicsEngine::get().getMeshManager().addResource(std::reinterpret_pointer_cast<Resource>(createSphereMesh()));
    GraphicsEngine::get().getMeshManager().addResource(std::reinterpret_pointer_cast<Resource>(createCapsuleMesh()));
}

MeshPtr MeshFactory::createSphereMesh()
{
    const float PI = 3.141592654f;
    const float PI_HALF = PI / 2.0f;
    uint16_t segments = 16;
    uint16_t rings = 32;
    uint16_t sectorCount = segments;
    uint16_t stackCount = (rings + 1);
    std::vector<VertexPositionColor> vertices;

    // Generate sphere vertices
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI_HALF - i * stackStep;
        float xz = cosf(stackAngle);
        float y = sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            float x = xz * cosf(sectorAngle);
            float z = xz * sinf(sectorAngle);
            vertices.push_back({ Vector3(x, y, z), Vector3((x + 1.0f) / 2.0f, (y + 1.0f) / 2.0f, (z + 1.0f) / 2.0f) });
        }
    }
    
    std::vector<unsigned int> indices;
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
                indices.push_back(k2);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    VertexBufferPtr vb = GraphicsEngine::get().getRenderSystem().createVertexBuffer(
        vertices.data(),
        sizeof(VertexPositionColor),
        vertices.size(),
        GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl"),
        VertexFormat::POSITION_COLOR);
    IndexBufferPtr ib = GraphicsEngine::get().getRenderSystem().createIndexBuffer(
        indices.data(),
        indices.size());

    return std::make_shared<Mesh>("sphere.obj", vb, ib);
}

MeshPtr MeshFactory::createCubeMesh()
{
    VertexPositionColor vertices[] =
    {
        // Front face
        { Vector3(-0.5f,-0.5f,-0.5f), Vector3(-0.5f,-0.5f,-0.5f) * 2.0f },
        { Vector3(-0.5f,0.5f,-0.5f), Vector3(-0.5f,0.5f,-0.5f) * 2.0f},
        { Vector3(0.5f,0.5f,-0.5f), Vector3(0.5f,0.5f,-0.5f) * 2.0f},
        { Vector3(0.5f,-0.5f,-0.5f), Vector3(0.5f,-0.5f,-0.5f) * 2.0f},
        // Back face
        { Vector3(0.5f,-0.5f,0.5f), Vector3(0.5f,-0.5f,0.5f) * 2.0f },
        { Vector3(0.5f,0.5f,0.5f), Vector3(0.5f,0.5f,0.5f) * 2.0f},
        { Vector3(-0.5f,0.5f,0.5f), Vector3(-0.5f,0.5f,0.5f) * 2.0f },
        { Vector3(-0.5f,-0.5f,0.5f), Vector3(-0.5f,-0.5f,0.5f) * 2.0f },
    };

    unsigned int indices[] =
    {
        // FRONT SIDE
        0,1,2, 
        2,3,0,
        // BACK SIDE
        4,5,6,
        6,7,4,
        // TOP SIDE
        1,6,5,
        5,2,1,
        // BOTTOM SIDE
        7,0,3,
        3,4,7,
        // RIGHT SIDE
        3,2,5,
        5,4,3,
        // LEFT SIDE
        7,6,1,
        1,0,7
    };

    VertexBufferPtr vb = GraphicsEngine::get().getRenderSystem().createVertexBuffer(
        vertices,
        sizeof(VertexPositionColor),
        ARRAYSIZE(vertices),
        GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl"),
        VertexFormat::POSITION_COLOR);
    IndexBufferPtr ib = GraphicsEngine::get().getRenderSystem().createIndexBuffer(
        indices,
        ARRAYSIZE(indices));

    return std::make_shared<Mesh>("cube_colored.obj", vb, ib);
}

MeshPtr MeshFactory::createTexturedCubeMesh()
{
    Vector3 position_list[] =
    {
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
        Vector3( 0.5f,  0.5f, -0.5f),
        Vector3( 0.5f, -0.5f, -0.5f),
        Vector3( 0.5f, -0.5f,  0.5f),
        Vector3( 0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f,  0.5f),
        Vector3(-0.5f, -0.5f,  0.5f)
    };

    Vector2 texcoord_list[] =
    {
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
    };

    VertexPositionUV vertices[] =
    {
        { position_list[0], texcoord_list[1] },
        { position_list[1], texcoord_list[0] },
        { position_list[2], texcoord_list[2] },
        { position_list[3], texcoord_list[3] },

        { position_list[4], texcoord_list[1] },
        { position_list[5], texcoord_list[0] },
        { position_list[6], texcoord_list[2] },
        { position_list[7], texcoord_list[3] },

        { position_list[1], texcoord_list[1] },
        { position_list[6], texcoord_list[0] },
        { position_list[5], texcoord_list[2] },
        { position_list[2], texcoord_list[3] },

        { position_list[7], texcoord_list[1] },
        { position_list[0], texcoord_list[0] },
        { position_list[3], texcoord_list[2] },
        { position_list[4], texcoord_list[3] },

        { position_list[3], texcoord_list[1] },
        { position_list[2], texcoord_list[0] },
        { position_list[5], texcoord_list[2] },
        { position_list[4], texcoord_list[3] },

        { position_list[7], texcoord_list[1] },
        { position_list[6], texcoord_list[0] },
        { position_list[1], texcoord_list[2] },
        { position_list[0], texcoord_list[3] }
    };


    unsigned int indices[] =
    {
        0,  1,  2,
        2,  3,  0,
        4,  5,  6,
        6,  7,  4,
        8,  9,  10,
        10, 11, 8,
        12, 13, 14,
        14, 15, 12,
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };

    VertexBufferPtr vb = GraphicsEngine::get().getRenderSystem().createVertexBuffer(
        vertices,
        sizeof(VertexPositionUV),
        ARRAYSIZE(vertices),
        GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("TexturedVertexShader.hlsl"),
        VertexFormat::POSITION_UV);
    IndexBufferPtr ib = GraphicsEngine::get().getRenderSystem().createIndexBuffer(
        indices,
        ARRAYSIZE(indices));

    return std::make_shared<Mesh>("cube.obj", vb, ib);
}

MeshPtr MeshFactory::createCapsuleMesh(float height, float radius)
{
    const float PI = 3.141592654f;
    const float PI_HALF = PI / 2.0f;
    uint16_t segments = 16;
    uint16_t rings = 32;
    uint16_t sectorCount = segments;
    uint16_t stackCount = (rings + 1);

    std::vector<VertexPositionColor> vertices;

    if (stackCount % 2 == 0)
        stackCount++;

    // Generate capsule vertices
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle, currentCapsuleHeight;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI_HALF - i * stackStep;
        float xz = cosf(stackAngle) * radius;
        float y = (sinf(stackAngle) * radius) + height;

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;
            float x = xz * cosf(sectorAngle);
            float z = xz * sinf(sectorAngle);
            vertices.push_back({ Vector3(x, y, z), Vector3((x + 1.0f) / 2.0f, (y + 1.0f) / 2.0f, (z + 1.0f) / 2.0f) });
        }

        if (i == stackCount / 2 && stackCount % i == 1)
            height *= -1;
    }

    std::vector<unsigned int> indices;
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
                indices.push_back(k2);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }
    
    VertexBufferPtr vb = GraphicsEngine::get().getRenderSystem().createVertexBuffer(
        vertices.data(),
        sizeof(VertexPositionColor),
        vertices.size(),
        GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl"),
        VertexFormat::POSITION_COLOR);
    IndexBufferPtr ib = GraphicsEngine::get().getRenderSystem().createIndexBuffer(
        indices.data(),
        indices.size());

    return std::make_shared<Mesh>("capsule.obj", vb, ib);
}

MeshPtr MeshFactory::createPlaneMesh()
{
    VertexPositionColor vertices[] =
    {
        { Vector3(-0.5f, 0.0f, -0.5f), Vector3(-1.0f, 0.0f, -1.0f) },
        { Vector3(-0.5f, 0.0f,  0.5f), Vector3(-1.0f, 0.0f,  1.0f) },
        { Vector3( 0.5f, 0.0f,  0.5f), Vector3( 1.0f, 0.0f,  1.0f) },
        { Vector3( 0.5f, 0.0f, -0.5f), Vector3( 1.0f, 0.0f, -1.0f) }
    };

    unsigned int indices[] =
    {
		0, 1, 2,
		2, 3, 0
    };

    VertexBufferPtr vb = GraphicsEngine::get().getRenderSystem().createVertexBuffer(
        vertices,
        sizeof(VertexPositionColor),
        ARRAYSIZE(vertices),
        GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("ColoredVertexShader.hlsl"),
        VertexFormat::POSITION_COLOR);
    IndexBufferPtr ib = GraphicsEngine::get().getRenderSystem().createIndexBuffer(
        indices,
        ARRAYSIZE(indices));

    return std::make_shared<Mesh>("plane.obj", vb, ib);
}
