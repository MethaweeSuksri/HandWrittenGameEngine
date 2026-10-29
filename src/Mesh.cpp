#include "Mesh.h"

Mesh::Mesh( const std::vector<float>& vertices, 
            const std::vector<unsigned int>& indices,
            VertexLayout& layout ){

    indicesSize = indices.size();

    // for easy comprehension of the buffer, we'll define vertex array object (VAO) first
    glGenVertexArrays(1, &VAO);    
    glBindVertexArray( VAO );
    
    glGenBuffers(1, &VBO);
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW );
    
    for ( VertexAttribute attribute : layout.attributes ){
        glVertexAttribPointer(  attribute.locationIndex, 
                                attribute.size,
                                attribute.type,
                                attribute.normalize,
                                layout.stride,
                                attribute.offset
                            );

        glEnableVertexAttribArray(attribute.locationIndex);
    }
    
    glGenBuffers(1, &EBO);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

Mesh::~Mesh(){
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );
}

void Mesh::drawMesh( GLenum mode​ ){
    glBindVertexArray( VAO );
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}
