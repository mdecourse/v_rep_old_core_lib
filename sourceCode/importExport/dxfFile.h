
#pragma once

#include "vrepMainHeader.h"
#include "importExport.h"
#include "shape.h"

class CDxfFile : public CImportExport
{
public:
    CDxfFile();
    virtual ~CDxfFile();
    bool importFunc(std::string fileName);
    bool exportFunc(std::string fileName,bool poly,std::vector<int>* sel);
    bool exportFunc(std::string fileName,bool poly,const std::vector<std::vector<float>*>& vertices,const std::vector<std::vector<int>*>& indices,const std::vector<std::string>& names);

private:
    void _readFile(VArchive& archive);
    bool _getCode(VArchive& archive);
    void _polyLineInBlock(VArchive &archive);
    void _threeDFaceInBlock(VArchive &archive);
    void _vertexInBlock(VArchive &archive);
    void _insertCommand(VArchive &archive,
                    std::vector<std::vector<float>*>* aVertices,
                    std::vector<std::vector<int>*>* aIndices,
                    std::vector<std::vector<float>*>* aNormals,
                    std::vector<std::vector<float>*>* aTexCoords,
                    std::vector<std::string>* groupNames,
                    std::vector<SObjMaterial*>* aMaterials);
    void _threeDFaceCommand(VArchive &archive,
                    std::vector<std::vector<float>*>* aVertices,
                    std::vector<std::vector<int>*>* aIndices,
                    std::vector<std::vector<float>*>* aNormals,
                    std::vector<std::vector<float>*>* aTexCoords,
                    std::vector<std::string>* groupNames,
                    std::vector<SObjMaterial*>* aMaterials);
    void _polylineCommand(VArchive &archive,
                    std::vector<std::vector<float>*>* aVertices,
                    std::vector<std::vector<int>*>* aIndices,
                    std::vector<std::string>* groupNames);
    bool _writeCode(VArchive& archive,int number,std::string text);
    bool _writeCodeInt(VArchive& archive,int number,int comm);
    bool _writeCodeFloat(VArchive& archive,int number,float comm);
    void _writeAShapeWith3DFace(CShape* it,VArchive& archive);
    void _writeAShapeWithPolyline(CShape* it,VArchive& archive,int& initialSize);
    void _writeAMeshWith3DFace(const std::vector<float>& vertices,const std::vector<int>& indices,VArchive& archive);
    void _writeAMeshWithPolyline(const std::vector<float>& vertices,const std::vector<int>& indices,VArchive& archive,int& initialSize);

    bool addIntoAnotherList;
    int triangleCountExport;
    int totalTriangleExport;

    std::string code;
    std::string value;

    std::vector<std::string> blockNames;
    std::vector<std::vector<float>*> blockVertices;
    std::vector<std::vector<int>*> blockIndices;
    std::vector<float> blockBasePoints;
    std::vector<float> tempVertices;
    std::vector<int> tempIndices;
};
