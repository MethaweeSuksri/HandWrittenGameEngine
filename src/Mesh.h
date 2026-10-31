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
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
        GLsizei indexCount = 0;
        
    public:
        Mesh( const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout );
        ~Mesh();
        void drawMesh( GLenum mode​ );

    };