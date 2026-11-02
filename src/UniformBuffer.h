#pragma once
#include <glad/gl.h>
#include <set>
#include "ShaderProgram.h"
#include <string>

class UniformBuffer{

    // TODO : if using multiple opengl context. i have to separate the max binding point since it's not shared 
    public:
        UniformBuffer(  GLsizeiptr size,
                        const GLvoid* data = nullptr,
                        GLenum usage = GL_STATIC_DRAW );
        ~UniformBuffer();
        static GLuint genBindingPoint();
        void bufferSubData( GLintptr offset,
                            GLsizeiptr size,
	                        const void *data);
        void bindUniformBlockToProgram( ShaderProgram& targetProgram, std::string uniformBlockName );

    private:
        static GLuint currentMax;
        static std::set<GLuint> returnedIndex_;
        
        GLuint UBO;
        GLuint buffer_;
        GLuint bindingPointIndex_;

        static GLint getMaxBindingPoint();
    };