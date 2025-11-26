#ifndef GEO_H
#define GEO_H

#include <glad/glad.h>
#include <vector>

class Geo {
public:
    Geo() = default;
    ~Geo() = default;

    void createCircle(int segments = 40);
    void createTriangle();
    void createSquare();

    void bind() const;
    unsigned int getCount() const { return count; }

private:
    unsigned int vao = 0, vbo = 0;
    unsigned int count = 0;
};

#endif
