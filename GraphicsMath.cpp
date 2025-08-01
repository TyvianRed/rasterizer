#include "GraphicsMath.h"

Mat4::Mat4()
    : Matrix(std::make_unique<std::unique_ptr<double[]>[]>(4))
{
    for (size_t row = 0; row < 4; ++row)
    {
        Matrix[row] = std::make_unique<double[]>(4);
        for (size_t column = 0; column < 4; ++column)
        {
            Matrix[row][column] = 0.0;
        }
    }
    for (size_t index = 0; index < 4; ++index)
    {
        Matrix[index][index] = 1.0;
    }
}

Mat4 Mat4::operator*(Mat4& rhs) const
{
    Mat4 newMatrix;
    for (size_t rowLhs = 0; rowLhs < 4; ++rowLhs)
    {
        for (size_t columnRhs = 0; columnRhs < 4; ++columnRhs)
        {
            newMatrix[rowLhs][columnRhs] =
                Matrix[rowLhs][0] * rhs[0][columnRhs]
                + Matrix[rowLhs][1] * rhs[1][columnRhs]
                + Matrix[rowLhs][2] * rhs[2][columnRhs]
                + Matrix[rowLhs][3] * rhs[3][columnRhs];
        }
    }
    
    return newMatrix;
}