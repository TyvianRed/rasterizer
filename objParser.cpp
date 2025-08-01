#include "GraphicsMath.h"
#include "ParseOBJ.h"

void testParseOBJ()
{
    std::string fileName;
    std::cout << "OBJ File name: ";
    std::cin >> fileName;
    fileName += ".obj";
    
    std::ifstream objFileStream(fileName, std::ios::in);
    if (!objFileStream)
    {
        std::cerr << "Failed to open file " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Opened file " << fileName << std::endl;
    
    std::vector<Vertex_t> vertices;
    vertices.reserve(1024);
    
    std::vector<Face_t> faces;
    faces.reserve(1024);
    
    std::vector<VertexNormal_t> vertexNormals;
    vertexNormals.reserve(1024);
    
    std::vector<VertexTextureCoordinate_t> vertexTextureCoordinates;
    vertexTextureCoordinates.reserve(1024);
    
    parseOBJ(objFileStream, vertices, vertexTextureCoordinates, vertexNormals, faces);
    std::cout << "Completed parsing the OBJ model." << std::endl;
}

void testMat()
{
    Mat4 identity0;
    Mat4 identity1;
    Mat4 identitySquared = identity0 * identity1;
    identitySquared.Print(std::cout);
}

int main()
{
    // testParseOBJ();
    testMat();

    std::cout << "Program complete." << std::endl;
}