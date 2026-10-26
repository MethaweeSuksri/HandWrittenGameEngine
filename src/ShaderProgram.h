#pragma once
#include <glad/gl.h>

class ShaderProgram{
    public:
        ShaderProgram( const char* file, GLenum shaderType);
        ~ShaderProgram();
        
        GLuint id();
        GLint getUniformLocation(const char* name) const;

    private:
        GLuint program_ = 0;
};
