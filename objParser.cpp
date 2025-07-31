#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

typedef struct Vertex
{
    double x;
    double y;
    double z;
} Vertex_t;

typedef struct Face
{
    size_t v[3];
    size_t vt[3];
    size_t vn[3];
} Face_t;

inline normalizeIndex(size_t currentVertexCount, int index)
{
    return index < 0 ? currentVertexCount + index : index;
}

int main()
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
    
    size_t vertexCount = 0;
    
    while (!objFileStream.eof())
    {
        std::string word;
        objFileStream >> word;
        
        if (word == "v")
        {
            Vertex_t vertex;
            objFileStream >> vertex.x;
            objFileStream >> vertex.y;
            objFileStream >> vertex.z;
            
            // std::cout << vertex.x << std::endl;
            // std::cout << vertex.y << std::endl;
            // std::cout << vertex.z << std::endl;
            
            vertices.push_back(vertex);
            ++vertexCount;
        }
        else if (word == "f")
        {
            Face_t face;
            memset(&face, 0, sizeof(face));
            
            std::string token;
            objFileStream >> token;
            
            bool bTokenSlash = token.find('/') != std::string::npos;
            
            if (bTokenSlash)
            {
                for (int count = 0; count < 3; ++count)
                {
                    size_t indexSlash = 0;
                    size_t indexSubstrStart = 0;
                    
                    std::vector<std::string> attributes;
                    
                    
                    std::cerr << token << std::endl;
                    while ((indexSlash = token.find('/', indexSubstrStart)) != std::string::npos) // TODO: don't use token here, otherwise infinite loop
                    {
                        std::string attr = token.substr(indexSubstrStart, indexSlash - indexSubstrStart);
                        attributes.push_back(attr);
                        
                        indexSubstrStart = indexSlash + 1;
                    }
                    attributes.push_back(token.substr(indexSubstrStart));
                    
                    if (count < 2) objFileStream >> token;
                    
                    face.v[count] = normalizeIndex(vertexCount, std::stoi(attributes[0]));
                    
                    if (attributes.size() == 3)
                    {
                        face.vn[count] = normalizeIndex(vertexCount, std::stoi(attributes[2]));
                        
                        if (!attributes[1].empty())
                        {
                            face.vt[count] = normalizeIndex(vertexCount, std::stoi(attributes[1]));
                            std::cerr << "v " << attributes[0] << " /vt " << attributes[1] << " /vn " << attributes[2] << std::endl;
                        }
                    }
                    else
                    {
                        face.vt[count] = std::stoi(attributes[1]);
                        std::cerr << "v " << attributes[0] << " /vt " << attributes[1] << std::endl;
                    }
                }
            }
            else // This face has only vertices
            {
                face.v[0] = normalizeIndex(vertexCount, std::stoi(token));
                
                signed int signedIndex = 0;
                // TODO : normalize the index for the case bTokenSlash
                objFileStream >> signedIndex;
                face.v[1] = normalizeIndex(vertexCount, signedIndex);
                objFileStream >> signedIndex;
                face.v[2] = normalizeIndex(vertexCount, signedIndex);
            }
            
            faces.push_back(face);
        }
        
        // std::cout << word << std::endl;
    }
    /* 
    for (const auto& vertex : vertices)
    {
        std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    */
    std::cout << "Program complete." << std::endl;
}