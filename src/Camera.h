#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

struct Fustrum{
    float fovY_radians; // in radians
    float aspect; //field of view in the x direction
    float zNear;
    float zFar;
};

class Camera{
    public:
        Camera( glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f),
                Fustrum fustrum = { 0.0f,0.0f,0.0f,0.0f} );
        ~Camera();
        
        Fustrum fustrum;
        
        void updateCameraDerivedData();
        void updatePosition( glm::vec3 newPosition );
        void updateTarget( glm::vec3 newTarget );
        void getView( glm::mat4& lookAt ) const ;
        void getProjection( glm::mat4& projection ) const ;

        // camera movement
        void moveForward( float scale );
        void moveRight( float scale );
        void moveUp( float scale);

        // camera yaw and pitch base on delta mouse position by time ( no roll ) + try to use quarternion

    private:
        // main 
        glm::vec3 position;
        glm::vec3 target;

        // derived
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;
};