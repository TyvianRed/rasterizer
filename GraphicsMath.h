#pragma once

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>   

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
    inline std::unique_ptr<double[]>& operator[](size_t row);
    Mat4 operator*(Mat4& rhs) const;
    inline void Print(std::ostream& stream) const;
    
public:
    // double Matrix[4][4];
    std::unique_ptr<std::unique_ptr<double[]>[]> Matrix;
};

std::unique_ptr<double[]>& Mat4::operator[](size_t row)
{
    return Matrix[row];
}

void Mat4::Print(std::ostream& stream) const
{
    stream << "[" << std::endl;
    for (size_t row = 0; row < 4; ++row)
    {
        stream << "[ " << Matrix[row][0] << ", " << Matrix[row][1] << ", " << Matrix[row][2] << ", " << Matrix[row][3] << " ]" << std::endl;
    }
    stream << "]" << std::endl;
}
