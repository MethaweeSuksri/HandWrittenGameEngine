#include "Renderer.h"

void Renderer::render( const Camera& camera, const Scene& scene ){
    for ( auto& object : scene.objects ){

        glm::mat4 model = object.getInWorldMatrix();
        glm::mat4 view;
        glm::mat4 projection;
        camera.getView(view);
        camera.getProjection(projection);

        cameraBuffer.bufferSubData(0, 64, &model);
        cameraBuffer.bufferSubData(64, 64, &view);
        cameraBuffer.bufferSubData(128, 64, &projection);

        cameraBuffer.bindUniformBlockToProgram( object.pipeline->vertex, "Matrices" );
        
        object.texture->bind();

        object.pipeline->bind();
        
        object.mesh->drawMesh( GL_TRIANGLES );

    }
}
