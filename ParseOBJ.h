#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "GraphicsMath.h"

bool parseOBJ(std::ifstream& objFileStream, std::vector<Vertex_t>& vertices, std::vector<VertexTextureCoordinate_t>& vertexTextureCoordinates, std::vector<VertexNormal_t>& vertexNormals, std::vector<Face_t>& faces)
{
    size_t vertexCount = 0;
    size_t vertexNormalCount = 0;
    size_t vertexTextureCoordinateCount = 0;
    
    if (objFileStream.eof())
    {
        std::cerr << "parseOBJ: reached objFileStream's EOF before parsing." << std::endl;
        return false;
    }
    
    while (!objFileStream.eof())
    {
        std::string word;
        objFileStream >> word;
        
        if (word[0] == 'v')
        {
            std::string type = word;
            
            Vertex_t vertex;
            objFileStream >> vertex.x;
            objFileStream >> vertex.y;
            objFileStream >> vertex.z;
            
            if (word[1] == '\0')
            {
                vertices.push_back(vertex);
                ++vertexCount;
            }
            else if (word[1] == 'n')
            {
                vertexNormals.push_back(vertex);
                ++vertexNormalCount;
            }
            else if (word[2] == 't')
            {
                vertexTextureCoordinates.push_back(static_cast<VertexTextureCoordinate_t>(vertex));
                ++vertexTextureCoordinateCount;
            }
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
                    while ((indexSlash = token.find('/', indexSubstrStart)) != std::string::npos)
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
                        face.vn[count] = normalizeIndex(vertexNormalCount, std::stoi(attributes[2]));
                        
                        if (!attributes[1].empty())
                        {
                            face.vt[count] = normalizeIndex(vertexTextureCoordinateCount, std::stoi(attributes[1]));
                            std::cerr << "v " << attributes[0] << " /vt " << attributes[1] << " /vn " << attributes[2] << std::endl;
                        }
                    }
                    else
                    {
                        face.vt[count] = normalizeIndex(vertexTextureCoordinateCount, std::stoi(attributes[1]));
                        std::cerr << "v " << attributes[0] << " /vt " << attributes[1] << std::endl;
                    }
                }
            }
            else // This face has only vertices
            {
                face.v[0] = normalizeIndex(vertexCount, std::stoi(token));
                
                signed int signedIndex = 0;
                objFileStream >> signedIndex;
                face.v[1] = normalizeIndex(vertexCount, signedIndex);
                objFileStream >> signedIndex;
                face.v[2] = normalizeIndex(vertexCount, signedIndex);
            }
            
            faces.push_back(face);
        }
        else if (word[0] == '#' || word == "g")
        {
            objFileStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }  
    }
    
    return true;
}