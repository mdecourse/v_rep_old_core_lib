
#pragma once

#include "vrepMainHeader.h"

struct SObjMaterial
{
    std::string materialName;
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float emission[3];
    float transparency; // 0-1
    float shininess; // 0-1
    std::string textureFile;
};

class CImportExport  
{
public:
    CImportExport();
    virtual ~CImportExport();

    virtual bool importFunc(std::string fileName);
    virtual bool exportFunc(std::string fileName,bool poly,std::vector<int>* sel);
    virtual bool exportFunc(std::string fileName,bool poly,const std::vector<std::vector<float>*>& vertices,const std::vector<std::vector<int>*>& indices,const std::vector<std::string>& names);

    int getFileFormat();
    int getGroupCount();
    int getVertexCount(int groupIndex);
    int getTriangleCount(int groupIndex);
    int getTextureCoordinateCount(int groupIndex);
    int getNormalsCount(int groupIndex);
    bool getVertex(int groupIndex,int vertexIndex,float vertex[3]);
    bool getTriangle(int groupIndex,int triangleIndex,int triangle[3]); 
    bool getTextureCoordinate(int groupIndex,int coordIndex,float coord[2]);
    bool getNormal(int groupIndex,int normalIndex,float normal[3]);
    std::string getName(int groupIndex);
    SObjMaterial* getAndClearMaterial(int groupIndex);

protected:
    std::vector<std::vector<float>*> _importedVertices;
    std::vector<std::vector<int>*> _importedIndices;
    std::vector<std::vector<float>*> _importedTextureCoordinates;
    std::vector<std::vector<float>*> _importedNormals;
    std::vector<SObjMaterial*> _importedMaterials;
    std::vector<std::string> _importedNames;

    unsigned int archiveLength;
    unsigned int actualPosition;
    bool addIntoAnotherList;
    int numberOfTrianglesRead;
    int fileFormat;
};
