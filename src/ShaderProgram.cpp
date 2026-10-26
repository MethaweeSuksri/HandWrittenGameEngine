#include "ShaderProgram.h"
#include <stdexcept>
#include <SDL3/SDL.h>
#include <vector>

// create/link shader program
ShaderProgram::ShaderProgram( const char* file, GLenum shaderType ){
    size_t fileSize_bytes;
    
    // load shader file to const char*
    void* fileBuffer = SDL_LoadFile( file, &fileSize_bytes );
    if ( !fileBuffer ){
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "Fail to load Shader file: %s", SDL_GetError());
        throw std::runtime_error("Failed to load shader");
    }

    const char* shaderSource = static_cast<const char*>(fileBuffer);
    program_ = glCreateShaderProgramv( shaderType, 1, &shaderSource);

    GLint isLinked = GL_FALSE;
    glGetProgramiv(program_, GL_LINK_STATUS, &isLinked);
    if ( isLinked == GL_FALSE ){
        GLint maxLength = 0;
	    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxLength);

    	// The maxLength includes the NULL character
    	std::vector<GLchar> errorLog(maxLength);
    	glGetProgramInfoLog(program_, maxLength, nullptr, errorLog.data());
        
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "Fail to compile Shader: %s", errorLog.data());
        SDL_free(fileBuffer);
        throw std::runtime_error("Fail to compile Shader");
    }

    SDL_free(fileBuffer);
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(program_);
}

GLuint ShaderProgram::id(){
    return program_;
}