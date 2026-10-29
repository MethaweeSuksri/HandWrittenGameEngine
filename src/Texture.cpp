#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"
#include "Texture.h"

#include <SDL3/SDL.h>

#include <stdexcept>


Texture::Texture( const char* image, std::vector<TextureParameter> textureParameters ){
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    for( TextureParameter texParam : textureParameters ){
        glTexParameteri(GL_TEXTURE_2D, texParam.name, texParam.param );
    }
    
    unsigned char *data = stbi_load( image, &width_, &height_, &nrChannels_, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        SDL_LogError( SDL_LOG_CATEGORY_VIDEO, "Failed to load Texture : %s", image );
        throw std::runtime_error("Fail to compile Shader");
    }
    stbi_image_free(data);
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, texture_);
}