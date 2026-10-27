#pragma once
#include <glad/gl.h>
#include <vector>

struct VertexAttribute{
    GLuint locationIndex;
    GLuint size;
    GLenum type;
    GLboolean normalize;
    const GLvoid* offset;
};

struct VertexLayout{
    size_t stride;
    std::vector<VertexAttribute> attributes;
};

class Mesh{
    private:
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int indicesSize = 0;
        
    public:
        Mesh( const std::vector<float>& vertices, const std::vector<unsigned int>& indices, VertexLayout& layout );
        ~Mesh();
        void drawMesh( GLenum mode​ );

    };