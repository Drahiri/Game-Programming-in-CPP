#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

class VertexArray {
public:
    VertexArray(const float* verts,
          unsigned int numVerts,
          const unsigned int* indices,
          unsigned int numIndices);
    ~VertexArray();

    // Activate this vertex array (so we can draw it)
    void setActive();

    unsigned int getNumIndices() const;
    unsigned int getNumVerts() const;

private:
    // How many vertices in the vertex buffer?
    unsigned int numVerts;
    // How many indices in the index buffer
    unsigned int numIndices;
    // OpenGL ID of the vertex buffer
    unsigned int vertexBuffer;
    // OpenGL ID of the index buffer
    unsigned int indexBuffer;
    // OpenGL ID of the vertex array object
    unsigned int vertexArray;
};

#endif