#pragma once
#include "glm/glm.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
    public:
        Camera( glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f)  );
        ~Camera();
        void updateCameraDerivedData();
        void updatePosition( glm::vec3 newPosition );
        void updateTarget( glm::vec3 newTarget );
        void getLookAt( glm::mat4& lookAt );

    private:
        // main 
        glm::vec3 position;
        glm::vec3 target;
        // derived
        glm::vec3 direction;
        glm::vec3 up;
        glm::vec3 right;
};