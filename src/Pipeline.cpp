#include "Pipeline.h"
#include <SDL3/SDL.h>

Pipeline::Pipeline( const char* vertexFile, const char* fragmentFile )
    : vertex_( vertexFile, GL_VERTEX_SHADER ),
      fragment_( fragmentFile, GL_FRAGMENT_SHADER )
{
    glGenProgramPipelines( 1, &pipeline_ );
    
    glUseProgramStages( pipeline_, GL_VERTEX_SHADER_BIT, vertex_.id());
    glUseProgramStages( pipeline_, GL_FRAGMENT_SHADER_BIT, fragment_.id());
}

void Pipeline::bind(){
    glBindProgramPipeline( pipeline_ );
}

Pipeline::~Pipeline(){
    glDeleteProgramPipelines( 1, &pipeline_ );

    // should destuct vertex_ and fragment_ autometically
}