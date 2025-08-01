#pragma once

#include <cassert>
#include <cstring>

typedef struct Vertex
{
    double x;
    double y;
    double z;
} Vertex_t;

typedef struct Vertex VertexNormal_t;
typedef struct Vertex VertexTextureCoordinate_t;

typedef struct Face
{
    size_t v[3];
    size_t vt[3];
    size_t vn[3];
} Face_t;

inline normalizeIndex(size_t currentVertexCount, int index)
{
    assert((index < 0) ? currentVertexCount + index + 1 > 0 : true);
    return index < 0 ? currentVertexCount + index + 1 : index;
}

class Mat4
{
public:
    Mat4();
    inline double* operator[](size_t row);
    Mat4 operator*(Mat4& rhs) const;
    
public:
    double Matrix[4][4];
};

double* Mat4::operator[](size_t row)
{
    return Matrix[row];
}

