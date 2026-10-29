#pragma once
#include <glad/gl.h>
#include <vector>

struct TextureParameter{
    GLenum name;
    GLfloat param;
};

class Texture{
    public:
        Texture( const char* image, std::vector<TextureParameter> textureParameters );
        void bind();
        ~Texture();

    private:
        unsigned int texture_;
        int width_;
        int height_; 
        int nrChannels_;
};