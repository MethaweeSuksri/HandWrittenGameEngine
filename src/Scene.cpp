#include "Scene.h"

glm::mat4 RenderObject::getInWorldMatrix() const{
    glm::mat4 model{1.0f};

    model = glm::translate( model, inWorldState.translate );

    model = glm::rotate(model, glm::radians(inWorldState.rotation.x), {1, 0, 0});
    model = glm::rotate(model, glm::radians(inWorldState.rotation.y), {0, 1, 0});
    model = glm::rotate(model, glm::radians(inWorldState.rotation.z), {0, 0, 1});

    model = glm::scale(model, inWorldState.scale);

    return model;
}

