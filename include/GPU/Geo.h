#ifndef GEO_H
#define GEO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Geo {
public:
    Geo() = default;
    ~Geo() = default;

    void createCircle(int segments = 40);
    void createTriangle();
    void createSquare();

    void bind();
    unsigned int getCount() const { return count; }

private:
    unsigned int vao, vbo;
    unsigned int count;
};

#endif // GEO_H
