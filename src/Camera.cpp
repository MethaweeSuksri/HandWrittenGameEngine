#include "Camera.h"

#define UNIVERSAL_UP glm::vec3(0.0f, 1.0f, 0.0f)

Camera::Camera( glm::vec3 position, glm::vec3 target, Fustrum fustrum )
    : position( position ),
      target( target ),
      fustrum( fustrum )
{
    updateCameraDerivedData();
}

Camera::~Camera(){

}

void Camera::updateCameraDerivedData(){
    forward = glm::normalize( target - position );
    right = glm::normalize( glm::cross( forward, UNIVERSAL_UP ) );
    up = glm::normalize( glm::cross( right, forward ) );
}

void Camera::updatePosition( glm::vec3 newPosition ){
    this->position = newPosition;
    updateCameraDerivedData();
}

void Camera::updateTarget( glm::vec3 newTarget ){
    this->target = newTarget;
    updateCameraDerivedData();
}

void Camera::getView( glm::mat4& lookAt ) const {
    lookAt = glm::lookAt(   position,
                            target,
                            UNIVERSAL_UP );
}

void Camera::getProjection( glm::mat4& projection ) const {
    projection = glm::perspective(
        fustrum.fovY_radians,
        fustrum.aspect,
        fustrum.zNear,
        fustrum.zFar
    );
}



