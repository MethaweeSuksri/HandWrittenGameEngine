#pragma once
#include "Mesh.h"
#include "Pipeline.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct InWorldState
{
    glm::vec3 translate{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    glm::mat4 matrix() const;
};

struct RenderObject{
    Mesh* mesh;
    Pipeline* pipeline;
    Texture* texture;
    InWorldState inWorldState;

    glm::mat4 getInWorldMatrix() const;
};

struct Scene{
    std::vector<RenderObject> objects;
};