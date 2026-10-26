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
    
    private:
        GLuint pipeline_ = 0;
        ShaderProgram vertex_;
        ShaderProgram fragment_;
};