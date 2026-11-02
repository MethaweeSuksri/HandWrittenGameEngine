#include "UniformBuffer.h"
#include <stdexcept>

GLuint UniformBuffer::currentMax = 0;
std::set<GLuint> UniformBuffer::returnedIndex_;

UniformBuffer::UniformBuffer( GLsizeiptr size, const GLvoid* data, GLenum usage ){
    bindingPointIndex_ = UniformBuffer::genBindingPoint();
    
    glGenBuffers( 1, &UBO );
    glBindBuffer( GL_UNIFORM_BUFFER, UBO );
    glBufferData( GL_UNIFORM_BUFFER, size, data, usage);

    glBindBufferBase( GL_UNIFORM_BUFFER, bindingPointIndex_, UBO );    
}

UniformBuffer::~UniformBuffer(){
    UniformBuffer::returnedIndex_.insert(bindingPointIndex_);
    glDeleteBuffers( 1, &UBO );
}

GLint UniformBuffer::getMaxBindingPoint(){
    GLint max = 0;
    glGetIntegerv( GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);
    return max;
}

GLuint UniformBuffer::genBindingPoint() {
    if (!returnedIndex_.empty()) {
        GLuint index = *returnedIndex_.begin();
        returnedIndex_.erase(returnedIndex_.begin());
        return index;
    }

    // Check whether we have reached the OpenGL limit.
    if (currentMax >= getMaxBindingPoint()) {
        throw std::runtime_error(
            "Maximum uniform buffer binding points reached"
        );
    }

    return currentMax++;
}

void UniformBuffer::bindUniformBlockToProgram( ShaderProgram& targetProgram, std::string uniformBlockName ){
    GLuint blockIndex = glGetUniformBlockIndex( targetProgram.id(), uniformBlockName.c_str() );   
    
    if ( blockIndex == GL_INVALID_INDEX ){
        throw std::runtime_error(
            "Uniform block not found: " + uniformBlockName
        );
    }

    glUniformBlockBinding( targetProgram.id(), blockIndex, bindingPointIndex_);
}

void UniformBuffer::bufferSubData(  GLintptr offset,
                                    GLsizeiptr size,
	                                const void *data ){
    glNamedBufferSubData( UBO, offset, size, data );
}