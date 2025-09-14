#include "azpch.h"
#include "MeshLibrary.h"
#include <glm/gtc/constants.hpp> // Add this include for glm::pi and glm::two_pi

namespace Azer {

    MeshData Azer::MeshLibrary::CreateCube(float size)
    {
        MeshData mesh;
        float h = size / 2.0f;

        // 每个面单独的顶点（因为法线不同，不能复用同一个位置）
        glm::vec3 faceNormals[] = {
            {0,0,-1}, // back
            {0,0, 1}, // front
            {-1,0,0}, // left
            {1,0,0},  // right
            {0,1,0},  // top
            {0,-1,0}  // bottom
        };

        glm::vec3 faceVertices[6][4] = {
            { {-h,-h,-h}, { h,-h,-h}, { h, h,-h}, {-h, h,-h} }, // back
            { {-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h} }, // front
            { {-h,-h,-h}, {-h, h,-h}, {-h, h, h}, {-h,-h, h} }, // left
            { { h,-h,-h}, { h, h,-h}, { h, h, h}, { h,-h, h} }, // right
            { {-h, h,-h}, { h, h,-h}, { h, h, h}, {-h, h, h} }, // top
            { {-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h} }  // bottom
        };

        glm::vec2 uvs[4] = {
            {0,0}, {1,0}, {1,1}, {0,1}
        };

        for (int f = 0; f < 6; f++) {
            for (int v = 0; v < 4; v++) {
                Vertex vert;
                vert.position = faceVertices[f][v];
                vert.normal = faceNormals[f];
                vert.texCoord = uvs[v];
                mesh.vertices.push_back(vert);
            }

            unsigned int base = f * 4;
            mesh.indices.insert(mesh.indices.end(), {
                base, base + 1, base + 2,
                base + 2, base + 3, base,
                });
        }

        return mesh;
    }

    MeshData Azer::MeshLibrary::CreatePlane(float width, float depth)
    {
        MeshData mesh;
        float hw = width / 2.0f;
        float hd = depth / 2.0f;

        mesh.vertices = {
            {{-hw, 0, -hd}, {0,1,0}, {0,0}},
            {{ hw, 0, -hd}, {0,1,0}, {1,0}},
            {{ hw, 0,  hd}, {0,1,0}, {1,1}},
            {{-hw, 0,  hd}, {0,1,0}, {0,1}}
        };

        mesh.indices = { 0,1,2, 2,3,0 };
        return mesh;    
    }

    MeshData Azer::MeshLibrary::CreateSphere(float radius, int stacks, int slices)
    {
        MeshData mesh;

        for (int i = 0; i <= stacks; ++i) {
            float v = (float)i / stacks;
            float phi = v * glm::pi<float>();

            for (int j = 0; j <= slices; ++j) {
                float u = (float)j / slices;
                float theta = u * glm::two_pi<float>();

                float x = radius * sin(phi) * cos(theta);
                float y = radius * cos(phi);
                float z = radius * sin(phi) * sin(theta);

                glm::vec3 pos = { x, y, z };
                glm::vec3 normal = glm::normalize(pos);

                mesh.vertices.push_back({ pos, normal, {u, v} });
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                int first = i * (slices + 1) + j;
                int second = first + slices + 1;

                mesh.indices.push_back(first);
                mesh.indices.push_back(second);
                mesh.indices.push_back(first + 1);

                mesh.indices.push_back(second);
                mesh.indices.push_back(second + 1);
                mesh.indices.push_back(first + 1);
            }
        }
        return mesh;
    }

    MeshData Azer::MeshLibrary::CreateCylinder(float radius, float height, int segments)
    {
        MeshData mesh;
        float halfH = height / 2.0f;

        // 圆周顶点
        for (int i = 0; i <= segments; i++) {
            float theta = (float)i / segments * glm::two_pi<float>();
            float x = cos(theta);
            float z = sin(theta);

            glm::vec3 normal = { x, 0, z };

            // 上下两圈
            mesh.vertices.push_back({ {radius * x, -halfH, radius * z}, normal, {(float)i / segments, 0} });
            mesh.vertices.push_back({ {radius * x,  halfH, radius * z}, normal, {(float)i / segments, 1} });
        }

        // 侧面索引
        for (int i = 0; i < segments; i++) {
            int base = i * 2;
            mesh.indices.push_back(base);
            mesh.indices.push_back(base + 1);
            mesh.indices.push_back(base + 2);

            mesh.indices.push_back(base + 1);
            mesh.indices.push_back(base + 3);
            mesh.indices.push_back(base + 2);
        }

        // 圆盖
        int centerBottom = mesh.vertices.size();
        mesh.vertices.push_back({ {0,-halfH,0},{0,-1,0},{0.5f,0.5f} });
        int centerTop = mesh.vertices.size();
        mesh.vertices.push_back({ {0, halfH,0},{0, 1,0},{0.5f,0.5f} });

        for (int i = 0; i < segments; i++) {
            int base = i * 2;

            // bottom
            mesh.indices.push_back(centerBottom);
            mesh.indices.push_back(base);
            mesh.indices.push_back((base + 2) % (segments * 2));

            // top
            mesh.indices.push_back(centerTop);
            mesh.indices.push_back((base + 3) % (segments * 2));
            mesh.indices.push_back(base + 1);
        }

        return mesh;
    }
}


