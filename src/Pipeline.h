#pragma once
#include <glad/gl.h>
#include "ShaderProgram.h"

// struct Stage{
//     GLuint program;
//     GLbitfield stageBitField;
// };

class Pipeline{
    public:
        Pipeline( const char* vertexFile, const char* fragmentFile );
        ~Pipeline();
        void bind();
        ShaderProgram vertex_;
        ShaderProgram fragment_;
    
    private:
        GLuint pipeline_ = 0;
};