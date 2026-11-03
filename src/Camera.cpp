#include "Camera.h"

#define UNIVERSAL_UP glm::vec3(0.0f, 1.0f, 0.0f)

Camera::Camera( glm::vec3 position, glm::vec3 target )
    : position( position ),
      target( target )
{
    updateCameraDerivedData();
}

Camera::~Camera(){

}

void Camera::updateCameraDerivedData(){
    direction = glm::normalize( position - target );
    right = glm::normalize( glm::cross( UNIVERSAL_UP, direction ) );
    up = glm::normalize( glm::cross( direction, right ) );
}

void Camera::updatePosition( glm::vec3 newPosition ){
    this->position = newPosition;
    updateCameraDerivedData();
}

void Camera::updateTarget( glm::vec3 newTarget ){
    this->target = newTarget;
    updateCameraDerivedData();
}

void Camera::getLookAt( glm::mat4& lookAt ){
    lookAt = glm::lookAt(   position,
                            target,
                            UNIVERSAL_UP );
}



