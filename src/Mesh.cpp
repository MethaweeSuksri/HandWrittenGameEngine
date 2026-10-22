#include "Mesh.h"

Mesh::Mesh( const std::vector<float>& vertices, const std::vector<unsigned int>& indices ){

    indicesSize = indices.size();

    glGenVertexArrays(1, &VAO);    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // for easy comprehension of the buffer, we'll define vertex array object (VAO) first
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(float), indices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
    glEnableVertexAttribArray( 0 );

    // glBindVertexArray( VAO );

}

Mesh::~Mesh(){
    // glDeleteVertexArrays( 1, VAO );
    // glDeleteBuffers( 1, EBO );
    // glDeleteBuffers( 1, VBO );
}

void Mesh::drawMesh( GLenum mode​ ){
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
