#pragma once
#include "Camera.h"
#include "Scene.h"
#include "UniformBuffer.h"

// should responsible for rendering each frame
class Renderer{
    public:
        void render(
            const Camera& camera, 
            const Scene& scene
        );

    private:
        UniformBuffer cameraBuffer{192};
};