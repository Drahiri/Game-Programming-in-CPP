#include "vertex_array.h"

#include <GL/glew.h>

VertexArray::VertexArray(Layout layout,
      const float* verts,
      unsigned int numVerts,
      const unsigned int* indices,
      unsigned int numIndices) :
    numVerts(numVerts),
    numIndices(numIndices) {
    // Create VBO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    unsigned vertexSize = 8 * sizeof(float);
    if(layout == Layout::PosNormSkinTex) {
        vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
    }

    // Create vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, // The active buffer to write to
          numVerts * vertexSize,  // Number of bytes to copy
          verts,                  // Source to copy from (pointer)
          GL_STATIC_DRAW          // How will we use this data?
    );

    // Create index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,    // Index buffer
          numIndices * sizeof(unsigned int), // Size of data
          indices,
          GL_STATIC_DRAW);

    // By changing layout I could remove repetitions
    // Activate vertex attribute
    if(layout == Layout::PosNormTex) {
        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, // Attribute index (0 for first one)
              3,                 // Number of components (3 in this case)
              GL_FLOAT,          // Type of the components
              GL_FALSE,          // (Only used for integral types)
              vertexSize,        // Stride (usually size of each vertex)
              0                  // offset from start of vertex to this attribute
        );

        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
              1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));

        // UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
              2, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));
    } else {
        // Position is 3 floats
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

        // Normal is 3 floats
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
              1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));

        // Skinning bones (keep as ints)
        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(
              2, 4, GL_UNSIGNED_BYTE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));

        // Skinning weights (convert to floats)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,
              4,
              GL_UNSIGNED_BYTE,
              GL_TRUE,
              vertexSize,
              reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

        // UV
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(2,
              2,
              GL_FLOAT,
              GL_FALSE,
              vertexSize,
              reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
    }
}

VertexArray::~VertexArray() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
}

void VertexArray::setActive() {
    glBindVertexArray(vertexArray);
}

unsigned int VertexArray::getNumIndices() const {
    return numIndices;
}

unsigned int VertexArray::getNumVerts() const {
    return numVerts;
}
