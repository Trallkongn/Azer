#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Core/UUID.hpp>
#include <glm/glm.hpp>


namespace Azer {

    struct AABB {
        std::array<float, 3> min;
        std::array<float, 3> max;
    };

    struct Vertex {
        std::array<float, 3> pos{ 0,0,0 };
        std::array<float, 3> normal{ 0,0,0 };
        std::array<float, 2> uv{ 0,0 };
        std::array<float, 4> tangent{ 0,0,0,0 };
    };

    struct Primitive {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        UUID materialId;
    };

    struct Mesh {
        UUID id;
        std::string name;
        std::vector<Primitive> primitives;
        AABB bounds;
    };
}